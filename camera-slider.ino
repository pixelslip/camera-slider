#include <LiquidCrystal.h>
#include "Menu.h"

#define NELEMS(x)  (sizeof(x) / sizeof((x)[0])) // number elems of array

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

static char* configInterval[] = {
  "15sec",
  "30sec",
  "60sec"
};
static char* configDuration[] = {
  "60min",
  "90min",
  "120min"
};
static char* configDistance[] = {
  "20cm",
  "40cm",
  "60cm",
  "80cm",
  "100cm"
};

static char* options[] = {
  "Interval",
  "Duration",
  "Distance",
  "Start!"
};

static Menu MainMenu = {
  "Configuration:",
  options,
  4,
  0,
  &actionMainMenu
};

static Menu IntervalMenu = {
  "Interval",
  configInterval,
  3,
  0,
  &actionIntervalMenu
};
static Menu DurationMenu = {
  "Duration",
  configDuration,
  3,
  0,
  &actionDurationMenu
};
static Menu DistanceMenu = {
  "Distance",
  configDistance,
  5,
  4,
  &actionDistanceMenu
};

byte glyphArrow[8] = {B00000, B00100, B00010, B11111, B00010, B00100, B00000, B00000};
byte glyphSubmenu[8] = {B10000, B10000, B10100, B10010, B11111, B00010, B00100, B00000};

byte p1[8] = {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10};
byte p2[8] = {0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18};
byte p3[8] = {0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C};
byte p4[8] = {0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E};
byte p5[8] = {0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F};

int buttonPinSelect = 2;
int buttonPinEnter = 3;
int buttonPinEsc = 4;
int motorPin = 6;

float currentPosition = 0;
boolean state = LOW;
long time;
long sleep;

float interval;
float distance;
float duration;

int lastButtonState = 0;

float buttonPushDebounce = 0;
float debounce = 100;

void setup()
{
  pinMode(buttonPinSelect, INPUT);
  pinMode(buttonPinEnter, INPUT);
  pinMode(buttonPinEsc, INPUT);

  pinMode(motorPin, OUTPUT);
  lcd.createChar(0, glyphArrow);
  lcd.createChar(1, glyphSubmenu);
  lcd.createChar(2, p1);
  lcd.createChar(3, p2);
  lcd.createChar(4, p3);
  lcd.createChar(5, p4);
  lcd.createChar(6, p5);
  //https://www.electronicsblog.net/arduino-lcd-horizontal-progress-bar-using-custom-characters/

  lcd.begin(16, 2);

  //TODO Screen presentation
  
  sleep = millis();
}

void loop()
{  
  checkMenu(MainMenu, false); 
  
  lcd.clear();
  lcd.print('Reset');
  delay(2000);
}

void checkMenu(Menu &menu, boolean isSubmenu) 
{
  byte quitMenu = false;
  byte select = 0;
  Button buttonState;

  do {
    lcd.clear();
    if(!isSubmenu) {
      // Menu name
      // 1.Submenu
      lcd.print(menu.name);
      lcd.setCursor(0, 1);      
      lcd.print(select + 1, 10);
      lcd.write('.');
      lcd.print(menu.items[select]);
    } else {
      // └> Menu
      // ->Value
      lcd.write(byte(1));
      lcd.print(menu.name);
      lcd.setCursor(0, 1);
      if(select == menu.current) {
        lcd.print("[#]");
      } else {
        lcd.print("[ ]");
      }
      lcd.print(menu.items[select]);
    }

    /* Attend le relâchement du bouton */
    while(readButtonState() != NONE);
 
    /* Attend l'appui sur un bouton */
    while((buttonState = readButtonState()) == NONE);
    delay(50);
 
    /* Attend le relâchement du bouton */
    while(readButtonState() != NONE);

    switch (buttonState) {
        case SELECT:
          if( select < (menu.nbItems - 1)) {
            select++;
          } else {
            select = 0;
          }
          break;
        case ENTER:
          menu.callbackFct(select);
          break;
        case ESC:
          quitMenu = true;
          break;
        default:
          break;
    }
    Serial.println(NELEMS(menu.items));
  } while(!quitMenu);
}

Button readButtonState(void)
{
  int buttonSelectState;
  int buttonEnterState;
  int buttonEscState;
  
  buttonSelectState = digitalRead(buttonPinSelect);
  buttonEnterState = digitalRead(buttonPinEnter);
  buttonEscState = digitalRead(buttonPinEsc);

  if(checkState(buttonSelectState)){ return SELECT; }
  if(checkState(buttonEnterState)){ return ENTER; }
  if(checkState(buttonEscState)){ return ESC; }
  
  return NONE;
}

byte checkState(int btn)
{
  int buttonState;
  buttonState = btn;

  if(buttonState != lastButtonState) {    
    if(buttonState == HIGH) { //&& (millis() - buttonPushDebounce) > debounce
      buttonPushDebounce = millis();
      return HIGH;
    }
    lastButtonState = buttonState;
  }
  return LOW;
}

void actionMainMenu(byte select)
{
  switch (select) {
      case 0:
        checkMenu(IntervalMenu, true); // Interval
        break;
      case 1:
        checkMenu(DurationMenu, true); // Duration
        break;
      case 2:
        checkMenu(DistanceMenu, true); // Distance
        break;
      case 3:
        travelingControl();
  }
}

void actionIntervalMenu(byte select)
{
  IntervalMenu.current = select;
  displayValue(select, configInterval);
}
void actionDurationMenu(byte select)
{
  DurationMenu.current = select;
  displayValue(select, configDuration);
}
void actionDistanceMenu(byte select)
{
  DistanceMenu.current = select;
  displayValue(select, configDistance);
}

void display(byte selectItem,  char** items)
{
  lcd.clear();
  lcd.write(byte(1));
  lcd.print(items[selectItem]);
  lcd.setCursor(0, 1);
  lcd.print("option to define");
  byte btn;
  do {
    btn = readButtonState();
  } while( btn != ESC );
}

void displayValue(byte value,  char** items)
{
  lcd.clear();
  lcd.print("SAVED!");
  lcd.setCursor(0, 1);
  lcd.print(items[value]);
  delay(1500);
}

void travelingControl(void) 
{
  byte btn;
  float percent = 0.0;
  float interval, duration, distance;

  /*
  TODO: change switch convertion, not efficient!
   */
  switch (IntervalMenu.current) {
      case 0:
        interval = 15.0;
        break;
      case 1:
        interval = 30.0;
        break;
      case 2:
        interval = 60.0;
        break;
      default:
        interval = 0.0;
  }
  switch (DurationMenu.current) {
      case 0:
        duration = 60.0;
        break;
      case 1:
        duration = 90.0;
        break;
      case 2:
        duration = 120.0;
        break;
      default:
        duration = 0.0;
  }
  switch (DistanceMenu.current) {
      case 0:
        distance = 20.0;
        break;
      case 1:
        distance = 40.0;
        break;
      case 2:
        distance = 60.0;
        break;
      case 3:
        distance = 80.0;
        break;
      case 4:
        distance = 100.0;
        break;
      default:
        distance = 0.0;
  }

  float lapse = distance / (duration * 60);
  float currentDistance = 0.0;

  do {
    currentDistance += lapse;
    percent = distance / 100 * currentDistance;
    startTraveling(lapse);
    duration -= 1;
    btn = readButtonState(); //TODO debounce
    delay(1000);

    lcd.clear();
    lcd.print("TRAVELING");
    lcd.print("  ");
    lcd.print((int)percent);
    lcd.print("%");
    lcd.setCursor(0, 1);
    lcd.print(currentDistance);
    lcd.print("cm");

  } while(btn != ESC || collisionTest() || duration == 0);
  stopTraveling();
}

void startTraveling(float distance) 
{
  //TODO convert distance for wheel
  digitalWrite(motorPin, HIGH);
}

void stopTraveling(void) 
{
  digitalWrite(motorPin, LOW);
}

boolean collisionTest(void)
{
  //TODO function to check if collision
  return false;
}