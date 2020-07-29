/** Stringify: Functions [IOSqueak]
 *  Version 1.0
 *
 *  Convert standard exceptions to strings.
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

#ifndef IOSQUEAK_STRINGIFY_EXCEPTION_HPP
#define IOSQUEAK_STRINGIFY_EXCEPTION_HPP

#include <exception>
#include <functional>
#include <future>
#include <memory>
#include <new>
#include <stdexcept>
#include <string>
#include <system_error>
#include <typeinfo>

#include "iosqueak/stringify/numbers.hpp"

[[maybe_unused]] static std::string stringify_exception(
	const std::exception& except)
{
	return std::string("[EXCEPTION: ") + except.what() + "]";
}

// "BAD" ERRORS

[[maybe_unused]] static std::string stringify_exception(
	const std::bad_alloc& except)
{
	return std::string("[BAD ALLOC: ") + except.what() + "]";
}

[[maybe_unused]] static std::string stringify_exception(
	const std::bad_array_new_length& except)
{
	return std::string("[BAD ARRAY NEW LENGTH: ") + except.what() + "]";
}

[[maybe_unused]] static std::string stringify_exception(
	const std::bad_cast& except)
{
	return std::string("[BAD CAST: ") + except.what() + "]";
}

[[maybe_unused]] static std::string stringify_exception(
	const std::bad_exception& except)
{
	return std::string("[BAD EXCEPTION: ") + except.what() + "]";
}

[[maybe_unused]] static std::string stringify_exception(
	const std::bad_function_call& except)
{
	return std::string("[BAD FUNCTION CALL: ") + except.what() + "]";
}

[[maybe_unused]] static std::string stringify_exception(
	const std::bad_typeid& except)
{
	return std::string("[BAD TYPEID: ") + except.what() + "]";
}

[[maybe_unused]] static std::string stringify_exception(
	const std::bad_weak_ptr& except)
{
	return std::string("[BAD WEAK_PTR: ") + except.what() + "]";
}

// LOGIC ERRORS

[[maybe_unused]] static std::string stringify_exception(
	const std::logic_error& except)
{
	return std::string("[LOGIC ERROR: ") + except.what() + "]";
}

[[maybe_unused]] static std::string stringify_exception(
	const std::domain_error& except)
{
	return std::string("[DOMAIN ERROR: ") + except.what() + "]";
}

[[maybe_unused]] static std::string stringify_exception(
	const std::future_error& except)
{
	return std::string("[FUTURE ERROR: ") + except.what() + "]";
}

[[maybe_unused]] static std::string stringify_exception(
	const std::invalid_argument& except)
{
	return std::string("[INVALID ARGUMENT ERROR: ") + except.what() + "]";
}

[[maybe_unused]] static std::string stringify_exception(
	const std::length_error& except)
{
	return std::string("[LENGTH ERROR: ") + except.what() + "]";
}

[[maybe_unused]] static std::string stringify_exception(
	const std::out_of_range& except)
{
	return std::string("[OUT OF RANGE ERROR: ") + except.what() + "]";
}

// RUNTIME ERRORS

[[maybe_unused]] static std::string stringify_exception(
	const std::runtime_error& except)
{
	return std::string("[RUNTIME ERROR: ") + except.what() + "]";
}

[[maybe_unused]] static std::string stringify_exception(
	const std::overflow_error& except)
{
	return std::string("[OVERFLOW ERROR: ") + except.what() + "]";
}

[[maybe_unused]] static std::string stringify_exception(
	const std::range_error& except)
{
	return std::string("[RANGE ERROR: ") + except.what() + "]";
}

[[maybe_unused]] static std::string stringify_exception(
	const std::underflow_error& except)
{
	return std::string("[UNDERFLOW ERROR: ") + except.what() + "]";
}

// SYSTEM ERROR

static std::string stringify_error_code(const std::error_code& code)
{
	return stringify_integral(code.value());
}

[[maybe_unused]] static std::string stringify_exception(
	const std::system_error& except)
{
	return std::string("[SYSTEM ERROR: (" +
					   stringify_error_code(except.code()) + ") " +
					   except.what() + "]");
}

// IOS ERROR

[[maybe_unused]] static std::string stringify_exception(
	const std::ios_base::failure& except)
{
	return std::string("[IOS_BASE FAILURE: (" +
					   stringify_error_code(except.code()) + ") " +
					   except.what() + "]");
}

#endif
