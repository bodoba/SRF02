/* *********************************************************************************** */
/*                                                                                     */
/*  Change I2C address of SRF02 sonar sensor                                           */
/*                                                                                     */
/* *********************************************************************************** */
/*                                                                                     */
/*  Copyright 2025 by Bodo Bauer <bb@bb-zone.com>                                      */
/*                                                                                     */
/*  This program is free software: you can redistribute it and/or modify               */
/*  it under the terms of the GNU General Public License as published by               */
/*  the Free Software Foundation, either version 3 of the License, or                  */
/*  (at your option) any later version.                                                */
/*                                                                                     */
/*  This program is distributed in the hope that it will be useful,                    */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of                     */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                      */
/*  GNU General Public License for more details.                                       */
/*                                                                                     */
/*  You should have received a copy of the GNU General Public License                  */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.              */
/* *********************************************************************************** */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <wiringPi.h>
#include <wiringPiI2C.h>

#define SRF02_COMMAND_REG    0x00

int sendCmd(int fd, int cmd) {
    printf("  Writing 0x%02x to register 0x%02x\n", cmd, SRF02_COMMAND_REG);
    if (wiringPiI2CWriteReg8(fd, SRF02_COMMAND_REG, cmd) == -1) {
        fprintf( stderr, "  ERROR: Failed to write 0x%02x to register 0x%02x\n", cmd, SRF02_COMMAND_REG);
        return -1;
    }
    usleep(50000);
    return 0;
}

int openDevice(int addr) {
    int dev = wiringPiI2CSetup(addr);

    if (dev >= 0) {
        // check is the device is actually present by trying to read the command register
        int val = wiringPiI2CReadReg8(dev, SRF02_COMMAND_REG);
        if (val<0) dev=-1;
    }
    return dev;
}

int main( int argc, char* argv[]) {
    int oldAddr, newAddr;

    // require commandline parameters: "old address" "new address" in hex notation  
    if (argc==3) {
        // parse commandline
        sscanf(argv[1], "%x", &oldAddr);    
        sscanf(argv[2], "%x", &newAddr);
    } else {
        // provide help
        printf("\nPlease provide the okd and the new address of the device in hex notation (7-bit) on the command line!\n\n");
        printf("For example:\n");
        printf("\t%s 0x70 0x72\n\n", argv[0]);
        printf("will expect the old addres to be 0x70 and change it to 0x72\n\n");
        exit(1);
    }

    // Initialize WiringPi
    if (wiringPiSetup() == -1) {
        fprintf( stderr, "ERROR: wiringPi setup failed\n");
        exit(1);
    }

    // initialize sensor
    int dev = openDevice(oldAddr);
    if (dev < 0) {
        fprintf( stderr, "ERROR: SRF02 not found at 0x%02x\n", oldAddr);
        exit(1);
    }
    
    printf("\n*** Changing SRF02' I2C address from 0x%02x to 0x%02x\n\n", oldAddr, newAddr);
    
    // prepare for address change
    if (sendCmd(dev, 0xA0) != 0) return 1;
    if (sendCmd(dev, 0xAA) != 0) return 1;
    if (sendCmd(dev, 0xA5) != 0) return 1;

    // send new address
    int newAddr8 = (newAddr<<1);
    if (sendCmd(dev, newAddr8) != 0) return 1;

    printf("\n*** Address change sequence complete\n\n");

    printf("  Check if device reacts to new address 0x%02x\n");
    
    int devNew = openDevice(newAddr);
    if (devNew < 0) {
        fprintf( stderr, "  -> SRF02 not found at 0x%02x\n", newAddr);
        printf("\n*** Address change failed!\n\n");
    } else {
        printf("  -> The new device address is working\n", newAddr);
        printf("\n*** Address change was successful\n\n");
    }
}