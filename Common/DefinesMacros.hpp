#pragma once

#include <vector>
#include <windows.h>
#include <filesystem>

#define NO_DISCARD [[nodiscard]]

#define UNUSED(var) [[maybe_unused]] var

#define DEBUG_PRINT(message) OutputDebugStringA(std::string(message).c_str())

using Byte = uint8_t;
using ByteVector = std::vector<Byte>;
using Address64 = uint64_t;
using Pid = uint32_t;
using Tid = uint32_t;

static constexpr size_t PAGE_SIZE = 0x1000;

static const std::filesystem::path PRIMAL_DEVICE_NAME = L"\\\\.\\HackSysExtremeVulnerableDriver";

static constexpr uint64_t ARBITRARY_WRITE_IOCTL = 0x22200B;
static constexpr uint64_t BUFFER_OVERFLOW_STACK_IOCTL = 0x222003;
static constexpr uint64_t DOUBLE_FETCH_IOCTL = 0x222037;
static constexpr uint64_t INTEGER_OVERFLOW_IOCTL = 0x222027;
static constexpr uint64_t ALLOCATE_FAKE_OBJECT_NX_IOCTL = 0x22205F;
static constexpr uint64_t UAF_ALLOCATE_OBJECT_NX_IOCTL = 0x222053;
static constexpr uint64_t UAF_USE_OBJECT_NX_IOCTL = 0x222057;
static constexpr uint64_t UAF_FREE_OBJECT_NX_IOCTL = 0x22205B;
static constexpr uint64_t ALLOCATE_FAKE_OBJECT_IOCTL = 0x22201F;
static constexpr uint64_t UAF_ALLOCATE_OBJECT_IOCTL = 0x222013;
static constexpr uint64_t UAF_USE_OBJECT_IOCTL = 0x222017;
static constexpr uint64_t UAF_FREE_OBJECT_IOCTL = 0x22201B;