#include "ESP8266WiFi.h"
#include "FS.h"

class ESP8266_multipart {
  public:
    ESP8266_multipart();
    ESP8266_multipart(String host);
    ESP8266_multipart(const char* host);
    int sendFile(String path, File file);
    int sendFile(const char* path, File file);
    void setPort(int port);
    void setHost(String host);
    void setHost(const char* host);
    void setTimeout(long timeout);
    void setDebug(bool debug);
    
  private:
    const char* host;
    int port;
    long timeout;
    bool debug;
    void init(const char* host, int port);
};


