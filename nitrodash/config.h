#ifndef CONFIG_H
#define CONFIG_H

#endif // CONFIG_H

//#define DESKTOP

#ifdef DESKTOP
    #define GPSFILE "/home/chris/Desktop/gpsLocation.json"
#else
    #define GPSFILE "/tmp/gpsLocation.json"
#endif

#ifdef DESKTOP
    #define UBLOXPORT "/dev/ttyACM0"
#else
    #define UBLOXPORT "/dev/ttymxc3"
#endif
