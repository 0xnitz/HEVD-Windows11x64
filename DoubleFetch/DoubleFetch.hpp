#pragma once

#include "DefinesMacros.hpp"

namespace DoubleFetch
{

static constexpr size_t OVERFLOW_BUFFER_SIZE = PAGE_SIZE;
static constexpr size_t KERNEL_BUFFER_SIZE = 0x800;

typedef struct
{
	Address64 buffer;
	size_t buffer_size;
} DoubleFetchParams;

void disable_smep_rop(ByteVector& in_buffer, Address64 shellcode_address);

void race_thread(ByteVector& data_buffer_to_change, size_t old_size, size_t new_size, size_t offset);

void run_race_thread(ByteVector& data_buffer_to_change, size_t old_size, size_t new_size, size_t offset);

void exploit();

}