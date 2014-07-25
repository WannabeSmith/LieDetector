int redPin = 9; 
int greenPin = 10; 
int bluePin = 11; 
int buzzerPin = 7; //piezotransducer
int potPin = 1; //potentiometer
int timerButton = 5;
int interruptButton = 2;
int sensorPin = 0; //input from thumbtacks
int band = 20; // adjust for sensitivity
long startTime; // store in starting time here
long duration; // variable to store how long the timer has been running
float secduration;  // variable to store the duration in seconds
int interroState = 0; //state of the interrogation
int toggle = 0;
volatile int pleadState = HIGH; //state of the interrogation according to the interrupt button


void setup() {
    pinMode(potPin, INPUT); 
    pinMode(sensorPin, INPUT); 
    pinMode(timerButton, INPUT);
    pinMode(interruptButton, INPUT);
    pinMode(redPin, OUTPUT); 
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT); 
    pinMode(buzzerPin, OUTPUT);
    Serial.begin(9600);
    attachInterrupt(0, handler, CHANGE);
}

void loop() 
{
  if (digitalRead(timerButton) == HIGH) {
    toggle = 1;
  }
  else {
    if (toggle){
      interroState = !interroState;
      toggle = 0;
      Serial.print("Interrogation has begun\n");
    }
  }
  
    startTime = millis();
    
    while (interroState) {
     
      if (!pleadState){
        Serial.print("Pleading the 5th\n");
        plead();
        Serial.print("Continuing Interrogation\n");
      }
      
      if (digitalRead(2) == LOW){
        pleadState = HIGH;
      }
      else {
        pleadState = LOW;
      }
      
        int gsr = analogRead(sensorPin); 
        int pot = analogRead(potPin); 
        
        //person is lying
        if (gsr > pot + band) {
          digitalWrite(redPin, HIGH);
          digitalWrite(greenPin, LOW);
          digitalWrite(bluePin, LOW);
          
          beep(); 
        }
        
        //variable resistor needs to be adjusted
        else if (gsr < pot - band) {
           digitalWrite(bluePin, HIGH);
           digitalWrite(greenPin, LOW);
           digitalWrite(redPin, LOW);
        }
        
        //voltage at the sensor pin (A0) and the potentiometer (A1) are within the same range (band)
        else {
           digitalWrite(greenPin, HIGH);
           digitalWrite(bluePin, LOW);
           digitalWrite(redPin, LOW);
        }
        
        //Calculate the length of the interrogation
        duration = millis() - startTime;
        secduration = (float) duration / (1000);
       
        if (digitalRead(timerButton) == HIGH) {
          toggle = 1;
        }
        else {
          if (toggle){
            interroState = !interroState;
            toggle = 0;
            
            if (!interroState) {
              digitalWrite(redPin, LOW);
              digitalWrite(bluePin, LOW);
              digitalWrite(greenPin, LOW);
              
              Serial.print("The interrogation is over after "); // print out your results
              Serial.print(secduration);
              Serial.println(" seconds");
            }
          }
        }
      //}//end of if pleadState
    }//end of while interroState high  
}//end of void loop  

void beep() {
    // frequency is 5 Khz
    //creating a square wave with the note’s corresponding frequency (Pulse Width Modulation)
    for (int i = 0; i < 1000; i++) {
        digitalWrite(buzzerPin, HIGH); 
        delayMicroseconds(100); 
        digitalWrite(buzzerPin, LOW); 
        delayMicroseconds(100);
    } 
}

//LED is off when the interrupt button is pressed
void plead(){
  while (!pleadState){
    digitalWrite(redPin, LOW);
    digitalWrite(bluePin, LOW);
    digitalWrite(greenPin, LOW);
  }
}

void handler(){
  pleadState = !pleadState;
}

