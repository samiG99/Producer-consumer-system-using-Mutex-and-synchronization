#include <Arduino_FreeRTOS.h>
#include <timers.h>

int intr1Pin = 2;
int led [7] = {6, 7, 8, 9, 10, 11, 12};
int highest = (sizeof(led) / sizeof(led[0]));
bool reverse = 0;
static int flag = 0;

TimerHandle_t autoReloadHandle1, autoReloadHandle2;

void autoReloadCallBackFunc1(TimerHandle_t timerHandle)
{
  static bool togel = 0;
  for (int i = 0; i < (sizeof(led) / sizeof(int)); i++) {
    if (togel == 0) {
      digitalWrite(led[i], HIGH);
      togel = 1;
    }
    else {
      digitalWrite(led[i], LOW);
      togel = 0;
    }
  }
}

void autoReloadCallBackFunc2(TimerHandle_t timerHandle)
{
  int i;
  for (i = 0; i < 7; i++)
  {
    digitalWrite(led[i], HIGH);
    delay(100);
    digitalWrite(led[i], LOW);
    delay(100);
  }

  for (i = 7 - 1; i >= 0; i--)
  {
    digitalWrite(led[i], HIGH);
    delay(100);
    digitalWrite(led[i], LOW);
    delay(100);
  }
}
/*
  void ISR1() {

  if (flag == 0) {
    xTimerStopFromISR(autoReloadHandle1, NULL);
    xTimerStartFromISR(autoReloadHandle2, NULL);
    flag  = 1;
    Serial.println(flag);
  }
  else {
    xTimerStopFromISR(autoReloadHandle2, NULL);
    xTimerStartFromISR(autoReloadHandle1, NULL);
    flag = 0;
    Serial.println(flag);
    //delay(20);
  }
  }
*/

/*
  void ISR1() {
  if (xTimerIsTimerActive(autoReloadHandle1) != pdFALSE) {
    xTimerStopFromISR(autoReloadHandle2, NULL);
  }
  if (xTimerIsTimerActive(autoReloadHandle2) != pdFALSE) {
    xTimerStopFromISR(autoReloadHandle1, NULL);
  }
  }
*/


void ISR1() {
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  if (xTimerIsTimerActive(autoReloadHandle1) != pdFALSE && flag == 0 ) {
    xTimerStopFromISR(autoReloadHandle2, xHigherPriorityTaskWoken);
    xTimerStartFromISR(autoReloadHandle1, xHigherPriorityTaskWoken);
    delay(10);
    flag = 1;
    delay(10);
    Serial.println(flag);
  }
  else {
    xTimerStopFromISR(autoReloadHandle1, xHigherPriorityTaskWoken);
    xTimerStartFromISR(autoReloadHandle2, xHigherPriorityTaskWoken);
    delay(10);
    flag  = 0;
    delay(10);
    Serial.println(flag);
  }
}


/*
void ISR1() {
  switch (flag) {
    case 0:
      xTimerStop(autoReloadHandle1, 0);
      xTimerStart(autoReloadHandle2, 0);
      //flag = 1;
      break;

    case 1:
      xTimerStop(autoReloadHandle2, 0);
      xTimerStart(autoReloadHandle1, 0);
      //flag = 0;
      break;

    default:
      Serial.println("pls enter num 2");
      break;

  }
}
*/

void setup() {
  Serial.begin(9600);
  pinMode(intr1Pin, INPUT_PULLUP);

  for (int j = 0; j < highest; j++)
  {
    pinMode(led[j], OUTPUT);
  }

  autoReloadHandle1 = xTimerCreate("AutoReload1", pdMS_TO_TICKS(500), pdTRUE, NULL, autoReloadCallBackFunc1);
  autoReloadHandle2 = xTimerCreate("AutoReload2", pdMS_TO_TICKS(500), pdTRUE, NULL, autoReloadCallBackFunc2);

  attachInterrupt(digitalPinToInterrupt(intr1Pin), ISR1, RISING);

  if (autoReloadHandle1 != NULL && autoReloadHandle2 != NULL) {
    xTimerStart(autoReloadHandle1, 0);
    // xTimerStart(autoReloadHandle2, 0);
  }
}

void loop() {
}
