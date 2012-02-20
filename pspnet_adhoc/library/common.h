#ifndef _ADHOC_COMMON_H_
#define _ADHOC_COMMON_H_

// --- Basic Includes ---

// Basic Types
#include <stdint.h>

// Heap Control
#include <malloc.h>

// String Handling
#include <string.h>

// Time Access
#include <time.h>

// PSP Utilities (like Mersenne Twister)
#include <psputils.h>

// PSP Module Loader
#include <psputility_modules.h>

// --- Kernel Debugger ---

// Kernel Debug Printer
#include "../../atpro/logs.h"

// --- Emulator Basics ---

// Adhoc Library Types
#include "structures.h"

// Adhoc Global Data
#include "globals.h"

// Adhoc Common Functions
#include "functions.h"

// --- Improved PSPSDK Header ---

// Adhoc Network Control Library
#include "pspsdk/pspnet_adhocctl.h"

// Internet Library
#include "pspsdk/pspnet_inet.h"

// Networking Library
#include "pspsdk/pspnet.h"

// --- Global Library Calls ---

// sceNetAdhocInit
#include "calls/init.h"

// sceNetAdhocTerm
#include "calls/term.h"

// sceNetAdhocSetSocketAlert
#include "calls/setalert.h"

// sceNetAdhocGetSocketAlert
#include "calls/getalert.h"

// --- PDP / UDP Related Calls ---

// sceNetAdhocPollSocket
#include "calls/pdp/poll.h"

// sceNetAdhocPdpCreate
#include "calls/pdp/create.h"

// sceNetAdhocPdpDelete
#include "calls/pdp/delete.h"

// sceNetAdhocPdpSend
#include "calls/pdp/send.h"

// sceNetAdhocPdpRecv
#include "calls/pdp/recv.h"

// sceNetAdhocGetPdpStat
#include "calls/pdp/getstat.h"

// --- PTP / TCP Related Calls ---

// sceNetAdhocPtpOpen
#include "calls/ptp/open.h"

// sceNetAdhocPtpConnect
#include "calls/ptp/connect.h"

// sceNetAdhocPtpListen
#include "calls/ptp/listen.h"

// sceNetAdhocPtpAccept
#include "calls/ptp/accept.h"

// sceNetAdhocPtpSend
#include "calls/ptp/send.h"

// sceNetAdhocPtpRecv
#include "calls/ptp/recv.h"

// sceNetAdhocPtpFlush
#include "calls/ptp/flush.h"

// sceNetAdhocPtpClose
#include "calls/ptp/close.h"

// sceNetAdhocGetPtpStat
#include "calls/ptp/getstat.h"

// --- Game Mode / Token Ring Related Calls ---

// sceNetAdhocGameModeCreateMaster
#include "calls/gamemode/createmaster.h"

// sceNetAdhocGameModeCreateReplica
#include "calls/gamemode/createreplica.h"

// sceNetAdhocGameModeUpdateMaster
#include "calls/gamemode/updatemaster.h"

// sceNetAdhocGameModeUpdateReplica
#include "calls/gamemode/updatereplica.h"

// sceNetAdhocGameModeDeleteMaster
#include "calls/gamemode/deletemaster.h"

// sceNetAdhocGameModeDeleteReplica
#include "calls/gamemode/deletereplica.h"

#endif
