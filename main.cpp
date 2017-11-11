#include <stdio.h>
#include <curl/curl.h>
#include <string>
#include <cstring>
#include <sstream>
#include <thread>

#include "TempSensor.h"

using namespace std::chrono_literals;

std::string buildBody(float temp, float humidity) {
    time_t timer;
    time(&timer);

    std::stringstream bodystream;
    bodystream << "{ \"readingDate\" : " << timer << ",\n    \"temperature\" : " << temp <<",\n	\"humidity\" : " << humidity << "\n}";
    return bodystream.str();
}

int putCurrentMeasures(float temp, float humidity) {

    std::string url = "http://thermopi:8080/ThermoPi/CurrentReadings";
    // temp for testing
    // url = "http://192.168.1.13:8080/thermopi-1.0-SNAPSHOT/rest/ThermoPi/CurrentTemperature";

    std::string body = buildBody(temp, humidity);

    CURL *hnd = curl_easy_init();

    curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "PUT");
    curl_easy_setopt(hnd, CURLOPT_URL, url.c_str());

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "cache-control: no-cache");
    headers = curl_slist_append(headers, "content-type: application/json");
    curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, body.c_str());

    CURLcode res = curl_easy_perform(hnd);

    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
    }

    curl_easy_cleanup(hnd);

    return res;
}

int main() {
    float temp;
    float humidity;
    int success;
    bool stopRequested = false;

    while(!stopRequested) {
        temp = 0;
        humidity = 0;

        success = TempSensor::readSensorData(temp, humidity);

        if(success) {
            putCurrentMeasures(temp, humidity);
            std::this_thread::sleep_for(60s);
        } else {
            // Measurement failed. Wait a few seconds and try again.
            std::this_thread::sleep_for(5s);
        }
    }

    return 0;
}
