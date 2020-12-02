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




#ifndef WIFITRI_H
#define WIFITRI_H

#include <HTTPClient.h>
#include <ArduinoJson.h>        // Version 6 Required

class WiFiTri
{
public:

    // Constructor

    WiFiTri();

    //Methods
    int getWiFiPosition(char* _Gkey); //This is a Google Developer Key
    virtual ~WiFiTri();

    //Variables

    struct
    {
        float lat;
        float lon;
    } WiFiPosition;

    const char* error_msgs[4] = {"No WiFi Connection",
                                 "Number MAC addresses too low",
                                 "HTTP error from Google API",
                                 "HTTP 404 error"
                                };


protected:

private:

    char* _Gkey;
    char _GURL1[52] = "https://www.googleapis.com/geolocation/v1/geolocate";
    char _GURL2[6] = "?key=";
    char _GURLheader[200];

};

#endif // WIFITRI_H
