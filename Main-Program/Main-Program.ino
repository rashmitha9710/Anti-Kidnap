/*
pin 9,10=gps
pin 6,7=gsm
pin 5=button
*/
#include <SoftwareSerial.h>
#include <TinyGPS.h>

SoftwareSerial gps1(9,10);//serial commn for gps
SoftwareSerial gsm(6,7);
TinyGPS gps;

void gpsdump(TinyGPS &gps);
//void printFloat(double f, int digits = 2);

long lat, lon;
float flat, flon;
unsigned long age, date, time, chars;
int year;
byte month, day, hour, minute, second, hundredths;
bool flag1=false;//fencing mechanism


void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
gps1.begin(9600);
gsm.begin(9600);
pinMode(5,INPUT_PULLUP);
}

void loop() 
{
  String message="";
  if(digitalRead(5)==LOW)
  {
    if(gsm.available()>0)
    {
      delay(6000);
      if(gps1.available()>0)
      {
         gpsdump(gps);
          gsm.println("AT+CMGF=1");
          delay(1000);
          gsm.println("AT+CMGS=\"+919167106508\"\r");
          delay(1000);
          message+="Alert from Outdoor Human Safety System Click the link below https://www.google.com/maps/place/@";
          message+=flat;
          message+=",";
          message+=flon;
          gsm.println(message);
         // gsm.println("Alert from Outdoor Human Safety System Click the link below https://www.google.com/maps/plaace/@"+flat+","+flon);
          delay(100);
          gsm.println((char)26);
          delay(1000);
      }
     }
          gpsdump(gps);
          gsm.println("AT+CMGF=1");
          delay(1000);
          gsm.println("AT+CMGS=\"+919167106508\"\r");
          delay(1000);
          gsm.println("Alert from Outdoor Human Safety System ,GPS could not be reached");
          delay(100);
          gsm.println((char)26);
          delay(1000);
  }

}



void gpsdump(TinyGPS &gps)
{
  gps.f_get_position(&flat, &flon, &age);
  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
}
/*
void printFloat(double number, int digits)
{
  // Handle negative numbers
  if (number < 0.0) 
  {
     Serial.print('-');
     number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (uint8_t i=0; i<digits; ++i)
    rounding /= 10.0;
  
  number += rounding;

  // Extract the integer part of the number and print it
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  Serial.print(int_part);

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0)
    Serial.print("."); 

  // Extract digits from the remainder one at a time
  while (digits-- > 0) 
  {
    remainder *= 10.0;
    int toPrint = int(remainder);
    Serial.print(toPrint);
    remainder -= toPrint;
  }
}*/
