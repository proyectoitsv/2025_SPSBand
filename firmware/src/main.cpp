#include "Oximetro.h"
#include "SMS.h"
#include "GPS.h"
/*
#define RX_D PIN_006
#define TX_D PIN_008
#define RX_2 PIN_010
#define TX_2 PIN_011
#define NUMERO "+549351xxxxxxx" //Cambiar por mi número de teléfono

gpsSerial *SerialGPS;
smsSerial *SerialSMS;

String url = SerialGPS.getUrl();
unsigned long time0 =0;

void setup()
{
  Serial.begin(115200);
  Serial.println("Initializing...");
  SerialGPS = new gpsSerial(RX_2, TX_2);
  SerialSMS = new smsSerial(RX_D, TX_D);
  if(i2cBegin(20, 0) != 0){
    Serial.println("Error");
    while(1);
  }
  Serial.println("I2C detected");
}

void loop(){ 
  time0 = millis(); 
  Serial.println("Mantené el dispositivo en tu muñeca o dedo durante al menos 10 segundos.");
  int beatAvg = getBeatAvg(10);
  String location = SerialGPS.getUrl();
  String message = "Ritmo cardíaco: "+ (String)beatAvg+" BPM "+ "Ubicación: " + location;
  SerialSMS->textMode(1);
  SerialSMS->sendMessage(NUMERO, message);
  SerialSMS->textMode(0);
  Serial.println("Mensaje enviado: \""+ message + "\"");
}

*/

#include "GPS.h"
#include "Oximetro.h"
#include "SMS.h"

// ========== CONFIGURACIÓN DE PINES ==========
// Pines para GPS (pines UART hardware)
#define GPS_RX PIN_029   // Pin RX hardware del nRF52840
#define GPS_TX PIN_031   // Pin TX hardware del nRF52840

// Pines para Módem SMS
#define SMS_RX PIN_006   // Pin D0 (P0.06)
#define SMS_TX PIN_008   // Pin D1 (P0.08)

// ========== CONFIGURACIÓN DE PARÁMETROS ==========
String numeroDestino = "+5493512345678"; // CAMBIAR ESTE NÚMERO
const unsigned long intervaloEnvio = 6000; // 6 segundos entre mensajes

// ========== OBJETOS GLOBALES ==========
gpsSerial* gps;
smsSerial* modem;

// ========== VARIABLES DE CONTROL ==========
unsigned long ultimoEnvio = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Iniciando sistema...");
  
  // Inicializar GPS
  Serial.println("Inicializando GPS...");
  gps = new gpsSerial(GPS_RX, GPS_TX);
  delay(1000);
  
  // Inicializar Módem SMS
  Serial.println("Inicializando Modem SMS...");
  modem = new smsSerial(SMS_RX, SMS_TX, 9600);
  delay(2000);
  
  // Configurar modo texto para SMS
  modem->textMode(1);
  delay(1000);
  
  // Inicializar Oxímetro (I2C)
  Serial.println("Inicializando Oximetro MAX30102...");
  if (i2cBegin(0x1F, 0) != 0) {
    Serial.println("ERROR: No se pudo inicializar el oximetro!");
    while(1) {
      delay(1000);
    }
  }
  Serial.println("Oximetro inicializado correctamente");
  
  Serial.println("\n========== SISTEMA LISTO ==========");
  Serial.println("Esperando datos del GPS...");
  Serial.println("===================================\n");
}

void loop() {
  unsigned long tiempoActual = millis();
  
  // Verificar si es momento de enviar SMS
  if (tiempoActual - ultimoEnvio >= intervaloEnvio) {
    
    Serial.println("\n--- Iniciando ciclo de medicion ---");
    
    // 1. Obtener ubicación GPS
    Serial.println("Obteniendo ubicacion GPS...");
    String ubicacion = gps->getUrl();
    Serial.print("Ubicacion: ");
    Serial.println(ubicacion);
    
    // 2. Obtener BPM del oxímetro
    Serial.println("Midiendo frecuencia cardiaca (10 segundos)...");
    int bpm = getBeatAvg(4); // Promedio de 4 valores
    Serial.print("BPM: ");
    Serial.println(bpm);
    
    // 3. Construir mensaje
    String mensaje = "Ubicacion: " + ubicacion + "\nBPM: " + String(bpm);
    
    Serial.println("\n--- Enviando SMS ---");
    Serial.print("Destinatario: ");
    Serial.println(numeroDestino);
    Serial.print("Mensaje: ");
    Serial.println(mensaje);
    
    // 4. Enviar SMS
    modem->sendMessage(numeroDestino, mensaje);
    
    Serial.println("SMS enviado!");
    Serial.println("--- Fin del ciclo ---\n");
    
    // Actualizar tiempo del último envío
    ultimoEnvio = millis();
  }
  
  // Pequeño delay para no saturar el procesador
  delay(100);
}