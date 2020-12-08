/* YourDuino Multiple DS18B20 Temperature Sensors on 1 wire
Connections:
DS18B20 Pinout (Left to Right, pins down, flat side toward you)
- Left   = Ground
- Center = Signal (Pin 2):  (with 3.3K to 4.7K resistor to +5 or 3.3 )
- Right  = +5 or +3.3 V

Questions: terry@yourduino.com
V1.01  01/17/2013 ...based on examples from Rik Kretzinger

/*-----( Import needed libraries )-----*/
// Get 1-wire Library here: http://www.pjrc.com/teensy/td_libs_OneWire.html
#include <OneWire.h>
//Get DallasTemperature Library here:  http://milesburton.com/Main_Page?title=Dallas_Temperature_Control_Library
#include <DallasTemperature.h>
//#include "LSGEEpromRW.h" 
#include <EEPROM.h> 

#include "SoftwareSerial.h"

SoftwareSerial* _softwareSerial = new SoftwareSerial(13, 5);

/*-----( Declare Constants and Pin Numbers )-----*/
#define ONE_WIRE_BUS_PIN 5

/*-----( Declare objects )-----*/
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS_PIN);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
//int y = 8;

//uint8_t _temperatureSensorAddress[50][8];

auto _temperatureSensorAddress = new uint8_t[1][8];


uint8_t _numbersOfSensor;
#include "LSGEEpromRW.h" 
#include <EEPROM.h> 
/*-----( Declare Variables )-----*/
// Assign the addresses of your 1-Wire temp sensors.
// See the tutorial on how to obtain these addresses:
// http://www.hacktronics.com/Tutorials/arduino-1-wire-address-finder.html


//byte checkProbeAddressInMemory(byte numberOfSensor)
//{
//	Serial.print("Finding Probe Address "); Serial.println(numberOfSensor);
//	//int init = numberOfSensor * 13;
//	for (uint16_t i = 0; i < 8; i++)
//	{
//		Serial.print("Memory value :"); Serial.println(EEPROM.read((numberOfSensor * 13) + i), HEX);
//		Serial.print("Array value :"); Serial.println(_temperatureSensorAddress[numberOfSensor][i], HEX);
//		if (_temperatureSensorAddress[numberOfSensor][i] != EEPROM.read((numberOfSensor * 13) + i))
//		{
//			//Creare suono e visualuzare su display procedura per registrare i probes.
//			Serial.println("Errore di lettura");
//		}
//	}
//}

//void registerProbeAddressInMemory()
//{
//	uint16_t address = 0;
//	for (int i = 0; i < _numbersOfSensor; i++)
//	{
//		for (int ii = 0; ii < 8; ii++)
//		{
//			EEPROM.write(address, _temperatureSensorAddress[i][ii]);
//			address++;
//		}
//		address = address + 5;
//	}
//}
int getHex(String hexstr) {
	return (int)strtol(hexstr.c_str(), 0, 16);
}
unsigned int getSubtring(int start, int end, String stringToSplit)
{
	char* partialArray = new char[5];
	String partial = stringToSplit.substring(start, end);
	partial.toCharArray(partialArray, partial.length() + 1);
	return getHex(partial);
}
void setup()   /****** SETUP: RUNS ONCE ******/
{
	Serial.begin(115200);
	_softwareSerial->begin(9600);

	Serial.print("Initializing Temperature Control Library Version ");
	Serial.println(DALLASTEMPLIBVERSION);

	//_temperatureSensorAddress = new uint8_t[_numbersOfSensor][8];

	discoverOneWireDevices();
	//return;
	// Initialize the Temperature measurement library
	sensors.begin();

	_numbersOfSensor = sensors.getDeviceCount();

	Serial.print("Number of Devices found on bus = ");

	Serial.println(_numbersOfSensor);
	
	// set the resolution to 10 bit (Can be 9 to 12 bits .. lower is faster)
	//registerProbeAddressInMemory();
	//Serial.println("Fine scrittura");
	/*for (int i = 0; i < _numbersOfSensor; i++)
	{
		checkProbeAddressInMemory(i);
	}*/
}
void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{

	//return;
	for (int i = 0; i < _numbersOfSensor; i++)
	{
		DeviceAddress probe = { _temperatureSensorAddress[i][0],
								_temperatureSensorAddress[i][1],
								_temperatureSensorAddress[i][2],
								_temperatureSensorAddress[i][3],
								_temperatureSensorAddress[i][4],
								_temperatureSensorAddress[i][5],
								_temperatureSensorAddress[i][6],
								_temperatureSensorAddress[i][7]
		};
		sensors.setResolution(probe, 10);
		delay(1000);
		Serial.println();
		Serial.print("Getting temperature from number "); Serial.print(i); Serial.println(" probe");
		sensors.requestTemperatures();
		Serial.print("temperature is:   ");
		printTemperature(probe);

		if (sensors.getTempC(probe) > 30.00)
		{

		}
	}

}
void printTemperature(DeviceAddress deviceAddress)
{
	float tempC = sensors.getTempC(deviceAddress);
	if (tempC == -127.00)
	{
		Serial.print("Error getting temperature  ");
	}
	else
	{
		Serial.print("C: ");
		Serial.print(tempC);
		/*Serial.print(" F: ");
		Serial.print(DallasTemperature::toFahrenheit(tempC));*/
	}
}
void discoverOneWireDevices(void) {
	byte addr[8];
	char probeAddressHexValue[5];
	char value[5];
	uint8_t numberOfTemperatureSensor = 0;
	while (oneWire.search(addr)) {
		for (byte i = 0; i < 8; i++) {
			strcpy(probeAddressHexValue, "0x");
			if (addr[i] < 16) {
				strcat(probeAddressHexValue, "0");
			}
			String(addr[i], HEX).toCharArray(value, 5);
			strcat(probeAddressHexValue, value);
			_temperatureSensorAddress[numberOfTemperatureSensor][i] = getHex(probeAddressHexValue);
			Serial.print(probeAddressHexValue);
			if (i < 7) {
				Serial.print(", ");
			}
			else
			{
				numberOfTemperatureSensor++;
				Serial.println();
			}
		}
		if (OneWire::crc8(addr, 7) != addr[7]) {
			Serial.print("CRC is not valid!\n");
			return;
		}
	}
	oneWire.reset_search();
	return;
}
void recognizeProbeDevice(uint8_t deviceNumber) {

}

