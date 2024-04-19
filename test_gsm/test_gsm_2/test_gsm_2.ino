#define SIM800L_RX     2
#define SIM800L_TX     3

String apn = "internet.vodafone.ro";                    //APN
String apn_u = "internet.vodafone.ro";                     //APN-Username
String apn_p = "";                     //APN-Password
String url = "https://disertatie-api.azurewebsites.net/check-connection";  //URL of Server

void setup()
{

  Serial.begin(9600);
  Serial.println("ESP32+SIM800L AT CMD Test");
  Serial2.begin(9600, SIM800L_TX, SIM800L_RX);
  delay(15000);
  while (Serial2.available()) {
    Serial.write(Serial2.read());
  }
  delay(2000);
  gsm_config_gprs();
}


void loop() {
  gsm_http_get();
  delay(30000);
}

void gsm_http_post( String postdata) {
  Serial.println(" --- Start GPRS & HTTP --- ");
  gsm_send_serial("AT+SAPBR=1,1");
  gsm_send_serial("AT+SAPBR=2,1");
  gsm_send_serial("AT+HTTPINIT");
  gsm_send_serial("AT+HTTPPARA=CID,1");
  gsm_send_serial("AT+HTTPPARA=URL," + url);
  gsm_send_serial("AT+HTTPPARA=CONTENT,application/x-www-form-urlencoded");
  gsm_send_serial("AT+HTTPDATA=192,5000");
  gsm_send_serial("AT+HTTPACTION=1");
  gsm_send_serial("AT+HTTPREAD");
  gsm_send_serial("AT+HTTPTERM");
  gsm_send_serial("AT+SAPBR=0,1");
}

void gsm_config_gprs() {
  Serial.println(" --- CONFIG GPRS --- ");
  gsm_send_serial("AT+SAPBR=3,1,Contype,GPRS");
  gsm_send_serial("AT+SAPBR=3,1,APN," + apn);
  if (apn_u != "") {
    gsm_send_serial("AT+SAPBR=3,1,USER," + apn_u);
  }
  if (apn_p != "") {
    gsm_send_serial("AT+SAPBR=3,1,PWD," + apn_p);
  }
}

void gsm_send_serial(String command) {
  Serial.println("Send ->: " + command);
  Serial2.println(command);
  long wtimer = millis();
  while (wtimer + 3000 > millis()) {
    while (Serial2.available()) {
      Serial.write(Serial2.read());
    }
  }
  Serial.println();
}
