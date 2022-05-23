#include <Arduino_FreeRTOS.h>

#define RED 3
#define YELLOW 2
#define GREEN 4
#define BUTTON 6


TaskHandle_t lampuMerahHandle;
TaskHandle_t lampuHijauHandle;
TaskHandle_t lampuKuningHandle;
TaskHandle_t buttonInterupsiHandle;

static void lampuMerah( void *pvParameters );
static void lampuHijau( void *pvParameters );
static void lampuKuning( void *pvParameters );
static void buttonInterupsi( void *pvParameters );

int buttonState;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(BUTTON, INPUT);

  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(TEST, OUTPUT);

  xTaskCreate(buttonInterupsi, "Button Interupsi", 100, NULL, 1, &buttonInterupsiHandle);
  xTaskCreate(lampuMerah, "Lampu Merah", 100, NULL, 1, &lampuMerahHandle);
  xTaskCreate(lampuKuning, "Lampu Kuning", 100, NULL, 2, &lampuKuningHandle);
  xTaskCreate(lampuHijau, "Lampu Hijau", 100, NULL, 2, &lampuHijauHandle);
 

}

void loop() {
  // put your main code here, to run repeatedly:
}


static void lampuMerah(void *pvParameters)
{
  while (1)
  { 
    //Serial.println(F("Lampu Merah"));
    vTaskSuspend(lampuKuningHandle);
    vTaskSuspend(lampuHijauHandle);
    digitalWrite(RED,HIGH);
    digitalWrite(YELLOW, LOW);
    digitalWrite(GREEN, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

static void lampuKuning(void *pvParameters)
{
  while (1)
  {
    vTaskDelay(500 / portTICK_PERIOD_MS);
    digitalWrite(YELLOW,HIGH);
    vTaskDelay( 1000 / portTICK_PERIOD_MS );
    vTaskSuspend(NULL);
    vTaskResume(lampuHijauHandle);
  }
}

static void lampuHijau(void *pvParameters)
{
  while (1)
  {
    vTaskDelay(500 / portTICK_PERIOD_MS);
    digitalWrite(YELLOW, LOW);
    digitalWrite(GREEN , HIGH);
    vTaskSuspend(lampuKuningHandle);
    vTaskDelay( 5000 / portTICK_PERIOD_MS );
    digitalWrite(GREEN, LOW);
    digitalWrite(YELLOW , HIGH);
    vTaskDelay(500/ portTICK_PERIOD_MS);
    vTaskResume(lampuMerahHandle);
  }
}

static void buttonInterupsi(void *pvParameters)
{
  while (1)
  {
    vTaskDelay(500 / portTICK_PERIOD_MS);
    if(digitalRead(BUTTON) == 1){
      digitalWrite(RED, LOW);
      Serial.println("button pressed");
      vTaskSuspend(lampuMerahHandle);
      vTaskResume(lampuKuningHandle);
      vTaskDelay(1000 /portTICK_PERIOD_MS);
    }
  }
}
