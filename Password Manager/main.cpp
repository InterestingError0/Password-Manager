#define SQLITE_HAS_CODEC
#define SQLITE_TEMP_STORE 2

#include "constants.hpp"
#include "miscellaneousFunctions.hpp"
#include "cryptographyFunctions.hpp"
#include "loginScreen.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include "sqlite_orm/sqlite_orm.h"
#include "sqlcipher/sqlite3.h"

struct logins {
	int id;
	std::string folder;
	std::string website;
	std::string username;
	std::string password;
};

struct passwordGeneratorHistory {
	int id;
	std::string generatedPassword;
};

struct secureNotes {
	int id;
	std::string folder;
	std::string title;
	std::string contents;
};

struct folders {
	int id;
	std::string folder;
};

int main() {
	using namespace sqlite_orm;
	auto storage = make_storage("data.sqlite",
	                            make_table("logins",
	                                       make_column("id", &logins::id, primary_key().autoincrement()),
	                                       make_column("folder", &logins::folder),
	                                       make_column("website", &logins::website),
	                                       make_column("username", &logins::username),
	                                       make_column("password", &logins::password)),
	                            make_table("passwordGeneratorHistory",
	                                       make_column("id", &passwordGeneratorHistory::id,
	                                                   primary_key().autoincrement()),
	                                       make_column("generatedPassword",
	                                                   &passwordGeneratorHistory::generatedPassword)),
	                            make_table("secureNotes",
	                                       make_column("id", &secureNotes::id, primary_key().autoincrement()),
	                                       make_column("folder", &secureNotes::folder),
	                                       make_column("title", &secureNotes::title),
	                                       make_column("contents", &secureNotes::contents)),
	                            make_table("folders",
	                                       make_column("id", &folders::id, primary_key().autoincrement()),
	                                       make_column("folder", &folders::folder)));
	storage.on_open = [&](sqlite3 *db) {
		std::string key = loginScreen();
		sqlite3_key(db, key.c_str(), key.size());
	};
	storage.open_forever();
	storage.sync_schema();

	if(storage.select(1, from<folders>()).empty()) {
		storage.insert(into<folders>(), columns(&folders::folder),
		               values(std::make_tuple("Email"), std::make_tuple("Entertainment"), std::make_tuple("Games"),
		                      std::make_tuple("News/Reference"), std::make_tuple("Productivity Tools"),
		                      std::make_tuple("None")));
	}
	const auto loginsSelect = storage.prepare(select(columns(&logins::folder, &logins::website, &logins::username, &logins::password)));
	auto loginExistsCheck = storage.prepare(select(1, from<logins>(), where(is_equal(&logins::website, "") and is_equal(&logins::username, "")), limit(1)));
	const auto loginsEmptyCheck = storage.prepare(select(1, from<logins>(), limit(1)));
	auto loginInsert = storage.prepare(insert(into<logins>(), columns(&logins::folder, &logins::website, &logins::username, &logins::password), values(std::make_tuple("", "", "", ""))));
	auto loginDelete = storage.prepare(remove_all<logins>(where(is_equal(&logins::website, "") and is_equal(&logins::username, ""))));

	auto passwordGeneratorHistoryInsert = storage.prepare(insert(into<passwordGeneratorHistory>(), columns(&passwordGeneratorHistory::generatedPassword), values(std::make_tuple(""))));
	const auto passwordGeneratorHistorySelect = storage.prepare(select(&passwordGeneratorHistory::generatedPassword));
	const auto passwordGeneratorHistoryEmptyCheck = storage.prepare(select(1, from<passwordGeneratorHistory>(), limit(1)));
	const auto passwordGeneratorHistoryDelete = storage.prepare(remove_all<passwordGeneratorHistory>());

	const auto secureNotesSelect = storage.prepare(select(columns(&secureNotes::folder, &secureNotes::title, &secureNotes::contents)));
	auto secureNoteExistsCheck = storage.prepare(select(1, from<secureNotes>(), where(is_equal(&secureNotes::title, "")), limit(1)));
	auto secureNoteInsert = storage.prepare(insert(into<secureNotes>(), columns(&secureNotes::folder, &secureNotes::title, &secureNotes::contents), values(std::make_tuple("", "", ""))));
	const auto secureNoteEmptyCheck = storage.prepare(select(1, from<secureNotes>(), limit(1)));
	auto secureNoteDelete = storage.prepare(remove_all<secureNotes>(where(is_equal(&secureNotes::title, ""))));

	auto folderExists = storage.prepare(select(1, from<folders>(), where(is_equal(&folders::folder, "")), limit(1)));
	const auto folderSelect = storage.prepare(select(&folders::folder));
	auto folderExistsCheck = storage.prepare(select(1, from<folders>(), where(is_equal(&folders::folder, "")), limit(1)));
	auto folderInsert = storage.prepare(insert(into<folders>(), columns(&folders::folder), values(std::make_tuple(""))));
	auto folderDelete = storage.prepare(remove_all<folders>(where(is_equal(&folders::folder, ""))));

	int mainMenuChoice;
	do {
		std::cout
				<< "\n\t1. Logins\n\t2. Password Generator\n\t3. Password Generator History\n\t4. Secure Notes\n\t5. Folders\n\t6. Exit\n\n";
		mainMenuChoice = menuChoice(6);
		switch(mainMenuChoice) {
			case 1: {
				auto loginsVec = storage.execute(loginsSelect);
				std::cout << "Here are the logins stored in your vault:\n\n";
				for(const auto &login: loginsVec) {
					std::cout << "Folder: " << std::get<0>(login) << "\nWeb Address: " << std::get<1>(login)
					          << "\nUsername: " << std::get<2>(login) << "\nPassword: " << std::get<3>(login) << "\n\n";
				}
				std::cout << "\t1. Store Logins\n\t2. Delete Logins\n\t3. Back to the Main Menu\n\n";
				switch(menuChoice(3)) {
					case 1: {
						auto foldersVec = storage.execute(folderSelect);
						std::cout << "Here are the folders currently available:\n\n";
						for(const auto &folder: foldersVec) {
							std::cout << folder << '\n';
						}
						std::cout << '\n';
						std::string inputFolderName;
						while((std::cout << "Enter the folder name: ") && (std::cin >> inputFolderName) &&
						      storage.execute(folderExists).empty()) {
							std::cout << "Invalid Input!\n\n";
						}
						std::vector<std::string> login{ inputFolderName };
						login.resize(4);
						std::cout << "Enter the web address followed by the username followed by the password: ";
						std::cin >> login[1] >> login[2] >> login[3];
						get<1>(loginExistsCheck) = login[1].c_str();
						get<2>(loginExistsCheck) = login[2].c_str();
						if(!storage.execute(loginExistsCheck).empty()) {
							std::cout << "Login with that web address and username already exists!\n\n";
						} else {
							get<0>(loginInsert) = login[0].c_str();
							get<1>(loginInsert) = login[1].c_str();
							get<2>(loginInsert) = login[2].c_str();
							get<3>(loginInsert) = login[3].c_str();
							storage.execute(loginInsert);
						}
					}
						break;
					case 2:
						if(!storage.execute(loginsEmptyCheck).empty()) {
							std::cout << "Enter the web address followed by the username: ";
							std::string inputWebAddress;
							std::string inputUsername;
							std::cin >> inputWebAddress >> inputUsername;
							get<1>(loginExistsCheck) = inputWebAddress.c_str();
							get<2>(loginExistsCheck) = inputUsername.c_str();
							if(!storage.execute(loginExistsCheck).empty()) {
								get<0>(loginDelete) = inputWebAddress.c_str();
								get<1>(loginDelete) = inputUsername.c_str();
								storage.execute(loginDelete);
								std::cout << "Login successfully deleted!\n\n";
							} else {
								std::cout << "Login doesn't exist!\n\n";
							}
						} else {
							std::cout << "No logins have been stored in your vault!\n\n";
						}
						break;
				}
			}
				break;
			case 2: {
				int lengthOfGeneratedPassword{ 0 };
				while((std::cout << "Enter the length of the generated password: " &&
				       !(std::cin >> lengthOfGeneratedPassword)) || lengthOfGeneratedPassword <= 0) {
					std::cout << "Invalid Input!\n\n";
					clearInputBuffer();
				}
				int includeSpecialCharacters{ 0 };
				int includeNumbers{ 0 };
				int includeUppercaseLetters{ 0 };
				int includeLowercaseLetters{ 0 };
				do {
					while((std::cout << "Enter '1' to include symbols, '0' to exclude symbols: " &&
					       !(std::cin >> includeSpecialCharacters)) ||
					      (includeSpecialCharacters != 1 && includeSpecialCharacters != 0)) {
						std::cout << "Invalid Input!\n\n";
						clearInputBuffer();
					}
					while((std::cout << "Enter '1' to include numbers, '0' to exclude numbers: " &&
					       !(std::cin >> includeNumbers)) || (includeNumbers != 1 && includeNumbers != 0)) {
						std::cout << "Invalid Input!\n\n";
						clearInputBuffer();
					}
					while((std::cout << "Enter '1' to include uppercase letters, '0' to exclude uppercase letters: " &&
					       !(std::cin >> includeUppercaseLetters)) ||
					      (includeUppercaseLetters != 1 && includeUppercaseLetters != 0)) {
						std::cout << "Invalid Input!\n\n";
						clearInputBuffer();
					}
					while((std::cout << "Enter '1' to include lowercase letters, '0' to exclude lowercase letters: " &&
					       !(std::cin >> includeLowercaseLetters)) ||
					      (includeLowercaseLetters != 1 && includeLowercaseLetters != 0)) {
						std::cout << "Invalid Input!\n\n";
						clearInputBuffer();
					}
				} while(!(includeSpecialCharacters || includeNumbers || includeUppercaseLetters ||
				          includeLowercaseLetters) && (std::cout << "Invalid Inputs!\n\n"));
				std::string generatedPassword;

				for(int x{ 0 }; x < lengthOfGeneratedPassword; x++) {
					char character{ generateCharacter() };

					if((includeSpecialCharacters &&
					    std::ranges::find(specialCharacters, character) != specialCharacters.end()) ||
					   (includeNumbers && std::ranges::find(numbers, character) != numbers.end()) ||
					   (includeUppercaseLetters &&
					    std::ranges::find(uppercaseLetters, character) != uppercaseLetters.end()) ||
					   (includeLowercaseLetters &&
					    std::ranges::find(lowercaseLetters, character) != lowercaseLetters.end())) {
						generatedPassword += character;
					} else {
						--x;
					}
				}

				std::cout << "The generated password is: " << generatedPassword << '\n';
				get<0>(passwordGeneratorHistoryInsert) = generatedPassword.c_str();
				storage.execute(passwordGeneratorHistoryInsert);
			}
				break;
			case 3:
				if(!storage.execute(passwordGeneratorHistoryEmptyCheck).size()) {
					auto passwordGeneratorHistoryVec = storage.execute(passwordGeneratorHistorySelect);
					std::cout << '\n';
					for(const auto &password: passwordGeneratorHistoryVec) {
						std::cout << password << "\n\n";
					}
					std::cout << "\n\t1. Delete Password Generator History\n\t2. Back to the Main Menu\n\n";
					if(menuChoice(2) == 1) {
						storage.execute(passwordGeneratorHistoryDelete);
						std::cout << "Password generator history has been deleted!\n\n";
					}
				} else {
					std::cout << "No passwords have been generated!\n\n";
				}
				break;
			case 4: {
				auto secureNotesVec = storage.execute(secureNotesSelect);
				std::cout << "Here are the secure notes stored in your vault:\n\n";
				for(const auto &secureNote: secureNotesVec) {
					std::cout << "Folder: " << std::get<0>(secureNote) << "\nTitle: " << std::get<1>(secureNote)
					          << "\nContent: " << std::get<2>(secureNote) << "\n\n";
				}
				std::cout << '\n';
				std::cout << "\t1. Store Secure Notes\n\t2. Delete Secure Notes\n\t3. Back to the Main Menu\n\n";
				switch(menuChoice(3)) {
					case 1: {
						auto foldersVec = storage.execute(folderSelect);
						std::cout << "Here are the folders currently available:\n\n";
						for(const auto &folder: foldersVec) {
							std::cout << folder << '\n';
						}
						std::cout << '\n';
						std::string inputFolderName;
						while((std::cout << "Enter the folder name: ") && (std::cin >> inputFolderName) &&
						      storage.execute(folderSelect).empty()) {
							std::cout << "Invalid Input!\n\n";
						}
						std::vector<std::string> secureNote{ inputFolderName };
						secureNote.resize(3);
						std::cout << "Enter the title: ";
						std::getline(std::cin >> std::ws, secureNote[1]);
						std::cout << "Enter the contents: ";
						std::getline(std::cin >> std::ws, secureNote[2]);
						get<1>(secureNoteExistsCheck) = secureNote[1].c_str();
						if(!storage.execute(secureNoteExistsCheck).empty()) {
							std::cout << "Secure note with title " << secureNote[1] << "already exists!\n\n";
						} else {
							get<0>(secureNoteInsert) = secureNote[0].c_str();
							get<1>(secureNoteInsert) = secureNote[1].c_str();
							get<2>(secureNoteInsert) = secureNote[2].c_str();
							storage.execute(secureNoteInsert);
						}
					}
						break;
					case 2:
						if(!storage.execute(secureNoteEmptyCheck).empty()) {
							std::cout << "Enter the title of the secure note you want to delete: ";
							std::string inputTitle;
							std::getline(std::cin >> std::ws, inputTitle);
							if(!storage.execute(secureNoteExistsCheck).empty()) {
								get<0>(secureNoteDelete) = inputTitle.c_str();
								storage.execute(secureNoteDelete);
								std::cout << "Secure note successfully deleted!\n\n";
							} else {
								std::cout << "Secure Note Doesn't Exist!\n\n";
							}
						} else {
							std::cout << "No secure notes have been stored in your vault!\n\n";
						}
						break;
				}
			}
				break;
			case 5:
				auto foldersVec = storage.execute(folderSelect);
				std::cout << "Here are the folders currently available:\n\n";
				for(const auto &folder: foldersVec) {
					std::cout << folder << '\n';
				}
				std::cout << '\n';
				std::cout << "\t1. Add Folders\n\t2. Delete Folders\n\t3. Back to the Main Menu\n\n";
				switch(menuChoice(3)) {
					case 1: {
						std::string inputFolderName;
						std::getline(std::cin >> std::ws, inputFolderName);
						get<1>(folderExistsCheck) = inputFolderName.c_str();
						if(!storage.execute(folderExistsCheck).empty()) {
							std::cout << "Folder already exists!\n\n";
						} else {
							get<0>(folderInsert) = inputFolderName.c_str();
							storage.execute(folderInsert);
						}
					}
						break;
					case 2:
						std::string inputFolderName;
						std::getline(std::cin >> std::ws, inputFolderName);
						if(inputFolderName != "None") {
							get<1>(folderExistsCheck) = inputFolderName.c_str();
							if(!storage.execute(folderExistsCheck).empty()) {
								get<0>(folderDelete) = inputFolderName.c_str();
								storage.execute(folderDelete);
								std::cout << "Folder successfully deleted!\n\n";
							} else {
								std::cout << "Login doesn't exist!\n\n";
							}
						} else {
							std::cout << "ERROR: That folder cannot be deleted.\n\n";
						}
						break;
				}
				break;
		}
	} while(mainMenuChoice != 6);
}
