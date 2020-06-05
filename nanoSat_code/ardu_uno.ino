/*
 *         MIT License
 *
 * Copyright (c) 2019 HrithiK_MrXHam1
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 */
 
#include <Wire.h>
#include <ADC121C_MQ131.h>
#include<HMC5883>
#define Addr 0x1E
ADC121C_MQ131 mq131;

int UVIN = A1;
int Lin  = A2;
int BAT_IN = A3;


float ozone = 0.00;
float uv    = 0.00;
float he    = 0.00;
float lux   = 0.00;



void setup(void)
{
    Serial.begin(9600);
	ozone_sensor_init();
	uv_init();
	mm_init();
    
}

void loop(void)
{
	ozone = read_ozone();
	uv    = read_uv();
	he    = read_he();
	lux   = analogRead(Lin);
	battery = analogRead(BAT_IN);
	Serial.println('lux:'+lux+' '+'uv:'+uv+' '+'hall_effect:'+he+' '+'ozone:'+ozone+'battery%:'+battery+' ');
	delay(1000);
}

void ozone_sensor_init()
{
	mq131.getAddr_ADC121C_MQ131(ADC121C_MQ131_DEFAULT_ADDRESS);
    mq131.setCycleTime(CYCLE_TIME_32);                   
    mq131.setAlertHold(ALERT_HOLD_CLEAR);                
    mq131.setAlertFlag(ALERT_FLAG_DISABLE);              
    mq131.setAlertPin(ALERT_PIN_DISABLE);                 
    mq131.setPolarity(POLARITY_LOW);                      
    mq131.begin();
    mq131.Calibration();
}

void uv_init()
{
	pinMode(UVIN, INPUT);
}


void mm_init()
{
	Wire.begin();  
	Wire.beginTransmission(Addr);  
	Wire.write(0x00);  
	Wire.write(0x60);  
	Wire.endTransmission();
	Wire.beginTransmission(Addr);  
	Wire.write(0x02);  
	Wire.write(0x00);  
	Wire.endTransmission();  
	delay(300);
}

float read_ozone()
{
	byte error;
    int8_t address;
    address = mq131.ADC121C_MQ131_i2cAddress;
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    (error == 0)
    {
        uint16_t result;
        float ppmO3 = mq131.Measure_Ozone(-0.90, 0.69);
        return (ppmO3, 3);
    }
    else
    {
        return 0;
    }

}

float read_uv()
{
	  int uvLevel = averageAnalogRead(UVIN);
	  float uvIntensity = mapfloat(outputVoltage, 0.99, 2.8, 0.0, 15.0);
}

int read_he()
{
	unsigned int data[6];  
	Wire.beginTransmission(Addr);  
	Wire.write(0x03);  
	Wire.endTransmission();
	Wire.requestFrom(Addr, 6);    
	if(Wire.available() == 6)  
	{    
	data[0] = Wire.read();    
	data[1] = Wire.read();    
	}  
	delay(300);          
	int xMag = ((data[0] * 256) + data[1]);  
	delay(300);
	return xMag;
}

int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0;
  for(int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
	runningValue /= numberOfReadings;

  return(runningValue); 
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}