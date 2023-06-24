//Codigp tomado de https://www.bitcraze.io/products/old-products/flow-breakout/
//Funciona

/*Conexiones con tarjeta bitcraze:
pines izquierdos vistos desde "front up":
 * 1. Vcc: 3.3 V
 * 4. A4 (SDA) 
 * 5. A5 (SCL)
 * 8. 10
 * 10. GND
pines derechos vistos desde "front up"
 * 3. 13
 * 4. 12
 * 5. 11
 * 9. 5.0 V

Según la hoja de datos el voltaje en los pines I2C y SPI debría ser hasta 3.3 V,
por tanto se recomienda usar la tarjeta con Arduinos de 3.3V.
 */

#include "Bitcraze_PMW3901.h"
#include <Wire.h>
#include <VL53L0X.h>

VL53L0X rangeSensor;

// Using digital pin 10 for chip select
Bitcraze_PMW3901 flow(10);

void setup() {
  Serial.begin(9600);

  // Initialize flow sensor
  if (!flow.begin()) {
    Serial.println("Initialization of the flow sensor failed");
    while(1) { }
  }

  // Initialize range sensor
  Wire.begin();

  rangeSensor.init();
  rangeSensor.setTimeout(500);
}

int16_t deltaX,deltaY;

void loop() {
  // Get motion count since last call
  flow.readMotionCount(&deltaX, &deltaY);

  // Get single range measurement
  float range = rangeSensor.readRangeSingleMillimeters();

  //Serial.print("X ");
  Serial.print(deltaX);
  Serial.print("\t");
  //Serial.print("Y ");
  Serial.print(deltaY);
  //Serial.print(", Range: ");
  if (range > 5000) {
    //Serial.print("N/A");
  } else {
    //Serial.print(range);
  }
  Serial.println("");
  //Serial.print("\n");

  delay(100);
}
