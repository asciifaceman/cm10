/*
 *
 * 
 */
#include <SimpleTimer.h>

// Import configuration
#include "ppqn.h"
#include "config.h"

// timer instantiation
SimpleTimer timer;

/*
 * Global Definitions
 */
const int analogReadCount = 10;


// Bootup configuration
void setup(){
    Serial.begin(115200);
    Serial.println("Booted...");
}

int analogFilteredRead(int port) {
    if (NOIO) {
        return 1023/2;
    }
    int aggregate = 0;
    for (int i = 0; i < analogReadCount; i++){
        aggregate += analogRead(port);
    }
    return (int) aggregate / analogReadCount;
}