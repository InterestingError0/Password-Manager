#include "Functions.hpp"
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <filesystem>
#include <thread>
#include <chrono>

namespace fs = std::filesystem;

int main() {
	const fs::path masterPasswordPath{ "masterPassword.txt" };
	if(!fs::exists(masterPasswordPath)) {
		std::string masterPassword;
		do {
			std::cout << "Enter the master password that you would like to use: ";
			std::cin >> masterPassword;
			std::cout << '\n';
		} while(!checkIfPasswordMeetsRequirements(masterPassword));
		saveMasterPasswordToFile(hash(masterPassword), masterPasswordPath);
	}
	const fs::path foldersPath{ "folders.txt" };
	std::vector <std::string> folders;
	if(!fs::exists(foldersPath)) {
		folders = { "Email", "Entertainment", "Games", "News/Reference", "Productivity Tools", "None" };
		writeFoldersToFile(folders, foldersPath);
	} else {
		readFoldersIntoVector(folders, foldersPath);
	}
	std::string masterPassword;
	while((std::cout << "Enter your master password: ") && (std::cin >> masterPassword)) {
		static int numberOfAttemptedLogins{ 1 };
		static int time{ 10 };
		if(checkIfEnteredMasterPasswordIsValid(masterPassword, masterPasswordPath)) {
			break;
		} else if(!(numberOfAttemptedLogins % 5)) {
			std::cout << "You have entered an invalid master password too many times. You must wait " << time << " seconds before trying again.\n\n";
			std::this_thread::sleep_for(std::chrono::seconds(time));
			time *= 4;
			numberOfAttemptedLogins++;
		} else {
			std::cout << "Invalid Master Password!\n\n";
			numberOfAttemptedLogins++;
		}
	}
	const fs::path loginsPath{ "logins.txt" };
	std::vector <std::array <std::string, 4>> logins;
	readLoginsIntoVector(logins, loginsPath);

	const fs::path passwordGeneratorHistoryPath{ "passwordGeneratorHistory.txt" };
	std::vector <std::string> passwordGeneratorHistory;
	readPasswordGeneratorHistoryIntoVector(passwordGeneratorHistory, passwordGeneratorHistoryPath);

	const fs::path secureNotesPath{ "secureNotes.txt" };
	std::vector <std::array <std::string, 3>> secureNotes;
	readSecureNotesIntoVector(secureNotes, secureNotesPath);

	int mainMenuChoice;
	do {
		std::cout << "\n\t1. Logins\n\t2. Password Generator\n\t3. Password Generator History\n\t4. Secure Notes\n\t5. Folders\n\t6. Exit\n\n";
		mainMenuChoice = menuChoice(1, 6);
		switch(mainMenuChoice) {
		case 1:
		{
			printAllLogins(logins);
			std::cout << "\t1. Store Logins\n\t2. Delete Logins\n\t3. Back to the Main Menu\n\n";
			switch(menuChoice(1, 3)) {
			case 1:
			{
				printFolders(folders);
				std::array <std::string, 4> login{ getFolderNameFromUser(folders) };
				std::cout << "Enter the web address followed by the username followed by the password: ";
				std::cin >> login[1] >> login[2] >> login[3];
				if(checkIfLoginIsDuplicate(logins, login[1], login[2])) {
					std::cout << "Login with that web address and username already exists!\n\n";
				} else {
					logins.push_back(login);
					writeLoginsToFile(logins, loginsPath);
				}
			}
			break;
			case 2:
				if(logins.size()) {
					bool loginExists{ false };
					do {
						std::cout << "Enter the web address followed by the username: ";
						std::string inputWebAddress{ "" };
						std::string inputUsername{ "" };
						std::cin >> inputWebAddress >> inputUsername;
						if(deleteLoginIfLoginExists(logins, inputWebAddress, inputUsername)) {
							std::cout << "Login successfully deleted!\n\n";
							writeLoginsToFile(logins, loginsPath);
							loginExists = true;
						} else {
							std::cout << "Login doesn't exist!\n\n";
						}
					} while(!loginExists);
				} else {
					std::cout << "No logins have been stored in your vault!\n\n";
				}
				break;
			}
		}
		break;
		case 2:
		{
			int lengthOfGeneratedPassword{ 0 };
			while((std::cout << "Enter the length of the generated password: ") && !(std::cin >> lengthOfGeneratedPassword) || lengthOfGeneratedPassword <= 0) {
				std::cout << "Invalid Input!\n\n";
				clearInputBuffer();
			}
			int includeSmybols{ 0 };
			int includeNumbers{ 0 };
			int includeUppercaseLetters{ 0 };
			int includeLowercaseLetters{ 0 };
			do {
				while((std::cout << "Enter '1' to include symbols, '0' to exclude symbols: ") && !(std::cin >> includeSmybols) || (includeSmybols != 1 && includeSmybols != 0)) {
					std::cout << "Invalid Input!\n\n";
					clearInputBuffer();
				}
				while((std::cout << "Enter '1' to include numbers, '0' to exclude numbers: ") && !(std::cin >> includeNumbers) || (includeNumbers != 1 && includeNumbers != 0)) {
					std::cout << "Invalid Input!\n\n";
					clearInputBuffer();
				}
				while((std::cout << "Enter '1' to include uppercase letters, '0' to exclude uppercase letters: ") && !(std::cin >> includeUppercaseLetters) || (includeUppercaseLetters != 1 && includeUppercaseLetters != 0)) {
					std::cout << "Invalid Input!\n\n";
					clearInputBuffer();
				}
				while((std::cout << "Enter '1' to include lowercase letters, '0' to exclude lowercase letters: ") && !(std::cin >> includeLowercaseLetters) || (includeLowercaseLetters != 1 && includeLowercaseLetters != 0)) {
					std::cout << "Invalid Input!\n\n";
					clearInputBuffer();
				}
			} while(!(includeSmybols || includeNumbers || includeUppercaseLetters || includeLowercaseLetters) && (std::wcout << "Invalid Inputs!\n\n"));
			std::string generatedPassword{ "" };
			for(int x{ 0 }; x < lengthOfGeneratedPassword; x++) {
				char character{ generateCharacter() };
				if(includeSmybols && (character >= 33 && character <= 47 || character >= 58 && character <= 64 || character >= 91 && character <= 96 || character >= 123 && character <= 126)) {
					generatedPassword += character;
				} else if(includeNumbers && (character >= 48 && character <= 57)) {
					generatedPassword += character;
				} else if(includeUppercaseLetters && (character >= 65 && character <= 90)) {
					generatedPassword += character;
				} else if(includeLowercaseLetters && (character >= 97 && character <= 122)) {
					generatedPassword += character;
				} else {
					--x;
				}
			}
			std::cout << "The generated password is: " << generatedPassword << '\n';
			passwordGeneratorHistory.push_back(generatedPassword);
			writePasswordGeneratorHistoryToFile(passwordGeneratorHistory, passwordGeneratorHistoryPath);
		}
		break;
		case 3:
			if(passwordGeneratorHistory.size()) {
				printPasswordGeneratorHistory(passwordGeneratorHistory);
				std::cout << "\n\t1. Delete Password Generator History\n\t2. Back to the Main Menu\n\n";
				if(menuChoice(1, 2) == 1) {
					deletePasswordGeneratorHistory(passwordGeneratorHistory);
					std::cout << "Password generator history has been deleted!\n\n";
					writePasswordGeneratorHistoryToFile(passwordGeneratorHistory, passwordGeneratorHistoryPath);
				}
			} else {
				std::cout << "No passwords have been generated!\n\n";
			}
			break;
		case 4:
		{
			printAllSecureNotes(secureNotes);
			std::cout << "\t1. Store Secure Notes\n\t2. Delete Secure Notes\n\t3. Back to the Main Menu\n\n";
			switch(menuChoice(1, 3)) {
			case 1:
			{
				printFolders(folders);
				std::array <std::string, 3> secureNote{ getFolderNameFromUser(folders) };
				std::cout << "Enter the title: ";
				std::getline(std::cin >> std::ws, secureNote[1]);
				std::cout << "Enter the contents: ";
				std::getline(std::cin >> std::ws, secureNote[2]);
				if(checkIfSecureNoteIsDuplicate(secureNotes, secureNote[1])) {
					std::cout << "Secure note with title " << secureNote[1] << "already exists!\n\n";
				} else {
					secureNotes.push_back(secureNote);
					writeSecureNotesToFile(secureNotes, secureNotesPath);
				}
			}
			break;
			case 2:
			{
				if(secureNotes.size()) {
					bool secureNoteExists{ false };
					do {
						std::cout << "Enter the title of the secure note you want to delete: ";
						std::string inputTitle{ "" };
						std::getline(std::cin >> std::ws, inputTitle);
						if(deleteSecureNoteIfSecureNoteExists(secureNotes, inputTitle)) {
							std::cout << "Secure note successfully deleted!\n\n";
							writeSecureNotesToFile(secureNotes, secureNotesPath);
							secureNoteExists = true;
						} else {
							std::cout << "Secure Note Doesn't Exist!\n\n";
						}
					} while(!secureNoteExists);
				} else {
					std::cout << "No secure notes have been stored in your vault!\n\n";
				}
			}
			break;
			}
		}
		break;
		case 5:
			printFolders(folders);
			std::cout << "\t1. Add Folders\n\t2. Delete Folders\n\t3. Back to the Main Menu\n\n";
			switch(menuChoice(1, 3)) {
			case 1:
			{
				if(checkIfFolderIsDuplicate(folders, getFolderNameFromUser(folders))) {
					std::cout << "Folder already exists!\n\n";
				} else {
					folders.push_back(getFolderNameFromUser(folders));
					writeFoldersToFile(folders, foldersPath);
				}
			}
			break;
			case 2:
				std::string inputFolderName{ getFolderNameFromUser(folders) };
				if(inputFolderName != "None") {
					deleteFolder(folders, inputFolderName);
					std::cout << "Folder successfully deleted!\n\n";
					writeFoldersToFile(folders, foldersPath);
				} else {
					std::cout << "ERROR: Cannot delete that folder! Access Denied!\n\n";
				}
				break;
			}
			break;
		}
	} while(mainMenuChoice != 6);

	return 0;
}
