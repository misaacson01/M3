#include "Adafruit_VL53L1X.h"

Adafruit_VL53L1X vl53A = Adafruit_VL53L1X();
Adafruit_VL53L1X vl53B = Adafruit_VL53L1X();
Adafruit_VL53L1X vl53C = Adafruit_VL53L1X();
int16_t distance;
int timingBudget = 100;
int samplePeriod = 500;
unsigned long beginMillis, startMillis, curMillis, endMillis;
#define TCAADDR 0x70

//tested sensor values:
//13 mm to 1600 mm

void tcaselect(uint8_t i) {
  if (i > 7) return;
 
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

void setup() {
  Wire.begin();
  Serial.begin(115200);
  // Wait until serial port is opened
  while (!Serial) { delay(1); }
  
  //connect to ToF 1
  Serial.print("Connecting to 1... ");
  tcaselect(0);
  if (! vl53A.begin(0x29, &Wire)) {
    Serial.print(F("Error on init of VL sensor: "));
    Serial.println(vl53A.vl_status);
    while (1)       delay(10);
  }
  if (! vl53A.startRanging()) {
    Serial.print(F("Couldn't start ranging: "));
    Serial.println(vl53A.vl_status);
    while (1)       delay(10);
  }
  vl53A.setTimingBudget(timingBudget);
  Serial.println("done.");

  //connect to ToF 2
  Serial.print("Connecting to 2... ");
  tcaselect(1);
  Wire.begin();
  if (! vl53B.begin(0x29, &Wire)) {
    Serial.print(F("Error on init of VL sensor: "));
    Serial.println(vl53B.vl_status);
    while (1)       delay(10);
  }
  if (! vl53B.startRanging()) {
    Serial.print(F("Couldn't start ranging: "));
    Serial.println(vl53B.vl_status);
    while (1)       delay(10);
  }
  vl53B.setTimingBudget(timingBudget);
  Serial.println("done.");
  
  //connect to ToF 3
  Serial.print("Connecting to 3... ");
  tcaselect(2);
  Wire.begin();
  if (! vl53C.begin(0x29, &Wire)) {
    Serial.print(F("Error on init of VL sensor: "));
    Serial.println(vl53C.vl_status);
    while (1)       delay(10);
  }
  if (! vl53C.startRanging()) {
    Serial.print(F("Couldn't start ranging: "));
    Serial.println(vl53C.vl_status);
    while (1)       delay(10);
  }
  vl53C.setTimingBudget(timingBudget);
  Serial.println("done.");

  delay(1000);
  startMillis = millis();
  beginMillis = startMillis;
  endMillis = 390000;
}

void loop() {
  tcaselect(0);
  if (vl53A.dataReady()) {
    distance = vl53A.distance();
    Serial.print(distance);
    Serial.print(",");
  }
  
  tcaselect(1);
  if (vl53B.dataReady()) {
    distance = vl53B.distance();
    Serial.print(distance);
    Serial.print(",");
  }

  tcaselect(2);
  if (vl53C.dataReady()) {
    distance = vl53C.distance();
    Serial.print(distance);
    Serial.println("");
  }
  curMillis = millis();
  while ((curMillis-startMillis)<samplePeriod) {
    delay(ceil(samplePeriod/10));
    curMillis = millis();
  }
  startMillis += samplePeriod;

  if ((curMillis-beginMillis)>=endMillis) {
    Serial.println("6.5 minute trial complete.");
    Serial.println("");
    while(1) {
      delay(1000);
    }
  }
}
