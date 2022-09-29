Blueshell
#############

This is the basic shell for use with IOSqueak related projects.
This will give the following features:

* Give the shell a custom name if desired. Defaults to "Blueshell".

* Stores previously typed commands that can be recalled via the
  up and down arrow keys.
  
* Recall a list of used commands using 'history' command to retrieve a
  list of commands with a number beside it to use with ! option.
  
* Use !<#> to recall the desired command (eg. !2 recalls the second
  command in the list).
  
* Double press of tab to recall all available commands, or start typing 
  a command and use double tab key press to auto-fill command.
  
* Register custom commands that can be used in any shell that 
  inherits Blueshell, as long as it follows the right signature.
  
* Running 'help' will display the list of available commands with
  a description of what it does. Any commands registered and stored
  in the 'stored_commands' container by the user will be listed here
  along with the description provided.
  
Including Blueshell
#######################

To include Blueshell in the code, include the following:
..  code-block:: c++
    #include "iosqueak/blueshell.hpp"
    
To instantiate Blueshell
####################

After including blueshell.hpp, create an object via:
..  code-block:: c++
    Blueshell <variable name>{"<name of shell>"};
    eg Blueshell game_shell{"Cool Game"};
Will display:
"Welcome to Cool Game shell" upon starting shell.

Registering and store a command to use
################################

To register a command:
..  code-block:: c++
    _register <command name>{std::bind(&<ClassName>::<command function>, this)};
	eg _register test_attempt{std::bind(&TestRegister::test_attempt, this)};
	
To store it in the list of available commands:
..  code-block:: c++
    stored_commands["<command name>"] = {"<Description of command for help section display>", <command name>};
	eg stored_commands["test_attempt"] = {"testregister test_attempt to test derived class function calls", test_attempt};
	
When you type 'help' in the shell, it will display like this:
test_attempt        testregister test_attempt to test derived class function calls

