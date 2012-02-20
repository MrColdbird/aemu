#ifndef _PSPNET_H_
#define _PSPNET_H_

// Error Codes
#define NET_NO_SPACE 0x80410001
#define NET_INTERNAL 0x80410002
#define CORE_INVALID_ARG 0x80410103

/**
 * Query Local MAC from System
 * @param addr OUT: MAC Address
 * @return 0 on success or... CORE_INVALID_ARG
 */
int sceNetGetLocalEtherAddr(SceNetEtherAddr * addr);

/**
 * Switch Host Byte Order Short to Network Byte Order
 * @param host16 LE Short
 * @return BE Short
 */
static __inline__ uint16_t sceNetHtons(uint16_t host16)
{
    return __builtin_allegrex_wsbh(host16);
}

/**
 * Switch Network Byte Order Short to Host Byte Order
 * @param net16 BE Short
 * @return LE Short
 */
static __inline__ uint16_t sceNetNtohs(uint16_t net16)
{
    return __builtin_allegrex_wsbh(net16);
}

/**
 * Switch Host Byte Order Long to Network Byte Order
 * @param host32 LE Long
 * @return BE Long
 */
static __inline__ uint32_t sceNetHtonl(uint32_t host32)
{
    return __builtin_allegrex_wsbw(host32);
}

/**
 * Switch Network Byte Order Long to Host Byte Order
 * @param net32 BE Long
 * @return LE Long
 */
static __inline__ uint32_t sceNetNtohl(uint32_t net32)
{
    return __builtin_allegrex_wsbw(net32);
}

#endif
