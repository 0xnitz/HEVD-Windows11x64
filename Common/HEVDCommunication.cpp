#include "HEVDCommunication.hpp"

HEVDCommunication::HEVDCommunication() :
	m_handle(open_handle_to_hevd())
{
}

ByteVector HEVDCommunication::call_ioctl(const uint32_t ioctl_code, ByteVector& in_buffer, const uint32_t out_buffer_size, const DWORD in_buffer_size)
{
	ByteVector out_buffer;
	out_buffer.resize(out_buffer_size);
	DWORD bytes_returned = 0;

	BOOL ioctl_result = DeviceIoControl(m_handle,
		ioctl_code,
		in_buffer.data(),
		in_buffer_size ? in_buffer_size : static_cast<DWORD>(in_buffer.size()),
		out_buffer.data(),
		out_buffer_size,
		&bytes_returned,
		nullptr);
	if (ioctl_result == FALSE && GetLastError() != ERROR_GEN_FAILURE) // The UAF handler returns GEN_FAILURE on success..
	{
		throw WindowsException(ArcaneErrors::ErrorCodes::DeviceIoControlFailed);
	}

	return out_buffer;
}

NO_DISCARD HANDLE HEVDCommunication::open_handle_to_hevd()
{
	HANDLE device_handle = CreateFileW(PRIMAL_DEVICE_NAME.c_str(), GENERIC_READ | GENERIC_WRITE, NULL, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (device_handle == INVALID_HANDLE_VALUE)
	{
		throw WindowsException(ArcaneErrors::ErrorCodes::CreateFileWFailed);
	}

	return device_handle;
}
