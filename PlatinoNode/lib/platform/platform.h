/*
 *  Function library for the testbed
 *
 *
 *  Version 1.0
 *  Author: Soledad Escolar
 *  Date: February 2019
 */

#ifndef platformClass_h
#define platformClass_h
#define ARDUINO_FEATHER_M0

#include "Arduino.h"

/* Libreries to use protobuffers */
#include <pb.h>
#include <pb_common.h>
#include <pb_decode.h>
#include <pb_encode.h>

//#include <platino.pb.h>
#include <mtpPlatino.pb.h>



//***************************************************************
// Variables and definitions					*
//***************************************************************
//! Set the version of the class
const int vs = 1;

#define ADDRESS0 		0x40
#define ADDRESS1 		0x41
#define ADDRESS2 		0x44


#define BATTERY			A0
#define HYGROMETER		A1
#define ANENOMETER		A2
#define AUX1			(3.3 / 1023)
#define AUX2			(32.4 / 1.6)
#define WRITE			0
#define READ			1

#define LED 			13
#define PINSET 			11
#define PINUNSET 		12
#define CS_SD 			10
#define OLED_RESET 		4
#define RFM95_CS 		8
#define RFM95_RST 		4
#define RFM95_INT 		3
#define RFM95_TIMEOUT 	1000

// sole: 868 for compatibility with LoPy
// Change to 433.0 or other frequency, must match RX's freq!
#define RF95_FREQ 		434.0

// SOLE: Taken from RH_RF95.h
#define RF95_FIFO_SIZE 		255
#define RF95_MAX_PAYLOAD_LEN 	RF95_FIFO_SIZE
#define RF95_HEADER_LEN 	4
#define	RF95_MAX_MESSAGE_LEN 	(RF95_MAX_PAYLOAD_LEN-RF95_HEADER_LEN)



// #define	SHT1X_ADDRESS	A1
// #define	SHT1X_CONTROL	A2
#define DHTPIN 			A1
#define DHTTYPE 		DHT22

#define REQ_TEMPERATURE 0xCD01
#define REQ_HUMIDITY    0xCD02
#define REQ_BATTERYVOLT 0xCD03

#define DISPLAY_ADDRESS 0x3C
#define WELCOME_MSG     "Starting Platform"
#define VERSION_MSG     (vs)
#define MAXPB           64  // Length of the array where the protobuffer es encoded


// Library interface description
class platformClass {
  static bool initializedSD;
  static bool initializedRTC;
  static bool initializedRFMLoRa;

 public:
  //***************************************************************
  // Constructor of the class					*
  //***************************************************************

  //! Class constructor.
  platformClass(void);

  //***************************************************************
  // Public Methods						*
  //***************************************************************
  //!  Returns the library version
  /*!
  \param void
  \return int : The library version.
  */
  int version(void);

  //! Initializes the Real Time Clock (RTC)
  /*!
  \param void
  \return int with the success (0) or fail (-1) of the initialization
  */
  int initializeRTC(void);

  //! Adjust the time for Real Time Clock (RTC)
  /*!
  \param void
  \return void
  */
  void adjustRTC(void);

  //! Initialize Lora module
  /*!
  \param void
  \return int with the success (0) or fail (-1) of the initialization
  */
  int initializeLoRa(void);

  //! Send data through Lora module
  /*!
  \param String with the data
  \return int with the success (0) or fail (-1) of the initialization
  */
  int sendLoRa(String);

  //! Send a protobuffer through Lora module
  /*!
  \param protobuffer PlatinoEvent with the data
  \return int with the success (0) or fail (-1) of the initialization
  */
  //int sendLoRaPB(pb_PlatinoEvent platino);

  //! Send a generic protobuffer through Lora module
	/*!
	\param protobuffer with the data
	\return int with the success (0) or fail (-1) 
	*/	int sendLoRaPB(uint8_t* data, uint8_t len);


  //! Receive data through LoRa module
  /*!
    \param void
    \return String with the data received
  */  String receiveLoRa(void);
  
  //! Receive data through Lora module
		/*!
		\param data- buffer to store the data and len of the received data 
		\return int: success or fail
		*/	int receiveLoRa(uint8_t* data, uint8_t* len);


  //! Returns the temperature
  /*!
  \param void
  \return float : The temperature value.
  */
  float getTemperature(void);

  //! Returns the humidity
  /*!
  \param void
  \return float : The humidity value.
  */
  float getHumidity(void);

  float getVolumetricWaterContent(void);

  //! Returns the battery voltage
  /*!
  \param void
  \return float : The battery voltage.
  */
  float getBatteryVoltage(void);

  //!  Initializes INA0
  /*!
  \param void
  \return void
  */
  void initINA0(void);
  //!  Initializes INA1
  /*!
  \param void
  \return void
  */
  void initINA1(void);
  //!  Initializes INA2
  /*!
  \param void
  \return void
  */
  void initINA2(void);
  //! Returns the load power (ina1)
  /*!
  \param void
  \return float with the load power (mW)
  */
  float getLoadPower(void);

  //! Returns the battery power (ina3)
  /*!
  \param void
  \return float with the power of the battery (mW)
  */
  float getBatteryPower(void);

  //! Returns the panel power (ina0)
  /*!
  \param void
  \return float with the power of the panel (mW)
  */
  float getPanelPower(void);

  //! Returns the load current (ina1)
  /*!
  \param void
  \return float with the load current (mA)
  */
  float getLoadCurrent(void);

  //! Returns the battery current (ina3)
  /*!
  \param void
  \return float with the current of the battery (mA)
  */
  float getBatteryCurrent(void);

  //! Returns the panel current (ina0)
  /*!
  \param void
  \return float with the current of the panel (mA)
  */
  float getPanelCurrent(void);

  //! Returns the speed of the wind (anenometer)
  /*!
  \param void
  \return float : The anenometer value
  */
  float getSpeedOfWind(void);

  //! Open a file to read/write on SD
  /*!
  \param String : filename
  \param mode : open mode (READ|WRITE)
  \return int: 0 if success and -1 if fail
  */
  static int open(String, int);

  //! Close a file
  /*!
  \param void
  \return void
  */
  static void close();

  //! Store data in SD
  /*!
  \param String : data to be stored
  \return int: number of bytes written
  */
  static int writeline(String);

  //! Read data from SD
  /*!
  \param File : file descriptor
  \return string: number of bytes written .
  */
  String readline();

  //! Activate debug by means of display
  /*!
  \param void
  \return void
  */
  void initializeDisplay();

  //! Display data on screen
  /*!
  \param String: title
  \param String: data
  \return void
  */
  void displayLCD(String, String);

  //! Initialize IoTnode
  /*!
  \param void
  \return void
  */
  void initIoTNode();

  //! Read Temperature Sensor
  float readTemperature();

  //! Read Humidity Sensor
  float readHumidity();

  //! Read battery voltage sensor
  float readBatteryVoltage();
  /* Initialize the micro SD
  \param void
  \return int with the success (0) or fail (-1) of the initialization
  */
  static int initializeSD(void);

  //! Get the time
  String getTime();
  /*
  \return String with the date and time */

 private:
  //***************************************************************
  // Private Methods						*
  //***************************************************************

  //! Change the state of the relay
  void relay(int status);

  //! Prepare the screen to display data
  /*!
  \param void
  \return void
  */
  void clean(void);

  //! Encode the protobuffer
  //struct encodedPB encodePB(pb_PlatinoEvent platino);
  //! Decode a buffer into a protobuffer
  //pb_PlatinoEvent decodePB(struct encodedPB ePB);

  //***************************************************************
  // Private Variables						*
  //***************************************************************
};
extern platformClass platform;

#endif
