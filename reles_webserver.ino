/*
   Copyright (c) 2015, Majenko Technologies
   All rights reserved.
   Redistribution and use in source and binary forms, with or without modification,
   are permitted provided that the following conditions are met:
 * * Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright notice, this
     list of conditions and the following disclaimer in the documentation and/or
     other materials provided with the distribution.
 * * Neither the name of Majenko Technologies nor the names of its
     contributors may be used to endorse or promote products derived from
     this software without specific prior written permission.
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
   ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
   ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/* Create a WiFi access point and provide a web server on it. */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#define APSSID "ESPap"
#define APPSK  "senha1234"

/* Set these to your desired credentials. */
const char *ssid = APSSID;
const char *password = APPSK;

ESP8266WebServer server(80);

#define qtdReleMAX 4

const uint8_t pinArray[qtdReleMAX] = { D0, D1, D2, D3};
const bool pinStatus[qtdReleMAX] = { LOW, LOW, LOW, LOW};

void setup() {
//  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.println("Iniciando reles...");
  for(int i=0; i< qtdReleMAX; i++)
  {
    pinMode(pinArray[i], OUTPUT);
    digitalWrite (pinArray[i], LOW);
  }
  
  Serial.println("Configuring access point...");
  WiFi.mode(WIFI_AP);
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();

  //Enquanto o ESP não se conectar à rede
   while (WiFi.status() != WL_CONNECTED)
   {
     //Esperamos 100 milisegundos
     delay(100);
     Serial.print(".");
   }
  
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  
  server.on("/", handleRoot);
  
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
   connected to this access point to see it.
*/
void handleRoot() {
  String html = 
    "<html>"
      "<head>"
        "<meta name='viewport' content='width=device-width, initial-scale=1, user-scalable=no'/>"
        "<title>Reles</title>"
        "<style>"
          "body{"
            "text-align: center;"
            "font-family: sans-serif;"
            "font-size:12px;"
            "padding: 25px;"
          "}"

          "p{"
            "color:#444;"
          "}"

          "button{"
            "outline: none;"
            "border: 2px solid #1fa3ec;"
            "border-radius:18px;"
            "background-color:#FFF;"
            "color: #1fa3ec;"
            "padding: 10px 50px;"
          "}"

          "button:active{"
            "color: #fff;"
            "background-color:#1fa3ec;"
          "}"
        "</style>"
      "</head>"
      "<body>";
      
  for(int i = 0; i < qtdReleMAX; i++)
    {   
      html =+"<p>Tomada "+ String(i+1)  +"</p>"
        "<p><a href='?pin="+ String(i)  +"&acao=On'><button>ON</button></a></p>"
        "<p><a href='?pin="+ String(i)  +"&acao=Off'><button>OFF</button></a></p>";
    }

  html =+"</body></html>";

  server.send(200, "text/html", html);
}
