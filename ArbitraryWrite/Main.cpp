#include "Exception.hpp"
#include "DefinesMacros.hpp"
#include "ArbitraryWrite.hpp"

#include <windows.h>

int wWinMain(
    UNUSED(_In_ HINSTANCE hInstance),
    UNUSED(_In_opt_ HINSTANCE hPrevInstance),
    UNUSED(_In_ LPWSTR lpCmdLine),
    UNUSED(_In_ int nShowCmd)
)
{
    try
    {
        ArbitraryWrite::exploit();
    }
    CATCH_ALL("Exception caught in Arcane main!")

    return 0;
}