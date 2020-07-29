/** Stringify: Types [IOSqueak]
 *  Version 1.0
 *
 *  Convert data types to strings.
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

#ifndef IOSQUEAK_STRINGIFY_TYPES_HPP
#define IOSQUEAK_STRINGIFY_TYPES_HPP

#include <typeindex>
#include <typeinfo>

#include "iosqueak/tools/typemap.hpp"

/* NOTE: The specialization must come first, lest we get a
 * stack overflow from infinite recursion. */

/**Convert a type index to a human-readable string representing the type.
 * \param the type index to convert
 * \return the type represented as a human-readable string */
std::string stringify_type(const std::type_index& type)
{
	return TypesMap::lookup(type);
}

std::string stringify_type(const std::type_info& type)
{
	return TypesMap::lookup(std::type_index(type));
}

/**Convert a basic data type to a human-readable string representing the type.
 * If the type is not in IOSqueak's type map, the compiler-provided type
 * name will be used instead.
 * \param any value to convert the type of
 * \return the type represented as a human-readable string */
template<typename T>
std::string stringify_type(const T&)
{
	return stringify_type(std::type_index(typeid(T)));
}

#endif
