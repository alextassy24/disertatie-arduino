#include <TinyGPS++.h>
#include <SoftwareSerial.h>

TinyGPSPlus gps;
SoftwareSerial ss(10, 11); // RX= 10, TX=11
SoftwareSerial mySerial(2, 3); // RX= 2, TX=3

void setup()
{
  Serial.begin(9600);
//  ss.begin(9600);
   mySerial.begin(9600);
   
  Serial.println("Initializing...");
  delay(1000);

  mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();
  mySerial.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
  updateSerial();
  mySerial.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
  updateSerial();
  mySerial.println("AT+CREG?"); //Check whether it has registered in the network
  updateSerial();
  mySerial.println("ATi"); //Returns the module name and revision
  updateSerial();
  mySerial.println("AT+COPS?"); //Checks which network you are connected to
  updateSerial();
  mySerial.println("AT+COPS=?"); //Returns the list of operators present in the network
  updateSerial();
  mySerial.println("AT+CBC"); //Returns Li-Po battery status. The second number is the battery level (in our case it is 93%)
  //and the third number is the actual voltage in mV
  updateSerial();
  Serial.println(F("Setup complete"));
}

void loop()
{
//  while (ss.available()) {
//    gps.encode(ss.read());
//  }
//  if (gps.location.isUpdated())
//  {
//    Serial.print("Satelite count: ");
//    Serial.println(gps.satellites.value());
//    Serial.print("Latitude: ");
//    Serial.print(gps.location.lat(), 6);
//    Serial.print(", ");
//    Serial.print("Longitude: ");
//    Serial.println(gps.location.lng(), 6);
//    Serial.print("Speed KMPH: ");
//    Serial.println(gps.speed.kmph());
//    Serial.print("Altitude Meters: ");
//    Serial.println(gps.altitude.meters());
//    Serial.println("");
//  }
//  else{
//    Serial.println("No GPS data available");
//  }
  updateSerial();
  delay(1000);
}

void updateSerial()
{
  delay(500);
  while (Serial.available())
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while (mySerial.available())
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}
