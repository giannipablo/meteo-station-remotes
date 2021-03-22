#include "TimeStamp.h"

// TimeStamp::TimeStamp(){
//     this->gmtOffset_sec = 0;
//     this->daylightOffset_sec = 0;
//     this->ntpServer = (char *) "pool.ntp.org";
//     configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
// }

TimeStamp::TimeStamp(long gmtOffset_sec, int daylightOffset_sec, const char * ntpServer){
    this->gmtOffset_sec = gmtOffset_sec;
    this->daylightOffset_sec = daylightOffset_sec;
    this->ntpServer = ntpServer;
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

String TimeStamp::getTimeStamp(void){
    char tmOut[30];
    struct tm timeinfo;
    
    if(!getLocalTime(&timeinfo)){
        Serial.println("Failed to obtain time");
        sprintf(tmOut,"N/A");
        return tmOut;
    }
    sprintf(tmOut,"%d-%d-%d-%d-%d-%d",timeinfo.tm_year + 1900,
                                      timeinfo.tm_mon + 1,
                                      timeinfo.tm_mday,
                                      timeinfo.tm_hour,
                                      timeinfo.tm_min,
                                      timeinfo.tm_sec);
  
  return String(tmOut);
}