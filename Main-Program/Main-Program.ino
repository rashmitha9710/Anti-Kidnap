#include <SoftwareSerial.h>
#include <TinyGPS.h>

SoftwareSerial mySerial(6,7);//gps
SoftwareSerial gsmSerial(9,10);//gsm
TinyGPS gps;

void gpsdump(TinyGPS &gps);
void printFloat(double f, int digits = 2);//same na yes
  long lat, lon;
  float flat, flon;
  int year;
  unsigned long age, date, time, chars;
  byte month, day, hour, minute, second, hundredths;
  String message="";
  String latFin="";
  String lonFin="";
  String coordDec="";
  String decimal="";
  int trunc_lat=0;
  int trunc_lon=0;
  //bool delayFlag=false;
  int i=1; //ye dekh ok?

void setup()  
{
  Serial.begin(9600);//arduino
  gsmSerial.begin(9600);
  mySerial.begin(9600);
}

void loop() 
{
 
 mySerial.listen();
    
  bool newdata = false;
  unsigned long start = millis();
  // Every 5 seconds we print an update
  while (millis() - start < 5000)
  {
    if (mySerial.available()) 
    
    {
      char c = mySerial.read();
      //Serial.print(c);
      if (gps.encode(c))
      {
        newdata = true;
        break;  // uncomment to print new data immediately!
      }
    }
  }
  
  if (newdata)
  {
    gpsdump(gps);
    Serial.println("lat:");
   printFloat(flat, 5);
   trunc_lat=trunc(flat);
   latFin+=trunc_lat;
   latFin+=".";
   latFin+=coordDec;
   Serial.print(latFin);
   Serial.println();
   Serial.println("long:");
   printFloat(flon, 5);
   trunc_lon=trunc(flon);
   lonFin+=trunc_lon;
   lonFin+=".";//same lon
   lonFin+=coordDec;
   Serial.print(lonFin);
   delay(5000);
   Serial.println();
  }
  
if(flat>0 && flon>0){//this is if location is avail
 SendMessage();
 delay(30000);
}
if(flat==0 && flon==0){// this is if this is not avail its worlking? yup
SendMessage1();
delay(10000);
}
if(gsmSerial.available()>0){//? gsm ka availaible hai ki nai check krne ok
  Serial.write(gsmSerial.read());
  }
/*if(delayFlag){
  delay(30000);
}
else if(!delayFlag){
  delay(10000);
*/}


void SendMessage1()
{
  //while(i>0)// ab first time its true na then andar ka perform krega and
  gsmSerial.listen();
  gsmSerial.println("AT+CMGF=1");
  delay(1000);
  gsmSerial.println("AT+CMGS=\"+917718833285\"\r");
  delay(1000);
          message+="Human Attacked,Location could not be traced";
          gsmSerial.println(message);
  delay(100);//ok?k aage chal rashmii?yes mai sogyi thi sorry arey sorry yaar bas thoda sa ye dekh
  gsmSerial.println((char)26);
  delay(1000);
}

void SendMessage()//same ok?
{
  gsmSerial.listen();
  
  gsmSerial.println("AT+CMGF=1");
  delay(1000);
  gsmSerial.println("AT+CMGS=\"+917718833285\"\r");
  delay(1000);
          message+="Human Attacked Click the link below https://www.google.com/maps/search/?api=1&query=";
          message+=latFin;
          message+=",";
          message+=lonFin;
  gsmSerial.println(message);
  delay(100);
  gsmSerial.println((char)26);
  delay(1000);
  
  //i--;
  latFin="";
  lonFin="";
  coordDec="";
  //delayFlag==true;
}

void gpsdump(TinyGPS &gps)
{
  gps.f_get_position(&flat, &flon, &age);
  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
}

void printFloat(double number, int digits)//ye default
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
// Serial.print(int_part);

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0)
 // Serial.print(".");  

  // Extract digits from the remainder one at a time
  while (digits-- > 0) 
  {
    remainder *= 10.0;
    int toPrint = int(remainder);
   // Serial.print(toPrint);
    decimal+=toPrint;
    remainder -= toPrint;
  }

//Serial.print(decimal);
coordDec=decimal;
 decimal="";
 
}
