#ifndef _MINIUPNP_COMMON_H_
#define _MINIUPNP_COMMON_H_

// --- Basic Includes ---

// Input & Output
#include <stdio.h>

// Basic Types
#include <stdint.h>

// Standard Library
#include <stdlib.h>

// Heap Control
#include <malloc.h>

// String Handling
#include <string.h>

// Time Access
#include <time.h>

// WLAN Switch Access
#include <pspwlan.h>

// Access Point Control
#include <pspnet_apctl.h>

// Netconfig Utility
#include <psputility.h>

// --- Kernel Debugger ---

// Kernel Debug Printer
#include "../../atpro/logs.h"

// --- Library Stuff ---

// Structures
#include "structures.h"

// Globals
#include "globals.h"

// --- Improved PSPSDK Header ---

// Internet Library
#include "../../pspnet_adhoc/library/pspsdk/pspnet_inet.h"

// Networking Library
#include "../../pspnet_adhoc/library/pspsdk/pspnet.h"

// DNS Resolver
#include "../../pspnet_adhocctl/library/pspsdk/pspnet_resolver.h"

#endif
