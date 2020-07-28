#ifndef Enjo_ESP32
#define Enjo_ESP32
 
#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>
 
class Enjo
{
public:
	Enjo();
	String ipToString(IPAddress ip);
	String getApiKeyControl(String nameHub, String type, String password, String token);
	void postLogDataToHTTPServer(String deviceID, String deviceName, String api);
	String getControlDataToHTTPServer(String deviceID, String field, String api);
	String addDeviceToHub(String formData, String api);
	void updateFieldsForDevice(String formData, String deviceID, String api);
	void wifiSetup(const char *SSID, const char *password);
};
 
#endif