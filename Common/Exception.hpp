#pragma once

#include "DefinesMacros.hpp"

#include <string>

namespace ArcaneErrors
{
	enum ErrorCodes
	{
		NotImplemented = 0,
		OpenSCManagerFailed,
		CloseServiceHandleFailed,
		CreateServiceFailed,
		DeleteServiceFailed,
		StartServiceFailed,
		ControlServiceFailed,
		CloseHandleCloseServiceHandleFailed,
		CreateFileWFailed,
		SetFilePointerExFailed,
		ReadFileFailed,
		GetFileSizeExFailed,
		WriteFileFailed,
		MultiByteToWideCharFailed,
		WideCharToMultiByteFailed,
		CreateProcessFailed,
		OpenProcessFailed,
		WriteProcessMemoryFailed,
		ReadProcessMemoryFailed,
		CreateRemoteThreadFailed,
		OpenThreadFailed,
		SuspendThreadFailed,
		ResumeThreadFailed,
		QueueUserApcFailed,
		CreateToolhelp32SnapshotFailed,
		Process32FirstWFailed,
		Process32NextWFailed,
		VirtualAllocExFailed,
		VirtualProtectExFailed,
		GetProcessIdOfThreadFailed,
		GetModuleFileNameWFailed,
		Thread32FirstWFailed,
		DeviceIoControlFailed
	};
}

class Exception
{
public:
	explicit Exception(ArcaneErrors::ErrorCodes error_code);

	virtual ~Exception() = default;

	NO_DISCARD ArcaneErrors::ErrorCodes get_error_code() const;

	NO_DISCARD virtual std::wstring message() const;

protected:
	ArcaneErrors::ErrorCodes m_error_code; // Exception error codes.
};

#define CATCH_ALL(error_message) \
	catch (const Exception& exception) { OutputDebugStringW(exception.message().c_str()); } \
	catch (const std::exception& exception) { OutputDebugStringA(std::string(exception.what()).c_str()); } \
	catch (...) { OutputDebugStringA(std::string(error_message).c_str()); }
