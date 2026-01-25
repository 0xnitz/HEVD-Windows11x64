#pragma once

#include <vector>
#include <windows.h>
#include <filesystem>

#define NO_DISCARD [[nodiscard]]

#define UNUSED(var) [[maybe_unused]] var

using Byte = uint8_t;
using ByteVector = std::vector<Byte>;
using Address64 = uint64_t;
using Pid = uint32_t;
using Tid = uint32_t;

static constexpr size_t PAGE_SIZE = 0x1000;

static const std::filesystem::path PRIMAL_DEVICE_NAME = L"\\\\.\\HackSysExtremeVulnerableDriver";

static constexpr uint64_t ARBITRARY_WRITE_IOCTL = 0x22200B;