/*
 *  Function library for the testbed
 *
 *  Version 1.0
 *  Author: Soledad Escolar
 */

// include this library's description file
#include "platform.h"
#include <Adafruit_GFX.h>
#include <Adafruit_INA219.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <DHT.h>
#include <RH_RF95.h>
#include <RTClib.h>
#include <SD.h>

Adafruit_INA219 ina0(ADDRESS0);
Adafruit_INA219 ina1(ADDRESS1);
Adafruit_INA219 ina2(ADDRESS2);


// Singleton instance of the SD
static File file;
bool platformClass::initializedSD = false;
bool platformClass::initializedRTC = false;
bool platformClass::initializedRFMLoRa = false;

// Singleton instance of the rfm95
RH_RF95 rf95(RFM95_CS, RFM95_INT);

// Singleton instance of the rtc
RTC_PCF8523 rtc;

// Singleto instance of the DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// display
Adafruit_SSD1306 display(OLED_RESET);

// This struct contains the protobuffer encoded
struct encodedPB {
  uint8_t len;
  uint8_t buffer[MAXPB];
};

// A global variable
struct encodedPB ePB;

//***************************************************************
// Constructor of the class					*
//***************************************************************

//! Function that handles the creation and setup of instances
platformClass::platformClass(void) {
  pinMode(LED, OUTPUT);
  pinMode(CS_SD, OUTPUT);
  pinMode(RFM95_CS, OUTPUT);
  pinMode(PINSET, OUTPUT);
  pinMode(PINUNSET, OUTPUT);
  digitalWrite(PINSET, LOW);
  digitalWrite(PINUNSET, LOW);
  digitalWrite(LED, HIGH);
}

//***************************************************************
// Public Methods						*
//***************************************************************

//!******************************************************************************
//!		Name:	initIoTNode() * 		Description: initializes the pins for
//!IoT node			* 		Param : void
//!* 		Returns: void							*
//!		Example: platform.initIoTnode(); *
//!******************************************************************************

void platformClass::initIoTNode(void) {
  pinMode(LED, OUTPUT);
  pinMode(BATTERY, INPUT);

  Serial1.setTimeout(2000);
  dht.begin();
}

//!******************************************************************************
//!		Name:	version() * 		Description: It check the version of the
//!library		* 		Param : void
//!* 		Returns: void							*
//!		Example: platform.version(); *
//!******************************************************************************

int platformClass::version(void) { return vs; }

//!******************************************************************************
//!	Name:	initializeRTC() * 	Description: Initializes the Real Time Clock
//!(RTC)			* 	Param : void
//!* 	Returns: int with the success (0) or fail (-1) of the initialization
//!* 	Example: platform.initializeRTC();					*
//!******************************************************************************

int platformClass::initializeRTC(void) {
  
  if (!rtc.begin()) {
    Serial.println("DEBUG: RTC not found!");
    return -1;
  }
  platformClass::initializedRTC = true;
  return int(platformClass::initializedRTC);
}

//!******************************************************************************
//!	Name:	adjustRTC() * 	Description: Set the RTC to the date & time this
//!sketch was compiled	* 	Param : void
//!* 	Returns: void * 	Example: platform.adjustRTC();
//!*
//!******************************************************************************
void platformClass::adjustRTC(void) {
  if (!rtc.initialized()) {
    Serial.println("DEBUG: RTC is NOT running!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  } else {
    Serial.println("DEBUG: RTC initialized!");
  }
}

//!******************************************************************************
//!	Name:	initializeLoRa() * 	Description: Initializes the LORA
//!communication module.			* 	Param : void
//!* 	Returns: int with the success (0) or fail (-1) of the initialization
//!* 	Example: platform.initializeLoRa(); *
//!******************************************************************************
int platformClass::initializeLoRa(void) {
  digitalWrite(CS_SD, HIGH);    // Disable SD
  digitalWrite(RFM95_CS, LOW);  // Enable Lora
  while (!rf95.init()) {
    Serial.println("DEBUG: RFM LoRa not initialized!");
    while (1)
      ;
  }
  platformClass::initializedRFMLoRa = true;
  Serial.println("DEBUG: RFM LoRa initialized!");

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("DEBUG: Setting LoRa frequency failed!");
    while (1)
      ;
  }
  Serial.println("DEBUG: Setting LoRa frequency !");

  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf =
  // 128chips/symbol, CRC on you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);
  return int(platformClass::initializedRFMLoRa);
}

//!******************************************************************************
//!	Name:	sendLoRaPB()							*
//!	Description: send a protobuffer through the LORA communication module.	*
//!	Param : protobuffer to send						*
//!	Returns: int with the success (0) or fail (-1) of the initialization	*
//!	Example: platform.sendLoRa();						*
//!******************************************************************************
int platformClass::sendLoRaPB(uint8_t* data, uint8_t len)
{
	//send(const uint8_t* uint8_t)
	rf95.send(data,len);
	delay(10);
	rf95.waitPacketSent();
	return 0;
}

//!******************************************************************************
//!	Name:	sendLoRaPB() * 	Description: send a protobuffer through the LORA
//!communication module.	* 	Param : protobuffer to send
//!* 	Returns: int with the success (0) or fail (-1) of the initialization
//!* 	Example: platform.sendLoRa(); *
//!******************************************************************************
/*int platformClass::sendLoRaPB(pb_PlatinoEvent platino) {
  struct encodedPB e;

  e = encodePB(platino);
  if (e.len == 0) {
    return -1;
  }
  rf95.send(e.buffer, e.len);
  delay(10);
  rf95.waitPacketSent();
  return 0;
}
*/

//!******************************************************************************
//!	Name:	sendLoRa() * 	Description: send data through the LORA
//!communication module.		* 	Param : data to send
//!* 	Returns: int with the success (0) or fail (-1) of the initialization
//!* 	Example: platform.sendLoRa(); *
//!******************************************************************************
int platformClass::sendLoRa(String data) {
  int len;

  digitalWrite(CS_SD, HIGH);    // Disable SD
  digitalWrite(RFM95_CS, LOW);  // Enable Lora

  const uint8_t* data2 = reinterpret_cast<const uint8_t*>(data.c_str());
  len = strlen(data.c_str());
  Serial.print("DEBUG: Sending Message: ");
  Serial.println(data);
  rf95.send(data2, len);
  delay(10);
  rf95.waitPacketSent();
  return 0;
}

//!******************************************************************************
//!	 Name:	receiveLoRa()                                                       * 	
//!  Description: receive data through the LORA communication module.	          *
//!  Param : void                                                               *
//!  Returns: String with the data received                                     *
//!  Example: platform.receiveLoRa();                                           *
//!                                                                             *
//!******************************************************************************
String platformClass::receiveLoRa(void) {
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  digitalWrite(CS_SD, HIGH);    // Disable SD
  digitalWrite(RFM95_CS, LOW);  // Enable Lora
  // Should be a reply message for us now
  if (rf95.waitAvailableTimeout(RF95_FREQ)) {
    if (rf95.recv(buf, &len)) {
      buf[len] = 0;
      // Serial.print("DEBUG: received from LoRA : ");
      // Serial.print("RSSI: ");
      // Serial.println(rf95.lastRssi(), DEC);
      return (char*)buf;
    } else {
      Serial.println("DEBUG: recv failed!");
    }
  }
  return "";
}

	//!******************************************************************************
	//!	Name:	receiveLoRa()							                                          *
	//!	Description: receive data through the LORA communication module.	        *
	//!	Param : pointer to a buffer of data and len of the data (initially 0)	  	*
	//!	Returns: 0 if failed 1 otherwise                                          *
	//!	Example: platform.receiveLoRa();					                                *
	//!******************************************************************************
	int platformClass::receiveLoRa(uint8_t* data, uint8_t* len)
	{
		uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  		//uint8_t len = sizeof(buf);
 
		digitalWrite(CS_SD, HIGH);	   //Disable SD
		digitalWrite(RFM95_CS, LOW);	   //Enable Lora 
    		// Should be a reply message for us now   
		if (rf95.waitAvailableTimeout(RF95_FREQ)) 
		{	
			if (rf95.recv(buf, len))
   			{
				buf[*len]=0;
      				//Serial.print("DEBUG: received from LoRA : ");
			      	//Serial.print("RSSI: ");
			      	//Serial.println(rf95.lastRssi(), DEC);    
				data = buf;
				return 1;
			}else{
				data = 0;
				len = 0;
				Serial.println("DEBUG: recv failed!");
				return 0;
			}
		}
		return 1;
	}

//!******************************************************************************
//!	Name:	getTemperature() * 	Description: Read the temperature sensor
//!* 	Param : void * 	Returns: float with the temperature
//!* 	Example: platform.getTemperature(); *
//!******************************************************************************
float platformClass::getTemperature(void) {
  // write request msg
  Serial1.println(REQ_TEMPERATURE);
  Serial1.flush();

  // try to read a float, waiting with a timeout
  return Serial1.parseFloat();

  // char inChar;
  // String temp="";
  // Serial1.print(TEMPERATURE);
  // bool end=false;
  // while (!end){
  // 	while (Serial1.available()) {
  // 		inChar = (char)Serial1.read();
  // 		temp += inChar;
  // 		if (inChar == '\n') {
  // 			end = true;
  // 			break;
  // 		}

  // 	}
  // 	if (end) {break;}
  // }
  // const char* foo = temp.c_str();
  // return (atof(foo));
}

//!******************************************************************************
//!	Name:	getHumidity() * 	Description: Read the humidity
//!* 	Param : void * 	Returns: float with the humidity
//!* 	Example: platform.getHumidity();					*
//!******************************************************************************
float platformClass::getHumidity(void) {
  // write request msg
  Serial1.println(REQ_HUMIDITY);
  Serial1.flush();

  // try to read a float, waiting with a timeout
  return Serial1.parseFloat();

  // char inChar;
  // String hum="";
  // Serial1.print(HUMIDITY);
  // bool end=false;
  // while (!end){
  // 	while (Serial1.available()) {
  // 		inChar = (char)Serial1.read();
  // 		hum += inChar;
  // 		if (inChar == '\n') {
  // 			end = true;
  // 			break;
  // 		}

  // 	}
  // 	if (end) {break;}
  // }
  // const char* foo = hum.c_str();
  // return (atof(foo));
}
//!******************************************************************************
//!	Name:	getBatteryVoltage() * 	Description: Read the battery voltage
//!* 	Param : void * 	Returns: float with the battery voltage
//!* 	Example: platform.getBatteryVoltage(); *
//!******************************************************************************
float platformClass::getBatteryVoltage(void) {
  // write request msg
  Serial1.println(REQ_BATTERYVOLT);
  Serial1.flush();

  // try to read a float, waiting with a timeout
  return Serial1.parseFloat();

  // char inChar;
  // String batt="";
  // Serial1.print(BATTERYVOLT);
  // bool end=false;
  // while (!end){
  // 	while (Serial1.available()) {
  // 		inChar = (char)Serial1.read();
  // 		batt += inChar;
  // 		if (inChar == '\n') {
  // 			end = true;
  // 			break;
  // 		}

  // 	}
  // 	if (end) {break;}
  // }
  // const char* foo = batt.c_str();
  // return (atof(foo));
}
//!******************************************************************************
//!	Name:	getTime() * 	Description: get the time in which a sample is
//!collected		* 	Param : void
//!* 	Returns: String * 	Example: platform.getTime();
//!*
//!******************************************************************************
String platformClass::getTime() {
  char date[20];

  if (!platformClass::initializedRTC) {
    return "";
  }
  DateTime now = rtc.now();  // Obtener fecha y hora actual.

  int day = now.day();
  int month = now.month();
  int year = now.year();
  int hour = now.hour();
  int minute = now.minute();
  int second = now.second();

  sprintf(date, "%.2d.%.2d.%.4d %.2d:%.2d:%.2d", day, month, year, hour, minute,
          second);
  return String(date);
}

//!******************************************************************************
//!	Name:	initINA0() * 	Description: Initializes sensor INA0
//!* 	Param : void * 	Returns: void
//!* 	Example: platform.initINA0(); *
//!******************************************************************************
void platformClass::initINA0(void) { ina0.begin(); }

//!******************************************************************************
//!	Name:	getPanelCurrent() * 	Description: Read the current sensor INA0
//!* 	Param : void * 	Returns: float with the current of the panel
//!* 	Example: platform.getPanelCurrent(); *
//!******************************************************************************
float platformClass::getPanelCurrent(void) {
  float current = 0.0;
  ina0.begin();

  relay(PINSET);
  delay(1000);
  current = ina0.getCurrent_mA();
  relay(PINUNSET);
  return current;
}

//!******************************************************************************
//!	Name:	initINA1() * 	Description: Initializes sensor INA1
//!* 	Param : void * 	Returns: void
//!* 	Example: platform.initINA1(); *
//!******************************************************************************
void platformClass::initINA1(void) { ina1.begin(); }
//!******************************************************************************
//!	Name:	getLoadCurrent() * 	Description: Read the current sensor INA1
//!* 	Param : void * 	Returns: float with the load current
//!* 	Example: platform.getLoadCurrent(); *
//!******************************************************************************
float platformClass::getLoadCurrent(void) {
  float current = 0.0;
  ina1.begin();
  current = ina1.getCurrent_mA();
  return current;
}

//!******************************************************************************
//!	Name:	initINA2() * 	Description: Initializes sensor INA2
//!* 	Param : void * 	Returns: void
//!* 	Example: platform.initINA2(); *
//!******************************************************************************
void platformClass::initINA2(void) { ina2.begin(); }
//!******************************************************************************
//!	Name:	getBatteryCurrent() * 	Description: Read the current sensor INA2
//!* 	Param : void * 	Returns: float with the current of the battery
//!* 	Example: platform.getBatteryCurrent(); *
//!******************************************************************************
float platformClass::getBatteryCurrent(void) {
  float current = 0.0;
  ina2.begin();
  current = ina2.getCurrent_mA();
  return current;
}

//!******************************************************************************
//!	Name:	getPanelPower() * 	Description: Read the power from sensor INA0
//!* 	Param : void * 	Returns: float with the power of the panel
//!* 	Example: platform.getPanelPower();					*
//!******************************************************************************
float platformClass::getPanelPower(void) {
  float power = 0.0;
  relay(PINSET);
  delay(1000);
  power = ina0.getPower_mW();
  relay(PINUNSET);
  return power;
}

//!******************************************************************************
//!	Name:	getLoadPower() * 	Description: Read the power from sensor INA1
//!* 	Param : void * 	Returns: float with the load power
//!* 	Example: platform.getLoadPower();					*
//!******************************************************************************
float platformClass::getLoadPower(void) {
  float power = 0.0;

  power = ina1.getPower_mW();
  return power;
}

//!******************************************************************************
//!	Name:	getBatteryPower() * 	Description: Read the power from sensor INA2
//!* 	Param : void * 	Returns: float with the power of the battery
//!* 	Example: platform.getBatteryPower(); *
//!******************************************************************************
float platformClass::getBatteryPower(void) {
  float power = 0.0;

  power = ina2.getPower_mW();
  return power;
}

//!******************************************************************************
//!	Name:	getSpeedOfWind() * 	Description: Returns the speed of the wind
//!(ANENOMETER)			* 	Param : void
//!* 	Returns: float with the ANENOMETER value in meters per second * 	Example:
//!platform.getSpeedOfWind();					*
//!******************************************************************************
float platformClass::getSpeedOfWind(void) {
  float windOfSpeed = 0.0;

  // Reading from ANENOMETER and conversion to meters/second according to the
  // datasheet
  windOfSpeed = ((AUX1 * analogRead(ANENOMETER)) - 0.4) * AUX2;
  return windOfSpeed;
}

float platformClass::getVolumetricWaterContent(void) {
  float value = analogRead(HYGROMETER) * (3.0 / 1023.0);

  if (value <= 1.1) return max(0, 10 * value - 1);
  if (value > 1.1 && value <= 1.3) return 25 * value - 17.5;
  if (value > 1.3 && value <= 1.82) return 48.08 * value - 47.5;
  if (value > 1.82 && value <= 2.2) return 26.32 * value - 7.89;
  return 62.5 * value - 87.5;
}

//!******************************************************************************
//!	Name:	open() * 	Description: open a file on the memory card
//!* 	Param : filename, mode * 	Returns: int 0 if ok and -1 if not ok
//!* 	Example: platform.open();						*
//!******************************************************************************
int platformClass::open(String filename, int mode) {
  digitalWrite(RFM95_CS, HIGH);  // Disable LORA
  digitalWrite(CS_SD, LOW);      // Enable SD
  if (platformClass::initializedSD) {
    if (mode == WRITE) {
      file = SD.open(filename, FILE_WRITE);
      if (!file) {
        Serial.println("DEBUG1: Open file failed!");
        return -1;
      }
    } else {
      file = SD.open(filename);
      if (!file) {
        Serial.println("DEBUG: Open file failed!");
        return -1;
      }
    }
    return 0;
  } else {
    Serial.println("DEBUG: Open file failed!");
  }
  return -1;
}

//!******************************************************************************
//!	Name:	close() * 	Description: close a file
//!* 	Param : void * 	Returns: void
//!* 	Example: platform.close();						*
//!******************************************************************************
void platformClass::close() {
  digitalWrite(RFM95_CS, HIGH);  // Disable LORA
  digitalWrite(CS_SD, LOW);      // Enable SD
  if (file) {
    file.close();
  }
}

//!******************************************************************************
//!	Name:	write() * 	Description: write data into memory card SD
//!* 	Param : String to write * 	Param : File to write
//!* 	Returns: 0 if success or -1 if fail * 	Example: platform.writeln();
//!*
//!******************************************************************************
int platformClass::writeline(String data) {
  digitalWrite(RFM95_CS, HIGH);  // Disable LORA
  digitalWrite(CS_SD, LOW);      // Enable SD
  if (file) {
    file.println(data);
    return 0;
  }
  return -1;
}

//!******************************************************************************
//!	Name:	readline() * 	Description: read data from memory card SD
//!* 	Param : File to read * 	Returns: String with the data
//!* 	Example: platform.readline(); *
//!******************************************************************************
String platformClass::readline() {
  String data = "";
  digitalWrite(RFM95_CS, HIGH);  // Disable LORA
  digitalWrite(CS_SD, LOW);      // Enable SD
  if ((file) and (file.available())) {
    data = file.read();
  }
  return data;
}

//!******************************************************************************
//!	Name:	initializeDisplay() * 	Description: Initialize the LCD
//!* 	Param : void * 	Returns: void
//!* 	Example: platform.initializeDisplay(); *
//!******************************************************************************
void platformClass::initializeDisplay(void) {
  display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDRESS);

  delay(2000);
  clean();
  display.println(WELCOME_MSG);
  display.println(VERSION_MSG);
  display.display();

  Serial.println("DEBUG: Display Initialized!");
}

//!******************************************************************************
//!	Name:	displayLCD() * 	Description: Display data on screen
//!* 	Param : String with the title and 					* 	Param
//!: String with the data 						*
//!	Returns: void * 	Example: platform.displayLCD();
//!*
//!******************************************************************************
void platformClass::displayLCD(String title, String data) {
  clean();

  if (title != "") {
    display.print(title);
    display.print(':');
  }
  display.print(data);
  display.display();
}

//! This function will read the temperature sensor of IoTnode
float platformClass::readTemperature() {
  // float value = sht1x.readTemperatureC();
  float value = dht.readTemperature();
  if (isnan(value)) {  // check if 'is not a number'
    return -1000.0;
  }
  return value;
}
//! This function will read the humidity sensor of IoTnode
float platformClass::readHumidity() {
  float value;
  // value = sht1x.readHumidity();
  value = dht.readHumidity();
  if (isnan(value)) {  // check if 'is not a number'
    return -1.0;
  } else {
    return value;
  }
}
//! This function will read the battery voltage of IoTnode

float platformClass::readBatteryVoltage() {
  float value;
  value = analogRead(BATTERY) * AUX1;
  // update value (remove voltage divisor effect)
  value *= 2;
  return value;
}

//***************************************************************
// Private Methods						*
//***************************************************************

//! This function will change the status of the relay:
// Short circuit current of the panel: we close relay and wait
// current be stablished again, then it puts the relay in its original state

void platformClass::relay(int status) {
  digitalWrite(status, HIGH);
  delay(10);
  digitalWrite(status, LOW);
}

//! This function will prepare the display for visualization
void platformClass::clean(void) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
}

//!******************************************************************************
//!	Name:	initializeSD() * 	Description: Initialize the memory card SD
//!* 	Param : void * 	Returns: int with the success (0) or fail (-1) of the
//!initialization 	* 	Example: platform.initializeSD();
//!*
//!******************************************************************************
int platformClass::initializeSD(void) {
  digitalWrite(RFM95_CS, HIGH);  // Disable LORA
  digitalWrite(CS_SD, LOW);      // Enable SD
  if (!SD.begin(CS_SD)) {
    Serial.println("DEBUG: SD initialization failed!");
    platformClass::initializedSD = false;
    return -1;
  }
  platformClass::initializedSD = true;
  Serial.println("DEBUG: SD Initialized!");
  return 0;
}

//! This function will encode the protobuffer into an unsigned chars buffer
/*struct encodedPB platformClass::encodePB(pb_PlatinoEvent e) {
  pb_ostream_t stream;

  stream = pb_ostream_from_buffer(ePB.buffer, sizeof(ePB.buffer));
  if (!pb_encode(&stream, pb_PlatinoEvent_fields, &e)) {
    Serial.println("DEBUG: Encode PB into buffer failed!");
    ePB.len = 0;
    return ePB;
  }
  ePB.len = (uint8_t)stream.bytes_written;
  return ePB;
}
*/
//! This function will decode an unsigned char buffer into a protobuffer
/*pb_PlatinoEvent platformClass::decodePB(struct encodedPB ePB) {
  pb_istream_t stream;
  pb_PlatinoEvent platino = pb_PlatinoEvent_init_zero;

  stream = pb_istream_from_buffer(ePB.buffer, sizeof(ePB.buffer));
  if (!pb_decode(&stream, pb_PlatinoEvent_fields, &platino)) {
    Serial.println("DEBUG: Decoding buffer into protobuffer failed!");
    platino.numPacket = -1;
  }
  return platino;
}*/
//***************************************************************
// Preinstantiate Objects					*
//***************************************************************

platformClass platform = platformClass();
