/* *********************************************************************************** */
/*                                                                                     */
/*  Send pings with a SRF02 sonar sensor and print the retured distance                */
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

#include "SRF02.h"

#include <stdlib.h>
#include <stdio.h>

#define ADDR 0x70 // default address for SRF02

int main( int argc, char* argv[]) {
    int addr=ADDR;

    // allow device address as optional commandline parameter in hex notation
    if (argc==2) {
        sscanf(argv[1], "%x", &addr);
    }
    // initialize sensor‚
    int srf02Device = srf02Init(addr);
    if (srf02Device < 0) {
        fprintf( stderr, "ERROR: SRF02 not found at 0x%02x\n", addr);
        exit(1);
    }

    for ( ;; ) {
        // request distance in cm
        srf02Ping(srf02Device, SRF02_CENTIMETERS);
        
        // poll device until valid reading is returned, or timeout (100ms) expired
        int timeout=10; // 10*10ms -> break after 100ms
        int distance = -1;
        do {
            distance = srf02Read(srf02Device);
            delay(10);
            timeout--;
        } while (timeout > 0 && distance < 0);

        if (distance >= 0) {
            printf("Distance: % 4d cm (%d ms)\n", distance, (100-timeout*10));
        } else {
            printf("No valid reading returned after % 4d ms\n",  (100-timeout*10) );
        }
    }
}