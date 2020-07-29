/** Stringify: Types Map [IOSqueak]
 *  Version 1.0
 *
 *  String-form names for common C++ and PawLIB data types.
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

#ifndef IOSQUEAK_TYPES_MAP_HPP
#define IOSQUEAK_TYPES_MAP_HPP

#include <cstdint>
#include <memory>
#include <string>
#include <typeindex>

class TypesMap
{
protected:
	inline static std::unordered_map<std::type_index, std::string> type_names =
		std::unordered_map<std::type_index, std::string>();
	inline static bool init = false;

	/* Initially registers the essential C++ and PawLIB data types with the map,
	 * but only if the map hasn't already been initialized. */
	static void initialize()
	{
		if (init)
		{
			return;
		}
		TypesMap::register_type<void>("void");
		TypesMap::register_type<bool>("bool");

		TypesMap::register_type<int8_t>("int8");
		TypesMap::register_type<uint8_t>("uint8");
		TypesMap::register_type<int16_t>("int16");
		TypesMap::register_type<uint16_t>("uint16");
		TypesMap::register_type<int32_t>("int32");
		TypesMap::register_type<uint32_t>("uint32");
		TypesMap::register_type<int64_t>("int64");
		TypesMap::register_type<uint64_t>("uint64");
		TypesMap::register_type<long long>("long long int");
		TypesMap::register_type<unsigned long long>("unsigned long long int");

		// signed char and unsigned char are handled by the int types
		TypesMap::register_type<char>("char");
		TypesMap::register_type<wchar_t>("wchar");
		TypesMap::register_type<char16_t>("char16");
		TypesMap::register_type<char32_t>("char32");

		TypesMap::register_type<float>("float");
		TypesMap::register_type<double>("double");
		TypesMap::register_type<long double>("long double");

		TypesMap::register_type<const char*>("const char*");
		TypesMap::register_type<std::string>("std::string");

		init = true;
	}

public:
	/** Register a new data type with the TypeMap. Automatically handles
	 * the corresponding pointer and pointer-to-pointer types.
	 *
	 * register_type<T, TypeOnly>(name)
	 * \param T (template) is the type to register. It should NOT be a pointer.
	 * \param TypeOnly (template) will prevent adding pointer and const forms.
	 * \param the human readable type name as a string. */
	template<typename T, bool TypeOnly = false>
	static void register_type(const char* name)
	{
		/* Although unordered map doesn't allow for duplicate keys,
		 * we'll save several function calls by checking first. */
		if (TypesMap::type_names.find(std::type_index(typeid(T))) !=
			TypesMap::type_names.end())
		{
			return;
		}

		// See
		// https://stackoverflow.com/questions/1143262/what-is-the-difference-between-const-int-const-int-const-and-int-const

		// Value
		TypesMap::type_names[std::type_index(typeid(T))] = std::string(name);

		if (TypeOnly)
		{
			return;
		}

		// Const value form
		TypesMap::type_names[std::type_index(typeid(T const))] =
			"const " + std::string(name);

		// Raw Pointers
		TypesMap::type_names[std::type_index(typeid(T*))] =
			std::string(name) + "*";
		TypesMap::type_names[std::type_index(typeid(T const*))] =
			"const " + std::string(name) + "*";
		TypesMap::type_names[std::type_index(typeid(T* const))] =
			std::string(name) + "* const";
		TypesMap::type_names[std::type_index(typeid(T const* const))] =
			"const " + std::string(name) + "* const";

		// Raw pointers-to-pointers
		TypesMap::type_names[std::type_index(typeid(T**))] =
			std::string(name) + "**";
		TypesMap::type_names[std::type_index(typeid(T * * const))] =
			std::string(name) + "** const";
		TypesMap::type_names[std::type_index(typeid(T* const*))] =
			std::string(name) + "* const *";

		// Smart pointers
		TypesMap::type_names[std::type_index(typeid(std::unique_ptr<T>))] =
			"std::unique_ptr<" + std::string(name) + ">";
		TypesMap::type_names[std::type_index(typeid(std::shared_ptr<T>))] =
			"std::shared_ptr<" + std::string(name) + ">";
		TypesMap::type_names[std::type_index(typeid(std::weak_ptr<T>))] =
			"std::weak_ptr<" + std::string(name) + ">";
	}

	template<typename T>
	static void register_type(const T&, const char* name)
	{
		register_type<T>(name);
	}

	/** Lookup the human-readable name of a type by its type_index.
	 * The first time this is called, the internal map of types is initialized.
	 * \param the type_index to look up
	 * \return the human-readable name as a std::string */
	static std::string lookup(const std::type_index& type)
	{
		TypesMap::initialize();
		try
		{
			return type_names.at(type);
		}
		catch (std::out_of_range)
		{
			return type.name();
		}
	}
};

#endif
