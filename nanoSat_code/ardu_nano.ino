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
 
#include <Servo.h> 

Servo trackerR;   
Servo trackerL;
int eastLDRPin = 0;  
int westLDRPin = 1;
int eastLDR = 0;   
int westLDR = 0;
int error = 0;
int calibration = 204;  
int trackerRPos = 90;    
int trackerLpos = 0;

void setup() 
{ 
  trackerR.attach(11);
  trackerL.attach(7);
} 
 
 
void loop() 
{ 
  eastLDR = calibration + analogRead(eastLDRPin);    
  westLDR = analogRead(westLDRPin);
  if(eastLDR<350 && westLDR<350)  
  {
    while(trackerPos<=160) 
    {
      trackerRPos++;
      trackerR.write(trackerRPos);
	  trackerL.write(trackerRPos-90);
      delay(100);
    }
	
  }
  error = eastLDR - westLDR;          
  if(error>15)        
  {
    if(trackerPos<=160)  
    {
      trackerRPos++;
      trackerR.write(trackerPos);  
	  trackerL.write(trackerRPos-90);
	}
  }
  else if(error<-15)  
  {
    if(trackerPos>20)  
    {
      trackerRPos--;
      trackerR.write(trackerRPos);
	  trackerL.write(trackerRPos-90);	
    }
  }
  delay(100);
} 
