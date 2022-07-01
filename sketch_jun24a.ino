/*********
EJEMPLO GET REQUEST FOR INPUT ESP32.
*********/

#include <Arduino.h>
#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
  #include <SPIFFS.h>
  #include <FS.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>


AsyncWebServer server(80);

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "LuisManuel ";
const char* password = "lu13edu4rd0";
// CREAR ACCESS POINT PARA LA CONFIGURACIÓN DE RED WIFI
const char* ssidAP     = "ESP32-Access-Point";
const char* passwordAP = "123456789";

const char* PARAM_INPUT_1 = "input1";
const char* PARAM_INPUT_2 = "input2";
const char* PARAM_INPUT_3 = "input3";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    listDir(SPIFFS, "/", 0);
    if (!SPIFFS.begin()) {
      Serial.println("SPIFFS failed to mount !\r\n");
    }

    String myFile = "/index.html";

    if(SPIFFS.exists(myFile)) {
      Serial.println(F("myFile founded on   SPIFFS"));   //ok
      File file = SPIFFS.open(myFile, "r");
      Serial.println(F("mi archivo"));   //ok
    }
    
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;
    }
    Serial.println();
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

  // Enviar pagina login al navegador
   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(SPIFFS, "/index.html", "text/html");
    });
  // Cwrgar estilo de login localmente
   server.on("/style1", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/all.min.css", "text/css");
  });
  // Cwrgar estilo de login localmente
  server.on("/style2", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/bootstrap.min.css", "text/css");
    });  
  
  // Cargar estilo de login localmente
   server.on("/style3", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/bootstrap.bundle.min.js", "text/js");
  });
  // Cargar estilo de login localmente
   server.on("/logo", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/logo.png", "image/png");
    });
  

  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  /*server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage1;
    String inputMessage2;
    String inputParam1;
    String inputParam2;
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputParam1 = PARAM_INPUT_1;
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      inputParam2 = PARAM_INPUT_2;
    }else {
      inputMessage1 = "No message sent";
      inputParam1 = "none";
      inputMessage2 = "No message sent";
      inputParam2 = "none";
    }
    Serial.println(inputMessage1);
    Serial.println(inputMessage2);
    request->send(SPIFFS, "/dashboardAdmin.html"+inputMessage1,"text/html");
  });*/
 server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request){  
    String inputMessage1;
    inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
    String formulario =                                        //Construcción del formulario HTML de respuesta en cadena de texto
     "<!DOCTYPE HTML>"
      "<html>"
      "<style>"
          "body {"
        "background: #0080bb;"
        "background: linear-gradient(to right, #0080bb, #33AEFF);"
      "}"
      ".btn-login {"
        "  font-size: 0.9rem;"
        " letter-spacing: 0.05rem;"
        " padding: 0.75rem 1rem;"
        "}"
        ".btn-google {"
          "color: white !important;"
          " background-color: #ea4335;"
          "}"
          ".btn-facebook {"
            "color: white !important;"
            "background-color: #0080bb;"
            "}"
            ".styleLogo{"
              "width:25rem;"
      "}"
      "</style>"
      "<head>"
            "<link rel='stylesheet' href='style1'/>"
            "<link rel='stylesheet' href='style2'/>"
            "<link rel='stylesheet' href='style3'/>"
            "<title>CONFIGURACI&Oacute;N</title>"
          "</head>"
      "<body>"
        "<div class='container'>"
          "<div class='row'>"
            "<div class='col-sm-9 col-md-7 col-lg-5 mx-auto'>"
              "<div class='card border-0 shadow rounded-3 my-5'>"
                "<div class='card-body p-4 p-sm-5'>"
                  "<h5 class='card-title text-center mb-5 fw-light fs-5'>CONFIGURACI&Oacute;N</h5>"
                  "<form  action='/get'>"
                    "<h6>Redes disponibles</h6>"
                      "<ol>"
                      "<li>";                                            //Fin del primer documento HTML
       String formulario2 = 
                 "</li>"                                                           
                "</ol>" 
                "<div class='form-floating mb-3'>"
                "<input name='input1' type='text' class='form-control' id='floatingInput' placeholder='pepito1234' required='true'>"
                "<label for='floatingInput'>'Escriba el n&uacute;mero de la red wifi (ejemplo: 1 o 2 o 3)'</label>"
              "</div>"
              "<div class='form-floating mb-3'>"
                "<input name='input2' type='password' class='form-control' id='floatingPassword' placeholder='Password' required='true'>"
                "<label for='floatingPassword'>Constrase&ntilde;a</label>"
              "</div>"  
              "<div class='d-grid'>"
                "<button class='btn btn-primary btn-login text-uppercase fw-bold' type='submit'>Conectar</button>"
              "</div>"
            "</form>"
          "</div>"
        "</div>"
      "</div>"
    "</div>"
  "</div>"
"</body>"
"</html>";                                               //Fin del documento HTML
    request->send(200, "text/html", formulario+inputMessage1+formulario2);                 //Envío del formulario como respuesta al cliente 
  });
  
  server.onNotFound(notFound);
  server.begin();
}

void loop() {}
void testFileIO(fs::FS &fs, const char * path){
   Serial.printf("Testing file I/O with %s\r\n", path);

   static uint8_t buf[512];
   size_t len = 0;
   File file = fs.open(path, FILE_WRITE);
   if(!file){
      Serial.println("− failed to open file for writing");
      return;
   }

   size_t i;
   Serial.print("− writing" );
   uint32_t start = millis();
   for(i=0; i<2048; i++){
      if ((i & 0x001F) == 0x001F){
         Serial.print(".");
      }
      file.write(buf, 512);
   }
   Serial.println("");
   uint32_t end = millis() - start;
   Serial.printf(" − %u bytes written in %u ms\r\n", 2048 * 512, end);
   file.close();

   file = fs.open(path);
   start = millis();
   end = start;
   i = 0;
   if(file && !file.isDirectory()){
      len = file.size();
         size_t flen = len;
         start = millis();
         Serial.print("− reading" );
         while(len){
            size_t toRead = len;
            if(toRead > 512){
                toRead = 512;
            }
            file.read(buf, toRead);
            if ((i++ & 0x001F) == 0x001F){
              Serial.print(".");
            }
            len -= toRead;
         }
      Serial.println("");
      end = millis() - start;
      Serial.printf("- %u bytes read in %u ms\r\n", flen, end);
      file.close();
   } else {
      Serial.println("- failed to open file for reading");
   }
}
void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
   Serial.printf("Listing directory: %s\r\n", dirname);

   File root = fs.open(dirname);
   if(!root){
      Serial.println("− failed to open directory");
      return;
   }
   if(!root.isDirectory()){
      Serial.println(" − not a directory");
      return;
   }

   File file = root.openNextFile();
   while(file){
      if(file.isDirectory()){
         Serial.print("  DIR : ");
         Serial.println(file.name());
         if(levels){
            listDir(fs, file.name(), levels -1);
         }
      } else {
         Serial.print("  FILE: ");
         Serial.print(file.name());
         Serial.print("\tSIZE: ");
         Serial.println(file.size());
      }
      file = root.openNextFile();
   }
}
