#ifndef DHTSENSOR_H
#define DHTSENSOR_H
#include <DHT.h>
struct DHTSensorData
{
    float humidity;
    float temparature;
};


DHTSensorData getDHTData(DHT &sensor) {
    DHTSensorData data;
    data.humidity = sensor.readHumidity();
    data.temparature = sensor.readTemperature();
    return data;
}

bool checkDHTData(DHTSensorData data) {
    return !isnan(data.temparature) && !isnan(data.temparature);
}

#endif