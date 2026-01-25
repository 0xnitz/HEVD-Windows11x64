#pragma once

#include "DefinesMacros.hpp"

namespace ArbitraryWrite
{

typedef struct
{
	Address64 What;
	Address64 Where;
} WriteWhatWhere;

void exploit();

}