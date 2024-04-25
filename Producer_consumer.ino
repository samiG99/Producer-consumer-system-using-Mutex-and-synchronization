#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <time.h>
#include <stdlib.h>

const int MAX_SIZE = 10;
//TaskHandle_t initThrd, ReadThrd, addThrd;
static int x;
typedef struct Monitor {
  SemaphoreHandle_t mutex;
  SemaphoreHandle_t bufFullSem;
  SemaphoreHandle_t bufEmptySem;
  int buffer [MAX_SIZE];
  int index = 0;
} MyMonitor;

MyMonitor  M;

void setup() {
  Serial.begin(9600);
  
  init(&M);
  
  xTaskCreate(producer1, "producer1", 100, NULL, 0, NULL);
  xTaskCreate(producer2, "producer2", 100, NULL, 0, NULL);
  xTaskCreate(consumer1, "consumer1", 100, NULL, 0, NULL);
  xTaskCreate(consumer2, "consumer2", 100, NULL, 0, NULL);
}


void init(MyMonitor* M) {
  M->mutex = xSemaphoreCreateMutex();
  M->bufFullSem = xSemaphoreCreateCounting (MAX_SIZE, MAX_SIZE);
  //The maximum count value that can be reached
  //The count value assigned to the semaphore when it is created.
  M->bufEmptySem = xSemaphoreCreateCounting (MAX_SIZE, 0);
}

void Read(MyMonitor* M, int *x) {

      xSemaphoreTake(M->bufEmptySem, portMAX_DELAY);
      xSemaphoreTake(M->mutex, portMAX_DELAY);
      *x = M->buffer[--(M->index)];
      xSemaphoreGive(M->mutex);
      xSemaphoreGive(M->bufFullSem);
}

void add(MyMonitor* M, int x) {

      Serial.println(x);
      xSemaphoreTake(M->bufFullSem, portMAX_DELAY);
      xSemaphoreTake(M->mutex, portMAX_DELAY);
      //Serial.println();
      M->buffer[M->index++] = x;
      xSemaphoreGive(M->mutex);
      xSemaphoreGive(M->bufEmptySem);

}

void producer1()
{
  for(;;){
    int r = rand() %10;
    add(&M, r);
  }
}
void producer2()
{
  for(;;){
    int r = rand()%10;
    add(&M, r);
  }
}

void consumer1()
{
  for(;;){
    
    Read(&M, &x);
  }
}
void consumer2()
{
  for(;;){
    Read(&M, &x);
  }
}
void loop()
{

}
