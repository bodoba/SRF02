# Support for SRF02 sonar sensor with wiringPi in plain C

This is a very simple interface to access a SRF02 sonar sensor over I2C using the [wiringPi library](https://github.com/WiringPi/WiringPi) as a backend. 

Usage is straight forward (sonarping.c):

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
pi@raspberrypi:~/SRF02/build $ ./sonarping 0x70
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

The `i2chelper` functions reassemble some convenience methods from `I2Cdev` and are used internally.