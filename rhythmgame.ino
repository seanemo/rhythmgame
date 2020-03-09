const int analogPin = 0; // read from multiplexer using analog input 0
const int strobePin = 2; // strobe is attached to digital pin 2
const int resetPin = 3;  // reset is attached to digital pin 3
const int row1 = 22;
const int row2 = 23;
const int row3 = 24;
const int row4 = 25;
const int row5 = 26;
const int row6 = 27;
const int row7 = 28;
const int row8 = 36;
const int col1 = 49;
const int col2 = 48;
const int col3 = 47;
const int col4 = 46;
const int button1 = 53;
const int button2 = 52;
const int button3 = 51;
const int button4 = 50;


//const long read_freq = 1000;
//long next_read_freq;
#define SPECVAL_LENGTH 7 
#define RUNSUM_LENGTH 5

int spectrumValue[7]; // array to hold a2d values
int runningSumValues[SPECVAL_LENGTH][RUNSUM_LENGTH]; //array to hold past values for running sum
int writeIndex; // keeps track of what place to write future values 
long int pastrow;
long int pastcol;
long int past;
long int past1;


//int curr_threshold = 0; 
bool beats[SPECVAL_LENGTH]; 
const int noise_threshold = 300; 

//led col checkers
bool newbeat;
int ledmatrix[7][4];
int temp[7][4];



void setup()
{
  pinMode(analogPin, INPUT);
  pinMode(strobePin, OUTPUT);
  pinMode(resetPin, OUTPUT);
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
  analogReference(DEFAULT);
  
  digitalWrite(resetPin, LOW);
  digitalWrite(strobePin, HIGH);
  
  Serial.begin(115200);
  //long t0 = millis();
  //next_read_freq = t0;
  /*
  long int start_time = millis();

  long int duration = 300000; 
  */ 
  PORTA = 0;
  PORTL = 15;
  past = millis();
  past1 = millis();
  
}

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
        for(int i = 0; i < RUNSUM_LENGTH; i++) {
            runningSumValues[band_index][i] = curr_specval;
        }
    }

    else {
        runningSumValues[band_index][writeIndex] = curr_specval; 
        writeIndex++;
        if (writeIndex > RUNSUM_LENGTH - 1){
          writeIndex = 0;
        }
    }
}


int getThreshold(int band_index) {
    int sum;
    for(int i = 0; i < RUNSUM_LENGTH; i++) {
        sum += runningSumValues[band_index][i]; 
    }

    return sum; 
}


void detectBeat() {
  for(int i = 0; i < SPECVAL_LENGTH; i++) {
    beats[i] = (spectrumValue[i] > noise_threshold) && (RUNSUM_LENGTH*spectrumValue[i] >= getThreshold(i));
    //Serial.println(beats[i]); 
    updateRunningSum(i, beats[i]);
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

void colrandom(){
  newbeat = false;
  int colNum = random(4) / 1;
  ledmatrix[0][colNum] = 1;
}

void lightLED(){
    for (int i=0;i<7;i++){
      for (int j=0;j<4;j++){
        if (ledmatrix[i][j] == 1){
          if (i < 6){
            temp[i+1][j] = 1;
          }
           
          if(i == 0){
            digitalWrite(row7,HIGH);
          }
          if(i == 1){
            digitalWrite(row6,HIGH);
          }
          if(i == 2){
            digitalWrite(row5,HIGH);
          }
          if(i == 3){
            digitalWrite(row4,HIGH);
          }
          if(i == 4){
            digitalWrite(row3,HIGH);
          }
          if(i == 5){
            digitalWrite(row2,HIGH);
          }
          if(i == 6){
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
          //delay(3);
          PORTL = 15;
          PORTA = 0;
          
        }
      }
    }
}
void shift(){
  for (int i=0;i<7;i++){
    for(int j=0;j<4;j++){
      ledmatrix[i][j] = temp[i][j];
    }
  }
  for (int i=0;i<7;i++){
    for(int j=0;j<4;j++){
      temp[i][j] = 0;
    }
  }
}

void printArray(int a[7][4]){
  for(int i=0;i<7;i++){
    for(int j=0;j<4;j++){
      Serial.print(a[i][j]);
      Serial.print(",");
      if (j==3){
        Serial.print("\n");
      }
    }
  }
}
  
void ledrowdrop(){
  
  if (newbeat == true){
    colrandom();
    //PORTA += 64;
  }
  
  if (millis()- pastrow > 250){
    pastrow = millis();
    lightLED();
    //PORTA /= 2;
  }  
  /*if (PORTA == 0){
    PORTA = 64;
  }*/
} 

void loop(){


  //readspectrum(); 
  //detectBeat(); 

  // Test Code

  //printspectrum(7);
  //printbeat(3, true);


 // End Test Code 
 //ledrowdrop();
 /*if (millis()-past1 > 1000){
  colrandom();
  past1 = millis();
 }*/
 
 if (millis()-past > 500){
  past = millis();
  colrandom();
 }
 lightLED();
 if (millis()- pastrow > 250){
    pastrow = millis();
    
    shift(); 
 }
 
}
