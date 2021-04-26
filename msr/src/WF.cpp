#include "WF.h"

////////////////////////////////////////////////////////////////////////////////
// Funcion: init                                                              //
// Descripción: inicializa el cliente de thingSepak                           //
////////////////////////////////////////////////////////////////////////////////
void WF::init(void)
{
    smem.init();

    int i = 0;
    WiFi.mode(WIFI_STA);
    sprintf(ssid_str, "%s", smem.readFile(SPIFFS, "/SSID.txt").c_str());
    sprintf(pass_str, "%s", smem.readFile(SPIFFS, "/PASS.txt").c_str());
    sprintf(host_str, "%s", smem.readFile(SPIFFS, "/HOST.txt").c_str());
    sprintf(path_str, "%s", smem.readFile(SPIFFS, "/PATH.txt").c_str());

    Serial.println("Connecting to WiFi network: " + String(ssid_str) + "..");
    WiFi.begin(ssid_str, pass_str);
    while ((WiFi.status() != WL_CONNECTED) && (i < 50))
    {
        i++;
        delay(1000);
        Serial.print(".");
    }
    if (WiFi.status() == WL_CONNECTED)
        Serial.println("Connected");
    else
        Serial.println("Not Connected");

    Serial.print("Host IP: ");
    Serial.println(WiFi.localIP());
}

////////////////////////////////////////////////////////////////////////////////
// Funcion: init                                                              //
// Descripción: inicializa el cliente de thingSepak                           //
////////////////////////////////////////////////////////////////////////////////
void WF::initAP(void)
{
    smem.init();

    WiFi.disconnect(true); // Desconecto WiFi

    if (!WiFi.softAP("MeteoStationConfig", "123456789"))
    {
        Serial.println("Falla AP");
    }

    delay(1000);

    if (!WiFi.softAPConfig(IPAddress(192, 168, 169, 100), IPAddress(192, 168, 169, 100), IPAddress(255, 255, 255, 0)))
    {
        Serial.println("STA Failed to configure");
    }

    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());

    this->initWebServer();
}

////////////////////////////////////////////////////////////////////////////////
// Funcion: initWebServer                                                     //
// Descripción: Starts the web server to register WiFi credentials            //
////////////////////////////////////////////////////////////////////////////////
void WF::initWebServer(void)
{
    server = new WebServer;
    if(server){
        server->on("/", [&](){
            if(server->method()==HTTP_POST){
                smem.writeFile(SPIFFS, "/SSID.txt", server->arg("SSID").c_str());
                smem.writeFile(SPIFFS, "/PASS.txt", server->arg("PASS").c_str());
                smem.writeFile(SPIFFS, "/HOST.txt", server->arg("HOST").c_str());
                smem.writeFile(SPIFFS, "/PATH.txt", server->arg("PATH").c_str());
                server->send(200, "text/html", done_html);
            } else {
                server->send(200, "text/html", index_html);
            }
        });
        server->begin(80);
        Serial.println("WebServer configured...");
    }
}

////////////////////////////////////////////////////////////////////////////////
// Funcion: runWebServer                                                      //
// Descripción: Runs the web server to register WiFi credentials              //
////////////////////////////////////////////////////////////////////////////////
void WF::runWebServer(void)
{
    if(server){
        // Serial.println("WebServer up and running...");
        server->handleClient();
    }
}

////////////////////////////////////////////////////////////////////////////////
// Funcion: getTime                                                           //
// Descripción: Obtiene fecha y hora por internet                             //
////////////////////////////////////////////////////////////////////////////////
int WF::getTime(int *day, int *month, int *year, int *wday, int *hour, int *min, int *seg)
{
    struct tm timeinfo;
    configTime(0, 0, "pool.ntp.org");

    if (!getLocalTime(&timeinfo))
    {
        return false;
    }
    *day = timeinfo.tm_mday;
    *month = timeinfo.tm_mon;
    *year = timeinfo.tm_year + 1900;
    *hour = timeinfo.tm_hour;
    *min = timeinfo.tm_min;
    *seg = timeinfo.tm_sec;
    *wday = timeinfo.tm_wday;
    return true;
}

////////////////////////////////////////////////////////////////////////////////
// Funcion: getStatus                                                         //
// Descripción: Devuelve 1 si esta conectado a internet o 0 en caso contratio //
////////////////////////////////////////////////////////////////////////////////
int WF::getStatus()
{
    if (WiFi.status() == WL_CONNECTED)
        return true;
    else
        return false;
}

////////////////////////////////////////////////////////////////////////////////
// Funcion: getSignalStrength                                                 //
// Descripción: Devuelve la potencia de la señal del wifi al que esta conectado //
////////////////////////////////////////////////////////////////////////////////
String WF::getSignalStrength(void)
{
    long rssi = 0;
    long averageRSSI = 0;

    for (int i = 0; i < 50; i++)
    {
        rssi += WiFi.RSSI();
        delay(10);
    }

    averageRSSI = rssi / 50;
    return String(averageRSSI);
}

////////////////////////////////////////////////////////////////////////////////
// Funcion: getSignalStrength                                                 //
// Descripción: Devuelve la potencia de la señal del wifi al que esta conectado //
////////////////////////////////////////////////////////////////////////////////
int WF::wfDisconnect(void)
{
    WiFi.disconnect();
    return 0;
}


////////////////////////////////////////////////////////////////////////////////
// Funcion: getServerUrl                                                      //
// Descripción: Devuelve la URL del server                                    //
////////////////////////////////////////////////////////////////////////////////
String WF::getServerUrl(void)
{
    return String(host_str) + "/" + String(path_str);
}