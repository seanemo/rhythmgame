/* Beat Detection Program

this program uses a MSGEQ7 chip to collect sound data,
the chip seperates the audio into 7 bands centered on 63hz, 160hz, 400hz, 1khz,
2.5khz, 6.25khz and 16khz(which from my tests is more like 15khz)

  then it takes the average of however many samples are in 1.25 seconds
and compares it to the current sample. If the current sample is higher
than the average multiplied by C, it counts a beat.

  it takes the average amounts of beats in ten seconds and converts that
to beats per minute.

 It then takes 60 divided by the average beats per minute and multiplies
it by 1000 to convert beats per minute into a delay to use between
animations.


 The animations are done on an eight by eight LED Matrix. I'm using four of the
74hc595 ic shift registers to extend the outputs of the Arduino Uno.
I used common cathode RGB LEDs so if the binary animations seem funny
it may be that you have common anode LEDs.


 

*/


#include <Wire.h>

#include <SimpleTimer.h>
SimpleTimer timer;

                  // these are for the MSGEQ7
int analogPin = 0; // read from multiplexer using analog input 0
int strobePin = 2; // strobe is attached to digital pin 2
int resetPin = 3; // reset is attached to digital pin 3
int spectrumValue=0; // to hold a2d values


const int numReadings2 = 20;
const int numReadings3 = 20;

float readings2[numReadings2];       // the readings from the analog input
float readings3[numReadings3];
int index2 = 0;                  // the index of the current reading
int index3 = 0;
float total2 = 0.0;                  // the running total
float total3 = 0.0;
int average = 0;                // the average
float cycles2 = 0.0;
float cycles3 = 0.0;
float cyclesS2 = 0.0;
float cyclesS3 = 0.0;
float cyclesV = 0.0;
float cyclesV3 = 0.0;
float sumBPM2 = 0.0;
float sumBPM3 = 0.0;
//  int BperM2 = 0;      
float spectrumSum2 = 0.0;
float spectrumSum3 = 0.0;
 float average2 = 0.0; //using
 float average3 = 0.0;
//  int average4 = 0;
//  int average5 = 0;  // not using these, I was thinking it would be cool to work in the different frequencies later.
//  int average6 = 0;
//  int average7 = 0;
 int holdValue1 = 0;   // this is for later if I want to do more with different frequencies.    frequency of 63 Hz
 int holdValue2 = 0;   // I'm currently using this one which is assigned the frequency of 160 Hz
 int holdValue3 = 0;   // not using frequency of 400 Hz
 int holdValue4 = 0;   // not using this one either    frequency of 1 KHz
 int holdValue5 = 0;   // or this    frequency of 2.5 KHz
 int holdValue6 = 0;   //or this    frequency of 6.25 KHz
 int holdValue7 = 0;   // or this frequency of 16 Hz (seems more like 15 KHz)
 int averageBPM1 = 0;  // not using
float aveBPM2 = 0.0;  // I'm using this
float aveBPM3 = 0.0;  // and this
 int holdBeat2 = 0;
 int holdBeat3 = 0;
 int beat2 = 0;
 int beat3 = 0;
unsigned long lastCycle ;
 int saveBPM2 = 0;
unsigned long saveAverage = 0;
 int b = 0;
unsigned long timesAround = 0;
float aveBPerM = 0.0;
float aveBPerM3 = 0.0;
int Fruit = 0;
int Fruit3 = 0;
float variance = 0;
float variance3 = 0;
float C = 0;
float C3 = 0;
float averageVariance = 0;
float averageVariance3 = 0;
float greatestAverageVariance;
float greatestVariance = 0;
unsigned long varianceSum = 0;
unsigned long varianceSum3 = 0;
int holdBPMDelay = 0;
double BPMDelay = 0.0;
int trigger2 = 0;
int trigger3 = 0;
int greg2 = 0;
int greg3 = 0;
int byteSending = 1;
//int toTransfer = 32767;
int Shift = BPMDelay;
int mask = 0xFF;
unsigned char toSend = 0;
int twenty2 = 0;
int twenty3 = 0;


void setup()
{
 Serial.begin(9600);
 Wire.begin(0x52); // Start I2C Bus as slave
 Wire.onRequest(requestInt);
 Serial.print("new Session");
 pinMode(analogPin, INPUT);
 pinMode(strobePin, OUTPUT);
 pinMode(resetPin, OUTPUT);
 analogReference(DEFAULT);
 digitalWrite(resetPin, LOW);
 digitalWrite(strobePin, HIGH);
 delayMicroseconds(36);
 timer.setInterval(1250, BPM2);
 timer.setInterval(1250, BPM3);
 timer.setInterval(1250, ResetSec);
//  timer.setInterval(11250, AverageBPM2);
//  timer.setInterval(11250, AverageBPM3);
//  timer.setInterval(11250, ResetTenSec);  
 timer.setInterval(11250, Fruity); // ran out of ideas for names so I chose Fruity all it does is switch a boolean statement
 for (int thisReading2 = 0; thisReading2 < numReadings2; thisReading2++)
 {
   readings2[thisReading2] = 0;
 }
 for (int thisReading3 = 0; thisReading3 < numReadings3; thisReading3++)
 {
   readings3[thisReading3] = 0;
 }
}



