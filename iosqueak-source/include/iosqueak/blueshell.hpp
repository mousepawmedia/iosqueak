/** Blueshell [IOSqueak]
 *
 * Author(s): Jack Tolmie, Jason C. McDonald
 */

/* LICENSE (BSD-3-Clause)
 * Copyright (c) 2016-2022 MousePaw Media.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 * CONTRIBUTING
 * See https://www.mousepawmedia.com/developers for information
 * on how to contribute to our projects.
 */

#ifndef BLUESHELL_H
#define BLUESHELL_H

#include <algorithm>
#include <deque>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include "iosqueak/channel.hpp"
// #include "iosqueak/stringify.hpp"

using std::cin;
using namespace std::placeholders;
using _register = std::function<int(std::string)>;
using cmd_map = std::map<std::string, std::pair<std::string, _register>>;

class Blueshell
{
	/* Integer to set if return was pressed after
	 *  using up/down/tab key press. */
	size_t check_return;

public:
	using reg_command = int (Blueshell::*)(std::string);
	/*A map that has the stored commands that are available during
	 * the running of Blueshell. Use the 'register' function to
	 * add new commands to the map. */
	cmd_map stored_commands;

	/* A vector to store previously called commands. */
	//	std::vector<std::string> previous_commands;
	//	std::deque<std::string> previous_commands;
	std::deque<std::pair<int, std::string>> previous_commands;

	Blueshell();
	virtual ~Blueshell();

	// Just clears the screen. Is run at the beginning of Blueshell.
	int clear_screen(std::string blank = "");

	// Displays the available commands that can be run.
	int help(std::string blank = "");

	// Just starts the shell to make it interactive.
	void initial_shell();

	/*This is for registering commands. All commands MUST already meet the
	 *  function signature before being able to be used. The signature is:
	 *  std::string <function name>(std::string)
	 * Call using something like:
	 * register_command("TestName", &<function>, "Optional description"); */
	int register_command(std::string, reg_command, std::string blank = "");
	void list_available_commands();

	int getch(void);

	// Functions for when arrow keys are pressed.
	void arrow_press(std::string&);

	// Function when tab key is pressed.
	void tab_press(std::string&);

	// Prints command on the current line in terminal.
	void print_line(const std::string& = std::string());

	// Function when backspace is pressed.
	void backspace(std::string&);

	// Prints history of commands on the screen.
	int history(std::string blank = std::string());

	// Function for !# history option.
	void bang(std::string&);

	// Delete when done.
	int test(std::string blank = std::string());

protected:
};

#endif  // BLUESHELL_H
