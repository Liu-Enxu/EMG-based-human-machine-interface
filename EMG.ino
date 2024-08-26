#include <Servo.h>

const int analogInPin = A0; 
int sensorValue = 0;
float voltageValue;
int output_percent;
int MIN_ADC_LEVEL = 1023;      // will be 0 for 0V     (set like this for calibration purposes)
int MAX_ADC_LEVEL = 0;   // will be 1023 for 5V        (set like this for calibration purposes)

//LED
int analogOutPin[] = {2,4,7,8,12};
int outputValue = 0;
int i = 0;
//Dimmer LED
const int LED_out = 3;
//VCO
const int audioOutput = 5;
int audioFrequency = 0;
//Servo
Servo myservo; 
int pos = 0;
const int servoPin = 6; 
//Dimmer LED
const int dimmer = 9; 
void setup() {
  Serial.begin(38400);

  for(i=0;i<5;i++){
    pinMode(analogOutPin[i],OUTPUT);
  }
  pinMode(LED_out,OUTPUT);
  pinMode(audioOutput,OUTPUT);

  pinMode(analogInPin,INPUT);
  myservo.attach(servoPin);
  myservo.write(0);
  //---------------------------------------------
  Serial.println("**PLEASE FULLY FLEX AND FULLY RELAX YOUR MUSCLES NOW AND FOR THE NEXT FEW SECONDS**");
  delay(3000);

  while (millis() < 10000)
  {
    
    sensorValue = analogRead(A0);
    voltageValue = (float(sensorValue)/1023)*5;

    Serial.print("ADC is ");
    Serial.print(sensorValue);
    Serial.print(", corresponding to ");
    Serial.print(voltageValue);
    Serial.print("V");
    Serial.println("    **CONTINUE TO FLEX AND RELAX MUSCLE**");

    // for minimum ADC value
    
    if (MIN_ADC_LEVEL > sensorValue)
    {
      MIN_ADC_LEVEL = sensorValue;
    }

    // for maximum sensor value

    if (MAX_ADC_LEVEL < sensorValue)
    {
      MAX_ADC_LEVEL = sensorValue;
    }
    
    }   // end of 5 seconds calibration period

    Serial.println(" ");
    Serial.println("***CALIBRATION COMPLETE***");
    Serial.print("Maximum ADC value recorded was ");
    Serial.print(MAX_ADC_LEVEL);
    Serial.print(" and the Mimimum ADC value was ");
    Serial.println(MIN_ADC_LEVEL);
    Serial.println(" ");
    Serial.println("The program will begin momenterily...");
    delay(3000);
}
  
void loop() {
  sensorValue = analogRead(analogInPin);
  voltageValue = (float(sensorValue)/1023)*5;
  //1
  outputValue = map(sensorValue, MIN_ADC_LEVEL, MAX_ADC_LEVEL, 0, 255);
  output_percent = float(outputValue)/255*100;

   if (outputValue > 255){
    outputValue = 255;
    output_percent = 100;
  }

  if (outputValue < 0){
    outputValue = 0;
    output_percent = 0;
  }
  Serial.print("ADC is ");
  Serial.print(sensorValue);
  Serial.print(", corresponding to ");
  Serial.print(voltageValue);
  Serial.print("V. Mapped output value is: ");
  Serial.print(outputValue);
  Serial.print(" or ");
  Serial.print(output_percent);
  Serial.println("%");
  //Dimmer LED
  analogWrite(dimmer, outputValue);
  //2
  for(i=4;i>=0;i--){
      digitalWrite(analogOutPin[i], LOW);
  }
  digitalWrite(analogOutPin[0], LOW);
  for(i=0;i<5;i++){
    if(outputValue>=i*50){
      digitalWrite(analogOutPin[i], HIGH);
    }
  }
  //3
  audioFrequency = map(outputValue, 0, 255, 50, 550);
  tone(audioOutput,audioFrequency);
  //4
  pos = map(outputValue,0,255,0,180);
  myservo.write(pos);
  
  // print the results to the Serial Monitor:
  //Serial.print("sensor = ");
  //Serial.print(sensorValue);
  Serial.print("\t output = ");
  Serial.println(outputValue);

  //delay(2);
}
