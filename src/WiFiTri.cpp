/*
WiFiTri Library
(c) 2020 Jessica Hershey
http://www.github.com/jhershey69

WiFi Triangulation
Uses Google Reverse Geocoding and WiFi Triangulation
Requirse user to possess a Google API Key
Requires a WiFi connection
This library DOES NOT handle the connection
It only checks for a valid connection

Requires JSON Assistant Library v6 or above <ArduinoJson>
Requires HTTPClient Library

Stores Latitude and Longitude in
WiFiPosition.lat and WiFiPosition.lon
Any place you need Latitude and Longitude just call
getWiFiPosition("Your Google API Key Here")
*/

#include "WiFiTri.h"


WiFiTri::WiFiTri()
{

}

int WiFiTri::getWiFiPosition(char* _Gkey)
{

    int error_code = 0;

    if (WiFi.status() != WL_CONNECTED)
        {
            error_code = 1;
            return error_code;
        }


    HTTPClient http;

    // Scan for nearby WiFi networks
    int numNetworks = WiFi.scanNetworks();

    if(numNetworks < 3)
        {
            error_code = 2;
            return error_code;
        }

    /*
     * Add 3 nearest WiFi MAC Addresses in this JSON form:
    {
      "considerIp" : "false",
      "wifiAccessPoints": [
      {
      "macAddress":"4A:5D:36:35:BA:6D"
      }
      {
      "macAddress":"48:5D:36:35:BA:6C",
      }
      {
      "macAddress":"B0:B9:8A:EF:D2:D8"
      }
    ]
    }
    */

    strncpy(_GURLheader,"{\n\"considerIp\" : \"false\",\n\"wifiAccessPoints\": [\n",200);
    for (int x = 0; x < 3; x++)
        {
            strncat(_GURLheader,"{\n\"macAddress\" : \"",200);
            strncat(_GURLheader,WiFi.BSSIDstr(x).c_str(),200);
            strncat(_GURLheader,"\",\n}",200);
            if(x<2)
                strncat(_GURLheader,",\n",200);
        }

    strncat(_GURLheader,"\n]\n}\n",200);

    // Setting up the url for the POST

    char geoLocURL[200];
    strncpy(geoLocURL,_GURL1,200);
    strncat(geoLocURL,_GURL2,200);
    strncat(geoLocURL,_Gkey,200);

    // Lets send the json in the POST and get back an answer with getString(),
    // add header for application/json

    http.begin(geoLocURL);
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST(_GURLheader);

    if (httpCode > 399)
        {
            if(httpCode == 404)
                {
                    errorCode = 4;
                    http.end();
                    return error_code;
                }
            else
                {
                    error_code = 3;
                    http.end();
                    return error_code;
                }
        }

    // Setting up the parsing routine
    const size_t capacity = 2 * JSON_OBJECT_SIZE(2) + 30;
    DynamicJsonDocument geo(capacity);

    deserializeJson(geo, http.getString());     // parsing the return from Google into geo Document

    // Variables in parse, only two to be used in API URL
    WiFiPosition.lat = geo["location"]["lat"];
    WiFiPosition.lon = geo["location"]["lng"];

    http.end();
    return 0;
} // End of Routine


WiFiTri::~WiFiTri()
{
    //dtor
}
