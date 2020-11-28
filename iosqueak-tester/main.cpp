/** IOSqueak Tester
 * Version: 2.0
 *
 * Allows running tests and custom code for IOSqueak.
 *
 * Author(s): Jason C. McDonald
 */

/* LICENSE (BSD-3-Clause)
 * Copyright (c) 2016-2020 MousePaw Media.
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

#include <iostream>
#include <string>

//#include "pawlib/goldilocks_shell.hpp"

#include "iosqueak/channel.hpp"
#include "iosqueak/stringify.hpp"
#include "iosqueak/tools/memlens.hpp"

//#include "test_stringify_numbers.hpp"

void dummy_func(int, int, bool) { return; }

void test_stringify()
{
	std::cout << "Bool: " << stringify(true) << std::endl;
	std::cout << "Char: " << stringify('c') << std::endl;
	std::cout << "Integer: " << stringify(-12345) << std::endl;
	std::cout << "Hex Integer: " << stringify(123, IOFormatBase::hex)
			  << std::endl;
	std::cout << "Float: " << stringify(123456789123456789.987654321)
			  << std::endl;

	long long val = 123456789123456789LL;

	std::cout << "Nullptr: "
			  << stringify(MemLens(nullptr), IOFormatPtr::address) << std::endl;
	std::cout << "Value: " << stringify(&val, IOFormatPtr::value) << std::endl;
	std::cout << "Address: " << stringify(&val, IOFormatPtr::address)
			  << std::endl;
	std::cout << "Type: " << stringify(&val, IOFormatPtr::pointer) << std::endl;
	std::cout << "Memory: "
			  << stringify(&val,
						   IOFormatPtr::memory,
						   IOFormatMemSep::all,
						   IOFormatBase::hex)
			  << std::endl;
	// std::cout << "Pointer Value: " << stringify(MemLens(ptr),
	// IOFormatPtr::value) << std::endl;

	char test = 'c';
	char* test_p = &test;
	std::cout << "Type: " << stringify(typeid(test)) << std::endl;
	std::cout << "Type*: " << stringify(typeid(&test_p)) << std::endl;

	TypesMap::register_type<std::string>("std::string");
	TypesMap::register_type<std::string>("std::string");
	std::string str = "Hello, world!";
	std::cout << "Type: " << stringify(typeid(str)) << std::endl;
	std::cout << "String: " << stringify(str) << std::endl;

	int ci = 42;
	std::cout << "Const Type: " << stringify(typeid(&ci)) << std::endl;

	std::cout << "Exception: "
			  << stringify(std::runtime_error("Error text here!")) << std::endl;

	std::cout << "Function: "
			  << stringify("dummy_func", dummy_func, 42, 42, true) << std::endl;
}

/** Temporary test code goes in this function ONLY.
 * All test code that is needed long term should be
 * moved to a dedicated Goldilocks Test and TestSuite.
 */
void test_code()
{
	/*int value = 42;
	channel << value << IOCtrl::endl;
	channel << IOFormatPtr::value << &value << IOCtrl::endl;

	channel << IOFormatPtr::address
		<< reinterpret_cast<void*>(test_code)
		<< IOCtrl::endl;*/

	channel << IOFormatTextAttr::bold << IOFormatTextFG::blue << "Hello!"
			<< IOCtrl::endl;
	channel << 42 << IOCtrl::endl;
	channel << IOFormatBase::hex << 254 << IOCtrl::endl;

	channel << IOFormatTextFG::green << IOFormatTextAttr::italic
			<< IOFormalBoolStyle::scott << true << IOCtrl::endl;
	std::cout << true << std::endl;

	channel << 123.456 << IOCtrl::endl;
	channel << IOFormatDecimalPlaces(2) << 10.152 << IOCtrl::endl;

	channel << IOFormatDecimalPlaces(5) << 51.00001
			<< IOCtrl::endl;  // This is a bug??
	std::cout << 51.00001 << std::endl;

	int thing = 123;
	int* thing_ptr = &thing;
	channel << IOFormatTextFG::red << IOFormatPtr::address << thing_ptr
			<< IOCtrl::endl;
	channel << IOFormatTextFG::red << IOFormatPtr::value << thing_ptr
			<< IOCtrl::endl;
	channel << IOFormatTextFG::red << IOFormatPtr::memory << thing_ptr
			<< IOCtrl::endl;
	channel << IOFormatTextFG::red << typeid(thing) << IOCtrl::endl;
}

/////// WARNING: DO NOT ALTER BELOW THIS POINT! ///////

int main(int argc, char* argv[])
{
	(void)argc;
	(void)argv;
	// Return code.
	int r = 0;

	test_code();

	// Set up signal handling.
	// ioc.configure_echo(IOEchoMode::cout);

	// GoldilocksShell* shell = new GoldilocksShell(">> ");
	// shell->register_suite<TestSuite_CoreTypes>("I-sB13");

	// // If we got command-line arguments.
	// if(argc > 1)
	// {
	//     r = shell->command(argc, argv);
	// }
	// else
	// {
	//     ioc << IOFormatTextAttr::bold << IOFormatTextFG::blue
	//         << "===== IOSqueak Tester =====\n" << IOCtrl::endl;

	// 	test_code();

	//     // Shift control to the interactive console.
	//     shell->interactive();
	// }

	// // Delete our GoldilocksShell.
	// delete shell;

	return r;
}
