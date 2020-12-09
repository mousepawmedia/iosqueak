/** Stringify [IOSqueak]
 *  Version 1.0
 *
 *  Convert any data type to strings.
 *
 * Author(s): Jason C. McDonald, Scott Taylor
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

#ifndef IOSQUEAK_STRINGIFY_HPP
#define IOSQUEAK_STRINGIFY_HPP

#include <stdexcept>
#include <string>

#include "iosqueak/ioformat.hpp"
#include "iosqueak/stringify/anything.hpp"
#include "iosqueak/stringify/exception.hpp"
#include "iosqueak/stringify/function.hpp"
#include "iosqueak/stringify/logic.hpp"
#include "iosqueak/stringify/memory.hpp"
#include "iosqueak/stringify/numbers.hpp"
#include "iosqueak/stringify/types.hpp"
#include "iosqueak/stringify/utilities.hpp"
#include "iosqueak/tools/memlens.hpp"

/* Implementation: Necessary forward declares for functions that must be
 * declared at the end of the header. */

std::string stringify_from_pointer(const void*);

template<typename T>
std::string stringify_from_pointer(const T* ptr);

template<typename T>
std::string stringify_from_pointer(const std::shared_ptr<T>& ptr);

template<typename T>
std::string stringify_from_pointer(const std::weak_ptr<T>& ptr);

template<typename T,
		 std::enable_if_t<std::is_floating_point<T>::value, int> = 0>
size_t lengthify(
	const T& val,
	const IOFormatDecimalPlaces& places = IOFormatDecimalPlaces(14),
	const IOFormatSciNotation& sci = IOFormatSciNotation::automatic,
	const IOFormatSign& sign = IOFormatSign::automatic)
{
	return lengthify_floating_point(val, places, sci, sign);
}

template<typename T, std::enable_if_t<std::is_integral<T>::value, int> = 0>
size_t lengthify(
	const T& val,
	const IOFormatBase& base = IOFormatBase::dec,
	const IOFormatSign& sign = IOFormatSign::automatic,
	const IOFormatBaseNotation& notation = IOFormatBaseNotation::prefix)
{
	return lengthify_integral(val, base, sign, notation);
}

size_t lengthify(const bool& val,
				 const IOFormalBoolStyle& fmt = IOFormalBoolStyle::lower)
{
	return lengthify_boolean(val, fmt);
}

template<typename T, typename Enable = void>
struct _StringifyImpl;

/** Convert anything to a string.
 * \param the value to convert
 * \return the string equivalent of the value*/
template<typename T>
std::string stringify(const T& val)
{
	// See http://www.gotw.ca/publications/mill17.htm
	return _StringifyImpl<T>::stringify(val);
}

template<typename T>
std::string stringify(const T& val, const IOFormat& fmt)
{
	return _StringifyImpl<T>::stringify(val, fmt);
}

/** Fallback **/

template<typename T, typename Enable /*=void*/>
struct _StringifyImpl {
	static std::string stringify(const T& val)
	{
		// If we reach here, there's no stringify function for the given value.
		return ::stringify_anything(val);
	}

	static std::string stringify(const T& val, const IOFormat&)
	{
		return ::stringify_anything(val);
	}
};

/* Stringify integers */

template<typename T>
std::string stringify(
	const T& val,
	const IOFormatBase& base /*= IOFormatBase::dec*/,
	const IOFormatSign& sign = IOFormatSign::automatic,
	const IOFormatNumCase& num_case = IOFormatNumCase::upper,
	const IOFormatBaseNotation& notation = IOFormatBaseNotation::prefix)
{
	return stringify_integral(val, base, sign, num_case, notation);
}

template<typename T>
struct _StringifyImpl<
	T,
	typename std::enable_if<std::is_integral<T>::value>::type> {
	static std::string stringify(const T& val)
	{
		return ::stringify(val, IOFormatBase::dec);
	}

	static std::string stringify(const T& val, const IOFormat& fmt)
	{
		return ::stringify(
			val,
			fmt.base(),
			fmt.sign(),
			fmt.numeral_case(),
			fmt.base_notation()
		);
	}
};

/* Stringify floating-point numbers. */

template<typename T>
std::string stringify(
	const T& val,
	const IOFormatDecimalPlaces& places /*= IOFormatDecimalPlaces(14)*/,
	const IOFormatSciNotation& sci = IOFormatSciNotation::automatic,
	const IOFormatSign& sign = IOFormatSign::automatic)
{
	return stringify_floating_point(val, places, sci, sign);
}

template<typename T>
struct _StringifyImpl<
	T,
	typename std::enable_if<std::is_floating_point<T>::value>::type> {
	static std::string stringify(const T& val)
	{
		return ::stringify(val, IOFormatDecimalPlaces(14));
	}

	static std::string stringify(const T& val, const IOFormat& fmt)
	{
		return ::stringify(
			val,
			fmt.decimal_places(),
			fmt.sci_notation(),
			fmt.sign()
		);
	}
};

/* Stringify char */

std::string stringify(
	const char& val,
	const IOFormatCharValue& as /*=IOFormatCharValue::as_char*/)
{
	return stringify_char(val, as);
}

template<>
struct _StringifyImpl<char> {
	static std::string stringify(const char& val)
	{
		return ::stringify(val, IOFormatCharValue::as_char);
	}

	static std::string stringify(const char& val, const IOFormat& fmt)
	{
		return ::stringify(val, fmt.char_value());
	}
};

/* Stringify bool */

std::string stringify(
	const bool& val,
	const IOFormalBoolStyle& fmt /*= IOFormalBoolStyle::lower*/)
{
	return stringify_boolean(val, fmt);
}

template<>
struct _StringifyImpl<bool> {
	static std::string stringify(const bool& val)
	{
		return ::stringify(val, IOFormalBoolStyle::lower);
	}

	static std::string stringify(const bool& val, const IOFormat& fmt)
	{
		return ::stringify(val, fmt.bool_style());
	}
};

/* Stringify exceptions */

template<typename T>
struct _StringifyImpl<
	T,
	typename std::enable_if<std::is_base_of<std::exception, T>::value>::type> {
	static std::string stringify(const T& except)
	{
		return ::stringify_exception(except);
	}

	static std::string stringify(const T& except, const IOFormat&)
	{
		return ::stringify_exception(except);
	}
};

/* Stringify functions */

template<typename T, typename... Args>
std::string stringify(const char* name_hint, const T& func, Args... args)
{
	return stringify_function(name_hint, func, args...);
}

/* Stringify bitsets */

template<size_t N>
std::string stringify(const std::bitset<N>& bits,
					  const IOFormatMemSep& sep)
{
	return stringify_bitset(bits, sep);
}

template<size_t N>
struct _StringifyImpl<std::bitset<N>> {
	static std::string stringify(const std::bitset<N>& bits)
	{
		return ::stringify(bits, IOFormatMemSep::all);
	}

	static std::string stringify(const std::bitset<N>& bits, const IOFormat& fmt)
	{
		return ::stringify(bits, fmt.mem_sep());
	}
};

/* Stringify MemLens */

/* Implementation Note: By writing the two functions with the default arguments
 * and function calls as I did, the effective result is as if I'd written
 * default arguments for when either IOFormatPtr::address or IOFormatPtr::memory
 * are passed. It's spooky, but it works without a hitch from a user
 * perspective!
 */

std::string stringify(const MemLens& lens,
					  const IOFormatPtr& as /*= IOFormatPtr::address*/,
					  const IOFormatNumCase& num_case = IOFormatNumCase::upper)
{
	switch (as) {
		case IOFormatPtr::value:
			throw std::invalid_argument("stringify() cannot retrieve the value "
										"from a MemLens-captured pointer");
		case IOFormatPtr::address:
			return stringify_address(lens, num_case);
		case IOFormatPtr::pointer:
			return stringify_pointer_data(lens);
		case IOFormatPtr::memory:
			return stringify_bytes(lens,
								   IOFormatMemSep::none,
								   IOFormatBase::hex,
								   num_case);
	}
	return "";
}

std::string stringify(const MemLens& lens,
					  const IOFormatPtr& as,
					  const IOFormatMemSep& sep,
					  const IOFormatBase& base = IOFormatBase::hex,
					  const IOFormatNumCase& num_case = IOFormatNumCase::upper)
{
	switch (as) {
		case IOFormatPtr::value:
			throw std::invalid_argument("stringify() cannot retrieve the value "
										"from a MemLens-captured pointer");
		case IOFormatPtr::address:
			return stringify_address(lens, num_case);
		case IOFormatPtr::pointer:
			return stringify_pointer_data(lens);
		case IOFormatPtr::memory:
			return stringify_bytes(lens, sep, base, num_case);
	}

	return "";
}

template<>
struct _StringifyImpl<MemLens> {
	static std::string stringify(const MemLens& lens)
	{
		return ::stringify(lens, IOFormatPtr::address);
	}

	static std::string stringify(const MemLens& lens, const IOFormat& fmt)
	{
		if (fmt.ptr() == IOFormatPtr::address) {
			return ::stringify(lens, fmt.ptr(), fmt.numeral_case());
		}

		auto base = fmt.base();
		switch(base) {
			case IOFormatBase::bin:
				[[fallthrough]];
			case IOFormatBase::oct:
				[[fallthrough]];
			case IOFormatBase::hex:
				break;
			default:
				base = IOFormatBase::hex;
		}

		return ::stringify(
			lens,
			fmt.ptr(),
			fmt.mem_sep(),
			base,
			fmt.numeral_case()
		);

	}
};

/* Stringify raw pointers */

template<typename T>
std::string stringify(const T* ptr,
					  const IOFormatPtr& as /*= IOFormatPtr::address*/,
					  const IOFormatNumCase& num_case = IOFormatNumCase::upper)
{
	if (as == IOFormatPtr::value) {
		return stringify_from_pointer(ptr);
	}
	return stringify(MemLens(ptr), as, num_case);
}

template<typename T>
std::string stringify(const T* ptr,
					  const IOFormatPtr& as,
					  const IOFormatMemSep& sep,
					  const IOFormatBase& base = IOFormatBase::hex,
					  const IOFormatNumCase& num_case = IOFormatNumCase::upper)
{
	if (as == IOFormatPtr::value) {
		return stringify_from_pointer(ptr);
	}
	return stringify(MemLens(ptr), as, sep, base, num_case);
}

template<typename T>
struct _StringifyImpl<T*> {
	static std::string stringify(const T* ptr)
	{
		return ::stringify(ptr, IOFormatPtr::address);
	}

	static std::string stringify(const T* ptr, const IOFormat& fmt)
	{
		if (fmt.ptr() == IOFormatPtr::address) {
			return ::stringify(ptr, fmt.ptr(), fmt.numeral_case());
		}

		auto base = fmt.base();
		switch(base) {
			case IOFormatBase::bin:
				[[fallthrough]];
			case IOFormatBase::oct:
				[[fallthrough]];
			case IOFormatBase::hex:
				break;
			default:
				base = IOFormatBase::hex;
		}

		return ::stringify(
			ptr,
			fmt.ptr(),
			fmt.mem_sep(),
			base,
			fmt.numeral_case()
		);
	}
};

/* Stringify shared pointers */

template<typename T>
std::string stringify(const std::shared_ptr<T>& ptr,
					  const IOFormatPtr& as /*= IOFormatPtr::address*/,
					  const IOFormatNumCase& num_case = IOFormatNumCase::upper)
{
	if (as == IOFormatPtr::value) {
		return stringify_from_pointer(ptr);
	}
	return stringify(MemLens(ptr), as, num_case);
}

template<typename T>
std::string stringify(const std::shared_ptr<T>& ptr,
					  const IOFormatPtr& as,
					  const IOFormatMemSep& sep,
					  const IOFormatBase& base = IOFormatBase::hex,
					  const IOFormatNumCase& num_case = IOFormatNumCase::upper)
{
	if (as == IOFormatPtr::value) {
		return stringify_from_pointer(ptr);
	}
	return stringify(MemLens(ptr), as, sep, base, num_case);
}

template<typename T>
struct _StringifyImpl<std::shared_ptr<T>> {
	static std::string stringify(const std::shared_ptr<T>& ptr)
	{
		return ::stringify(ptr, IOFormatPtr::address);
	}

	static std::string stringify(const std::shared_ptr<T>& ptr,
								 const IOFormat& fmt)
	{
		if (fmt.ptr() == IOFormatPtr::address) {
			return ::stringify(ptr, fmt.ptr(), fmt.numeral_case());
		}

		auto base = fmt.base();
		switch(base) {
			case IOFormatBase::bin:
				[[fallthrough]];
			case IOFormatBase::oct:
				[[fallthrough]];
			case IOFormatBase::hex:
				break;
			default:
				base = IOFormatBase::hex;
		}

		return ::stringify(
			ptr,
			fmt.ptr(),
			fmt.mem_sep(),
			base,
			fmt.numeral_case()
		);
	}
};

/* Stringify weak pointers */

template<typename T>
std::string stringify(const std::weak_ptr<T>& ptr,
					  const IOFormatPtr& as /*= IOFormatPtr::address*/,
					  const IOFormatNumCase& num_case = IOFormatNumCase::upper)
{
	if (as == IOFormatPtr::value) {
		return stringify_from_pointer(ptr);
	}
	return stringify(MemLens(ptr), as, num_case);
}

template<typename T>
std::string stringify(const std::weak_ptr<T>& ptr,
					  const IOFormatPtr& as,
					  const IOFormatMemSep& sep,
					  const IOFormatBase& base = IOFormatBase::hex,
					  const IOFormatNumCase& num_case = IOFormatNumCase::upper)
{
	if (as == IOFormatPtr::value) {
		return stringify_from_pointer(ptr);
	}
	return stringify(MemLens(ptr), as, sep, base, num_case);
}

template<typename T>
struct _StringifyImpl<std::weak_ptr<T>> {
	static std::string stringify(const std::weak_ptr<T>& ptr)
	{
		return ::stringify(ptr, IOFormatPtr::address);
	}

	static std::string stringify(const std::weak_ptr<T>& ptr,
								 const IOFormat& fmt)
	{
		if (fmt.ptr() == IOFormatPtr::address) {
			return ::stringify(ptr, fmt.ptr(), fmt.numeral_case());
		}

		auto base = fmt.base();
		switch(base) {
			case IOFormatBase::bin:
				[[fallthrough]];
			case IOFormatBase::oct:
				[[fallthrough]];
			case IOFormatBase::hex:
				break;
			default:
				base = IOFormatBase::hex;
		}

		return ::stringify(
			ptr,
			fmt.ptr(),
			fmt.mem_sep(),
			base,
			fmt.numeral_case()
		);
	}
};

/* Stringify types */

template<>
struct _StringifyImpl<std::type_info> {
	static std::string stringify(const std::type_info& type)
	{
		return ::stringify_type(type);
	}

	static std::string stringify(const std::type_info& type, const IOFormat&)
	{
		return ::stringify_type(type);
	}
};

template<>
struct _StringifyImpl<std::type_index> {
	static std::string stringify(const std::type_index& type)
	{
		return ::stringify_type(type);
	}

	static std::string stringify(const std::type_index& type, const IOFormat&)
	{
		return ::stringify_type(type);
	}
};

/* Stringify strings. */
template<>
struct _StringifyImpl<const char*> {
	static std::string stringify(const char* str) { return str; }

	static std::string stringify(const char* str, const IOFormat&)
	{
		return str;
	}
};

template<>
struct _StringifyImpl<std::string> {
	static std::string stringify(const std::string& str) { return str; }

	static std::string stringify(const std::string& str, const IOFormat&)
	{
		return str;
	}
};

/* Stringify from a variadic list of arguments. */
template<typename T, typename... Args>
std::string stringify_variadic(const T& val, [[maybe_unused]] Args... rem)
{
	if constexpr (sizeof...(Args) > 0) {
		return stringify(val) + ", " + stringify_variadic(rem...);
	}
	return stringify(val);
}

/* Implementation Note: The stringify_from_pointer() functions MUST come
 * after all other functions (which they rely on), but be forward declared
 * at the top of the header.
 */

std::string stringify_from_pointer(const void*) { return "(void pointer)"; }

template<typename T>
std::string stringify_from_pointer(const T* ptr)
{
	// We only guarantee as much memory safety as the user provides.
	// We can't check for bad pointers; only null pointers!
	if (ptr != nullptr) {
		return stringify(*ptr);
	}
	return "(null pointer)";
}

template<typename T>
std::string stringify_from_pointer(const std::shared_ptr<T>& ptr)
{
	return stringify_from_pointer(ptr.get());
}

template<typename T>
std::string stringify_from_pointer(const std::weak_ptr<T>& ptr)
{
	auto shared = ptr.lock();
	return stringify_from_pointer(shared.get());
}

#endif
