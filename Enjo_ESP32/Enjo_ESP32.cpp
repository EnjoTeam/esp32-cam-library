#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <Enjo_ESP32.h>
 
// 54:00:04:D1:E8:C8:DF:AA:65:E3:10:85:D2:3C:BC:7B:A1:24:20:11 
Enjo::Enjo(){
}

String Enjo::ipToString(IPAddress ip){
	String s = "";
	for (int i = 0; i < 4; i++)
		s += i  ? "." + String(ip[i]) : String(ip[i]);
	return s;
}

String Enjo::getApiKeyControl(String nameHub, String type, String password, String token){
	if (WiFi.status() == WL_CONNECTED) { // Check Wi-Fi status

		const char *root_ca = \
		"-----BEGIN CERTIFICATE-----\n" \
		"MIIFsTCCBJmgAwIBAgIQVSm0i1r/BntxrIgAbW5r3jANBgkqhkiG9w0BAQsFADB/\n" \
		"MQswCQYDVQQGEwJTRzESMBAGA1UECBMJU2luZ2Fwb3JlMRIwEAYDVQQHEwlTaW5n\n" \
		"YXBvcmUxGzAZBgNVBAoTEk9uZSBTaWduIFB0ZS4gTHRkLjErMCkGA1UEAxMiT25l\n" \
		"U2lnblNTTCBSU0EgRFYgU2VjdXJlIFNlcnZlciBDQTAeFw0yMDAyMTIwMDAwMDBa\n" \
		"Fw0yMTAyMTEyMzU5NTlaMBsxGTAXBgNVBAMTEHd3dy5lbmpvLWlvdC54eXowggEi\n" \
		"MA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDimQUFZpUzjrNhov/NRgF6IA2v\n" \
		"lJle4nEjTz0JNX/pb/+z8bavutjPn2mXAAKXrlBGxgKNMxx75uukxrSf3lRaWGA9\n" \
		"00wXzqWBK7dS3y7Wzoh5jNGfYe7x1bSEZD69PF9wE/irP1aICz8tqD+M6dWHd9jY\n" \
		"oQ0dB5P65xa4RUYFsYZlav1EViAf7jEb63pVG0sreW1mJxVKxOWd29Jaj451A0wz\n" \
		"ivvAFGmoZrqoHlFEn4+k1XCSCSXtBb/ckCVlTcOgLsiTcl+RoUYnCltsRLDuHQhw\n" \
		"j+VHd1sFJp5YgWIAMTfa3mpn1e0JCqQdwLUFWi5nVMHQXmcDe+bB0caC+ZxbAgMB\n" \
		"AAGjggKLMIIChzAfBgNVHSMEGDAWgBTZ2/swDqfXirFbcGBz/O1+g5UCUjAdBgNV\n" \
		"HQ4EFgQUzlaZh2i02d0W0SK/Q21IKpw0nLcwDgYDVR0PAQH/BAQDAgWgMAwGA1Ud\n" \
		"EwEB/wQCMAAwHQYDVR0lBBYwFAYIKwYBBQUHAwEGCCsGAQUFBwMCMEkGA1UdIARC\n" \
		"MEAwNAYLKwYBBAGyMQECAkcwJTAjBggrBgEFBQcCARYXaHR0cHM6Ly9zZWN0aWdv\n" \
		"LmNvbS9DUFMwCAYGZ4EMAQIBMIGJBggrBgEFBQcBAQR9MHswTAYIKwYBBQUHMAKG\n" \
		"QGh0dHA6Ly9vbmVzaWduLmNydC5zZWN0aWdvLmNvbS9PbmVTaWduU1NMUlNBRFZT\n" \
		"ZWN1cmVTZXJ2ZXJDQS5jcnQwKwYIKwYBBQUHMAGGH2h0dHA6Ly9vbmVzaWduLm9j\n" \
		"c3Auc2VjdGlnby5jb20wKQYDVR0RBCIwIIIQd3d3LmVuam8taW90Lnh5eoIMZW5q\n" \
		"by1pb3QueHl6MIIBBAYKKwYBBAHWeQIEAgSB9QSB8gDwAHYAfT7y+I//iFVoJMLA\n" \
		"yp5SiXkrxQ54CX8uapdomX4i8NcAAAFwOhABCQAABAMARzBFAiAgvwsVU4ilKAMB\n" \
		"lRsoVPdleA7x0cgDCaEoABIrwxBU1AIhAKVp6hY9CoqGHcSpEZJjr1jtql9PSgli\n" \
		"getic3vKu9ZtAHYAlCC8Ho7VjWyIcx+CiyIsDdHaTV5sT5Q9YdtOL1hNosIAAAFw\n" \
		"OhAA6QAABAMARzBFAiAbay5w7jDeKPslH3sI1MwxLKz0uArD64CITszK7sZO0wIh\n" \
		"AOY/PWiEv5T1jEfpp1PHVhvVcGj2CbAl67YmQ6PPW5ZPMA0GCSqGSIb3DQEBCwUA\n" \
		"A4IBAQCOVMljJXi5HHlt3Ex3CS/oKWCpZn1N3/WuAdUL2Rd2wCA8PabLfrvwJ9v6\n" \
		"sjJlUq58JduXhGA4vAgmwt9s8lxu8PnmCtHFY8OwDvknSijbxYDVlV1PRxB8cGUZ\n" \
		"pesLv83bp2EGkrUVJJ3m8w5ANK2yiW2EXhhj5vof+i74r8n1khV4/clsNr8pZaeU\n" \
		"WeXNedWkvQrTnOM8y/J6pDVfBKxU6TCBXdtPA8gvfhE3M8+ANhSKF9b1KabO8pSn\n" \
		"C6viNfAE0J+z1Nvu7DJOksZCuwt9X13eBRbu/xZeEXRN4Ys4bOBf/9TwX4ZSEpmC\n" \
		"mgekhDCtNx/Im2I+aSjUbvaHwjhL\n" \
		"-----END CERTIFICATE-----\n";

		HTTPClient https;

		Serial.println("\n---------- [Function]: getApiKeyControl Begin ----------\n");
		
		Serial.print("[HTTPS] begin...\n");
		
		if (https.begin("https://enjo-iot.xyz/add/control", root_ca)) {  // HTTPS

			https.addHeader("Content-Type", "application/x-www-form-urlencoded");

			Serial.print("[HTTPS] POST...\n");
			// start connection and send HTTP header

			String formData = "MAC=" + WiFi.macAddress()+ "&"
								+ "name=" + nameHub + "&"
								+ "type=" + type + "&"
								+ "password=" + password + "&"
								+ "token=" + token;
			
			int httpCode = https.POST(formData);

			// httpCode will be negative on error
			if (httpCode > 0) {
				// HTTP header has been send and Server response header has been handled
				Serial.printf("[HTTPS] POST... code: %d\n", httpCode);

				// file found at server
				if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
					String payload = https.getString();
					Serial.println(payload);

					// allocate the memory for the document
					const size_t capacity = JSON_OBJECT_SIZE(2);
					DynamicJsonBuffer jsonBuffer(capacity);
					JsonObject& root = jsonBuffer.parseObject(payload);

					const char *key = root["hubID"];
										
					return String(key);
				}
			} else {
				Serial.printf("[HTTPS] POST... failed, error: %s\n", https.errorToString(httpCode).c_str());
			}
			https.end();
		} else {
		  Serial.printf("[HTTPS] Unable to connect\n");
		}
		Serial.println("\n---------- [Function]: getApiKeyControl End ----------\n");
	} else {
		Serial.println("Error in WiFi connection");
	}
}

String Enjo::addDeviceToHub(String formData, String api){
	if (WiFi.status() == WL_CONNECTED) { // Check Wi-Fi status
		const char *root_ca = \
		"-----BEGIN CERTIFICATE-----\n" \
		"MIIFsTCCBJmgAwIBAgIQVSm0i1r/BntxrIgAbW5r3jANBgkqhkiG9w0BAQsFADB/\n" \
		"MQswCQYDVQQGEwJTRzESMBAGA1UECBMJU2luZ2Fwb3JlMRIwEAYDVQQHEwlTaW5n\n" \
		"YXBvcmUxGzAZBgNVBAoTEk9uZSBTaWduIFB0ZS4gTHRkLjErMCkGA1UEAxMiT25l\n" \
		"U2lnblNTTCBSU0EgRFYgU2VjdXJlIFNlcnZlciBDQTAeFw0yMDAyMTIwMDAwMDBa\n" \
		"Fw0yMTAyMTEyMzU5NTlaMBsxGTAXBgNVBAMTEHd3dy5lbmpvLWlvdC54eXowggEi\n" \
		"MA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDimQUFZpUzjrNhov/NRgF6IA2v\n" \
		"lJle4nEjTz0JNX/pb/+z8bavutjPn2mXAAKXrlBGxgKNMxx75uukxrSf3lRaWGA9\n" \
		"00wXzqWBK7dS3y7Wzoh5jNGfYe7x1bSEZD69PF9wE/irP1aICz8tqD+M6dWHd9jY\n" \
		"oQ0dB5P65xa4RUYFsYZlav1EViAf7jEb63pVG0sreW1mJxVKxOWd29Jaj451A0wz\n" \
		"ivvAFGmoZrqoHlFEn4+k1XCSCSXtBb/ckCVlTcOgLsiTcl+RoUYnCltsRLDuHQhw\n" \
		"j+VHd1sFJp5YgWIAMTfa3mpn1e0JCqQdwLUFWi5nVMHQXmcDe+bB0caC+ZxbAgMB\n" \
		"AAGjggKLMIIChzAfBgNVHSMEGDAWgBTZ2/swDqfXirFbcGBz/O1+g5UCUjAdBgNV\n" \
		"HQ4EFgQUzlaZh2i02d0W0SK/Q21IKpw0nLcwDgYDVR0PAQH/BAQDAgWgMAwGA1Ud\n" \
		"EwEB/wQCMAAwHQYDVR0lBBYwFAYIKwYBBQUHAwEGCCsGAQUFBwMCMEkGA1UdIARC\n" \
		"MEAwNAYLKwYBBAGyMQECAkcwJTAjBggrBgEFBQcCARYXaHR0cHM6Ly9zZWN0aWdv\n" \
		"LmNvbS9DUFMwCAYGZ4EMAQIBMIGJBggrBgEFBQcBAQR9MHswTAYIKwYBBQUHMAKG\n" \
		"QGh0dHA6Ly9vbmVzaWduLmNydC5zZWN0aWdvLmNvbS9PbmVTaWduU1NMUlNBRFZT\n" \
		"ZWN1cmVTZXJ2ZXJDQS5jcnQwKwYIKwYBBQUHMAGGH2h0dHA6Ly9vbmVzaWduLm9j\n" \
		"c3Auc2VjdGlnby5jb20wKQYDVR0RBCIwIIIQd3d3LmVuam8taW90Lnh5eoIMZW5q\n" \
		"by1pb3QueHl6MIIBBAYKKwYBBAHWeQIEAgSB9QSB8gDwAHYAfT7y+I//iFVoJMLA\n" \
		"yp5SiXkrxQ54CX8uapdomX4i8NcAAAFwOhABCQAABAMARzBFAiAgvwsVU4ilKAMB\n" \
		"lRsoVPdleA7x0cgDCaEoABIrwxBU1AIhAKVp6hY9CoqGHcSpEZJjr1jtql9PSgli\n" \
		"getic3vKu9ZtAHYAlCC8Ho7VjWyIcx+CiyIsDdHaTV5sT5Q9YdtOL1hNosIAAAFw\n" \
		"OhAA6QAABAMARzBFAiAbay5w7jDeKPslH3sI1MwxLKz0uArD64CITszK7sZO0wIh\n" \
		"AOY/PWiEv5T1jEfpp1PHVhvVcGj2CbAl67YmQ6PPW5ZPMA0GCSqGSIb3DQEBCwUA\n" \
		"A4IBAQCOVMljJXi5HHlt3Ex3CS/oKWCpZn1N3/WuAdUL2Rd2wCA8PabLfrvwJ9v6\n" \
		"sjJlUq58JduXhGA4vAgmwt9s8lxu8PnmCtHFY8OwDvknSijbxYDVlV1PRxB8cGUZ\n" \
		"pesLv83bp2EGkrUVJJ3m8w5ANK2yiW2EXhhj5vof+i74r8n1khV4/clsNr8pZaeU\n" \
		"WeXNedWkvQrTnOM8y/J6pDVfBKxU6TCBXdtPA8gvfhE3M8+ANhSKF9b1KabO8pSn\n" \
		"C6viNfAE0J+z1Nvu7DJOksZCuwt9X13eBRbu/xZeEXRN4Ys4bOBf/9TwX4ZSEpmC\n" \
		"mgekhDCtNx/Im2I+aSjUbvaHwjhL\n" \
		"-----END CERTIFICATE-----\n"; 

		HTTPClient https;

		Serial.println("\n---------- [Function]: addDeviceToHub Begin ----------\n");

		Serial.print("[HTTPS] begin...\n");
		if (https.begin("https://www.enjo-iot.xyz/hub/add/device/" + api, root_ca)) {  // HTTPS

			https.addHeader("Content-Type", "application/x-www-form-urlencoded");

			Serial.print("[HTTPS] POST...\n");
			// start connection and send HTTP header
			
			int httpCode = https.POST(formData);
			Serial.println(formData);

			// httpCode will be negative on error
			if (httpCode > 0) {
				// HTTP header has been send and Server response header has been handled
				Serial.printf("[HTTPS] POST... code: %d\n", httpCode);

				// file found at server
				if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
					String payload = https.getString();
					Serial.println(payload);
					
					// allocate the memory for the document
					const size_t capacity = JSON_OBJECT_SIZE(1);
					DynamicJsonBuffer jsonBuffer(capacity);
					JsonObject& root = jsonBuffer.parseObject(payload);

					const char *id = root["deviceID"];
					
					Serial.println(String(id));
					
					return String(id);
				}
			} else {
				Serial.printf("[HTTPS] POST... failed, error: %s\n", https.errorToString(httpCode).c_str());
			}
			https.end();
		} else {
		  Serial.printf("[HTTPS] Unable to connect\n");
		}
		Serial.println("\n---------- [Function]: addDeviceToHub End ----------\n");
	} else {
		Serial.println("Error in WiFi connection");
	}
}

void Enjo::postLogDataToHTTPServer(String deviceId, String deviceName, String api){
	if (WiFi.status() == WL_CONNECTED) { // Check Wi-Fi status

		const char *root_ca = \
		"-----BEGIN CERTIFICATE-----\n" \
		"MIIFsTCCBJmgAwIBAgIQVSm0i1r/BntxrIgAbW5r3jANBgkqhkiG9w0BAQsFADB/\n" \
		"MQswCQYDVQQGEwJTRzESMBAGA1UECBMJU2luZ2Fwb3JlMRIwEAYDVQQHEwlTaW5n\n" \
		"YXBvcmUxGzAZBgNVBAoTEk9uZSBTaWduIFB0ZS4gTHRkLjErMCkGA1UEAxMiT25l\n" \
		"U2lnblNTTCBSU0EgRFYgU2VjdXJlIFNlcnZlciBDQTAeFw0yMDAyMTIwMDAwMDBa\n" \
		"Fw0yMTAyMTEyMzU5NTlaMBsxGTAXBgNVBAMTEHd3dy5lbmpvLWlvdC54eXowggEi\n" \
		"MA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDimQUFZpUzjrNhov/NRgF6IA2v\n" \
		"lJle4nEjTz0JNX/pb/+z8bavutjPn2mXAAKXrlBGxgKNMxx75uukxrSf3lRaWGA9\n" \
		"00wXzqWBK7dS3y7Wzoh5jNGfYe7x1bSEZD69PF9wE/irP1aICz8tqD+M6dWHd9jY\n" \
		"oQ0dB5P65xa4RUYFsYZlav1EViAf7jEb63pVG0sreW1mJxVKxOWd29Jaj451A0wz\n" \
		"ivvAFGmoZrqoHlFEn4+k1XCSCSXtBb/ckCVlTcOgLsiTcl+RoUYnCltsRLDuHQhw\n" \
		"j+VHd1sFJp5YgWIAMTfa3mpn1e0JCqQdwLUFWi5nVMHQXmcDe+bB0caC+ZxbAgMB\n" \
		"AAGjggKLMIIChzAfBgNVHSMEGDAWgBTZ2/swDqfXirFbcGBz/O1+g5UCUjAdBgNV\n" \
		"HQ4EFgQUzlaZh2i02d0W0SK/Q21IKpw0nLcwDgYDVR0PAQH/BAQDAgWgMAwGA1Ud\n" \
		"EwEB/wQCMAAwHQYDVR0lBBYwFAYIKwYBBQUHAwEGCCsGAQUFBwMCMEkGA1UdIARC\n" \
		"MEAwNAYLKwYBBAGyMQECAkcwJTAjBggrBgEFBQcCARYXaHR0cHM6Ly9zZWN0aWdv\n" \
		"LmNvbS9DUFMwCAYGZ4EMAQIBMIGJBggrBgEFBQcBAQR9MHswTAYIKwYBBQUHMAKG\n" \
		"QGh0dHA6Ly9vbmVzaWduLmNydC5zZWN0aWdvLmNvbS9PbmVTaWduU1NMUlNBRFZT\n" \
		"ZWN1cmVTZXJ2ZXJDQS5jcnQwKwYIKwYBBQUHMAGGH2h0dHA6Ly9vbmVzaWduLm9j\n" \
		"c3Auc2VjdGlnby5jb20wKQYDVR0RBCIwIIIQd3d3LmVuam8taW90Lnh5eoIMZW5q\n" \
		"by1pb3QueHl6MIIBBAYKKwYBBAHWeQIEAgSB9QSB8gDwAHYAfT7y+I//iFVoJMLA\n" \
		"yp5SiXkrxQ54CX8uapdomX4i8NcAAAFwOhABCQAABAMARzBFAiAgvwsVU4ilKAMB\n" \
		"lRsoVPdleA7x0cgDCaEoABIrwxBU1AIhAKVp6hY9CoqGHcSpEZJjr1jtql9PSgli\n" \
		"getic3vKu9ZtAHYAlCC8Ho7VjWyIcx+CiyIsDdHaTV5sT5Q9YdtOL1hNosIAAAFw\n" \
		"OhAA6QAABAMARzBFAiAbay5w7jDeKPslH3sI1MwxLKz0uArD64CITszK7sZO0wIh\n" \
		"AOY/PWiEv5T1jEfpp1PHVhvVcGj2CbAl67YmQ6PPW5ZPMA0GCSqGSIb3DQEBCwUA\n" \
		"A4IBAQCOVMljJXi5HHlt3Ex3CS/oKWCpZn1N3/WuAdUL2Rd2wCA8PabLfrvwJ9v6\n" \
		"sjJlUq58JduXhGA4vAgmwt9s8lxu8PnmCtHFY8OwDvknSijbxYDVlV1PRxB8cGUZ\n" \
		"pesLv83bp2EGkrUVJJ3m8w5ANK2yiW2EXhhj5vof+i74r8n1khV4/clsNr8pZaeU\n" \
		"WeXNedWkvQrTnOM8y/J6pDVfBKxU6TCBXdtPA8gvfhE3M8+ANhSKF9b1KabO8pSn\n" \
		"C6viNfAE0J+z1Nvu7DJOksZCuwt9X13eBRbu/xZeEXRN4Ys4bOBf/9TwX4ZSEpmC\n" \
		"mgekhDCtNx/Im2I+aSjUbvaHwjhL\n" \
		"-----END CERTIFICATE-----\n"; 

		HTTPClient https;

		Serial.println("\n---------- [Function]: postLogDataToHTTPServer Begin ----------\n");
		
		Serial.print("[HTTPS] begin...\n");
		
		if (https.begin("https://enjo-iot.xyz/update/log/" + api, root_ca)) {  // HTTPS

			https.addHeader("Content-Type", "application/x-www-form-urlencoded");

			Serial.print("[HTTPS] POST...\n");
			// start connection and send HTTP header

			String formData = "deviceID=" + deviceId + "&"
			       + "name=" + deviceName + "&"
                   + "ipAddress=" + ipToString(WiFi.localIP()) + "&"
                   + "MAC=" + WiFi.macAddress() + "&"
                   + "ssidConnected=" + WiFi.SSID() + "&"
                   + "freeHeapSize=" + ESP.getFreeHeap() + "&"
                   + "flashChipSize=" + ESP.getFlashChipSize() + "&"
                   + "flashChipFrequency=" + ESP.getFlashChipSpeed();
				   
			int httpCode = https.POST(formData);
			Serial.println(formData);

			// httpCode will be negative on error
			if (httpCode > 0) {
				// HTTP header has been send and Server response header has been handled
				Serial.printf("[HTTPS] POST... code: %d\n", httpCode);

				// file found at server
				if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
					String payload = https.getString();
					Serial.println(payload);
				}
			} else {
				Serial.printf("[HTTPS] POST... failed, error: %s\n", https.errorToString(httpCode).c_str());
			}
			https.end();
		} else {
		  Serial.printf("[HTTPS] Unable to connect\n");
		}
		Serial.println("\n---------- [Function]: postLogDataToHTTPServer End ----------\n");
	} else {
		Serial.println("Error in WiFi connection");
	}
}

void Enjo::updateFieldsForDevice(String formData, String deviceID, String api){
	if (WiFi.status() == WL_CONNECTED) { // Check Wi-Fi status
		const char *root_ca = \
		"-----BEGIN CERTIFICATE-----\n" \
		"MIIFsTCCBJmgAwIBAgIQVSm0i1r/BntxrIgAbW5r3jANBgkqhkiG9w0BAQsFADB/\n" \
		"MQswCQYDVQQGEwJTRzESMBAGA1UECBMJU2luZ2Fwb3JlMRIwEAYDVQQHEwlTaW5n\n" \
		"YXBvcmUxGzAZBgNVBAoTEk9uZSBTaWduIFB0ZS4gTHRkLjErMCkGA1UEAxMiT25l\n" \
		"U2lnblNTTCBSU0EgRFYgU2VjdXJlIFNlcnZlciBDQTAeFw0yMDAyMTIwMDAwMDBa\n" \
		"Fw0yMTAyMTEyMzU5NTlaMBsxGTAXBgNVBAMTEHd3dy5lbmpvLWlvdC54eXowggEi\n" \
		"MA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDimQUFZpUzjrNhov/NRgF6IA2v\n" \
		"lJle4nEjTz0JNX/pb/+z8bavutjPn2mXAAKXrlBGxgKNMxx75uukxrSf3lRaWGA9\n" \
		"00wXzqWBK7dS3y7Wzoh5jNGfYe7x1bSEZD69PF9wE/irP1aICz8tqD+M6dWHd9jY\n" \
		"oQ0dB5P65xa4RUYFsYZlav1EViAf7jEb63pVG0sreW1mJxVKxOWd29Jaj451A0wz\n" \
		"ivvAFGmoZrqoHlFEn4+k1XCSCSXtBb/ckCVlTcOgLsiTcl+RoUYnCltsRLDuHQhw\n" \
		"j+VHd1sFJp5YgWIAMTfa3mpn1e0JCqQdwLUFWi5nVMHQXmcDe+bB0caC+ZxbAgMB\n" \
		"AAGjggKLMIIChzAfBgNVHSMEGDAWgBTZ2/swDqfXirFbcGBz/O1+g5UCUjAdBgNV\n" \
		"HQ4EFgQUzlaZh2i02d0W0SK/Q21IKpw0nLcwDgYDVR0PAQH/BAQDAgWgMAwGA1Ud\n" \
		"EwEB/wQCMAAwHQYDVR0lBBYwFAYIKwYBBQUHAwEGCCsGAQUFBwMCMEkGA1UdIARC\n" \
		"MEAwNAYLKwYBBAGyMQECAkcwJTAjBggrBgEFBQcCARYXaHR0cHM6Ly9zZWN0aWdv\n" \
		"LmNvbS9DUFMwCAYGZ4EMAQIBMIGJBggrBgEFBQcBAQR9MHswTAYIKwYBBQUHMAKG\n" \
		"QGh0dHA6Ly9vbmVzaWduLmNydC5zZWN0aWdvLmNvbS9PbmVTaWduU1NMUlNBRFZT\n" \
		"ZWN1cmVTZXJ2ZXJDQS5jcnQwKwYIKwYBBQUHMAGGH2h0dHA6Ly9vbmVzaWduLm9j\n" \
		"c3Auc2VjdGlnby5jb20wKQYDVR0RBCIwIIIQd3d3LmVuam8taW90Lnh5eoIMZW5q\n" \
		"by1pb3QueHl6MIIBBAYKKwYBBAHWeQIEAgSB9QSB8gDwAHYAfT7y+I//iFVoJMLA\n" \
		"yp5SiXkrxQ54CX8uapdomX4i8NcAAAFwOhABCQAABAMARzBFAiAgvwsVU4ilKAMB\n" \
		"lRsoVPdleA7x0cgDCaEoABIrwxBU1AIhAKVp6hY9CoqGHcSpEZJjr1jtql9PSgli\n" \
		"getic3vKu9ZtAHYAlCC8Ho7VjWyIcx+CiyIsDdHaTV5sT5Q9YdtOL1hNosIAAAFw\n" \
		"OhAA6QAABAMARzBFAiAbay5w7jDeKPslH3sI1MwxLKz0uArD64CITszK7sZO0wIh\n" \
		"AOY/PWiEv5T1jEfpp1PHVhvVcGj2CbAl67YmQ6PPW5ZPMA0GCSqGSIb3DQEBCwUA\n" \
		"A4IBAQCOVMljJXi5HHlt3Ex3CS/oKWCpZn1N3/WuAdUL2Rd2wCA8PabLfrvwJ9v6\n" \
		"sjJlUq58JduXhGA4vAgmwt9s8lxu8PnmCtHFY8OwDvknSijbxYDVlV1PRxB8cGUZ\n" \
		"pesLv83bp2EGkrUVJJ3m8w5ANK2yiW2EXhhj5vof+i74r8n1khV4/clsNr8pZaeU\n" \
		"WeXNedWkvQrTnOM8y/J6pDVfBKxU6TCBXdtPA8gvfhE3M8+ANhSKF9b1KabO8pSn\n" \
		"C6viNfAE0J+z1Nvu7DJOksZCuwt9X13eBRbu/xZeEXRN4Ys4bOBf/9TwX4ZSEpmC\n" \
		"mgekhDCtNx/Im2I+aSjUbvaHwjhL\n" \
		"-----END CERTIFICATE-----\n"; 

		HTTPClient https;

		Serial.println("\n---------- [Function]: updateFieldsForDevice Begin ----------\n");

		Serial.print("[HTTPS] begin...\n");
		if (https.begin("https://www.enjo-iot.xyz/update/control/" + api + "/" + deviceID, root_ca)) {  // HTTPS

			https.addHeader("Content-Type", "application/x-www-form-urlencoded");

			Serial.print("[HTTPS] POST...\n");
			// start connection and send HTTP header
			
			int httpCode = https.POST(formData);
			Serial.println(formData);

			// httpCode will be negative on error
			if (httpCode > 0) {
				// HTTP header has been send and Server response header has been handled
				Serial.printf("[HTTPS] POST... code: %d\n", httpCode);

				// file found at server
				if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
					String payload = https.getString();
					Serial.println(payload);
					
					// allocate the memory for the document
					const size_t capacity = JSON_OBJECT_SIZE(1);
					DynamicJsonBuffer jsonBuffer(capacity);
					JsonObject& root = jsonBuffer.parseObject(payload);
				}
			} else {
				Serial.printf("[HTTPS] POST... failed, error: %s\n", https.errorToString(httpCode).c_str());
			}
			https.end();
		} else {
		  Serial.printf("[HTTPS] Unable to connect\n");
		}
		Serial.println("\n---------- [Function]: updateFieldsForDevice End ----------\n");
	} else {
		Serial.println("Error in WiFi connection");
	}
}

String Enjo::getControlDataToHTTPServer(String deviceID, String field, String api){
	if (WiFi.status() == WL_CONNECTED) { // Check Wi-Fi status
		
		const char *root_ca = \
		"-----BEGIN CERTIFICATE-----\n" \
		"MIIFsTCCBJmgAwIBAgIQVSm0i1r/BntxrIgAbW5r3jANBgkqhkiG9w0BAQsFADB/\n" \
		"MQswCQYDVQQGEwJTRzESMBAGA1UECBMJU2luZ2Fwb3JlMRIwEAYDVQQHEwlTaW5n\n" \
		"YXBvcmUxGzAZBgNVBAoTEk9uZSBTaWduIFB0ZS4gTHRkLjErMCkGA1UEAxMiT25l\n" \
		"U2lnblNTTCBSU0EgRFYgU2VjdXJlIFNlcnZlciBDQTAeFw0yMDAyMTIwMDAwMDBa\n" \
		"Fw0yMTAyMTEyMzU5NTlaMBsxGTAXBgNVBAMTEHd3dy5lbmpvLWlvdC54eXowggEi\n" \
		"MA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDimQUFZpUzjrNhov/NRgF6IA2v\n" \
		"lJle4nEjTz0JNX/pb/+z8bavutjPn2mXAAKXrlBGxgKNMxx75uukxrSf3lRaWGA9\n" \
		"00wXzqWBK7dS3y7Wzoh5jNGfYe7x1bSEZD69PF9wE/irP1aICz8tqD+M6dWHd9jY\n" \
		"oQ0dB5P65xa4RUYFsYZlav1EViAf7jEb63pVG0sreW1mJxVKxOWd29Jaj451A0wz\n" \
		"ivvAFGmoZrqoHlFEn4+k1XCSCSXtBb/ckCVlTcOgLsiTcl+RoUYnCltsRLDuHQhw\n" \
		"j+VHd1sFJp5YgWIAMTfa3mpn1e0JCqQdwLUFWi5nVMHQXmcDe+bB0caC+ZxbAgMB\n" \
		"AAGjggKLMIIChzAfBgNVHSMEGDAWgBTZ2/swDqfXirFbcGBz/O1+g5UCUjAdBgNV\n" \
		"HQ4EFgQUzlaZh2i02d0W0SK/Q21IKpw0nLcwDgYDVR0PAQH/BAQDAgWgMAwGA1Ud\n" \
		"EwEB/wQCMAAwHQYDVR0lBBYwFAYIKwYBBQUHAwEGCCsGAQUFBwMCMEkGA1UdIARC\n" \
		"MEAwNAYLKwYBBAGyMQECAkcwJTAjBggrBgEFBQcCARYXaHR0cHM6Ly9zZWN0aWdv\n" \
		"LmNvbS9DUFMwCAYGZ4EMAQIBMIGJBggrBgEFBQcBAQR9MHswTAYIKwYBBQUHMAKG\n" \
		"QGh0dHA6Ly9vbmVzaWduLmNydC5zZWN0aWdvLmNvbS9PbmVTaWduU1NMUlNBRFZT\n" \
		"ZWN1cmVTZXJ2ZXJDQS5jcnQwKwYIKwYBBQUHMAGGH2h0dHA6Ly9vbmVzaWduLm9j\n" \
		"c3Auc2VjdGlnby5jb20wKQYDVR0RBCIwIIIQd3d3LmVuam8taW90Lnh5eoIMZW5q\n" \
		"by1pb3QueHl6MIIBBAYKKwYBBAHWeQIEAgSB9QSB8gDwAHYAfT7y+I//iFVoJMLA\n" \
		"yp5SiXkrxQ54CX8uapdomX4i8NcAAAFwOhABCQAABAMARzBFAiAgvwsVU4ilKAMB\n" \
		"lRsoVPdleA7x0cgDCaEoABIrwxBU1AIhAKVp6hY9CoqGHcSpEZJjr1jtql9PSgli\n" \
		"getic3vKu9ZtAHYAlCC8Ho7VjWyIcx+CiyIsDdHaTV5sT5Q9YdtOL1hNosIAAAFw\n" \
		"OhAA6QAABAMARzBFAiAbay5w7jDeKPslH3sI1MwxLKz0uArD64CITszK7sZO0wIh\n" \
		"AOY/PWiEv5T1jEfpp1PHVhvVcGj2CbAl67YmQ6PPW5ZPMA0GCSqGSIb3DQEBCwUA\n" \
		"A4IBAQCOVMljJXi5HHlt3Ex3CS/oKWCpZn1N3/WuAdUL2Rd2wCA8PabLfrvwJ9v6\n" \
		"sjJlUq58JduXhGA4vAgmwt9s8lxu8PnmCtHFY8OwDvknSijbxYDVlV1PRxB8cGUZ\n" \
		"pesLv83bp2EGkrUVJJ3m8w5ANK2yiW2EXhhj5vof+i74r8n1khV4/clsNr8pZaeU\n" \
		"WeXNedWkvQrTnOM8y/J6pDVfBKxU6TCBXdtPA8gvfhE3M8+ANhSKF9b1KabO8pSn\n" \
		"C6viNfAE0J+z1Nvu7DJOksZCuwt9X13eBRbu/xZeEXRN4Ys4bOBf/9TwX4ZSEpmC\n" \
		"mgekhDCtNx/Im2I+aSjUbvaHwjhL\n" \
		"-----END CERTIFICATE-----\n";

		HTTPClient https;

		Serial.println("\n---------- [Function]: getControlDataToHTTPServer Begin ----------\n");
		
		Serial.print("[HTTPS] begin...\n");
		
		if (https.begin("https://enjo-iot.xyz/get/control/" + api + "/" + deviceID + "/" + field, root_ca)) {  // HTTPS
			https.addHeader("Content-Type", "application/x-www-form-urlencoded");
			Serial.print("[HTTPS] GET...\n");
			int httpCode = https.GET();
			 
			// httpCode will be negative on error
			if (httpCode > 0) {
				// HTTP header has been send and Server response header has been handled
				Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

				// file found at server
				if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
					String payload = https.getString();
					Serial.println(payload);

					// allocate the memory for the document
					const size_t capacity = JSON_OBJECT_SIZE(1);
					DynamicJsonBuffer jsonBuffer(capacity);
					JsonObject& root = jsonBuffer.parseObject(payload);

					const char *state = root[field];
					Serial.println(String(state));
					return String(state);
			}
			} else {
				Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
			}
			https.end();
		} else {
			Serial.printf("[HTTPS] Unable to connect\n");
		}
		Serial.println("\n---------- [Function]: getControlDataToHTTPServer End ----------\n");
	} else {
		Serial.println("Error in WiFi connection");
	}
}

void Enjo::wifiSetup(const char *SSID, const char *password){
	Serial.println("\n---------- [Function]: wifiSetup Begin ----------\n");
	WiFi.begin(SSID, password);
	Serial.print("Connecting ");
	while (WiFi.status() != WL_CONNECTED) { // Wait for connection
		Serial.print(".");
		delay(300);
	}
	Serial.println("");
	Serial.print("WiFi connected to: ");
	Serial.println(WiFi.SSID());  // Print conneccted WiFi SSID
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());
	Serial.print("MAC address: ");
	Serial.println(WiFi.macAddress());
	Serial.println("\n---------- [Function]: wifiSetup Begin ----------\n");
}