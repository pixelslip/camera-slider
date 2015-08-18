#define WHEEL_RADIUS 10
#define TRANSLATE_TIME 2 // second
#define COUNTDOWN_TIME 35 // second

// set items
int buttonPin = 2;
int motorPin = 9;

// set shift register
int data = 5;
int clock = 6;
int latch = 7;

// set segment
int alim_unite = 11;
int alim_dizaine = 12;

float currentPosition; // position of camera on rails
boolean state;
long time;

void displayNumber(char nb);
void countdown(int from);
void startTraveling();
void stopTraveling();
void shiftNumber(short nb);

void setup()
{
  pinMode(buttonPin, INPUT);
  pinMode(motorPin, OUTPUT);

  pinMode(alim_unite, OUTPUT);
  pinMode(alim_dizaine, OUTPUT);

  pinMode(data, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(latch, OUTPUT);

  state = LOW; // Allow timelapse start - at first, stop
  currentPosition = 0;
}

void loop()
{
  if(state) {
    countdown(COUNTDOWN_TIME);
    startTraveling();
    delay(TRANSLATE_TIME * 1000);
    stopTraveling();
  } else {
    displayNumber(-2); // TODO: must display letter C instead of T
  }

  if(digitalRead(buttonPin) == HIGH) {
    state = HIGH;
    } else {
      if((millis() - time) > 100) {
        time = millis();
      }
    }
}

void displayNumber(char nb) 
{
  long currentTime;
  char unite = 0, dizaine = 0;

  if(nb > 9) {
    dizaine = nb / 10;
  }

  unite = nb - (dizaine * 10);

  currentTime = millis();
  // set display to be able to see number
  while((millis() - currentTime) < 1000) {
    digitalWrite(alim_dizaine, HIGH);
    shiftNumber(dizaine);
    digitalWrite(alim_unite, LOW);
    delay(10);

    digitalWrite(alim_dizaine, LOW);
    shiftNumber(unite);
    digitalWrite(alim_unite, HIGH);
    delay(10);
  } 
}

void countdown(int from) 
{
  for( short i = from; i > 0; i-- ) {
    displayNumber(i);
  }
  displayNumber(-1); // T letter for 'translation'
}

void startTraveling() 
{
    currentPosition += 0.001; // TODO define radius of wheel
    digitalWrite(motorPin, HIGH);

    //TODO: add detector of collision to stop train
}

void stopTraveling() 
{
  digitalWrite(motorPin, LOW);
}

void shiftNumber(short nb) 
{
  digitalWrite(latch, LOW);
  short val;
  
  // TODO : Maybe could do better... It's late, my brain doesn't work well
  switch (nb) {
    case 0:
      val = 192;
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
    case -1: // t letter
      val = 135;
      break;
    case -2: // c letter
      val = 198;
      break;
    default:
      val = 192;
  }

  shiftOut(data, clock, MSBFIRST, val);
  digitalWrite(latch, HIGH);
}
