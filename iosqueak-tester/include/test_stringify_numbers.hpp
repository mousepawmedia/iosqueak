/** Tests for Stringify: Numbers [IOSQueak]
 *
 * Author(s): Jason C. McDonald, Graham Mix
 */

/* LICENSE (BSD-3-Clause)
 * Copyright (c) 2020 MousePaw Media.
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

#ifndef IOSQUEAK_STRINGIFY_NUMBERS_TESTS_HPP
#define IOSQUEAK_STRINGIFY_NUMBERS_TESTS_HPP

#include "iosqueak/stringify/numbers.hpp"
#include "goldilocks/goldilocks.hpp"
#include "goldilocks/assertions.hpp"

class Test_LengthifyIntegral : public Test
{
public:
	const int target = 424200;
	const int pos_dec_len = 6;
	const int neg_dec_len = 7;
	const int pos_bin_len = 19;
	const int pos_hex_len = 5;

	Test_LengthifyIntegral() = default;

	testdoc_t get_title() { return "Test Lengthify Integral"; }

	testdoc_t get_docs() { return "Test the lengthify_integral() function."; }

	bool run() override
	{
		// test length of positive number
		PL_ASSERT_EQUAL(lengthify_integral(target), pos_doc_len);

		// test length of negative number
		PL_ASSERT_EQUAL(lengthify_integral(-target), neg_doc_len);

		// test sign always
		PL_ASSERT_EQUAL(lengthify_integral(target,
										   IOFormatBase::dec,
										   IOFormatSign::always,
										   IOFormatBaseNotation::prefix),
						pos_doc_len + 1);

		// test hexadecimal length
		PL_ASSERT_EQUAL(lengthify_integral(target, IOFormatBase::hex),
						pos_hex_len);

		// test binary length
		PL_ASSERT_EQUAL(lengthify_integral(target, IOFormatBase::bin),
						pos_bin_len);

		// test prefix on hexadecimal
		PL_ASSERT_EQUAL(lengthify_integral(target,
										   IOFormatBase::hex,
										   IOFormatSign::automatic,
										   IOFormatBaseNotation::prefix),
						pos_hex_len + 2);

		// test suffix on binary
		PL_ASSERT_EQUAL(lengthify_integral(target,
										   IOFormatBase::bin,
										   IOFormatSign::automatic,
										   IOFormatBaseNotation::subscript),
						pos_bin_len + 2);
	}

	~Test_LengthifyIntegral() = default;
}

class TestSuite_StringifyNumbers : public TestSuite
{
public:
	explicit TestSuite_FlexArray() = default;

	void load_test() override;

	testdoc_t get_title() override { return "IOSqueak: Stringify Numbers"; }

	~TestSuite_FLexArray() = default;
}
