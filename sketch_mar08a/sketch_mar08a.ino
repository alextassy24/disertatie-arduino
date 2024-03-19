#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ezButton.h>

TinyGPSPlus gps;
TinyGPSPlus lastGps;
SoftwareSerial mygps(4, 3); // tx-4, rx-3
#define rxPin 0
#define txPin 1
SoftwareSerial sim800L(rxPin, txPin);

ezButton toggleSwitch(7);
bool switchStatePrev = false;

void setup() {
  Serial.begin(9600);
  mygps.begin(9600);
  sim800L.begin(9600);
  pinMode(13, OUTPUT);

  toggleSwitch.setDebounceTime(50);

  //  Serial.println("Initializing SIM800L...");
  //  delay(1000);
  //
  //  // Configure SIM800L for GPRS connection
  //  // Check if SIM800L is responding
  //  sim800L.println("AT");
  //  delay(100);
  //  while (sim800L.available()) {
  //    Serial.write(sim800L.read());
  //  }
  //
  //  // Check if SIM800L responded with "OK"
  //  if (sim800L.find("OK")) {
  //    // Enable GPRS connection with appropriate APN settings
  //    sim800L.println("AT+SAPBR=3,1,\"APN\",\"live.vodafone.com\""); // Set APN
  //    delay(100);
  //    while (sim800L.available()) {
  //      Serial.write(sim800L.read());
  //    }
  //
  //    sim800L.println("AT+SAPBR=1,1"); // Enable GPRS connection
  //    delay(100);
  //    while (sim800L.available()) {
  //      Serial.write(sim800L.read());
  //    }
  //
  //    // Check if GPRS connection is successfully enabled
  //    if (sim800L.find("OK")) {
  //      Serial.println("GPRS enabled!");
  //      Serial.println("Setup Complete!");
  //    }
  //    else {
  //      Serial.println("Failed to enable GPRS connection!");
  //    }
  //  }
  //  else {
  //    Serial.println("SIM800L initialization failed!");
  //  }
}

void loop() {
  toggleSwitch.loop();

  //  while (sim800L.available()) {
  //    Serial.println(sim800L.readString());
  //  }
  //
  //  while (Serial.available()) {
  //    sim800L.println(Serial.readString());
  //  }

  if (toggleSwitch.getState() != switchStatePrev) {
    switchStatePrev = toggleSwitch.getState();
    Serial.println(switchStatePrev ? "The switch is ON" : "The switch is OFF");
  }

  if (toggleSwitch.getState() == HIGH) {
    while (mygps.available() > 0)
      if (gps.encode(mygps.read())) {
        displayInfo();
        digitalWrite(13, LOW);
      }
    if (millis() > 5000 && gps.charsProcessed() < 10)
    {
      //      Serial.println(F("No GPS detected: check wiring."));
      digitalWrite(13, HIGH); // Turn the LED on (HIGH voltage)

    }
  }
  delay(1000);
}


void displayInfo()
{
  Serial.print(F("Location: "));
  if (gps.location.isValid())
  {
    Serial.print("Latitude = ");
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print("Longitude = ");
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("Location is INVALID"));
  }

  Serial.print(F("  Date "));
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
  Serial.println();
}
