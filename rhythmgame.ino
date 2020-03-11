#include "CurrentSongData.h"
#if GENERATE_BEATMAP
const int analogPin = 0; // read from multiplexer using analog input 0
const int strobePin = 2; // strobe is attached to digital pin 2
const int resetPin = 3;  // reset is attached to digital pin 3
#endif
#if !GENERATE_BEATMAP
const int row1 = 22;
const int row2 = 23;
const int row3 = 24;
const int row4 = 25;
const int row5 = 26;
const int row6 = 27;
const int row7 = 28;
const int row8 = 29;
const int col1 = 49;
const int col2 = 48;
const int col3 = 47;
const int col4 = 46;
const int button1 = 37;
const int button2 = 36;
const int button3 = 35;
const int button4 = 34;
#endif


//const long read_freq = 1000;
//long next_read_freq;
#if GENERATE_BEATMAP
const int specval_len = 7; 
const int runsum_len = 5;

int spectrumValue[7]; // array to hold a2d values
int runningSumValues[specval_len][runsum_len]; //array to hold past values for running sum
int writeIndex; // keeps track of what place to write future values 
int prevBeat[specval_len]; //writes time last beat was recorded for each band
#endif
#if !GENERATE_BEATMAP
long int pastrow;
long int pastcol;
long int ledpast;
long int past;
long int past1;
long int firsttime;
long int secondtime;
long int thirdtime;
long int fourthtime;
#endif

#if GENERATE_BEATMAP
//int curr_threshold = 0; 
bool beats[specval_len]; 
const int noise_threshold = 300; 
#endif

#if !GENERATE_BEATMAP
//led col checkers
bool newbeat;
int ledmatrix[8][4];
int temp[8][4];
int points;
#endif






void setup()
{
#if GENERATE_BEATMAP
  pinMode(analogPin, INPUT);
  pinMode(strobePin, OUTPUT);
  pinMode(resetPin, OUTPUT);
#endif
#if !GENERATE_BEATMAP
  pinMode(row1,OUTPUT);
  pinMode(row2,OUTPUT);
  pinMode(row3,OUTPUT);
  pinMode(row4,OUTPUT);
  pinMode(row5,OUTPUT);
  pinMode(row6,OUTPUT);
  pinMode(row7,OUTPUT);
  pinMode(row8,OUTPUT);
  pinMode(col1,OUTPUT);
  pinMode(col2,OUTPUT);
  pinMode(col3,OUTPUT);
  pinMode(col4,OUTPUT);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  pinMode(button4, INPUT);
#endif
#if GENERATE_BEATMAP
  analogReference(DEFAULT);
  
  digitalWrite(resetPin, LOW);
  digitalWrite(strobePin, HIGH);
#endif  
  Serial.begin(115200);
  //long t0 = millis();
  //next_read_freq = t0;
  /*
  long int start_time = millis();

  long int duration = 300000; 
  */ 
#if !GENERATE_BEATMAP
  PORTA = 0;
  PORTL = 15;
  past = millis();
  past1 = millis();
  points = 0;
#endif
  
}

#if GENERATE_BEATMAP
void readspectrum(){
  digitalWrite(resetPin, HIGH); //resets current frequency band_index to lowest frequency
  digitalWrite(resetPin, LOW);
  delayMicroseconds(75);
  
  for (int i = 0; i < 7; i++)
    {
      delayMicroseconds(36); 
      digitalWrite(strobePin, LOW); //tells chip to take values of current frequency band_index
      delayMicroseconds(36); 
      spectrumValue[i] = analogRead(analogPin);//records spectrum value in array
      //Serial.println(analogRead(analogPin));
      digitalWrite(strobePin, HIGH); //closes current frequency band_index and goes to the next band_index
    }
}

void printspectrum(int band_index, bool cumulative = true){ //takes an integer value band_index that tells what frequency values to read, also takes in cumulative, which gathers all the lower frequencies underneath if true
  if(cumulative) {
    for(int i = 0; i < band_index; i++)//prints out band_indexs
      {
        Serial.print(spectrumValue[i]);
        Serial.print(", ");
      }
    Serial.print("\n");
  }

  else {
    Serial.println(spectrumValue[band_index]); 
  }
}


void updateRunningSum(int band_index, bool curr_beat) {

    int curr_specval = spectrumValue[band_index]; 
   
    if(curr_beat) {
        for(int i = 0; i < runsum_len; i++) {
            runningSumValues[band_index][i] = curr_specval;
        }
    }

    else {
        runningSumValues[band_index][writeIndex] = curr_specval; 
        writeIndex++;
        if (writeIndex > runsum_len - 1){
          writeIndex = 0;
        }
    }
}


int getThreshold(int band_index) {
    int sum;
    for(int i = 0; i < runsum_len; i++) {
        sum += runningSumValues[band_index][i]; 
    }

    return sum; 
}


void detectBeat() {
  for(int i = 0; i < specval_len; i++) {
    beats[i] = (spectrumValue[i] > noise_threshold) && (runsum_len*spectrumValue[i] >= getThreshold(i));
    //Serial.println(beats[i]); 
    updateRunningSum(i, beats[i]);
    
    if beats[i] {

    int curr_time = millis();
    Serial.print(curr_time - prevBeat);
    prevBeat = curr_time;
    }
  }
}

void printbeat(int band_index, bool cumulative=true){
  if(cumulative) {
  for(int i = 0; i < band_index; i++) {
    Serial.print(beats[i]*(i+1)); //mul by i+1 to separate pulses out for each band_index
    Serial.print(", "); 
    }
  }
  else {
    Serial.print(beats[band_index]); //mul by i+1 to separate pulses out for each band_index
    }
    Serial.print("\n");
}

#endif
#if !GENERATE_BEATMAP

void colrandom(){
  newbeat = false;
  int colNum = random(4) / 1;
  ledmatrix[1][colNum] = 1;
}

void lightLED(){
    for (int i=0;i<8;i++){
      for (int j=0;j<4;j++){
        if (ledmatrix[i][j] == 1){
          if (i < 7 && i > 0){
            temp[i+1][j] = 1;
          }
           
          if(i == 0){
            digitalWrite(row8,HIGH);
          }
          if(i == 1){
            digitalWrite(row7,HIGH);
          }
          if(i == 2){
            digitalWrite(row6,HIGH);
          }
          if(i == 3){
            digitalWrite(row5,HIGH);
          }
          if(i == 4){
            digitalWrite(row4,HIGH);
          }
          if(i == 5){
            digitalWrite(row3,HIGH);
          }
          if(i == 6){
            digitalWrite(row2,HIGH);
          }
          if(i == 7){
            digitalWrite(row1,HIGH);
          }
          if(j == 0){
            digitalWrite(col1,LOW);
          }
          if(j == 1){
            digitalWrite(col2,LOW);
          }
          if(j == 2){
            digitalWrite(col3,LOW);
          }
          if(j == 3){
            digitalWrite(col4,LOW);
          }
            
          //Serial.println(ledmatrix[i][j]);
          delay(1);
          PORTL = 15;
          PORTA = 0;
          
        }
      }
    }
}
void shift(){
  for (int i=0;i<8;i++){
    for(int j=0;j<4;j++){
      ledmatrix[i][j] = temp[i][j];
    }
  }
  for (int i=0;i<8;i++){
    for(int j=0;j<4;j++){
      temp[i][j] = 0;
    }
  }
}

void createtimingwindow(){
  for(int i = 0; i<4;i++){
    if(ledmatrix[7][i]==1){
      if(i==0){
        firsttime = millis();
      }
      if(i==1){
        secondtime = millis();
      }
      if(i==2){
        thirdtime = millis();
      }
      if(i==3){
        fourthtime = millis();
      }
    }
  }
}

void timingwindow(){
  if(millis()-firsttime <= 250 && ledmatrix[0][0] == 1){
    if(ledmatrix[0][0]==1){
        points += 1;
        Serial.println(points);
    }
  }
  if(millis()-secondtime <= 250 && ledmatrix[0][1] == 1){
    if(ledmatrix[0][1]==1){
        points += 1;
        Serial.println(points);
    }
  }
  if(millis()-thirdtime <= 250 && ledmatrix[0][2] == 1){
    if(ledmatrix[0][2]==1){
        points += 1;
        Serial.println(points);
    }
  }
  if(millis()-fourthtime <= 250 && ledmatrix[0][3] == 1){
    if(ledmatrix[0][3]==1){
        points += 1;
        Serial.println(points);
    }
  }
  
}

void detectbuttonpress(){
  if (digitalRead(button1)==LOW){
    ledmatrix[0][0] = 1;
  }
  if (digitalRead(button2)==LOW){
    ledmatrix[0][1] = 1;

  }
  if (digitalRead(button3)==LOW){
    ledmatrix[0][2] = 1;

  }
  if (digitalRead(button4)==LOW){
    ledmatrix[0][3] = 1;

  }
}
#endif
void loop(){

#if GENERATE_BEATMAP
  prevBeat = millis();
  readspectrum(); 
  detectBeat(); 

  // Test Code

  //printspectrum(7);
//  printbeat(3, true);
#endif

 // End Test Code 
 /*if (millis()-past1 > 1000){
  colrandom();
  past1 = millis();
 }*/
#if !GENERATE_BEATMAP
 if (millis()-past > 500){
  past = millis();
  colrandom();
 }
 lightLED();
 detectbuttonpress();
 createtimingwindow();
 timingwindow();
 if (millis()- pastrow > 250){
    pastrow = millis();
    
    shift(); 
 }
#endif
 
}
