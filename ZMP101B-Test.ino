#include "ZMPT101B.h"
#include "UnitCal.h"
#include "WiFi.h"
#include "ESPAsyncWebServer.h"

#define RXD2 16
#define TXD2 17

float watt = 0;
bool flush_flag = false;
float amp = 0;

AsyncWebServer server(80);
unsigned long time_now = 0;
const int period = 1000;

ZMPT101B voltmeter(34);
unsigned int volts = 0;
unsigned int avgV = 0;
unsigned int airVolts = 0;

void readVoltages() {
  volts = 0;
  
  for(int i = 0; i < 60; i++) 
  {
    avgV += voltmeter.getVoltageAC();
  }
  
  volts = avgV/60;
  avgV = 0;
  airVolts = volts; 

  if(volts < 15) {
    Serial.print("Volts: ");
    Serial.println("0"); 
    watt = volts * amp;
  } else {
    Serial.print("Volts: ");
    Serial.println(volts); 
    watt = volts * amp;  
  }
}

UnitCal units(0.9666666667);

String GetEnergyMonitor() {  
  if(airVolts > 15) {
     return "Volts: " + String(airVolts) + "V " + "Amperes: " + String(amp) + "A " + "Watts: " + String(watt) + "W " + String(units.getUnits()) + "U ";
  } else {
     return "Volts: " + String(0) + "V " + "Amperes: " + String(0) + "A " + "Watts " + String(0) + "W " + String(units.getUnits()) + "U ";
  }
}

void setup() {
  Serial.begin(115200);
  int calibrated_volts = voltmeter.calibrate();
  Serial.println(calibrated_volts);

  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  WiFi.softAP("iMeters", "alpha24245");
  IPAddress IP = WiFi.softAPIP();
  Serial.println(IP);

  server.on("/energy_monitor", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", GetEnergyMonitor().c_str());
  });

  server.begin();
  
}

void loop() {

  if(millis() >= time_now + period) {
    time_now += period;
    amp = Serial2.parseFloat();
    if(amp >= 0) {
      readVoltages();
      flush_flag = !flush_flag;
    } 
  
    //Recovery from the serial hold
    if (watt <= 0 && flush_flag) { 
      Serial2.end();
      Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
      flush_flag = !flush_flag;
    }
  }

  //Calculates Unit Calc()
  units.recordUnit(watt);
  
}
