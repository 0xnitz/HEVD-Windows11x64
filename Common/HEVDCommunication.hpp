#pragma once

#include "DefinesMacros.hpp"
#include "WindowsException.hpp"

class HEVDCommunication final
{
public:
	explicit HEVDCommunication();

	HEVDCommunication(HEVDCommunication const&) = delete;
	HEVDCommunication(HEVDCommunication&&) = delete;
	HEVDCommunication operator=(HEVDCommunication const&) = delete;
	HEVDCommunication operator=(HEVDCommunication&&) = delete;

	NO_DISCARD ByteVector call_ioctl(const uint32_t ioctl_code, ByteVector& in_buffer, const uint32_t out_buffer_size, const DWORD in_buffer_size = 0);

private:
	static NO_DISCARD HANDLE open_handle_to_hevd();

	HANDLE m_handle;
};