#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>

Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */

// Replace with your network credentials
const char* ssid = "Mehdi";
const char* password = "11111111";

ESP8266WebServer server(80);   //instantiate server at port 80 (http port)

String page = "";
double data; 
void setup(void)
{
  Wire.begin(D2, D1);
  pinMode(A0, INPUT);
  
  delay(1000);
  Serial.begin(115200);
  WiFi.begin(ssid, password); //begin WiFi connection
  
  Serial.println("Getting single-ended readings from AIN0..3");
  Serial.println("ADC Range: +/- 6.144V (1 bit = 3mV/ADS1015, 0.1875mV/ADS1115)");
  
  Serial.println("");
  
  while (WiFi.status() != WL_CONNECTED) // Wait for connection
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/", [](){
    page = "<h1>Sensor to Node MCU Web Server</h1><h3>Data:</h3> <h4>"+String(data)+"</h4>";
    server.send(200, "text/html", page);
  });
  
  server.begin();
  Serial.println("Web server started!");


  // The ADC input range (or gain) can be changed via the following
  // functions, but be careful never to exceed VDD +0.3V max, or to
  // exceed the upper and lower limits if you adjust the input range!
  // Setting these values incorrectly may destroy your ADC!
  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
  
  ads.begin(); 
}
 
void loop(void)
{
  int16_t adc0, adc1, adc2, adc3, diff_result1, diff_result2;
  
  adc0 = ads.readADC_SingleEnded(0);
  adc1 = ads.readADC_SingleEnded(1);
  adc2 = ads.readADC_SingleEnded(2);
  adc3 = ads.readADC_SingleEnded(3);
  diff_result1 = abs (adc0 - adc1);
  diff_result2 = abs (adc2 - adc3);
  //Serial.print("AIN0: "); Serial.println(adc0);
  //Serial.print("AIN1: "); Serial.println(adc1);
  //Serial.print("AIN2: "); Serial.println(adc2);
  //Serial.print("AIN3: "); Serial.println(adc3);
  //Serial.println(" ");
  Serial.println(diff_result1);
  
  delay(100);
  
  //data = analogRead(A0);
  data = diff_result1;
  //delay(1000);
  server.handleClient();
}
