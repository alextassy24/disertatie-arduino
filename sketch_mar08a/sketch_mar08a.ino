#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ezButton.h>

TinyGPSPlus gps;
//TinyGPSPlus lastGps;
SoftwareSerial mygps(10, 11); // tx - 11, rx - 10
SoftwareSerial sim800L(9, 8); // tx - 8, rx - 9
//ezButton toggleSwitch(7);
//bool switchStatePrev = false;

String apn = "";                       //APN
String apnUser = "";                     //APN-Username
String apnPamygpsword = "";                     //APN-Pamygpsword
String url = "https://disertatie-api.azurewebsites.net/";  //URL for HTTP-POST-REQUEST
String serialNumber;
String deviceID;

void setup() {
  Serial.begin(9600);
  mygps.begin(9600);
  sim800L.begin(9600);
  pinMode(13, OUTPUT);

  //  toggleSwitch.setDebounceTime(50);

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

    // Check if GPRS connection is succemygpsfully enabled
    if (sim800L.find("OK")) {
      Serial.println("GPRS enabled!");
      Serial.println("Setup Complete!");
    }
    else {
      Serial.println("Failed to enable GPRS connection!");
    }
  }
  else {
    Serial.println("SIM800L initialization failed!");
  }
}

void loop() {
  //  toggleSwitch.loop();

  //  while (sim800L.available()) {
  //    Serial.println(sim800L.readString());
  //  }
  //
  //  while (Serial.available()) {
  //    sim800L.println(Serial.readString());
  //  }

  //  if (toggleSwitch.getState() != switchStatePrev) {
  //    switchStatePrev = toggleSwitch.getState();
  //    Serial.println(switchStatePrev ? "The switch is ON" : "The switch is OFF");
  //  }
  //  if (toggleSwitch.getState() == HIGH) {
  //
  //  }

  while (mygps.available()) {
    gps.encode(mygps.read());
  }
  if (gps.location.isUpdated())
  {
    Serial.print("Satelite count: ");
    Serial.println(gps.satellites.value());
    Serial.print("Latitude: ");
    Serial.print(gps.location.lat(), 6);
    Serial.print(", ");
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("Speed KMPH: ");
    Serial.println(gps.speed.kmph());
    Serial.print("Altitude Meters: ");
    Serial.println(gps.altitude.meters());
    Serial.println("");
  }
}

//void send_http() {
//  sim800L.println("AT");
//  runsl();//Print GSM Status an the Serial Output;
//  delay(4000);
//  sim800L.println("AT+SAPBR=3,1,Contype,GPRS");
//  runsl();
//  delay(100);
//  sim800L.println("AT+SAPBR=3,1,APN," + apn);
//  runsl();
//  delay(100);
//  sim800L.println("AT+SAPBR=3,1,USER," + apn_u); //Comment out, if you need username
//  runsl();
//  delay(100);
//  sim800L.println("AT+SAPBR=3,1,PWD," + apn_p); //Comment out, if you need pamygpsword
//  runsl();
//  delay(100);
//  sim800L.println("AT+SAPBR =1,1");
//  runsl();
//  delay(100);
//  sim800L.println("AT+SAPBR=2,1");
//  runsl();
//  delay(2000);
//  sim800L.println("AT+HTTPINIT");
//  runsl();
//  delay(100);
//  sim800L.println("AT+HTTPPARA=CID,1");
//  runsl();
//  delay(100);
//  sim800L.println("AT+HTTPPARA=URL," + url);
//  runsl();
//  delay(100);
//  sim800L.println("AT+HTTPPARA=CONTENT,application/x-www-form-urlencoded");
//  runsl();
//  delay(100);
//  sim800L.println("AT+HTTPDATA=192,10000");
//  runsl();
//  delay(100);
//  sim800L.println("params=" + data1 + "~" + data2);
//  runsl();
//  delay(10000);
//  sim800L.println("AT+HTTPACTION=1");
//  runsl();
//  delay(5000);
//  sim800L.println("AT+HTTPREAD");
//  runsl();
//  delay(100);
//  sim800L.println("AT+HTTPTERM");
//  runsl();
//}
//
//void runsl() {
//  while (sim800L.available()) {
//    Serial.write(sim800L.read());
//  }
//
//}
