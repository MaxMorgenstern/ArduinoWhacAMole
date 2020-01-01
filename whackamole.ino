
// Init
int BUTTON_COUNT = 5;
int PIN_READ[] = {2, 4, 6, 8, 10};
int PIN_WRITE[] = {3, 5, 7, 9, 11};

int PIN_STATE[] = {0, 0, 0, 0, 0};
int ACTIVE_PIN_COUNT = 0;

int PIN_ACTIVE_SINCE[] = {-1, -1, -1, -1, -1};

int SPEED = 5000;
int SPEED_INIT = 5000;
int SPEED_STEP = 500;
int SPEED_MIN = 1000;

long lastActivation = 0;
long lastPress = 0;

// ---------------------------------

void setup() {
    Serial.begin(19200); // Debugging only
    for(int i = 0; i < BUTTON_COUNT; i = i + 1) {
      pinMode(PIN_READ[i], INPUT);
    }
    for(int i = 0; i < BUTTON_COUNT; i = i + 1) {
      pinMode(PIN_WRITE[i], OUTPUT);
    }

    lastActivation = millis() - (SPEED / 2);
    lastPress = millis();
}

// ---------------------------------

void loop() {
  checkPress();
  if(lastActivation + SPEED < millis()){
    if(activateRandomPin())
      lastActivation = millis();
  }
}

// ---------------------------------

//Activate a random pin/button if there are any left
bool activateRandomPin() {
  if(ACTIVE_PIN_COUNT < BUTTON_COUNT){
    int r = random(BUTTON_COUNT);
    if(PIN_STATE[r] == 0) {
      activate(r);
      return true;
    } else {
      return activateRandomPin();
    }
  }
  return false;
}

// deactivate all buttons, show error and reset
void showErrorAndReset() {
  for(int i = 0; i < BUTTON_COUNT; i = i + 1) {
    deactivate(i);
  }
  
  flashButtons(4, 500);
  
  SPEED = SPEED_INIT;
  ACTIVE_PIN_COUNT = 0;
}

// flash buttons
void flashButtons(int count, int switchDelay) {
  for(int i = 0; i < count; i = i + 1) {
    for(int i = 0; i < BUTTON_COUNT; i = i + 1) {
      activate(i);
    }
    delay(switchDelay);
    for(int i = 0; i < BUTTON_COUNT; i = i + 1) {
      deactivate(i);
    }
    delay(switchDelay);
  }
}

// check if anything has been presses
void checkPress() {
  for(int i = 0; i < BUTTON_COUNT; i = i + 1) {
    if(digitalRead(PIN_READ[i]) == HIGH) {
      bool pressValid = validPress(i);
      if(pressValid) {
        deactivate(i);
        lastPress = millis();
        if( SPEED > SPEED_MIN) {
          SPEED = SPEED - SPEED_STEP;
        }
        
        if(digitalRead(PIN_READ[i]) == HIGH) {
        }
        
      } else {
        showErrorAndReset();
        return;
      }
      delay(250);
    } 
  }
}

// is this a valid press or not
bool validPress(int ID) {
  return (PIN_STATE[ID] == 1);
}

// deactivate a button
void deactivate(int ID) {
  digitalWrite(PIN_WRITE[ID], LOW);
  PIN_STATE[ID] = 0;
  PIN_ACTIVE_SINCE[ID] = -1;
  ACTIVE_PIN_COUNT = ACTIVE_PIN_COUNT - 1;
}

// activate pin/button if not active
void activate(int ID) {
  if( PIN_STATE[ID] != 1) {
    digitalWrite(PIN_WRITE[ID], HIGH);
    PIN_STATE[ID] = 1;
    PIN_ACTIVE_SINCE[ID] = millis();
    ACTIVE_PIN_COUNT = ACTIVE_PIN_COUNT + 1;
  }
}


