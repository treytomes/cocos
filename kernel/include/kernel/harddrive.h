#ifndef __HARDDRIVE_H__
#define __HARDDRIVE_H__

#include <stdint.h>

#define ATADEV_UNKNOWN 0
#define ATADEV_PATAPI 1
#define ATADEV_SATAPI 2
#define ATADEV_PATA 3
#define ATADEV_SATA 4

/**
 * On Primary bus: base =0x1F0,dev_ctl =0x3F6.
 */
uint8_t harddrive_detect_devtype(int slavebit, uint16_t base, uint16_t dev_ctl);

/**
 * Sector is in LBA28 format.
 */
uint8_t* harddrive_load_sector(uint32_t sector);

#endif
