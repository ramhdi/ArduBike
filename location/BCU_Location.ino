/**************************************************************

   TinyGSM Getting Started guide:
     http://tiny.cc/tiny-gsm-readme

   NOTE:
   Some of the functions may be unavailable for your modem.
   Just comment them out.

 **************************************************************/

/*
 * Suryapraka Bike Controller Unit
 * v1 24/07/18
 * Successfully connected device to freeboard.io and send GPS location
 * data via dweet.io by the means of POST
 */
// Select your modem:
#define TINY_GSM_MODEM_SIM808

// Set serial for debug console (to the Serial Monitor, speed 115200)
#define SerialMon Serial

// Set serial for AT commands (to the module)
// Use Hardware Serial on Mega, Leonardo, Micro
//#define SerialAT Serial1

// or Software Serial on Uno, Nano
#include <SoftwareSerial.h>
SoftwareSerial SerialAT(2, 3); // RX, TX


//#define DUMP_AT_COMMANDS
#define TINY_GSM_DEBUG SerialMon

// Your GPRS credentials
// Leave empty, if missing user or pass
const char apn[]  = "m2mdev";
const char user[] = "";
const char pass[] = "";

#include <TinyGsmClient.h>
#include <ArduinoHttpClient.h>

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif

int commaTimes;
int lat_start, lat_end, lon_start, lon_end, alt_start, alt_end;
float lat_gsm, lon_gsm, lat_gps, lon_gps, lat_appr, lon_appr, speed;
int alt, usat, vsat;

const char server[] = "dweet.io";
int port = 80;

TinyGsmClient client(modem);

void setup() {
  // Set console baud rate
  SerialMon.begin(9600);
  delay(10);

  // Set GSM module baud rate
  SerialAT.begin(9600);
  delay(3000);

  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  DBG("Initializing modem...");
  if (!modem.restart()) {
    delay(10000);
    return;
  }

  String modemInfo = modem.getModemInfo();
  DBG("Modem:", modemInfo);

  // Unlock your SIM card with a PIN
  //modem.simUnlock("1234");

  DBG("Waiting for network...");
  if (!modem.waitForNetwork()) {
    delay(10000);
    return;
  }

  DBG("Connecting to", apn);
  if (!modem.gprsConnect(apn, user, pass)) {
    delay(10000);
    return;
  }

  bool res;

  String ccid = modem.getSimCCID();
  DBG("CCID:", ccid);

  String imei = modem.getIMEI();
  DBG("IMEI:", imei);

  String cop = modem.getOperator();
  DBG("Operator:", cop);

  IPAddress local = modem.localIP();
  DBG("Local IP:", local);

  int csq = modem.getSignalQuality();
  DBG("Signal quality:", csq);

  // This is NOT supported on M590
  int battLevel = modem.getBattPercent();
  DBG("Battery lavel:", battLevel);

  // This is only supported on SIMxxx series
  float battVoltage = modem.getBattVoltage() / 1000.0F;
  DBG("Battery voltage:", battVoltage);

  modem.enableGPS();
}

void loop() {
  HttpClient http(client, server, port);
  // This is only supported on SIMxxx series
  String gsmLoc = modem.getGsmLocation();
  DBG("GSM location:", gsmLoc);

  int commaTimes = 0;
  //membaca posisi dari gsm
  //mencari koma
  int i;
  for (i = 0; i < gsmLoc.length(); i++)
  {
    if (gsmLoc[i] == ',')
    {
      commaTimes += 1;
    }

    if (commaTimes == 0)
    {
      lon_start = i + 2;
    }

    else if (commaTimes == 1)
    {
      lon_end = i + 1;
      lat_start = i + 2;
    }

    else if (commaTimes == 2)
    {
      lat_end = i + 1;
    }
  }

  //mencari lat
  String str_lat = gsmLoc.substring(lat_start, lat_end);
  lat_gsm = str_lat.toFloat();

  //mencari lon
  String str_lon = gsmLoc.substring(lon_start, lon_end);
  lon_gsm = str_lon.toFloat();

  Serial.print("lat_gsm: "); Serial.println(lat_gsm, 6);
  Serial.print("lon_gsm: "); Serial.println(lon_gsm, 6);
  Serial.println("");

  //membaca posisi dari gps
  if (!modem.getGPS(&lat_gps, &lon_gps, &speed, &alt, &usat, &vsat))
  {
    Serial.println("GPS no fix");
  } else {
    String gps_raw = modem.getGPSraw();
    Serial.print("GPS Location: "); Serial.println(gps_raw);

    Serial.print("lat_gps: "); Serial.println(lat_gps, 6);
    Serial.print("lon_gps: "); Serial.println(lon_gps, 6);
    Serial.print("alt: "); Serial.println(alt);

    Serial.println("Mengirimkan data gps ke dweet 1te-bike");
    String dweetName = "1te-bike";
    String path = "/dweet/for/" + dweetName;
    String contentType = "application/json";
    String postData = "{";
    postData += " \"lat\" : \" "; postData += String(lat_gps, 6); postData += "\",";
    postData += " \"lon\" : \" "; postData += String(lon_gps, 6); postData += "\"";
    postData += "}";

    // send the POST request
    http.post(path, contentType, postData);
    http.stop();
    // read the status code and body of the response
    /*
      int statusCode = http.responseStatusCode();
      String response = http.responseBody();

      Serial.print("Status code: ");
      Serial.println(statusCode);
      Serial.print("Response: ");
      Serial.println(response);
    */
  }


  Serial.println("");

  delay(10000);
}

