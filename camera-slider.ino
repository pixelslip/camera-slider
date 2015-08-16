#define MAX_SPEED 120
#define WHEEL_RADIUS 10
#define RAIL_LENGTH 100 // cm

// set items
int buttonPin = 2;
int ledPin = 3;
int motorPin = 9;

// set shift register
int data = 5;
int clock = 6;
int latch = 7;

double currentPosition; // position of camera on rails
long time;


void setup()
{
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(motorPin, OUTPUT);

  pinMode(data, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(latch, OUTPUT);

  currentPosition = 0;
  digitalWrite(ledPin, HIGH);

}

void loop()
{
  countdown();

  if(digitalRead(buttonPin) == HIGH) {
    startTraveling();
  } else {
    if((millis() - time) > 100) {
      stopTraveling(); 
      time = millis();
    }
  }
}

void displayNumber(char nb) 
{
  digitalWrite(latch, LOW);
  short val;
  // TODO : Maybe could do better... It's late, my brain doesn't work well
  switch (nb) {
      case 0:
        val = 191;
        break;
      case 1:
        val = 249;
        break;
      case 2:
        val = 164;
        break;
      case 3:
        val = 48;
        break;
      case 4:
        val = 25;
        break;
      case 5:
        val = 18;
        break;
      case 6:
        val = 2;
        break;
      case 7:
        val = 248;
        break;
      case 8:
        val = 0;
        break;
      case 9:
        val = 16;
        break;
      
      default:
        val = 64;
  }
  shiftOut(data, clock, MSBFIRST, val);
  digitalWrite(latch, HIGH);
}

void countdown() 
{
  for( short i = 10; i > 0; i-- ) {
    displayNumber(i);
    delay(1000);
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

