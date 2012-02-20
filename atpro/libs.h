#ifndef LIBS_H
#define LIBS_H

// ASM Patch
#define MAKE_JUMP(f) (0x08000000 | (((u32)(f) >> 2) & 0x03ffffff))
#define NOP 0

// ASM Redirect Patch
#define REDIRECT_FUNCTION(new_func, original) \
	do { \
		_sw(MAKE_JUMP((u32)new_func), ((u32)original)); \
		_sw(NOP, ((u32)original)+4); \
	} while ( 0 )

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

int hook_import_bynid(SceModule *pMod, char *library, unsigned int nid, void *func);

#endif
