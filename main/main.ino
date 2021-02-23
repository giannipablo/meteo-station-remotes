////////////////////////////////////////////////
// Timer stuff
volatile uint32_t timedInterruptCounter;

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
 
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  timedInterruptCounter++;
  portEXIT_CRITICAL_ISR(&timerMux);
}

////////////////////////////////////////////////
// Ext Int stuff
struct ExtSwitch {
  const uint8_t PIN;
  uint32_t extIntCounter;
};
ExtSwitch ExtSwitch1 = {3, 0};

portMUX_TYPE extIntMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR isr() {
  portENTER_CRITICAL_ISR(&extIntMux);
  ExtSwitch1.extIntCounter += 1;
  portEXIT_CRITICAL_ISR(&extIntMux);
}


////////////////////////////////////////////////
// Loop stuff
bool flagAt0000 = false;
bool flagAt0020 = false;


////////////////////////////////////////////////
// Setup
void setup() {
  // Serial port configuration
  Serial.begin(115200);

  ////////////////////////////////////////////////
  // Timer stuff
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 100000, true);
  timerAlarmEnable(timer);

  ////////////////////////////////////////////////
  // Ext Int stuff
  pinMode(ExtSwitch1.PIN, INPUT_PULLUP);
//  attachInterrupt(ExtSwitch1.PIN, isr, FALLING);
}


////////////////////////////////////////////////
// Loop
void loop() {

  
  if(timedInterruptCounter==0 & flagAt0000==true){
    Serial.printf("Init %u counter at\n", timedInterruptCounter);
    
    attachInterrupt(ExtSwitch1.PIN, isr, FALLING);
    
    flagAt0000 = false;
  }


  if(timedInterruptCounter==20 & flagAt0020==true){
    detachInterrupt(ExtSwitch1.PIN);
    
    Serial.printf("Two seconds interrupt %u counter at %u\n", timedInterruptCounter, ExtSwitch1.extIntCounter);
    
    ExtSwitch1.extIntCounter = 0;

    flagAt0020 = false;
  }


  if(timedInterruptCounter==600){
    Serial.printf("End %u counter at\n", timedInterruptCounter);  
  
    portENTER_CRITICAL_ISR(&timerMux);
    timedInterruptCounter = 0;
    portEXIT_CRITICAL_ISR(&timerMux);

    flagAt0000 = true;
    flagAt0020 = true;
  }

}
