const int analogPin = 0; // read from multiplexer using analog input 0
const int strobePin = 2; // strobe is attached to digital pin 2
const int resetPin = 3;  // reset is attached to digital pin 3


//const long read_freq = 1000;
//long next_read_freq;
#define SPECVAL_LENGTH 7 //gets length of array
#define RUNSUM_LENGTH 5

int spectrumValue[7]; // array to hold a2d values
int runningSumValues[SPECVAL_LENGTH][5]; //array to hold past values for running sum



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
  digitalWrite(resetPin, HIGH);
  digitalWrite(resetPin, LOW);
  delayMicroseconds(75);
  
  for (int i = 0; i < 7; i++)
    {
      delayMicroseconds(36); 
      digitalWrite(strobePin, LOW);
      delayMicroseconds(36); 
      spectrumValue[i] = analogRead(analogPin);
      //Serial.println(analogRead(analogPin));
      digitalWrite(strobePin, HIGH);
    }
}
void printspectrum(int max_index, bool cumulative){
  if(cumulative) {
  for(int i = 0; i < max_index; i++)
    {
      Serial.print(spectrumValue[i]);
      Serial.print(", ");
    }
  Serial.print("\n");
}

else {
  Serial.println(spectrumValue[max_index]); 
}
}


int getThreshold(int band) {
    int sum;
    for(int i = 0; i < RUNSUM_LENGTH; i++) {
        sum += runningSumValues[band][i]; 
    }

    return sum; 
}

void updateRunningSum(int band, bool curr_beat) {

    int curr_specval = spectrumValue[band]; 

    int temp[RUNSUM_LENGTH]; 
    
    temp[0] = curr_specval; // 0th element is most recent value, last element is oldest 

    if(curr_beat)  {
      
      for(int i = 0; i < RUNSUM_LENGTH - 1; i++) {
        temp[i+1] = curr_specval; 
      }
    }
     else {
      for(int i = 0; i < RUNSUM_LENGTH - 1; i++) {
        temp[i+1] = runningSumValues[band][i]; 
      }
     }

    

    for(int i = 0; i < RUNSUM_LENGTH; i++) {
      runningSumValues[band][i] = temp[i];
    }


}

void loop()
{
  //readspectrum();
  //printspectrum();

  
  readspectrum(); 

  for(int i = 0; i < SPECVAL_LENGTH; i++) {
    beats[i] = (spectrumValue[i] > noise_threshold) && (5*spectrumValue[i] >= getThreshold(i));
    //Serial.println(beats[i]); 
    updateRunningSum(beats[i], spectrumValue[i]);
    Serial.print((i+1)*beats[i]); 
    Serial.print(", "); 
  }
  Serial.print("\n"); 
  
  
   
  
  
  //printspectrum(3, true);
  
  
 
  
}
