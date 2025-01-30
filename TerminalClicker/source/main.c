/*
	3ds Terminal Clicker by PyJulian
	https://github.com/pyjulian/
	https://wortelacrhief.netlify.app/

	Pls do not Copy without consent!!

	And I know this code is shit ass garbage I've just started with C.
*/

#include <string.h>
#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>

// Write the savefile
void writeSaveToFile(const char *filename, const char *savefiledata) {
    FILE *file = fopen(filename, "w");
    if (file != NULL) {
        fprintf(file, "%s", savefiledata);
        fclose(file);
        //printf("Successfully wrote save data to %s\n", filename);
    } else {
        //printf("Failed to open %s for writing\n", filename);
    }
}

// Read the savefile
void readSaveFromFile(const char *filename, int *sceen, int *duper, int *index, int *sub, int *rob, int *gen, int *fraud, long long *cash) {
    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        char buffer[1000];
        fgets(buffer, sizeof(buffer), file);
        fclose(file);
        printf("Read from file: '%s'\n", buffer);
        int result = sscanf(buffer, "%d|%d|%d|%d|%d|%d|%lld", duper, index, sub, rob, gen, fraud, cash);
        if (result != 7) {
            //printf("Error: Failed to read all values correctly. Only %d values were read.\n", result);
        } else {
            //printf("Successfully parsed all values!\n");
        }
        //printf("duper=%d, index=%d, sub=%d, rob=%d, gen=%d, fraud=%d, cash=%lld\n", *duper, *index, *sub, *rob, *gen, *fraud, *cash);
    } else {
        //printf("Failed to open %s for reading\n", filename);
    }
}

void print_option(int sceen, int select, int option_number, int count, int price, const char *label) {
    char buffer[100];
    int text_len;

    if (count < 999) snprintf(buffer, sizeof(buffer), "%s: %d ($%d) (A to Buy)", label, count, price);
    else snprintf(buffer, sizeof(buffer), "%s: %d ($%d) (MAX)", label, count, price);
    text_len = strlen(buffer);

    if (sceen == 1 && select == option_number) {
        printf("\x1b[47;30m %s%*s\x1b[0m", buffer, 49 - text_len, "");
    } else {
        printf(" %s: %d\n", label, count);
    }
}

int main(int argc, char **argv)
{
	bool code = false; // Check if code is entered, false will ignore the check, true will run the check and make it false after
	long long cash = 0LL; // Player Balance
	int multi = 1; // Click Multiplier
	int select = 0; // Menu Selection / Upgrade Selection
	int sceen = 0, duper = 0, index = 0, sub = 0, rob = 0, gen = 0, fraud = 0; // Upgrades :3
	int tut = 0; // What part of the tutorial the user is on
	float progress = 0; // Progress percentage
	bool sav = 0; // Save progress
	bool dev = false;

	size_t save_size = sizeof(long long) + (6 * sizeof(int)); // Savefile data size
	char *savefiledata = (char *)malloc(1000 * save_size); // Savefile data string

	gfxInitDefault();
	fsInit();

	PrintConsole topScreen, bottomScreen;



	consoleInit(GFX_TOP, &topScreen);
	consoleInit(GFX_BOTTOM, &bottomScreen);

	// Main loop
	while (aptMainLoop())
	{	
		static SwkbdState swkbd;
		static char mybuf[60];
		SwkbdButton button = SWKBD_BUTTON_NONE;

		multi = 1 + (duper + index + sub + rob + gen + fraud);
		progress = (duper + index + sub + rob + gen + fraud) * 0.01668335001;

		sprintf(savefiledata, "%d|%d|%d|%d|%d|%d|%lld", duper, index, sub, rob, gen, fraud, cash);

		consoleSelect(&topScreen);
		printf("\033[2J\033[1;1H"); // Clear Screen
		printf("\x1b[47;30m3ds Terminal Clicker                              \x1b[0m\n"); // Top Bar
		printf(" Money: $%lld\n", cash);
		printf(" Game Progress: %f%%\n", progress);
		printf("\n\x1b[47;30mUpgrades                                          \x1b[0m\n"); // Upgrade Bar

    	print_option(sceen, select, 1, duper, 15, "Money Dupers");
    	print_option(sceen, select, 2, index, 500, "Index Funds");
    	print_option(sceen, select, 3, sub, 5000, "Fake Subscriptions Active");
    	print_option(sceen, select, 4, rob, 100000, "Robbery Bonus");
    	print_option(sceen, select, 5, gen, 1000000, "Coin Generator");
    	print_option(sceen, select, 6, fraud, 10000000, "Fraud Skill");

		if (sceen == 1) printf("\n Push Y to switch back to the menu\n");
		else printf("\n NOTE: All upgrades give a extra $ per click\n");

		if (dev == true) {
			printf("\n\x1b[47;30mDeveloper Tools                                  \x1b[0m\n\n"); // Top Bar
			printf(" Target SaveFileData: '%s'\n", savefiledata);
			printf(" Target SaveFilePath: 'sdmc:/termiclicker.dat'\n Push (B) to free savefile Memory");
		}



		consoleSelect(&bottomScreen);
		printf("\033[2J\033[1;1H"); // Clear Screen
		printf("\x1b[47;30mSystem Messages                         \x1b[0m\n"); // Bottom Bar

		if (progress >= 100) printf(" You finished the game.\n So now what?\n Yeah I don't know, email me this\n screen I guess for cool reasons:\n wortelarchief@gmail.com\n Also tell me why you took the time\n and if you used cheatcodes :3.\n Goodnight.\n\n\n\n");
		else if (tut == 0) {
			printf(" Message from the system:\n\n Welcome to Terminal Clicker\n Terminal Clicker is a remaster\n of Hacker Clicker which is unreleased.\n Push A to get cash.\n\n (Push X to skip the tutorial)\n\n\n\n");
			if (cash > 14) tut = 1;
		}
		else if (tut == 1) {
			printf(" Message from the system:\n\n To select a option, Push Y to go to\n the selection bar\n Go to 'Purchase Upgrade(s) and buy a\n duper for $15.\n\n (Push X to skip the tutorial)\n\n\n\n");
			if (duper > 0) tut = 2;
		}
		else if (tut == 2) {
			printf(" Message from the system:\n\n This will increase your click power.\n Click until you can buy a Index Fund\n You can still open the menu with Y.\n\n (Push X to skip the tutorial)\n\n\n\n\n");
			if (index > 0) tut = 3;
		}
		else if (tut == 3) {
			printf(" Message from the system:\n\n Now buy a Subscription Scam.\n Click until you can buy it.\n\n (Push X to skip the tutorial)\n\n\n\n\n\n");
			if (sub > 0) tut = 4;
		}
		else if (tut == 4) {
			printf(" Message from the system:\n\n Now buy a Rob Bonus.\n Click until you can buy it.\n\n (Push X to skip the tutorial)\n\n\n\n\n\n");
			if (sub > 0) tut = 5;
		}
		else if (tut == 5) printf(" Message from the system:\n\n You have to buy every upgrade to win.\n You can see your game progress under your Balance.\n\n (Push X to close the tutorial)\n\n\n\n\n\n");
		else printf("\n\n\n\n\n\n\n\n\n\n\n");

		printf("\x1b[47;30mTerminal Options (Y)                    \x1b[0m\n");

		if (sceen == 0 && select == 1) printf("\x1b[47;30m Purchase Upgrade(s)                    \x1b[0m");
		else printf(" Purchase Upgrade(s)\n");
		if (sceen == 0 && select == 2) printf("\x1b[47;30m Cheatcodes                             \x1b[0m");
		else printf(" Cheatcodes\n");
		if (sceen == 0 && select == 3) printf("\x1b[47;30m Exit Menu                              \x1b[0m");
		else printf(" Exit Menu\n");
		if (sceen == 0 && select == 4 && sav == true) printf("\x1b[47;30m Save Progress (Saved)                  \x1b[0m");
		else if (sceen == 0 && select == 4) printf("\x1b[47;30m Save Progress                          \x1b[0m");
		else printf(" Save Progress\n");
		if (sceen == 0 && select == 5 && sav == true) printf("\x1b[47;30m Load Progress (Loaded)                 \x1b[0m");
		else if (sceen == 0 && select == 5) printf("\x1b[47;30m Load Progress                          \x1b[0m");
		else printf(" Load Progress\n");


		if (sceen == 0 && select == 6) printf("\x1b[47;30m Quit Game (START)                      \x1b[0m");
		else printf(" Quit Game (START)\n");

		printf("\n\x1b[47;30mAbout                                   \x1b[0m\n Created by PyJulian on Github\n https://github.com/pyjulian");

		if (code == true) {
			if (strcmp(mybuf, "2008") == 0) cash = 99999999999999LL;
			else if (strcmp(mybuf, "2007") == 0) cash = 0;
			else if (strcmp(mybuf, "12") == 0) duper = 990;
			else if (strcmp(mybuf, "122") == 0) index = 990;
			else if (strcmp(mybuf, "1222") == 0) sub = 990;
			else if (strcmp(mybuf, "12222") == 0) rob = 990;
			else if (strcmp(mybuf, "122222") == 0) gen = 990;
			else if (strcmp(mybuf, "1222222") == 0) fraud = 990;
			else if (strcmp(mybuf, "130") == 0) progress = 100;
			else if (strcmp(mybuf, "83") == 0) dev = true;
			code = false;
		} // Cheatcode System, you can find all of them easely if you know how to read xd

		hidScanInput();

		u32 kDown = hidKeysDown();

		if (kDown & KEY_B && dev == true) free(savefiledata);

		if (kDown & KEY_START) {
			if (sceen == 0 && select == 6) break;
			sceen = 0;
			select = 6;
			//break;
		}
		else if (kDown & KEY_A) {
			if (select == 0) cash = cash + multi;

			else if (select == 1) {
				if (sceen == 0) {
					select = 1;
					sceen = 1;
				}
				else if (cash > 14 && duper < 999) {
					duper = duper + 1;
					cash = cash - 15;
				}
			}
			else if (select == 2) {
				if (sceen == 0) {
					swkbdInit(&swkbd, SWKBD_TYPE_NUMPAD, 1, 8);
					swkbdSetPasswordMode(&swkbd, SWKBD_PASSWORD_HIDE_DELAY);
					swkbdSetValidation(&swkbd, SWKBD_ANYTHING, 0, 0);
					swkbdSetFeatures(&swkbd, SWKBD_FIXED_WIDTH);
					swkbdSetNumpadKeys(&swkbd, L'ツ', L'益');
					button = swkbdInputText(&swkbd, mybuf, sizeof(mybuf));
					code = true;
				}
				else if (cash > 99 && index < 999) {
					index = index + 1;
					cash = cash - 100;
				}
			}
			else if (select == 3) {
				if (sceen == 0) select = 0;
				else if (cash > 4999 && sub < 999) {
					sub = sub + 1;
					cash = cash - 5000;
				}
			}
			else if (select == 4) {
				if (sceen == 0) {
					writeSaveToFile("sdmc:/termiclicker.dat", savefiledata);
					free(savefiledata);
					savefiledata = (char *)malloc(1000 * save_size);
					sav = true;
				}
				else if (cash > 99999 && rob < 999) {
					rob = rob + 1;
					cash = cash - 100000;
				}
			}
			else if (select == 5) {
				if (sceen == 0) {
					readSaveFromFile("sdmc:/termiclicker.dat", &sceen, &duper, &index, &sub, &rob, &gen, &fraud, &cash);
					free(savefiledata);
					savefiledata = (char *)malloc(1000 * save_size);
					sav = true;
				}
				else if (cash > 999999 && gen < 999) {
					gen = gen + 1;
					cash = cash - 1000000;
				}
			}
			else if (select == 6) {
				if (sceen == 0) break;
				else if (cash > 9999999 && fraud < 999) {
					fraud = fraud + 1;
					cash = cash - 10000000;
				}
			}
			
		}
		else if (kDown & KEY_Y) {
			if (select == 0) select = 1;
			else {
				select = 1;
				sceen = 0;
			}
		}
		else if (kDown & KEY_X) tut = 99999;
		else if (kDown & KEY_UP) {
			sav = false;
			if (select > 1) select = select - 1;
		}
		else if (kDown & KEY_DOWN) {
			sav = false;
			if (sceen == 0) {if (select > 0 && select < 6) select = select + 1;}
			else {if (select > 0 && select < 6) select = select + 1;}
		}

		gfxFlushBuffers();
		gfxSwapBuffers();
		gspWaitForVBlank();
	}

	free(savefiledata);
	gfxExit();
	return 0;
}
