#pragma once

#if defined(BUILD_DS3)
#include "DS3.h"
namespace Game = DS3;
#elif defined(BUILD_ELDENRING)
#include "EldenRing.h"
using Game = EldenRing;
#elif defined(BUILD_DS2SOTFS)
#include "DS2Sotfs.h"
namespace Game = DS2Sotfs;
#elif defined(BUILD_DSR)
#include "DSR.h"
namespace Game = DSR;
#endif
