/* *********************************************************************************** */
/*                                                                                     */
/*  Controll of a SRF02 sonar sensor                                                   */
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

int srf02Init(int addr) {
    int dev = wiringPiI2CSetup(addr);
    
    if (dev >= 0) {
       // check is the device is actually present by trying to read the command register
        int val = wiringPiI2CReadReg8(dev, SRF02_COMMAND_REG);
        if (val<0)  dev=-1;
    }
    return dev;
}

void srf02Ping(int dev, int mode) {
    wiringPiI2CWriteReg8(dev, SRF02_COMMAND_REG, mode);    
}

int srf02Read(int dev) {
    int distance = -1;
    int msb, lsb;

    msb = wiringPiI2CReadReg8(dev, SRF02_RANGE_REG_1);
    lsb = wiringPiI2CReadReg8(dev, SRF02_RANGE_REG_2);

    if ( (msb != 0xFF) && (lsb != 0xFF)) {
        distance = (msb << 8) | lsb;
    }
    return distance;
}