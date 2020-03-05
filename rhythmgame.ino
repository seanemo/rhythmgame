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
const int specval_len = 7;
const int runsum_len  = 5;

int spectrumValue[7]; // array to hold a2d values
int runningSumValues[specval_len][runsum_len]; //array to hold past values for running sum
int writeIndex; // keeps track of what place to write future values 
long int pastrow;


//int curr_threshold = 0; 
bool beats[specval_len]; 
const int noise_threshold = 300; 


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
  
  Serial.begin(9600);
  //long t0 = millis();
  //next_read_freq = t0;
  /*
  long int start_time = millis();

  long int duration = 300000; 
  */ 
  PORTA = 64;
  PORTL = 0;
  
  
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

    if(curr_beat) {
        for(int i = 0; i < runsum_len; i++) {
            runningSumValues[band_index][i] = spectrumValue[band_index]; // accesses the current value for a particular band from spectrumValue
        }
    }

    else {
        runningSumValues[band_index][writeIndex] = spectrumValue[band_index]; 
        if (++writeIndex > runsum_len - 1){ // writeIndex is first incremented, and then the value is returned so the conditional can be evaluated
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
  
void ledrowdrop(){
  if (millis()- pastrow > 250){
    PORTA /= 2;
    pastrow = millis();
    if (PORTA == 0){
      PORTA = 64;
    }
  }
  
}
  

void loop(){


  readspectrum(); 
  detectBeat(); 

  // Test Code

  //printspectrum(7);
  printbeat(3, true);

 // End Test Code 
 ledrowdrop(); 
  
}
