#include "ESP8266_multipart.h"

/*
Multipart Reference:
https://www.yoheim.net/blog.php?q=20171201

POST PATH HTTP/1.1
HOST: HOST
User-Agent: esp8266_multipart/1.0
Content-Type: multipart/form-data; boundary=X-ESP8266_MULTIPART
Content-Length: xxx

--X-ESP8266_MULTIPART
Content-Disposition: form-data; name="upfile"; filename="a.jpg"
Content-Type: Content-Type: application/octet-stream

binary-data

--X-ESP8266_MULTIPART--

*/


ESP8266_multipart::ESP8266_multipart() {
  ESP8266_multipart::init("127.0.0.1", 80);
}
ESP8266_multipart::ESP8266_multipart(String host) {
  ESP8266_multipart::init(host.c_str(), 80);
}
ESP8266_multipart::ESP8266_multipart(const char* host) {
  ESP8266_multipart::init(host, 80);
}
void ESP8266_multipart::init(const char* host, int port) {
  this->host = host;
  this->port = port;
  this->timeout = 1000;
  this->debug = false;
}
int ESP8266_multipart::sendFile(String path, File file) {
  return sendFile(path.c_str(), file);
}
void ESP8266_multipart::setPort(int port) {
  this->port = port;
}
void ESP8266_multipart::setHost(String host) {
  ESP8266_multipart::setHost(host.c_str());
}
void ESP8266_multipart::setHost(const char* host) {
  this->host = host;
}
void ESP8266_multipart::setTimeout(long timeout) {
  this->timeout = timeout;
}
void ESP8266_multipart::setDebug(bool debug) {
  this->debug = debug;
}
int ESP8266_multipart::sendFile(const char* path, File file) {
  WiFiClient client;
  
  // Remove the first character of the file name
  if (this->debug) Serial.print("file.name=");Serial.println(file.name());
  String fileName = file.name();
  fileName = fileName.substring(1);
  if (this->debug) Serial.print("fileName=");Serial.println(fileName);

  if(client.connect(this->host, this->port)) {
    client.print(F("POST ")); client.print(path); client.print(F(" HTTP/1.1\r\n"));
    client.print(F("Host: ")); client.print(this->host); client.print(F("\r\n"));
    client.print(F("User-Agent: esp8266_multipart/1.0\r\n"));
    client.print(F("Accept: */*\r\n"));
    //client.print(F("Connection: close\r\n"));
    //client.print(F("Keep-Alive: timeout=5, max=1000\r\n"));
    client.print(F("Content-Type: multipart/form-data; boundary=X-ESP8266_MULTIPART\r\n"));

    String partBody = "--X-ESP8266_MULTIPART\r\nContent-Disposition: form-data; name=\"upfile\"; filename=\"";
    partBody += fileName;
    partBody += "\"\r\n";
    partBody += "Content-Type: application/octet-stream\r\n\r\n";

    String partEnd = "\r\n--X-ESP8266_MULTIPART--\r\n\r\n";

    int dataLength = partBody.length() + partEnd.length() + file.size();
    client.print(F("Content-Length: ")); client.print(dataLength); client.print(F("\r\n\r\n"));

    client.print(partBody);

    // Save current file position
    int tempPos = file.position();
    file.seek(0, SeekSet);

    // Read file
    uint8_t dataBuffer[128];
    while(file.available()) {
      while(file.position() < file.size()) {
        size_t len = file.readBytes( (char *)dataBuffer, sizeof(dataBuffer) );
        client.write(dataBuffer, len);

#if 0
Serial.print("len=");Serial.println(len);
Serial.print("file.size=");Serial.println(file.size());
Serial.print("file.position=");Serial.println(file.position());
for(int i=0;i<len;i++) {
    Serial.print(dataBuffer[i], HEX);
    Serial.print(" ");
    if ( ( (i+1) % 10) == 0) {
        Serial.println();
        delay(1);
    }
}
#endif

      }
    }

    client.print(partEnd);
    client.flush();
    if (this->debug) Serial.println("http send complete");

    // Receive responce
    String tmpRes = "";
    long lastMillis = millis();
    while(client.connected()) {
      while(client.available()) {
        char c = client.read();
        tmpRes += c;
        lastMillis = millis();
      }
      long nowMillis = millis();
      long diffMillis = nowMillis - lastMillis;

      if (this->debug) {
        Serial.print("lastMillis=");Serial.println(lastMillis);
        Serial.print("nowMillis=");Serial.println(nowMillis);
        Serial.print("diffMillis=");Serial.println(diffMillis);
        Serial.print("this->timeout=");Serial.println(this->timeout);
      }
      if ( diffMillis > this->timeout) break;
    }
    client.stop();
    if (this->debug) {
      Serial.println("tmpRes:");
      Serial.println("-----------------------------------");
      Serial.println(tmpRes);
      Serial.println("-----------------------------------");
    }
    
    // Analyze responce
    int status = -1;
    int firstSpace = tmpRes.indexOf(" ");
    tmpRes.remove(0, firstSpace +1);
    status = tmpRes.substring(0,3).toInt();

    // restore file position
    file.seek(tempPos, SeekSet);
    return status;

  } else {
    return 0;
  }
}


