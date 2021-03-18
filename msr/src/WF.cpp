#include "WF.h"

////////////////////////////////////////////////////////////////////////////////
// Funcion: init                                                              //
// Descripción: inicializa el cliente de thingSepak                           //
////////////////////////////////////////////////////////////////////////////////
void WF::init(void)
{
    smem.init();

    smem.writeFile(SPIFFS, "/SSID.txt", "Fibertel WiFi030 2.4GHz");
    smem.writeFile(SPIFFS, "/PASS.txt", "01430030175");
    smem.writeFile(SPIFFS, "/HOST.txt", "http://noseperobueno.com");
    smem.writeFile(SPIFFS, "/ENDPOINT.txt", "hola");

    int i = 0;
    WiFi.mode(WIFI_STA);
    sprintf(ssid_str, "%s", smem.readFile(SPIFFS, "/SSID.txt").c_str());
    sprintf(pass_str, "%s", smem.readFile(SPIFFS, "/PASS.txt").c_str());
    sprintf(host_str, "%s", smem.readFile(SPIFFS, "/HOST.txt").c_str());
    sprintf(endp_str, "%s", smem.readFile(SPIFFS, "/ENDPOINT.txt").c_str());

    Serial.print("Connecting to WiFi network: " + String(ssid_str) + "..");
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

    if (!WiFi.softAP("MultBoard", "123456789"))
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
        return FALSE;
    }
    *day = timeinfo.tm_mday;
    *month = timeinfo.tm_mon;
    *year = timeinfo.tm_year + 1900;
    *hour = timeinfo.tm_hour;
    *min = timeinfo.tm_min;
    *seg = timeinfo.tm_sec;
    *wday = timeinfo.tm_wday;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
// Funcion: getStatus                                                         //
// Descripción: Devuelve 1 si esta conectado a internet o 0 en caso contratio //
////////////////////////////////////////////////////////////////////////////////
int WF::getStatus()
{
    if (WiFi.status() == WL_CONNECTED)
        return TRUE;
    else
        return FALSE;
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
