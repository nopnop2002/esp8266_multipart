#include <ESP8266WiFi.h>
#include <ESP8266_multipart.h>
#include "FS.h"

const char* SSID = "your_ssid";
const char* PASSWORD = "your_password";
const char* SERVER = "your_server_ip";
const int PORT = 8080;
const char* URL = "/upload_multipart";

int listDir() {
  char cwdName[2];
  int fnum = 0;
  strcpy(cwdName,"/");
  Dir dir=SPIFFS.openDir(cwdName);
  while( dir.next() ) {
    String fn, fs;
    fn = dir.fileName();
    fn.remove(0, 1);
    fs = String(dir.fileSize());
    Serial.println("<" + fn + "> size=" + fs);
    fnum++;
  } // end while
  return fnum;
}

int getFileName(int index, char* name, int nlen) {
  char cwdName[2];
  int fnum = 0;
  strcpy(cwdName,"/");
  strcpy(name,"/");
  Dir dir=SPIFFS.openDir(cwdName);
  while( dir.next() ) {
    String fn, fs;
    fn = dir.fileName();
    fn.remove(0, 1);
    fs = String(dir.fileSize());
    Serial.println("<" + fn + "> size=" + fs);
    if (fnum == index) {
      fn.toCharArray(&name[1], nlen-1);
      return fs.toInt();
    }
    fnum++;
  } // end while
  return 0;
}


void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.print(ESP.getFullVersion());
  Serial.println();
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);

  Serial.print("Wait for WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  if (SPIFFS.begin()) {
    Serial.println("SPIFFS Initialize....ok");
    int fnum = listDir();
    Serial.print("fnum=");Serial.println(fnum);
    char fname[32];

    ESP8266_multipart multipart(SERVER); // Sets the host
    multipart.setPort(PORT); // Sets the port
    //multipart.setTimeout(2000); // Sets the timeout of server responce
    //multipart.setDebug(true); // Sets the debug
    
    
    for (int i=0;i<fnum;i++) {
      int ret = getFileName(i,fname,sizeof(fname));
      Serial.print("ret=");Serial.println(ret);
      Serial.print("fname=");Serial.println(fname);

      File file = SPIFFS.open(fname, "r");
      if (file) {
        int status = multipart.sendFile(URL, file);
        file.close();
        Serial.print("status=");Serial.println(status);
      } else {
        Serial.println("file open failed");
      }
    }
  } else {
    Serial.println("SPIFFS Initialization...failed");
  }
    
}

void loop() {
}
