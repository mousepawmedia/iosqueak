/** IODecor [IOSqueak]
 *
 * Quickly create banners and align text.
 *
 * Author: Jason C. McDonald
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

#ifndef IOSQUEAK_IODECOR_HPP
#define IOSQUEAK_IODECOR_HPP

#include <algorithm>
#include <iostream>

#include "iosqueak/ioformat.hpp"
#include "iosqueak/stringify.hpp"
#include "iosqueak/window.hpp"

class IODecor_Base
{
protected:
	static unsigned short columns();

public:
	virtual std::string to_string() const = 0;

	friend std::ostream& operator<<(std::ostream& out, const IODecor_Base& d);
};

class IODecor_Absorber : public IODecor_Base
{
protected:
	std::string str;

public:
	IODecor_Absorber() : str("") {}

	template<typename T> IODecor_Absorber& operator<<(const T& rhs)
	{
		this->str.append(stringify(rhs));
		return *this;
	};
};

class IODecorFill : public IODecor_Base
{
protected:
	char fill_char;

public:
	explicit IODecorFill(char c) : fill_char(c) {}

	std::string to_string() const override;
};

class IODecorLeft : public IODecor_Absorber
{
protected:
	char fill_char;

public:
	explicit IODecorLeft(char c = ' ') : fill_char(c) {}

	std::string to_string() const override;
};

class IODecorCenter : public IODecor_Absorber
{
protected:
	char fill_char;

public:
	explicit IODecorCenter(char c = ' ') : fill_char(c) {}

	std::string to_string() const override;
};

class IODecorRight : public IODecor_Absorber
{
protected:
	char fill_char;

public:
	explicit IODecorRight(char c = ' ') : fill_char(c) {}

	std::string to_string() const override;
};

enum class IODecorAlign 
{
	left,
	right,
	center 
};	

class IODecorPad : public IODecor_Absorber
{
	protected:
		uint8_t padding;
		IODecorAlign alignment = IODecorAlign::left;

	public: 
		std::string to_string() const override;
		IODecorPad(uint8_t padding, IODecorAlign alignment) : padding(padding), alignment(alignment) {};

		static std::string alignToString(IODecorAlign align);
};




#endif  // IOSQUEAK_IODECOR_HPP
