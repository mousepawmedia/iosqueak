#include "../include/iosqueak/testregister.hpp"

TestRegister::TestRegister()
{
	std::cout << "\nTestRegister called\n";
	_register test_attempt{std::bind(&TestRegister::test_attempt, this)};
	stored_commands["test_attempt"] =
		{"testregister test_attempt to test derived class function calls",
		 test_attempt};
}

TestRegister::~TestRegister()
{
	// dtor
}
