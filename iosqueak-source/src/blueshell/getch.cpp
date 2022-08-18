#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include "../include/iosqueak/blueshell.hpp"

/* I found this online. I cannot remember where
 *  so I cannot give credit to whom it belongs */

int Blueshell::getch(void)
{
	int ch;
	struct termios oldt, newt;

	tcgetattr(STDIN_FILENO, &oldt);

	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);

	tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	ch = getchar();

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

	return ch;
}
