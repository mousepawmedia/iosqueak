#include "test_stringify_numbers.hpp"

void TestSuite_StringifyNumbers::load_tests()
{
	register_test("I-tB1301", new Test_LengthifyIntegral());
}
