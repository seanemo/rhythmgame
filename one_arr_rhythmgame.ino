const int analogPin = 0; // read from multiplexer using analog input 0
const int strobePin = 2; // strobe is attached to digital pin 2
const int resetPin = 3;  // reset is attached to digital pin 3


//const long read_freq = 1000;
//long next_read_freq;

int spectrumValue[7]; // array to hold a2d values
int runningSumValues[5]; //array to hold past values for running sum

#define LENGTH(a) sizeof(a)/sizeof(a[0]) //gets length of array

//int curr_threshold = 0; 
bool beat = false; 
const int noise_threshold = 300; 

int counter = 0; // This will keep track of how many times we have written to the array 

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


int getThreshold() {
    int sum;
    for(int i = 0; i < LENGTH(runningSumValues); i++) {
        sum += runningSumValues[i]; 
    }

    return sum; 
}

void updateRunningSum(int curr_specval, bool curr_beat) {
    if(counter == LENGTH(runningSumValues)) 
    {
        counter = 0
    }

    if(bool) { 
        for(int i = 0; i < LENGTH(runningSumValues); i++) {
            runningSumValues[i] = curr_specval;
        }
    }
    else {
        runningSumValues[counter] = curr_specval; 
        counter++; 
    }

}


void loop()
{
  //readspectrum();
  //printspectrum();

  
  readspectrum(); 
  beat =  (spectrumValue[0] > noise_threshold) && (LENGTH(runningSumValues)*spectrumValue[0] >= getThreshold());  
  updateRunningSum(beat, spectrumValue[0]); 
  
  
  //printspectrum(3, true);
  Serial.println(beat); 
  
 
  
}