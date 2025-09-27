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

#include "SRF02.h"

#define OLD_ADDR 0x70  // SRF02 default address
#define NEW_ADDR 0x74  // New address: 0xE0 to 0xFE, only even numbers

int sendCmd(int fd, int cmd) {
    if (wiringPiI2CWriteReg8(fd, SRF02_COMMAND_REG, cmd) == -1) {
        fprintf( stderr, "ERROR: Failed to write 0x%02x to register 0x%02x\n", cmd, SRF02_COMMAND_REG);
        return -1;
    }
    usleep(100000);
    return 0;
}

int main( int argc, char* argv[]) {
    int oldAddr = OLD_ADDR;
    int newAddr = NEW_ADDR;


    // allow optional commandline parameters: "old address" "new address" in hex notation  
    if (argc==2 || argc==3) {
        sscanf(argv[1], "%x", &oldAddr);
    }
    if (argc==3) {
        sscanf(argv[2], "%x", &newAddr);
    }

    // Initialize WiringPi
    if (wiringPiSetup() == -1) {
        fprintf( stderr, "ERROR: wiringPi setup failed\n");
        exit(1);
    }

    // initialize sensor
    int srf02Device = srf02Init(oldAddr);
    if (srf02Device < 0) {
        fprintf( stderr, "ERROR: SRF02 not found at 0x%02x\n", oldAddr);
        exit(1);
    }

    printf("Changing SRF02' I2C address from 0x%02x to 0x%02x\n", oldAddr, newAddr);
    
    // prepare for address change
    if (sendCmd(srf02Device, 0xA0) != 0) return 1;
    if (sendCmd(srf02Device, 0xAA) != 0) return 1;
    if (sendCmd(srf02Device, 0xA5) != 0) return 1;

    // send new address
    if (sendCmd(srf02Device, newAddr) != 0) return 1;

    printf("\n*** Address change complete! ***\n\n");
    printf("The new device address is: 0x%02x\n", newAddr);
}