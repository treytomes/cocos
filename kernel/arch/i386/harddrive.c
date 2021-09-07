#include <kernel/harddrive.h>
#include <kernel/util.h>
#include <stdlib.h>

#define REG_CYL_LO 4
#define REG_CYL_HI 5
#define REG_DEVSEL 6

/*
Status Register (I/O base + 7)
Bit	Abbreviation	Function
0	ERR	Indicates an error occurred. Send a new command to clear it (or nuke it with a Software Reset).
1	IDX	Index. Always set to zero.
2	CORR	Corrected data. Always set to zero.
3	DRQ	Set when the drive has PIO data to transfer, or is ready to accept PIO data.
4	SRV	Overlapped Mode Service Request.
5	DF	Drive Fault Error (does not set ERR).
6	RDY	Bit is clear when drive is spun down, or after an error. Set otherwise.
7	BSY	Indicates the drive is preparing to send/receive data (wait for it to clear). In case of 'hang' (it never clears), do a software reset.
*/

#include <stdio.h>
void ata_soft_reset(uint16_t base, uint16_t dev_ctl) {
	//uint8_t value = inportb(dev_ctl + 0);
	//value = value | 0x04; // Set bit 2.
	//outportb(dev_ctl + 0, value);
	//value = value & 0xFB; // Clear bit 2.
	//outportb(dev_ctl + 1, value);

	outportb(dev_ctl, 0x4);
	inportb(dev_ctl);			/* wait 400ns for software reset to work */
	inportb(dev_ctl);
	inportb(dev_ctl);
	inportb(dev_ctl);
    
    outportb(dev_ctl, 0x0);
	inportb(dev_ctl);			/* wait 400ns for software reset to work */
	inportb(dev_ctl);
	inportb(dev_ctl);
	inportb(dev_ctl);

    while (1) {
        uint8_t al = inportb(dev_ctl); //base + 7);
        if ((al & 0xC0) == 0x40) {
            // Wait for BSY clear and RDY set.
            break;
        }
    }
}

uint8_t harddrive_detect_devtype(int slavebit, uint16_t base, uint16_t dev_ctl) {
	outportb(base + REG_DEVSEL, 0xA0 | slavebit<<4);
	inportb(dev_ctl);			/* wait 400ns for drive select to work */
	inportb(dev_ctl);
	inportb(dev_ctl);
	uint8_t al = inportb(dev_ctl);
    if (al == 0) {
        return ATADEV_UNKNOWN;
    }

	ata_soft_reset(base, dev_ctl);		/* waits until master drive is ready again */

	unsigned cl=inportb(base + REG_CYL_LO);	/* get the "signature bytes" */
	unsigned ch=inportb(base + REG_CYL_HI);
 
	/* differentiate ATA, ATAPI, SATA and SATAPI */
	if (cl==0x14 && ch==0xEB) return ATADEV_PATAPI;
	if (cl==0x69 && ch==0x96) return ATADEV_SATAPI;
	if (cl==0 && ch == 0) return ATADEV_PATA;
	if (cl==0x3c && ch==0xc3) return ATADEV_SATA;
    //printf("\r\ncl=%d, ch=%d\r\n", cl, ch);
	return ATADEV_UNKNOWN;
}

#define SECTOR_SIZE (256 * 2)
uint8_t* harddrive_load_sector(uint32_t sector) {
    uint8_t slavebit = 0;
    uint8_t sectorCount = 1;
    outportb(0x1F6, 0xE0 | (slavebit << 4) | ((sector >> 24) & 0x0F));
    outportb(0x1F1, 0x00);
    outportb(0x1F2, sectorCount);
    outportb(0x1F3, (uint8_t)(sector & 0xFF));
    outportb(0x1F4, (uint8_t)((sector >> 8) & 0xFF));
    outportb(0x1F5, (uint8_t)((sector >> 16) & 0xFF));
    outportb(0x1F7, 0x20);

    while ((inportb(0x1F7) & 0x8) == 0) {
        // Wait until bit 3 is set?
    }

	uint8_t* data = (uint8_t*)malloc(SECTOR_SIZE);
	int d = 0;

    for (int n = 0; n < 256; n++) {
        uint16_t value = inportw(0x1F0);
		data[d++] = value & 0xFF;
		data[d++] = (value >> 8);
        //printf("%c%c", value & 0xFF, (value >> 8));
        //if ((n % 20) == 0) {
        //    printf("\r\n");
        //}
    }

	return data;
}