#define TINY_GSM_MODEM_SIM800 // Define the modem model for TinyGSM

#include <TinyGsmClient.h>
#include <ArduinoHttpClient.h>
#include <SoftwareSerial.h>

// Set serial for debug console (to the Serial Monitor, default speed 9600)
#define SerialMon Serial

// Set serial for AT commands (to the module)
#define SerialAT Serial1  // Use Serial1 on boards that have it

// Uncomment below line and update the pins if you're using SoftwareSerial
SoftwareSerial SerialAT(3, 2); // RX, TX pins if using SoftwareSerial

// Your GPRS credentials
const char apn[]  = "live.vodafone.ro"; // Your APN
const char user[] = "live";                     // Your user
const char pass[] = "";                     // Your password

// Server details
const char server[] = "disertatie-api.azurewebsites.net";
const char resource[] = "/t";
const int  port = 80;

TinyGsm modem(SerialAT);
TinyGsmClient client(modem);
HttpClient http(client, server, port);

void setup() {
  // Set console baud rate
  Serial.begin(9600);
  SerialMon.begin(9600);
  delay(1000);

  // Set GSM module baud rate
  SerialAT.begin(9600);
  delay(3000);

  // Initialize modem
  SerialMon.println("Initializing modem...");
  modem.restart();
  String modemInfo = modem.getModemInfo();
  SerialMon.print("Modem Info: ");
  SerialMon.println(modemInfo);

  // Network check and connect to GPRS
  SerialMon.println("Waiting for network...");
  if (!modem.waitForNetwork()) {
    SerialMon.println("Fail");
    while (true);
  }
  SerialMon.println("Network connected");

  // GPRS connect
  SerialMon.println("Connecting to GPRS...");
  if (!modem.gprsConnect(apn, user, pass)) {
    SerialMon.println("Fail");
    while (true);
  }
  SerialMon.println("GPRS connected");
}

void loop() {
  sendData();
  delay(10000);  // Send data every 10 seconds
}

void sendData() {
  SerialMon.println("Performing HTTP GET request...");
  int err = http.get(resource);
  if (err != 0) {
    SerialMon.println("Failed to start request");
    return;
  }

  int status = http.responseStatusCode();
  SerialMon.print("Response status code: ");
  SerialMon.println(status);

  String body = http.responseBody();
  SerialMon.print("Response body: ");
  SerialMon.println(body);

  SerialMon.println("HTTP request complete.");
}
