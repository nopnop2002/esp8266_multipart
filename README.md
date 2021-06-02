# esp8266_multipart
A multipart file upload library for the ESP8266.   

I ported from [here](https://github.com/nailujx86/ESP8266_multipart).

# Changes from the original

- Binary file support   
The original support only Text file.   
This library support both of Text and Binary file.   

- Add method   
void setTimeout(long timeout);   
void setDebug(bool debug);   


# Server Side
Download the server from [here](https://github.com/nopnop2002/multipart-upload-server).

---

# ESP8266 Side

## Installation
Download this repository as a zip file and extract it to your arduino library folder.   

## Upload files to SPIFFS
Open example scketch, and Upload files to SPIFFS using [ESP8266 Sketch Data Uploder](https://github.com/esp8266/arduino-esp8266fs-plugin).

## Usage
Refer to example.
Change this to your environment.

```
const char* SSID = "your_ssid";
const char* PASSWORD = "your_password";
const char* SERVER = "your_server_ip";
```

## About multipart/form-data
This library send this HTTP header.   

```
POST PATH HTTP/1.1
HOST: HOST
User-Agent: esp8266_multipart/1.0
Accept: */*
Content-Type: multipart/form-data; boundary=X-ESP8266_MULTIPART
Content-Length: xxx

--X-ESP8266_MULTIPART
Content-Disposition: form-data; name="upfile"; filename="hoge.jpg"
Content-Type: Content-Type: application/octet-stream

binary-data

--X-ESP8266_MULTIPART--
```
