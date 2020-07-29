/** MemLens [IOSqueak]
 * Version: 1.0
 *
 * Mediates safe access to random sections of memory, especially for
 * dumping the memory to string.
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

#ifndef IOSQUEAK_MEMLENS_HPP
#define IOSQUEAK_MEMLENS_HPP

#include <cstdint>
#include <memory>
#include <typeindex>
#include <typeinfo>

enum class PtrType
{
	raw,
	shared,
	weak
};

// NOTE: We cannot support unique_ptr because it cannot be copied.

class MemLens
{
protected:
	const void* focus;
	size_t size;
	std::type_index type;
	PtrType ptr_type;
	std::vector<uint8_t> snapshot;

	void take_snapshot()
	{
		if (focus == nullptr)
		{
			return;
		}

		const uint8_t* root = reinterpret_cast<const uint8_t*>(focus);

		for (uint8_t lens = 0; lens < size; ++lens)
		{
			snapshot.push_back(*(root + lens));
		}
	}

public:
	explicit MemLens(const void* ptr,
					 const IOMemReadSize& readsize = IOMemReadSize(1))
	: focus(ptr), size(readsize.readsize), type(std::type_index(typeid(void))),
	  ptr_type(PtrType::raw)
	{
		snapshot.reserve(size);
		take_snapshot();
	}

	template<typename T>
	explicit MemLens(const T* ptr)
	: focus(reinterpret_cast<const void*>(ptr)), size(sizeof(T)),
	  type(std::type_index(typeid(T))), ptr_type(PtrType::raw)
	{
		snapshot.reserve(size);
		take_snapshot();
	}

	explicit MemLens(const std::shared_ptr<void>& ptr,
					 const IOMemReadSize& readsize = IOMemReadSize(1))
	: focus(ptr.get()), size(readsize.readsize),
	  type(std::type_index(typeid(void))), ptr_type(PtrType::shared)
	{
		snapshot.reserve(size);
		take_snapshot();
	}

	template<typename T>
	explicit MemLens(const std::shared_ptr<T>& ptr)
	: focus(ptr.get()), size(sizeof(T)), type(std::type_index(typeid(T))),
	  ptr_type(PtrType::shared)
	{
		snapshot.reserve(size);
		take_snapshot();
	}

	explicit MemLens(const std::weak_ptr<void>& ptr,
					 const IOMemReadSize& readsize = IOMemReadSize(1))
	: focus(nullptr), size(readsize.readsize),
	  type(std::type_index(typeid(void))), ptr_type(PtrType::weak)
	{
		snapshot.reserve(size);

		if (!ptr.expired())
		{
			// Steal the raw pointer out of the weak pointer.
			auto shared = ptr.lock();
			focus = shared.get();
		}

		take_snapshot();
	}

	template<typename T>
	explicit MemLens(const std::weak_ptr<T>& ptr)
	: focus(nullptr), size(sizeof(T)), type(std::type_index(typeid(T))),
	  ptr_type(PtrType::weak)
	{
		snapshot.reserve(size);

		if (!ptr.expired())
		{
			// Steal the raw pointer out of the weak pointer.
			auto shared = ptr.lock();
			focus = shared.get();
		}

		take_snapshot();
	}

	uint64_t address() const { return reinterpret_cast<uint64_t>(focus); }

	size_t data_size() const { return size; }

	// TODO: Can we return ref?
	std::type_index data_type() const { return type; }

	// TODO: Can we return ref?
	virtual std::vector<uint8_t> memory() const { return snapshot; }

	PtrType pointer_type() const { return ptr_type; }
};

template<typename T>
class DynamicMemLens : public MemLens
{
protected:
	std::weak_ptr<T> handle;

public:
	explicit DynamicMemLens(const std::shared_ptr<T> ptr)
	: handle(ptr), MemLens(ptr)
	{
	}

	explicit DynamicMemLens(const std::weak_ptr<T> ptr)
	: handle(ptr), MemLens(ptr)
	{
	}

	virtual std::vector<uint8_t> memory() const override
	{
		if (handle.expired())
		{
			return std::vector<uint8_t>();
		}

		take_snapshot();
		return snapshot;
	}
};

#endif
