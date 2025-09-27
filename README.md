# Support for SRF02 sonar sensor with wiringPi in plain C

## Support functions to measure distance using the SRF02 sensor

This is a very simple interface to access a SRF02 sonar sensor over I2C using the [wiringPi library](https://github.com/WiringPi/WiringPi) as a backend.

Usage is straight forward (see `sonarping.c`):

```C
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
```

This will show the current readings as soon as they are available:

```bash
$ ./sonarping 0x70
Distance:   14 cm (20 ms)
Distance:  258 cm (30 ms)
Distance:  213 cm (30 ms)
Distance:  213 cm (10 ms)
Distance:  259 cm (30 ms)
Distance:  258 cm (30 ms)
Distance:  258 cm (30 ms)
Distance:  256 cm (10 ms)
Distance:   16 cm (10 ms)
Distance:   14 cm (10 ms)
Distance:  258 cm (10 ms)
Distance:   14 cm (10 ms)
```

## Chaning the I2C address of the sensor

The `set_address` application lets you change the i2c address of the sensor.

```bash
set_address <old address> <new address>
```

Where `old address` is the current I2C address of the sensor and `new address` is the desired address. Use 7-bit notation for both addresses. For example:

```bash
$ ./set_address 0x74 0x70

*** Changing SRF02' I2C address from 0x74 to 0x70

  Writing 0xa0 to register 0x00
  Writing 0xaa to register 0x00
  Writing 0xa5 to register 0x00
  Writing 0xe0 to register 0x00

*** Address change sequence complete

  Check if device reacts to new address 0x00
  -> The new device address is working

*** Address change was successful
```

The tool will
* check is the sensor is accessible at the old address
* reprogramm the adress setting
* check if the new address is reachable
