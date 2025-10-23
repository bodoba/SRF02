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
#ifndef SONARSRF02_H
#define SONARSRF02_H

#include <wiringPi.h>
#include <wiringPiI2C.h>

// Measurement modes
#define SRF02_INCHES         0x50
#define SRF02_CENTIMETERS    0x51
#define SRF02_MICROSECONDS   0x52

// Register set
#define SRF02_COMMAND_REG    0x00
#define SRF02_RANGE_REG_1    0x02
#define SRF02_RANGE_REG_2    0x03

/* *********************************************************************************** *
 * @brief Setup SRF02 sensor over I2C
 * @param addr   sensor address
 * @return       device handle, or -1 on failure
 * *********************************************************************************** */
int srf02Init(int addr);

/* *********************************************************************************** *
 * @brief Send out sonar ping
 * Get the measuremend with srf02Read(), but wait at lease 70ms before to give the 
 * sensor time for the measurement
 * @param dev   Device handle
 * @param mode  Operation mode, defines the return unit (cm, IN, ms)
 * *********************************************************************************** */
void srf02Ping(int dev, int mode);

/* *********************************************************************************** *
 * @brief Read last measurement
 * Requires that a srf02Ping has been sent at least 65ms ago. 
 * @param dev Device hanlde
 * @return measured distance, or -1 if reading has been blocked by the device 
 * *********************************************************************************** */
int srf02Read(int dev);

/* *********************************************************************************** *
 * @brief Get distance
 * Sends a ping and polls sensor for distance reading. A maximum timeout will be
 * respected and the remining timout time will be returned.
 * @param dev Device hanlde
 * @param *timeout Maximum time in ms to wait for the measurement. If the sensor 
 *                 returns a qucker reply, the remaining time will be in the variable. 
 *                 Sensor shoudl return a value within 70ms
 * @return measured distance, or -1 no ping reply was received  
 * *********************************************************************************** */
int srf02GetDistance(int dev, int mode, int *timeout);

#endif