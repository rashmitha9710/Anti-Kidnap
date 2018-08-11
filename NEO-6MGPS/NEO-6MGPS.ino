#include <SoftwareSerial.h>
#include <TinyGPS.h>

SoftwareSerial mySerial(9,10);
TinyGPS gps;

void gpsdump(TinyGPS &gps);
void printFloat(double f, int digits = 2);
 long lat, lon;
  float flat, flon;
  unsigned long age, date, time, chars;
  int year;
  byte month, day, hour, minute, second, hundredths;
void setup()  
{
  // Oploen serial communications and wait for port to open:
  Serial.begin(9600);
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  delay(1000);
  Serial.println("uBlox Neo 6M");
  Serial.print("Testing TinyGPS library v. "); Serial.println(TinyGPS::library_version());
  Serial.println("by Mikal Hart");
  Serial.println();
  Serial.print("Sizeof(gpsobject) = "); 
  Serial.println(sizeof(TinyGPS));
  Serial.println(); 
}

void loop() // run over and over
{
  bool newdata = false;
  unsigned long start = millis();
  // Every 5 seconds we print an update
  while (millis() - start < 5000) 
  {
    if (mySerial.available()) 
    
    {
      char c = mySerial.read();
      //Serial.print(c);  // uncomment to see raw GPS data
      if (gps.encode(c)) 
      {
        newdata = true;
        break;  // uncomment to print new data immediately!
      }
    }
  }
  
  if (newdata) 
  {
    Serial.println("Acquired Data");
    Serial.println("-------------");
    gpsdump(gps);
    Serial.println("lat:");printFloat(flat, 5);
    Serial.println("long:");printFloat(flon, 5);
    delay(5000);
    Serial.println("-------------");
    Serial.println();
  }
  
}

void gpsdump(TinyGPS &gps)
{
 
  unsigned short sentences, failed;

  
  gps.f_get_position(&flat, &flon, &age);
 // Serial.print("Lat/Long(float): "); printFloat(flat, 5); Serial.print(", "); printFloat(flon, 5);
    //Serial.print(" Fix age: "); Serial.print(age); Serial.println("ms.");

 

  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  //Serial.print("Date: "); Serial.print(static_cast<int>(month)); Serial.print("/"); 
    //Serial.print(static_cast<int>(day)); Serial.print("/"); Serial.print(year);
  //Serial.print("  Time: "); Serial.print(static_cast<int>(hour+05));  Serial.print(":"); //Serial.print("UTC +08:00 Malaysia");
    //Serial.print(static_cast<int>(minute+30)); Serial.print(":"); Serial.print(static_cast<int>(second));
   // Serial.print("."); Serial.print(static_cast<int>(hundredths)); Serial.print(" UTC +05:30 India");
 // Serial.print("  Fix age: ");  Serial.print(age); Serial.println("ms.");


 // Serial.print("Alt(float): "); printFloat(gps.f_altitude()); Serial.print(" Course(float): ");
   // printFloat(gps.f_course()); Serial.println();
  //Serial.print("Speed");Serial.print(" (kmph): ");
   // printFloat(gps.f_speed_kmph()); Serial.println();

}

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
}
