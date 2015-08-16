#define MAX_SPEED 120
#define WHEEL_RADIUS 10
#define RAIL_LENGTH 100 // cm

// set items
int buttonPin = 2;
int ledPin = 3;
int motorPin = 9;

double currentPosition; // position of camera on rails
long time;

void setup()
{
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(motorPin, OUTPUT);

  currentPosition = 0;
  digitalWrite(ledPin, HIGH);

}

void loop()
{
  if(digitalRead(buttonPin) == HIGH) {
    startTraveling();
  } else {
    if((millis() - time) > 100) {
      stopTraveling(); 
      time = millis();
    }
  }
}

void startTraveling() 
{
  if(currentPosition < RAIL_LENGTH) 
  {
    currentPosition += 0.001; // TODO ajouter valeur perimetre roue  
    analogWrite(motorPin, MAX_SPEED);
  } else {
    digitalWrite(ledPin, LOW);
  }
}

void stopTraveling() 
{
  analogWrite(motorPin, LOW);
}

