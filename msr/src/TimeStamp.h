#ifndef TIMESTAMP_H_
#define TIMESTAMP_H_

#include <Arduino.h>
#include <time.h>

class TimeStamp{
    public:
        // TimeStamp();

        TimeStamp(long gmtOffset_sec, 
                  int daylightOffset_sec,
                  const char * ntpServer);

        String getTimeStamp(void);

    private:
        long gmtOffset_sec;
        int daylightOffset_sec;
        const char * ntpServer;

};
#endif //TIMESTAMP_H_