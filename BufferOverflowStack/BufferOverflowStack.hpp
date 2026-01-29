#pragma once

#include "DefinesMacros.hpp"

namespace BufferOverflowStack
{

static constexpr size_t IN_BUFFER_SIZE = PAGE_SIZE;
static constexpr size_t KERNEL_BUFFER_SIZE = 0x800;

// Disable SMEP and jump to usermode shellcode
void prepare_rop(ByteVector& in_buffer, Address64 shellcode_address);

void exploit();

}