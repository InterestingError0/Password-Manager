#include "constants.hpp"
#include "miscellaneousFunctions.hpp"
#include "cryptographyFunctions.hpp"
#include <thread>
#include "ncurses.h"

std::string loginScreen() {
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

	std::string masterPassword;

	while(true) {
		printw("Enter your master password: ");
		masterPassword = getMasterPasswordFromUser();
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

	return masterPassword;
}