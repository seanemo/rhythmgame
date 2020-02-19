const int analogPin = 0; // read from multiplexer using analog input 0
const int strobePin = 2; // strobe is attached to digital pin 2
const int resetPin = 3;  // reset is attached to digital pin 3
int avg_value = 0;
int counter = 0;
boolean beat;

//const long read_freq = 1000;
//long next_read_freq;

int spectrumValue[7]; // array to hold a2d values

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
void printspectrum(int max_index){
  for(int i = 0; i < max_index; i++)
    {
      Serial.print(spectrumValue[i]);
      Serial.print(" ");
    }
  Serial.print("\n");
}

int running_avg(int number){
  
  
  avg_value += spectrumValue[0];
  if (counter == number){
    avg_value /= number;
    counter = 0;
    return avg_value; 
  }
  else{
    counter+=1;
  }
}

void bpm(){
  if (spectrumValue[0] >= avg_value){
     beat = true;
  }
}

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

void loop()
{
  //readspectrum();
  //printspectrum();

  
  readspectrum();
  printspectrum(7);
 
  
}
