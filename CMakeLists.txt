# *********************************************************************************** #
#                                                                                     #
#  Simple interface to SRF02 sonar sensor                                             #
#                                                                                     #
# *********************************************************************************** #
#                                                                                     #
#  Copyright 2025 by Bodo Bauer <bb@bb-zone.com>                                      #
#                                                                                     #
#  This program is free software: you can redistribute it and/or modify               #
#  it under the terms of the GNU General Public License as published by               #
#  the Free Software Foundation, either version 3 of the License, or                  #
#  (at your option) any later version.                                                #
#                                                                                     #
#  This program is distributed in the hope that it will be useful,                    #
#  but WITHOUT ANY WARRANTY; without even the implied warranty of                     #
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                      #
#  GNU General Public License for more details.                                       #
#                                                                                     #
#  You should have received a copy of the GNU General Public License                  #
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.              #
# *********************************************************************************** #

cmake_minimum_required(VERSION 3.10.0)
project(TankBot VERSION 0.1.0 LANGUAGES C)

include_directories(/opt/local/include)
link_directories(/opt/local/lib)

add_executable(sonarping   sonarping.c   SRF02.c  SRF02.h)
target_link_libraries(sonarping wiringPi)

add_executable(set_address set_address.c)
target_link_libraries(set_address wiringPi)
