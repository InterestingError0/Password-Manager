#include "constants.hpp"
#include "miscellaneousFunctions.hpp"
#include "cryptographyFunctions.hpp"
#include "readFromFileFunctions.hpp"
#include "writeToFileFunctions.hpp"
#include "deleteFunctions.hpp"
#include "checkIfDuplicateFunctions.hpp"
#include "printFunctions.hpp"
#include <cryptopp/osrng.h>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <thread>
#include "ncurses.h"

int main() {
	initscr();
	if(!exists(masterPasswordPath) || is_empty(masterPasswordPath)) {
		std::string masterPassword;
		do {
			printw("Enter the master password that you would like to use: ");
			refresh();
			masterPassword = getMasterPasswordFromUser();
			printw("\n");
		} while(!checkIfPasswordMeetsRequirements(masterPassword));
		saveMasterPasswordToFile(hashMasterPassword(masterPassword));
	}
    CryptoPP::AutoSeededRandomPool prng;
    CryptoPP::SecByteBlock foldersIv(CryptoPP::AES::BLOCKSIZE);
	std::vector <std::string> folders;
	if(!exists(foldersPath) || is_empty(foldersPath)) {
		folders = { "Email", "Entertainment", "Games", "News/Reference", "Productivity Tools", "None" };
        prng.GenerateBlock(foldersIv, foldersIv.size());
        writeToFile(vecToStr(folders), foldersIv, foldersPath);
	} else {
        loadToVec(folders, loadFile(foldersIv, foldersPath));
	}
	while(true) {
		printw("Enter your master password: ");
		std::string masterPassword = getMasterPasswordFromUser();
		printw("\n");
		static int numberOfAttemptedLogins{ 1 };
		static int time{ 10 };
		if(checkIfEnteredMasterPasswordIsValid(masterPassword)) {
			break;
		}
		if(!(numberOfAttemptedLogins % 5)) {
			printw("You have entered an invalid master password too many times. You must wait %d seconds before trying again.\n\n", time);
			refresh();
			std::this_thread::sleep_for(std::chrono::seconds(time));
			time *= 4;
		} else {
			printw("Invalid Master Password!\n\n");
		}
		numberOfAttemptedLogins++;
	}
	refresh();
	endwin();

	CryptoPP::SecByteBlock loginsIv(CryptoPP::AES::BLOCKSIZE);
    std::vector <std::vector <std::string>> logins;

	if(!exists(loginsPath) || is_empty(loginsPath)) {
		prng.GenerateBlock(loginsIv, loginsIv.size());
	} else {
        loadToVec(logins, loadFile(loginsIv, loginsPath), 4);
    }

	CryptoPP::SecByteBlock passwordGeneratorHistoryIv(CryptoPP::AES::BLOCKSIZE);
    std::vector <std::string> passwordGeneratorHistory;

	if(!exists(passwordGeneratorHistoryPath) || is_empty(passwordGeneratorHistoryPath)) {
		prng.GenerateBlock(passwordGeneratorHistoryIv, passwordGeneratorHistoryIv.size());
	} else {
        loadToVec(logins, loadFile(passwordGeneratorHistoryIv, passwordGeneratorHistoryPath));
    }

	CryptoPP::SecByteBlock secureNotesIv(CryptoPP::AES::BLOCKSIZE);
    std::vector <std::vector <std::string>> secureNotes;

	if(!exists(secureNotesPath) || is_empty(secureNotesPath)) {
		prng.GenerateBlock(secureNotesIv, secureNotesIv.size());
	} else {
        loadToVec(logins, loadFile(secureNotesIv, secureNotesPath), 3);
    }

	int mainMenuChoice;
	do {
		std::cout << "\n\t1. Logins\n\t2. Password Generator\n\t3. Password Generator History\n\t4. Secure Notes\n\t5. Folders\n\t6. Exit\n\n";
		mainMenuChoice = menuChoice(1, 6);
		switch(mainMenuChoice) {
		case 1: {
			printAllLogins(logins);
			std::cout << "\t1. Store Logins\n\t2. Delete Logins\n\t3. Back to the Main Menu\n\n";
			switch(menuChoice(1, 3)) {
			case 1: {
				printFolders(folders);
				std::vector <std::string> login{ getFolderNameFromUser(folders) };
                login.resize(4);
				std::cout << "Enter the web address followed by the username followed by the password: ";
				std::cin >> login[1] >> login[2] >> login[3];
				if(checkIfLoginIsDuplicate(logins, login[1], login[2])) {
					std::cout << "Login with that web address and username already exists!\n\n";
				} else {
					logins.push_back(login);
					writeToFile(vecToStr(logins), loginsIv, loginsPath);
				}
			}
			break;
			case 2:
				if(!logins.empty()) {
					std::cout << "Enter the web address followed by the username: ";
					std::string inputWebAddress;
					std::string inputUsername;
					std::cin >> inputWebAddress >> inputUsername;
					if(deleteLoginIfLoginExists(logins, inputWebAddress, inputUsername)) {
						std::cout << "Login successfully deleted!\n\n";
                        writeToFile(vecToStr(logins), loginsIv, loginsPath);
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
			while((std::cout << "Enter the length of the generated password: " && !(std::cin >> lengthOfGeneratedPassword)) || lengthOfGeneratedPassword <= 0) {
				std::cout << "Invalid Input!\n\n";
				clearInputBuffer();
			}
			int includeSpecialCharacters{ 0 };
			int includeNumbers{ 0 };
			int includeUppercaseLetters{ 0 };
			int includeLowercaseLetters{ 0 };
			do {
				while((std::cout << "Enter '1' to include symbols, '0' to exclude symbols: " && !(std::cin >> includeSpecialCharacters)) || (includeSpecialCharacters != 1 && includeSpecialCharacters != 0)) {
					std::cout << "Invalid Input!\n\n";
					clearInputBuffer();
				}
				while((std::cout << "Enter '1' to include numbers, '0' to exclude numbers: " && !(std::cin >> includeNumbers)) || (includeNumbers != 1 && includeNumbers != 0)) {
					std::cout << "Invalid Input!\n\n";
					clearInputBuffer();
				}
				while((std::cout << "Enter '1' to include uppercase letters, '0' to exclude uppercase letters: " && !(std::cin >> includeUppercaseLetters)) || (includeUppercaseLetters != 1 && includeUppercaseLetters != 0)) {
					std::cout << "Invalid Input!\n\n";
					clearInputBuffer();
				}
				while((std::cout << "Enter '1' to include lowercase letters, '0' to exclude lowercase letters: " && !(std::cin >> includeLowercaseLetters)) || (includeLowercaseLetters != 1 && includeLowercaseLetters != 0)) {
					std::cout << "Invalid Input!\n\n";
					clearInputBuffer();
				}
			} while(!(includeSpecialCharacters || includeNumbers || includeUppercaseLetters || includeLowercaseLetters) && (std::cout << "Invalid Inputs!\n\n"));
			std::string generatedPassword;

			for(int x{ 0 }; x < lengthOfGeneratedPassword; x++) {
				char character{ generateCharacter() };

				if((includeSpecialCharacters && std::ranges::find(specialCharacters, character) != specialCharacters.end()) || (includeNumbers && std::ranges::find(numbers, character) != numbers.end()) || (includeUppercaseLetters && std::ranges::find(uppercaseLetters, character) != uppercaseLetters.end()) || (includeLowercaseLetters && std::ranges::find(lowercaseLetters, character) != lowercaseLetters.end())) {
					generatedPassword += character;
				} else {
					--x;
				}
			}

			std::cout << "The generated password is: " << generatedPassword << '\n';
			passwordGeneratorHistory.push_back(generatedPassword);
            writeToFile(vecToStr(passwordGeneratorHistory), passwordGeneratorHistoryIv, passwordGeneratorHistoryPath);
		}
		break;
		case 3:
			if(!passwordGeneratorHistory.empty()) {
				printPasswordGeneratorHistory(passwordGeneratorHistory);
				std::cout << "\n\t1. Delete Password Generator History\n\t2. Back to the Main Menu\n\n";
				if(menuChoice(1, 2) == 1) {
					clearPasswordGeneratorHistory(passwordGeneratorHistory);
					std::cout << "Password generator history has been deleted!\n\n";
                    writeToFile(vecToStr(passwordGeneratorHistory), passwordGeneratorHistoryIv, passwordGeneratorHistoryPath);
				}
			} else {
				std::cout << "No passwords have been generated!\n\n";
			}
			break;
		case 4: {
			printAllSecureNotes(secureNotes);
			std::cout << "\t1. Store Secure Notes\n\t2. Delete Secure Notes\n\t3. Back to the Main Menu\n\n";
			switch(menuChoice(1, 3)) {
			case 1: {
				printFolders(folders);
				std::vector <std::string> secureNote{ getFolderNameFromUser(folders) };
                secureNote.resize(3);
				std::cout << "Enter the title: ";
				std::getline(std::cin >> std::ws, secureNote[1]);
				std::cout << "Enter the contents: ";
				std::getline(std::cin >> std::ws, secureNote[2]);
				if(checkIfSecureNoteIsDuplicate(secureNotes, secureNote[1])) {
					std::cout << "Secure note with title " << secureNote[1] << "already exists!\n\n";
				} else {
					secureNotes.push_back(secureNote);
                    writeToFile(vecToStr(secureNotes), secureNotesIv, secureNotesPath);
				}
			}
			break;
			case 2:
				if(!secureNotes.empty()) {
					std::cout << "Enter the title of the secure note you want to delete: ";
					std::string inputTitle;
					std::getline(std::cin >> std::ws, inputTitle);
					if(deleteSecureNoteIfSecureNoteExists(secureNotes, inputTitle)) {
						std::cout << "Secure note successfully deleted!\n\n";
                        writeToFile(vecToStr(secureNotes), secureNotesIv, secureNotesPath);
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
			printFolders(folders);
			std::cout << "\t1. Add Folders\n\t2. Delete Folders\n\t3. Back to the Main Menu\n\n";
			switch(menuChoice(1, 3)) {
			case 1: {
				if(checkIfFolderIsDuplicate(folders, getFolderNameFromUser(folders))) {
					std::cout << "Folder already exists!\n\n";
				} else {
					folders.push_back(getFolderNameFromUser(folders));
                    writeToFile(vecToStr(folders), foldersIv, foldersPath);
                }
			}
			break;
			case 2:
				std::string inputFolderName{ getFolderNameFromUser(folders) };
				if(inputFolderName != "None") {
					if(deleteFolderIfFolderExists(folders, inputFolderName)) {
						std::cout << "Folder successfully deleted!\n\n";
                        writeToFile(vecToStr(folders), foldersIv, foldersPath);
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
