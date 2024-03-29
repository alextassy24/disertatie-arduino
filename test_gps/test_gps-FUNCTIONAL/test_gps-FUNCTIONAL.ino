#include <TinyGPS++.h>
#include <SoftwareSerial.h>

TinyGPSPlus gps;
SoftwareSerial ss(10, 11); // RX= 10, TX=11

void setup()
{
  Serial.begin(9600);
  ss.begin(9600);
  Serial.println(F("Setup complete"));
}

void loop()
{
  while (ss.available()) {
    gps.encode(ss.read());
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

//void displayInfo()
//{
//  Serial.print(F("Location: "));
//  if (gps.location.isValid())
//  {
//    Serial.print("Latitude = ");
//    Serial.print(gps.location.lat(), 6);
//    Serial.print(F(","));
//    Serial.print("Longitude = ");
//    Serial.print(gps.location.lng(), 6);
//  }
//  else
//  {
//    Serial.print(F("Location is INVALID"));
//  }
//
//  Serial.print(F("  Date "));
//  if (gps.date.isValid())
//  {
//    Serial.print(gps.date.month());
//    Serial.print(F("/"));
//    Serial.print(gps.date.day());
//    Serial.print(F("/"));
//    Serial.print(gps.date.year());
//  }
//  else
//  {
//    Serial.print(F("INVALID"));
//  }
//  Serial.println();
//}
