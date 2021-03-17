#include <TinyGPS++.h>
#include <SoftwareSerial.h>

//Variables propias
String jsonPOST = "";
double latitud = 0;
double longitud = 0;
double presion = 0;
bool okUbi = false;

SoftwareSerial SIM900(7, 8); // MODULO GPRS
SoftwareSerial NEO6M(4, 3);  // MODULO GPS

// The TinyGPS++ object
TinyGPSPlus gps;


void setup() {
  

  SIM900.begin(19200);
  NEO6M.begin(9600);
  Serial.begin(19200);

  initGprs();
}

void loop() {

  while (NEO6M.available() > 0) {
    //Serial.println(F("avaible TRUE"));
    if (gps.encode(NEO6M.read())) {
      obtenerDatos();
      if (okUbi == true) {
        armarJson();
        enviar();
      }
    } else {
      //Serial.println(F("Encode falso"));
    }
  }
  //Serial.println(F("Fuera While"));

  if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println(F("No se detectó el GPS. Revise la conexion."));
    while (true);
  }

}

void obtenerDatos() {
  if (gps.location.isValid()) {
    latitud = gps.location.lat();
    longitud = gps.location.lng();
    okUbi = true;
    Serial.println(F("OK!"));
  } else {
    Serial.println(F("NO HAY GPS!"));
  }
  presion = random(150,210);
}

void armarJson() {
  jsonPOST = "{\"latitud\": " + String(latitud, 6) + ",\"longitud\": " + String(longitud, 6) + ",\"presion\": " + String(presion, 2) + "}";
  Serial.println("armo json");
}

void initGprs() {
  Serial.println(F("Empezando el INIT..."));
  SIM900.println("AT");
  delay(5000);
  showSerialData();
  delay(5000);
  SIM900.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
  delay(5000);
  showSerialData();
  delay(5000);
  SIM900.println("AT+SAPBR=3,1,\"APN\",\"int.movil.com.bo\"");
  delay(5000);
  showSerialData();
  delay(2000);
  SIM900.println("AT+SAPBR=1,1");
  delay(5000);
  showSerialData();
  delay(5000);
  SIM900.println("AT+SAPBR=2,1");
  delay(5000);
  showSerialData();
  delay(5000);
  Serial.println(F("XXXXXXX  *  XXXXXXX"));
}

void enviar() {
  Serial.println(F("Empezando a enviar..."));
  SIM900.println("AT+HTTPINIT");
  delay(2000);
  showSerialData();
  delay(2000);
  SIM900.println("AT+HTTPPARA=\"CID\",1");
  delay(2000);
  showSerialData();
  delay(2000);
  SIM900.println("AT+HTTPPARA=\"URL\",\"http://glacial-eyrie-03654.herokuapp.com/api/actualizarEquipo/1\"");
  delay(2000);
  showSerialData();
  delay(2000);
  SIM900.println("AT+HTTPPARA=\"CONTENT\",\"application/json\"");
  delay(2000);
  showSerialData();
  delay(2000);
  SIM900.println("AT+HTTPDATA=" + String(jsonPOST.length()) + ",10000");
  delay(2000);
  showSerialData();
  delay(2000);
  SIM900.println(jsonPOST);
  delay(2000);
  showSerialData();
  delay(2000);
  SIM900.println("AT+HTTPACTION=1");
  delay(5000);
  showSerialData();
  delay(5000);
  SIM900.println("AT+HTTPREAD");  /* Read data from HTTP server */
  delay(5000);
  showSerialData();
  delay(5000);
  SIM900.println("AT+HTTPTERM");  /* Terminate HTTP service */
  delay(2000);
  showSerialData();
  delay(2000);
  Serial.println(F("Terminando de enviar..."));
  okUbi = false;
}

void showSerialData() {
  while (SIM900.available() > 0)
    Serial.write(char(SIM900.read()));
}