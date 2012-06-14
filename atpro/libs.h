/*
 * This file is part of PRO ONLINE.

 * PRO ONLINE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * PRO ONLINE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PRO ONLINE. If not, see <http://www.gnu.org/licenses/ .
 */

#ifndef LIBS_H
#define LIBS_H

#include "systemctrl.h"

// ASM Patch
#define MAKE_CALL(f) (0x0C000000 | (((u32)(f) >> 2) & 0x03ffffff))
#define MAKE_JUMP(f) (0x08000000 | (((u32)(f) >> 2) & 0x03ffffff))
#define MAKE_SYSCALL(n) ((n<<6)|12)
#define NOP 0

// JAL Target Getter
#define GET_CALL_ADDR(i) ((i & 0x03ffffff) << 2)

// ASM Redirect Patch
#define REDIRECT_FUNCTION(new_func, original) \
	do { \
		_sw(MAKE_JUMP((u32)new_func), ((u32)original)); \
		_sw(NOP, ((u32)original)+4); \
	} while ( 0 )

// ASM Dummy Function Patch
#define MAKE_DUMMY_FUNCTION_RETURN_0(a) do {\
	_sw(0x03E00008, a);\
	_sw(0x00001021, a + 4);\
} while (0)

typedef struct
{
	const char *name;
	unsigned short version;
	unsigned short attribute;
	unsigned char entLen;
	unsigned char varCount;
	unsigned short funcCount;
	unsigned int *fnids;
	unsigned int *funcs;
	unsigned int *vnids;
	unsigned int *vars;
} PspModuleImport;

PspModuleImport *find_import_lib(SceModule *pMod, char *library);

unsigned int find_import_bynid(SceModule *pMod, char *library, unsigned int nid);

void api_hook_addr(int addr, void *func);
void api_hook_import(int addr, void *func);

int hook_global_module_jal(SceModule2 * mod, uint32_t oldcall, uint32_t newcall);
int hook_weak_user_bynid(SceModule2 * mod, char * library, unsigned int nid, void * func);
int hook_import_bynid(SceModule *pMod, char *library, unsigned int nid, void *func);

#endif
