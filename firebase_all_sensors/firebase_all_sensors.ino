//Funciona

#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "RAMIREZ_GARCIA"//"RamirezGarcia"
#define WIFI_PASSWORD "83862900"//"user15371333"

// Insert Firebase project API Key
#define API_KEY "AIzaSyBQDCotOdVybQB5dxW7kOSaGogeUntSeXA"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://diploma-waterflow-default-rtdb.firebaseio.com" 

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;

////////////////Sensor related variables
float temperature, pressure, dHeight, velocity, dFlow;


void setup(){
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop(){
  read_pressure_temperature();
  read_water_flow_delta();
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 10000 || sendDataPrevMillis == 0)){ //Publish data each 10 seconds
    sendDataPrevMillis = millis();
    
    // Write temperature value
    if (Firebase.RTDB.setFloat(&fbdo, "RIVER1/temperature", temperature)){
      Serial.println("Wrote to: "+fbdo.dataPath());
    }
    else {
      Serial.println("ERROR: " + fbdo.errorReason());
    }
    
    // Write pressure value
    if (Firebase.RTDB.setFloat(&fbdo, "RIVER1/pressure", pressure)){
      Serial.println("Wrote to: "+fbdo.dataPath());
    }
    else {
      Serial.println("ERROR: " + fbdo.errorReason());
    }

    //Write dHeight value 
    if (Firebase.RTDB.setFloat(&fbdo, "RIVER1/dHeight", dHeight)){
      Serial.println("Wrote to: "+fbdo.dataPath());
    }
    else {
      Serial.println("ERROR: " + fbdo.errorReason());
    }

    //Write velocity value
    if (Firebase.RTDB.setFloat(&fbdo, "RIVER1/velocity", velocity)){
      Serial.println("Wrote to: "+fbdo.dataPath());
    }
    else {
      Serial.println("ERROR: " + fbdo.errorReason());
    }

    //Write delta flow value
    if (Firebase.RTDB.setFloat(&fbdo, "RIVER1/dFlow", dFlow)){
      Serial.println("Wrote to: "+fbdo.dataPath());
    }
    else {
      Serial.println("ERROR: " + fbdo.errorReason());
    }

    //Add point to plot //This part is not tested yet
    if (Firebase.RTDB.pushTimestamp(&fbdo, "RIVER1/Xtemp_plot")) {
      Serial.println(fbdo.dataPath());//For debugging
      Serial.println(fbdo.pushName());
      Serial.println(fbdo.dataPath() + "/"+ fbdo.pushName());
    }
    else {
      Serial.println(fbdo.errorReason());
    }
    if (Firebase.RTDB.pushFloat(&fbdo, "RIVER1/Ytemp_plot", temperature)) {
      Serial.println(fbdo.dataPath());//For debugging
      Serial.println(fbdo.pushName());
      Serial.println(fbdo.dataPath() + "/"+ fbdo.pushName());
    }
    else {
      Serial.println(fbdo.errorReason());
    }
    
  }
}


/////////////////////////////////////FUNCTIONS

void read_pressure_temperature(){
   temperature=26.3; //deg Cfake data for now
   pressure=810; //hPa
}

void read_water_flow_delta(){
  float width=2.3; //m This value is known for each river
  float initial_height=1.00; //This value is known for each river at a given time //m
  float height=1.49; //m Value from sensor (fake value for now)
  dHeight=height-initial_height;
  velocity=0.1; //m/s Value from sensor (fake value for now)
  dFlow=dHeight*width*velocity; //m3 per second
}
