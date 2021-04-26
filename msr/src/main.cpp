#include <Arduino.h>
#include <HTTPClient.h>

#include "WF.h"
#include "TimeStamp.h"

////////////////////////////////////////////////
// Generl stuff
#define BOARDLED 2
#define CFGBTN 22

byte CONFIG_MODE = 0;

////////////////////////////////////////////////
// WiFi stuff
WF wf;

////////////////////////////////////////////////
// TimeStamp stuff
TimeStamp dts(0, 0, "pool.ntp.org");

////////////////////////////////////////////////
// Timer stuff
volatile uint32_t timedInterruptCounter;

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
 
void IRAM_ATTR onTimer() {
    portENTER_CRITICAL_ISR(&timerMux);
    timedInterruptCounter++;
    portEXIT_CRITICAL_ISR(&timerMux);
}

////////////////////////////////////////////////
// Ext Int stuff
struct ExtSwitch {
    const uint8_t PIN;
    uint32_t extIntCounter;
};
ExtSwitch ExtSwitch1 = {23, 0};

portMUX_TYPE extIntMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR isr() {
    portENTER_CRITICAL_ISR(&extIntMux);
    ExtSwitch1.extIntCounter += 1;
    portEXIT_CRITICAL_ISR(&extIntMux);
}


////////////////////////////////////////////////
// Loop stuff
bool flagStart      = false;
bool flagAnemometer = false;
bool flagPost       = false;

////////////////////////////////////////////////
// Setup
void setup() {
    // Serial port configuration
    Serial.begin(115200);

    // Configuration pin
    pinMode(BOARDLED, OUTPUT);
    pinMode(CFGBTN, INPUT_PULLUP);


    if(digitalRead(CFGBTN)==0){
        CONFIG_MODE = 1;

        ////////////////////////////////////////////////
        // WiFi stuff
        Serial.println("Starting WiFi on AP mode");
        wf.initAP();

        digitalWrite(BOARDLED, HIGH);
    }else{
        CONFIG_MODE = 0;

        ////////////////////////////////////////////////
        // WiFi stuff
        Serial.println("Starting WiFi on Station mode");
        wf.init();

        ////////////////////////////////////////////////
        // TimeStamp stuff
        Serial.println("Synchronizing with NTS");
        Serial.println(dts.getTimeStamp());

        ////////////////////////////////////////////////
        // Timer stuff
        timer = timerBegin(0, 80, true);
        timerAttachInterrupt(timer, &onTimer, true);
        timerAlarmWrite(timer, 100000, true);
        timerAlarmEnable(timer);

        ////////////////////////////////////////////////
        // Ext Int stuff
        pinMode(ExtSwitch1.PIN, INPUT_PULLUP);
        // attachInterrupt(ExtSwitch1.PIN, isr, FALLING);

        digitalWrite(BOARDLED, LOW);
    }


}


////////////////////////////////////////////////
// Loop
void loop() {

    if(CONFIG_MODE==1){
        wf.runWebServer();
    } else {

        // START
        if((timedInterruptCounter==0) & (flagStart==true)){
            // Serial.printf("Init %u counter at\n", timedInterruptCounter);

            attachInterrupt(ExtSwitch1.PIN, isr, FALLING);

            flagStart = false;
        }


        // ANEMOMETER
        if((timedInterruptCounter==20) & (flagAnemometer==true)){
            detachInterrupt(ExtSwitch1.PIN);

            Serial.printf("Two seconds interrupt %u counter at %u\n", timedInterruptCounter, ExtSwitch1.extIntCounter);

            ExtSwitch1.extIntCounter = 0;

            flagAnemometer = false;
        }

        
        // SERVER POST
        if((flagStart==false) & (flagAnemometer==false) & (flagPost==true)){

            if(wf.getStatus()){
                HTTPClient http;
                http.begin(wf.getServerUrl().c_str());

                http.addHeader("Content-Type", "application/json");
                int httpResponseCode = http.POST("{\"api_key\":\"tPmAT5Ab3j7F9\",\"sensor\":\"BME280\",\"value1\":\"24.25\",\"value2\":\"49.54\",\"value3\":\"1005.14\"}");
     
                Serial.print("HTTP Response code: ");
                Serial.println(httpResponseCode);
        
                // Free resources
                http.end();
            } else {
                Serial.println("WiFi Disconnected");
            }
            flagPost = false;
        }


        // ENABLE SAMPLE CYCLE
        if(timedInterruptCounter==200){
            Serial.printf("End %u counter at\n", timedInterruptCounter);  
    
            portENTER_CRITICAL_ISR(&timerMux);
            timedInterruptCounter = 0;
            portEXIT_CRITICAL_ISR(&timerMux);

            flagStart      = true;
            flagAnemometer = true;
            flagPost       = true;
        }
        // Serial.println(dts.getTimeStamp());
    }
}
