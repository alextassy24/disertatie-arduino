#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ezButton.h>

TinyGPSPlus gps;
TinyGPSPlus lastGps;
SoftwareSerial mygps(8, 9);
#define rxPin 0
#define txPin 1
SoftwareSerial sim800L(rxPin, txPin);

ezButton toggleSwitch(7);
bool switchStatePrev = false;
bool gpsDataAvailable = false;

void setup() {
  Serial.begin(9600);
  mygps.begin(9600);
  sim800L.begin(9600);
  toggleSwitch.setDebounceTime(50);

  Serial.println("Initializing SIM800L...");
  delay(1000);

  // Configure SIM800L for GPRS connection
  // Check if SIM800L is responding
  sim800L.println("AT");
  delay(100);
  while (sim800L.available()) {
    Serial.write(sim800L.read());
  }

  // Check if SIM800L responded with "OK"
  if (sim800L.find("OK")) {
    // Enable GPRS connection with appropriate APN settings
    sim800L.println("AT+SAPBR=3,1,\"APN\",\"live.vodafone.com\""); // Set APN
    delay(100);
    while (sim800L.available()) {
      Serial.write(sim800L.read());
    }

    sim800L.println("AT+SAPBR=1,1"); // Enable GPRS connection
    delay(100);
    while (sim800L.available()) {
      Serial.write(sim800L.read());
    }

    // Check if GPRS connection is successfully enabled
    if (sim800L.find("OK")) {
      Serial.println("GPRS enabled!");
      Serial.println("Setup Complete!");
    } else {
      Serial.println("Failed to enable GPRS connection!");
    }
  } else {
    Serial.println("SIM800L initialization failed!");
  }
}

void loop() {
  toggleSwitch.loop();

  while (sim800L.available()) {
    Serial.println(sim800L.readString());
  }
  while (Serial.available())  {
    sim800L.println(Serial.readString());
  }

  int state = toggleSwitch.getState();
  
  if (state != switchStatePrev) {
    switchStatePrev = state;
    if (state == HIGH) {
      Serial.println("The switch is ON");
    }
    else {
      Serial.println("The switch is OFF");
    }
    delay(1000);
  }

  if (state == HIGH) {
    if (mygps.available()) {
      while (mygps.available()) {
        lastGps = gps;
        char c = mygps.read();
        gps.encode(c);
      }

      if (gps.location.isUpdated()) {
        Serial.println("GPS location updated");
        Serial.print("Latitude= ");
        Serial.print(gps.location.lat(), 6);
        Serial.print(" Longitude= ");
        Serial.println(gps.location.lng(), 6);
        gpsDataAvailable = true;
      }
      else if (gps.location.lat() == lastGps.location.lat() &&
               gps.location.lng() == lastGps.location.lng() &&
               gps.location.lat() != 0 &&
               gps.location.lng() != 0) {
        if (!gpsDataAvailable) {
          gpsDataAvailable = true;
          Serial.print("Same position");
          Serial.print("Latitude= ");
          Serial.print(lastGps.location.lat(), 6);
          Serial.print("Longitude= ");
          Serial.println(lastGps.location.lng(), 6);
        }
      }
    }
    else {
      if (gpsDataAvailable) {
        gpsDataAvailable = false;
        Serial.println("No data available from GPS module");
      }
    }
  }

  delay(1000); // Delay between loop iterations
}
