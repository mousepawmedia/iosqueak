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
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include "iosqueak/channel.hpp"
#include "iosqueak/cmd_map.hpp"

using namespace std::placeholders;
using _register = std::function<int(std::deque<std::string>&)>;

class Blueshell
{
public:
	using arguments = std::deque<std::string>;

private:
	/* Used to store the command entered. Needed here as it will
	 * be used in multiple cpp files. */
	std::string command{std::string()};

	/* A string to hold value after each keypress. Testing for
	 * inserting chars between characters in the string.*/
	std::string prev_cmd_holder;

	// Checks if prev_cmd_holder or command should be used.
	std::string check_command;

	// Used to store location of cursor when moving left/right.
	int cursor_moves{0};

	// Bools for checking for open ' or " in commands.
	bool inner_quote{false};
	bool outer_quote{false};
	bool inner_single{false};
	bool outer_single{false};

	// A container to store previously called commands.
	std::deque<std::pair<int, std::string>> previous_commands;

	// This is for previous_commands vector call.
	size_t vec_size{0};

	// Returns the key that was pressed.
	int getch(void);

	// Functions for when arrow keys are pressed.
	size_t arrow_press(std::string&);

	// Function when tab key is pressed.
	size_t tab_press(std::string&);

	// Prints command on the current line in terminal.
	void print_line(const std::string& = std::string());

	// Function when backspace is pressed.
	size_t backspace(std::string&);

	// Prints history of commands on the screen.
	int history(arguments&);

	// Function for !# history option.
	int bang(std::string&);

	// Function to insert characters typed
	void insert_char(std::string&, int);

	// Function to remove char from front of cursor.
	size_t delete_char(std::string&);

	// Just clears the screen. Is run at the beginning of Blueshell.
	int clear_screen(arguments&);

	// Displays the available commands that can be run.
	int help(arguments&);

	// Process the command when enter is pressed.
	void process_command(std::string&);

	// Process string for finding flags/options sent.
	arguments process_options(std::string&);

	// Print out description in help command
	void print_string(const std::string&);

	// Function to load up default commands.
	void registerdefaults();

	// Function to check for closing quotes
	void check_quote(std::string&);

	/* Function to break string into tokens
	 * that will be used in various functions.*/
	std::deque<std::string> tokens(std::string&);

	// Function to add command to stored_commands container.
	void add_command(std::string&);

	// Function to list all registered commands.
	int list_commands(arguments&);

public:
	/* A map that has the stored commands that are available during
	 * the running of Blueshell. Use the 'register' function to
	 * add new commands to the map. */
	Cmd_map stored_commands;

	// Name of the shell. Defaults to Blueshell.
	std::string shell_name;

	Blueshell(std::string sent_name = "Blueshell");
	virtual ~Blueshell();

	// Just starts the shell to make it interactive.
	void initial_shell();

	/*This is for registering commands. All commands MUST already meet the
	 *  function signature before being able to be used. The signature is:
	 *  int <function name>(arguments&)*/
	int register_command(const std::string&,
						 _register,
						 const std::string& short_desc = std::string(),
						 const std::string& long_desc = std::string(),
						 size_t number_of_options = 0,
						 size_t num_of_args = 0);

	// Empty container for passing to functions.
	arguments empty_container;

protected:
};

#endif  // BLUESHELL_H
