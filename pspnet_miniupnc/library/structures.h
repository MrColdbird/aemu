#ifndef _ADHOC_STRUCTURES_H_
#define _ADHOC_STRUCTURES_H_

// Ethernet Address
#define ETHER_ADDR_LEN 6
typedef struct SceNetEtherAddr {
	uint8_t data[ETHER_ADDR_LEN];
} SceNetEtherAddr;

#endif
