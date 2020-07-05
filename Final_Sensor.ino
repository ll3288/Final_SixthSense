#include <LedControl.h>                   // Library for LED Control
#include <SPI.h>
#include <SD.h>

const int chipSelect = 4;

const int trigPin = 7;                          // Assign Arduino Pin 7 as Trig Pin
const int echoPin = 8;                       // Assign Arduino Pin 8 as Echo Pin

long duration;                                   // Initialize Variable
int distance;                                     // Initialize Variable

int DIN = 12;                                     // Assign Arduino Pin D12
int CS =  11;                                     // Assign Arduino Pin D11 as Chip Select
int CLK = 10;                                   // Assign Arduino Pin D10 as Clock
//byte smile[8]=   {0x04,0x02,0x24,0x40,0x40,0x24,0x02,0x04};        // Matrix to Print Smile
byte nosmile[8]=   {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};    // Matrix to Print NoSmile
byte frown[8]=   {0x3C,0x42,0xA5,0x89,0x89,0xA5,0x42,0x3C};
 

LedControl lc=LedControl(DIN,CLK,CS,0);           
 

/* The setup() function is called when a sketch starts. It is used to initialize variables, pin modes, start using libraries, etc. This function will only run once, after each power up or reset of the Arduino board. */

void setup()

{

 pinMode(trigPin, OUTPUT);                                 // Sets the trigPin as an Output

 pinMode(echoPin, INPUT);                                  // Sets the echoPin as an Input

 Serial.begin(9600);                                              // Starts the serial communication

 lc.shutdown(0,false);                                           // The MAX72XX is in power-saving mode on                                                                 

 lc.setIntensity(0,15);                                            // Set the brightness to maximum value

 lc.clearDisplay(0);                                               // and clear the display
 
 while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
}

/* This Particular Function is used for Repeated Execution of the Circuit until Specified. */
/* digitalWrite is used to write a HIGH or a LOW value to a digital pin (LOW is the voltage level) */

void loop()

{

digitalWrite(trigPin, LOW);

delayMicroseconds(2);

digitalWrite(trigPin, HIGH);                                  // Sets the trigPin on HIGH state for 10 micro
                                                                        
delayMicroseconds(10);

digitalWrite(trigPin, LOW);                                  // Reads the echoPin, returns the sound wave

                                                                              
/* waits for the pin to go from LOW to HIGH, starts timing, then waits for the pin to go LOW and stops timing. Returns the length of the pulse in microseconds or gives up and returns 0 if no complete pulse was received within the timeout */

 

duration = pulseIn(echoPin, HIGH);

distance= duration*0.034/2;                                  // Calculating the distance
File dataFile = SD.open("datalog.txt", FILE_WRITE);

//Serial.print("Distance: ");                                       // Prints the distance on the Serial Monitor
Serial.println(distance);

if (dataFile) {
    dataFile.println(distance);
    dataFile.close();
    // print to the serial port too:
   Serial.print("Distance: "); 
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }

if(distance<182)                                                        // When the distance is less than 6ft or 182 cm

{
   printByte(frown);                                                // Print Smile on LCD

   delay(150);                                                        // Hold By 150 ms

  } 

  else

  {
                                                                    // Print No smile on LCD
    printByte(nosmile);
    delay(150);                                                      // Hold By 150 ms

    }

}

 
void printByte(byte character [])                         // Function to Print Characters

{

  int i = 0;                                                           

  for(i=0;i<8;i++)                                                 // Loop for Printing Characters

  {

    lc.setRow(0,i,character[i]);                             // Set the Row to Print Characters

  }

}
