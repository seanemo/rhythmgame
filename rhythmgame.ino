const int analogPin = 0; // read from multiplexer using analog input 0
const int strobePin = 2; // strobe is attached to digital pin 2
const int resetPin = 3;  // reset is attached to digital pin 3


//const long read_freq = 1000;
//long next_read_freq;
#define SPECVAL_LENGTH 7 
#define RUNSUM_LENGTH 5

int spectrumValue[7]; // array to hold a2d values
int runningSumValues[SPECVAL_LENGTH][RUNSUM_LENGTH]; //array to hold past values for running sum
int writeIndex; // keeps track of what place to write future values 


//int curr_threshold = 0; 
bool beats[SPECVAL_LENGTH]; 
const int noise_threshold = 300; 


void setup()
{
  pinMode(analogPin, INPUT);
  pinMode(strobePin, OUTPUT);
  pinMode(resetPin, OUTPUT);
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

  

void loop()
{

  readspectrum(); 
  detectBeat(); 

  // Test Code

  //printspectrum(7);
  printbeat(3, true);

 // End Test Code 
  
  
}
