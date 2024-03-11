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

	int mainMenuChoice;
	do {
		std::cout
				<< "\n\t1. Logins\n\t2. Password Generator\n\t3. Password Generator History\n\t4. Secure Notes\n\t5. Folders\n\t6. Exit\n\n";
		mainMenuChoice = menuChoice(1, 6);
		switch(mainMenuChoice) {
			case 1: {
				auto loginsVec = storage.select(
						columns(&logins::folder, &logins::website, &logins::username, &logins::password));
				std::cout << "Here are the logins stored in your vault:\n\n";
				for(const auto &login: loginsVec) {
					std::cout << "Folder: " << std::get<0>(login) << "\nWeb Address: " << std::get<1>(login)
					          << "\nUsername: " << std::get<2>(login) << "\nPassword: " << std::get<3>(login) << "\n\n";
				}
				std::cout << "\t1. Store Logins\n\t2. Delete Logins\n\t3. Back to the Main Menu\n\n";
				switch(menuChoice(1, 3)) {
					case 1: {
						auto foldersVec = storage.select(columns(&folders::folder));
						std::cout << "Here are the folders currently available:\n\n";
						for(const auto &folder: foldersVec) {
							std::cout << std::get<0>(folder) << '\n';
						}
						std::cout << '\n';
						std::string inputFolderName;
						while(true) {
							inputFolderName = getFolderNameFromUser();
							if(storage.select(1, from<folders>(),
							                  where(is_equal(&folders::folder, inputFolderName))).empty()) {
								std::cout << "Invalid Input!\n\n";
							} else {
								break;
							}
						}
						std::vector<std::string> login{ inputFolderName };
						login.resize(4);
						std::cout << "Enter the web address followed by the username followed by the password: ";
						std::cin >> login[1] >> login[2] >> login[3];
						if(!storage.select(1, from<logins>(), where(is_equal(&logins::website, login[1]) and
						                                            is_equal(&logins::username, login[2]))).empty()) {
							std::cout << "Login with that web address and username already exists!\n\n";
						} else {
							storage.insert(into<logins>(),
							               columns(&folders::folder, &logins::website, &logins::username,
							                       &logins::password),
							               values(std::make_tuple(login[0], login[1], login[2], login[3])));
						}
					}
						break;
					case 2:
						if(!storage.select(1, from<logins>()).empty()) {
							std::cout << "Enter the web address followed by the username: ";
							std::string inputWebAddress;
							std::string inputUsername;
							std::cin >> inputWebAddress >> inputUsername;
							if(!storage.select(1, from<folders>(), where(is_equal(&logins::website, inputWebAddress) and
							                                             is_equal(&logins::username,
							                                                      inputUsername))).empty()) {
								storage.remove_all<folders>(where(is_equal(&logins::website, inputWebAddress) and
								                                  is_equal(&logins::username, inputUsername)));
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
				storage.insert(into<passwordGeneratorHistory>(), columns(&passwordGeneratorHistory::generatedPassword),
				               values(std::make_tuple(generatedPassword)));
			}
				break;
			case 3:
				if(!storage.select(1, from<passwordGeneratorHistory>()).empty()) {
					auto passwordGeneratorHistoryVec = storage.select(
							columns(&passwordGeneratorHistory::generatedPassword));
					std::cout << '\n';
					for(const auto &password: passwordGeneratorHistoryVec) {
						std::cout << std::get<0>(password) << '\n';
					}
					std::cout << "\n\t1. Delete Password Generator History\n\t2. Back to the Main Menu\n\n";
					if(menuChoice(1, 2) == 1) {
						storage.remove_all<passwordGeneratorHistory>();
						std::cout << "Password generator history has been deleted!\n\n";
					}
				} else {
					std::cout << "No passwords have been generated!\n\n";
				}
				break;
			case 4: {
				auto secureNotesVec = storage.select(
						columns(&secureNotes::folder, &secureNotes::title, &secureNotes::contents));
				std::cout << "Here are the secure notes stored in your vault:\n\n";
				for(const auto &secureNote: secureNotesVec) {
					std::cout << "Folder: " << std::get<0>(secureNote) << "\nTitle: " << std::get<1>(secureNote)
					          << "\nContent: " << std::get<2>(secureNote) << "\n\n";
				}
				std::cout << '\n';
				std::cout << "\t1. Store Secure Notes\n\t2. Delete Secure Notes\n\t3. Back to the Main Menu\n\n";
				switch(menuChoice(1, 3)) {
					case 1: {
						auto foldersVec = storage.select(columns(&folders::folder));
						std::cout << "Here are the folders currently available:\n\n";
						for(const auto &folder: foldersVec) {
							std::cout << std::get<0>(folder) << '\n';
						}
						std::cout << '\n';
						std::string inputFolderName;
						while(true) {
							inputFolderName = getFolderNameFromUser();
							if(storage.select(1, from<folders>(),
							                  where(is_equal(&folders::folder, inputFolderName))).empty()) {
								std::cout << "Invalid Input!\n\n";
							} else {
								break;
							}
						}
						std::vector<std::string> secureNote{ inputFolderName };
						secureNote.resize(3);
						std::cout << "Enter the title: ";
						std::getline(std::cin >> std::ws, secureNote[1]);
						std::cout << "Enter the contents: ";
						std::getline(std::cin >> std::ws, secureNote[2]);
						if(!storage.select(1, from<secureNotes>(),
						                   where(is_equal(&secureNotes::title, secureNote[1]))).empty()) {
							std::cout << "Secure note with title " << secureNote[1] << "already exists!\n\n";
						} else {
							storage.insert(into<secureNotes>(),
							               columns(&secureNotes::folder, &secureNotes::title, &secureNotes::contents),
							               values(std::make_tuple(secureNote[0], secureNote[1], secureNote[2])));
						}
					}
						break;
					case 2:
						if(!storage.select(1, from<folders>()).empty()) {
							std::cout << "Enter the title of the secure note you want to delete: ";
							std::string inputTitle;
							std::getline(std::cin >> std::ws, inputTitle);
							if(!storage.select(1, from<folders>(),
							                   where(is_equal(&secureNotes::title, inputTitle))).empty()) {
								storage.remove_all<folders>(where(is_equal(&secureNotes::title, inputTitle)));
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
				auto foldersVec = storage.select(columns(&folders::folder));
				std::cout << "Here are the folders currently available:\n\n";
				for(const auto &folder: foldersVec) {
					std::cout << std::get<0>(folder) << '\n';
				}
				std::cout << '\n';
				std::cout << "\t1. Add Folders\n\t2. Delete Folders\n\t3. Back to the Main Menu\n\n";
				switch(menuChoice(1, 3)) {
					case 1: {
						std::string inputFolderName{ getFolderNameFromUser() };
						if(!storage.select(1, from<folders>(),
						                   where(is_equal(&folders::folder, inputFolderName))).empty()) {
							std::cout << "Folder already exists!\n\n";
						} else {
							storage.insert(into<folders>(), columns(&folders::folder),
							               values(std::make_tuple(inputFolderName)));
						}
					}
						break;
					case 2:
						std::string inputFolderName{ getFolderNameFromUser() };
						if(inputFolderName != "None") {
							if(!storage.select(1, from<folders>(),
							                   where(is_equal(&folders::folder, inputFolderName))).empty()) {
								storage.remove_all<folders>(where(is_equal(&folders::folder, inputFolderName)));
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
