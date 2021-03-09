#include <TinyGPS++.h>
#include <SoftwareSerial.h>
/*
   This sample sketch demonstrates the normal use of a TinyGPS++ (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/
static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;

//Variables propias
double lat = 0;
double lng = 0;
double presion = 100;
String jsonPost = "";
bool okUbi = false;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup() {
  Serial.begin(115200);
  ss.begin(GPSBaud);
}

void loop() {
  
  while (ss.available() > 0) {
    if (gps.encode(ss.read())){
      mostrarUbicacion();
      mostrarFechaHora();
    }
  }

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No se detectó el GPS. Revise la conexion."));
    while(true);
  }
  
}

void mostrarUbicacion() {
  if (gps.location.isValid()) {
    Serial.print(F("Ubicacion: ")); 
    Serial.print(gps.location.lat(),8);
    Serial.print(F(","));
    Serial.println(gps.location.lng(),8);
    okUbi = true;
  }
}

void mostrarFechaHora() {
  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}

void obtenerUbicacion() {
  if (gps.location.isValid()) {
    lat = gps.location.lat();
    lng = gps.location.lng();
  }
}

void generarJson() {
  jsonPost = ""
}
