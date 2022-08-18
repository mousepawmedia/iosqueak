#ifndef TESTREGISTER_HPP
#define TESTREGISTER_HPP

#include "../../iosqueak-source/include/iosqueak/blueshell.hpp"

class TestRegister : public Blueshell
{
public:
	TestRegister();
	virtual ~TestRegister();
	int test_attempt()
	{
		std::cout << "\nRunning testregister test_attempt.\n";
		return 0;
	}

protected:
private:
};

#endif  // TESTREGISTER_HPP
