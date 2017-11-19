//
// Created by david on 03/11/17.
//

#ifndef RESTTEST_TEMPSENSOR_H
#define RESTTEST_TEMPSENSOR_H

class TempSensor {

public:
    static int init();
    static int readSensorData(float &temp, float &humidity);
};


#endif //RESTTEST_TEMPSENSOR_H
