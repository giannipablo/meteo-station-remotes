#ifndef WF_H_
#define WF_H_

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#include "config.h"
#include "SPImem.h"
#include "WFIndexHtml.h"
// const char index_html[] PROGMEM = R"rawliteral(
//      <!DOCTYPE HTML>
//      <html>
//      <head>
//          <title>Meteo Station Config</title>
//          <style>
//              input.text {
//                background-color: linen;
//                height: 30px;
//                width: 300px;
//                border-radius: 5px;
//                border-color: black;
//              }
//              input.submit {
//                background-color: rgb(95, 226, 171);
//                height: 30px;
//                width: 307px;
//                border-radius: 5px;
//                border-color: black;
//              }
//          </style>
//      </head>
//      <body>
//          <form action='/configure' method='POST'>
//              <h1>Meteo Station WiFi and Server Configuration</h1><br>
//              <input class='text' type='text' name='SSID' placeholder='wifi ssid'><br>
//              <br>
//              <input class='text' type='text' name='PASS' placeholder='wifi pass'><br>
//              <br>
//              <input class='text' type='text' name='HOST' placeholder='server url '><br>
//              <br>
//              <input class='text' type='text' name='PATH' placeholder='server path'><br>
//              <br>
//              <input class='submit' type='submit' name='SUBMIT' value='Submit'>
//          </form>
//      </body>
//      </html>)rawliteral";

class WF{
    public:
        void init(void);
        void initAP(void);
        void initWebServer(void);

        void runWebServer(void);

        int wfDisconnect(void);
        int getTime(int* day, int* month, int* year, int* wday, int* hour, int* min, int*seg);
        int getStatus(void);
        String getSignalStrength(void);


    private:
        SMEM smem;

        String ssid;
        String pass;
        
        char ssid_str[30] = "Red Visita";
        char pass_str[20] =  "$visita$$";
        char host_str[50] = "Red Visita";
        char endp_str[50] =  "$visita$$";

        WebServer * server = NULL;
};
#endif
