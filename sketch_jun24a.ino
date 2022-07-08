/*********
EJEMPLO GET REQUEST FOR INPUT ESP32.
*********/

#include <Arduino.h>
#ifdef ESP32
  #include <WiFi.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <AsyncTCP.h>
  #include <SPIFFS.h>
  #include <FS.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>


AsyncWebServer server(80);
//User y password wifi AP
const char* userAdmin = "admin";
const char* passwordAdmin = "admin";

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
 server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request){  
    String inputMessage1;
    String inputMessage2;
    inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
    inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
    if(inputMessage1.equals(userAdmin) && inputMessage2.equals(passwordAdmin) ){
      String formulario =                                        //Construcción del formulario HTML de respuesta en cadena de texto
     "<!DOCTYPE HTML>"
      "<html>"
      "<style>"
          "body {"
        "background: #0080bb;"
        "background: linear-gradient(to right, #0080bb, #33AEFF);"
      "}"
      "@media (min-width: 1400px)"
      ".container, .container-lg, .container-md, .container-sm, .container-xl, .container-xxl" 
      "{"
        "zoom: 0;"
       "}"
       "@media (min-width: 768px)"
      ".container, .container-md, .container-sm" 
      "{"
        "zoom: 2;"
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
            "<div class='col-sm-9 col-md-12 col-lg-5 mx-auto'>"
              "<div class='card border-0 shadow rounded-3 my-5'>"
                "<div class='card-body p-4 p-sm-5'>"
                  "<h5 class='card-title text-center mb-5 fw-light fs-5'>CONFIGURACI&Oacute;N</h5>"
                  "<form  action='/connection'>"
                    "<h6>Redes disponibles</h6>"
                      "<ol>"
                      "<li>";                                            //Fin del primer documento HTML
                      
       String formulario2 =                                              //Inicio del segundo documento HTML
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
                "<div>"
                "<label for='mac'>"
                "<strong>MAC: </strong>"
                + WiFi.macAddress() + 
                "</label>"
                "</div>"
              "<div class='d-grid mt-3'>"
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
    request->send(200, "text/html", formulario+inputMessage1+formulario2);  
  }else{
    int IPLocal = WiFi.localIP();
    String IPRedirect = inet_ntoa(IPLocal);
    request->send(404, "text/html", "<script>alert ('Usuario y/o Contrasenia incorrectos');window.location.href='http://"+IPRedirect+"'</script>");
  }
  });

//Tercer documento html para mostrar el mensaje de conectado exitosamente o error al conectar con la red wifi
 server.on("/connection", HTTP_GET, [] (AsyncWebServerRequest *request){  
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
      "@media (min-width: 1400px)"
      ".container, .container-lg, .container-md, .container-sm, .container-xl, .container-xxl" 
      "{"
        "zoom: 0;"
       "}"
       "@media (min-width: 768px)"
      ".container, .container-md, .container-sm" 
      "{"
        "zoom: 2;"
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
            "<div class='col-sm-9 col-md-12 col-lg-5 mx-auto'>"
              "<div class='card border-0 shadow rounded-3 my-5'>"
                "<div class='card-body p-4 p-sm-5'>"
                  "<h5 class='card-title text-center mb-5 fw-light fs-5'>CONFIGURACI&Oacute;N</h5>"
                  "<form  action='/get'>"
                    "<h6>Redes disponibles</h6>"
                      "<ol>"
                      "<li>";                                            //Fin del primer documento HTML
                      
       String formulario2 =                                              //Inicio del segundo documento HTML
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
               "<div>"
                "<label for='mac'>"
                "<strong>MAC: </strong>"
                + WiFi.macAddress() + 
                "</label>"
                "</div>"  
              "<div class='d-grid'>"
                "<button class='btn btn-primary btn-login text-uppercase fw-bold' type='submit'>Conectar</button>"
              "</div>"
            "</form>"
            "<h5 style='text-align:center' class='mt-4'>"; //Fin de documento

 String formulario3 =                                              //Inicio del segundo documento HTML
          "</h5>"
          "</div>"
        "</div>"
      "</div>"
    "</div>"
  "</div>"
"</body>"
"</html>"; //Fin del documento HTML //Fin del documento HTML
    request->send(200, "text/html", formulario+inputMessage1+formulario2+"Conexi&oacute;n exitosa o error al conectar"+formulario3);                 //Envío del formulario como respuesta al cliente 
  });
  server.onNotFound(notFound);
  server.begin();
}

void loop() {}
