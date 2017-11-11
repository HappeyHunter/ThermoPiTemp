//
// Created by david on 03/11/17.
//

#include "TempSensor.h"
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define DHT_PULSES      41
#define DHT_PIN         7       /* GPIO-22 */

int TempSensor::readSensorData(float &temp, float &humidity) {
    uint8_t laststate       = HIGH;
    uint8_t counter         = 0;
    uint8_t j               = 0, i;

    int pulseCounts[DHT_PULSES*2] = {0};

    pinMode( DHT_PIN, OUTPUT );

    digitalWrite( DHT_PIN, HIGH );
    delay( 500 );
    digitalWrite( DHT_PIN, LOW );
    delay( 20 );

    /* prepare to read the pin */
    pinMode( DHT_PIN, INPUT );

    /* detect change and read data */
    for ( i = 0; i < DHT_PULSES*2+1; i++ )
    {
        counter = 0;
        while ( digitalRead( DHT_PIN ) == laststate )
        {
            counter++;
            delayMicroseconds( 1 );
            if ( counter == 255 )
            {
                break;
            }
        }
        laststate = digitalRead( DHT_PIN );

        if ( !i ) {
            continue;
        }

        if ( counter == 255 )
            break;

        pulseCounts[i-1] = counter;
    }

    // Compute the average low pulse width to use as a 50 microsecond reference threshold.
    // Ignore the first two readings because they are a constant 80 microsecond pulse.
    uint32_t threshold = 0;
    for (int i=2; i < DHT_PULSES*2; i+=2) {
        threshold += pulseCounts[i];
    }
    threshold /= DHT_PULSES-1;

    uint8_t data[5] = {0};
    for (int i=3; i < DHT_PULSES*2; i+=2) {
        int index = (i-3)/16;
        data[index] <<= 1;
        if (pulseCounts[i] >= threshold) {
            // One bit for long pulse.
            data[index] |= 1;
        }
        // else zero bit for short pulse.
    }

    /*
     * check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
     * print it out if data is good
     */
    if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
        humidity = (float)((data[0] * 256) + data[1]) / 10.0f;
        if ( humidity > 100 )
        {
            humidity = data[0];
        }
        float temp = (float)(((data[2] & 0x7F) * 256) + data[3]) / 10.0f;
        if ( temp > 125 )
        {
            temp = data[2];    // for DHT11
        }
        if ( data[2] & 0x80 )
        {
            temp = -temp;
        }
        //printf( "Humidity = %.1f %% Temperature = %.1f *C\n", h, c );

        return 0;
    } else {
        return 1;
    }
}