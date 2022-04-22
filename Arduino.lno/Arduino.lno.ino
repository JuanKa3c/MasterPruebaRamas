#include <HID.h>

#include <DHTesp.h>
#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 3 
#define DHTTYPE DHT11  
#define RELE 2
#define SIMULADOR 4
#define LED 13

int push_lee = 0;
bool estadoLed=true;
  
//Crear el objeto lcd  direccin  0x3F y 16 columnas x 2 filas
LiquidCrystal_I2C lcd(0x27,16,2);
DHT dht(DHTPIN, DHTTYPE);
DHT dht (SENSOR, DHT22);

void setup() {
  // Inicializar el LCD
  lcd.init();
  //Encender la Bombillo.
  lcd.backlight();

  //Inicio Sensor Temperatura
  dht.begin();
 
  pinMode(RELE, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(SIMULADOR, INPUT);
  digitalWrite(LED, HIGH);
}

void encender_bombilla()
  {
  digitalWrite(RELE, HIGH);
  lcd.setCursor(0, 1);
  lcd.print("  CALEFACCION");  
  }

void encender_aire()
  {
  digitalWrite(RELE, LOW);
  lcd.setCursor(0, 1);
  lcd.print("REFRIGERACION");  
  }

void real()
  {
  float humedad = dht.readHumidity();
  float temperatura = dht.readTemperature();
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(temperatura);
  lcd.print(" - ");
  lcd.print(humedad);
  lcd.print(" %  ");   
  
  if (temperatura>39)
    encender_aire();
  else
    encender_bombilla();
  delay(500);      
  }

void simulacion()
  {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("INICIO SIMULACION");
  delay(1000);
  lcd.clear();
  float humedad = dht.readHumidity();
  float temperatura=22;  
  for (int i=0;i<20;i++)
    {
    if (temperatura>25)
      encender_aire();
    else
      encender_bombilla();
    delay(1000);
    temperatura+=0.1;
    lcd.setCursor(0, 0);
    lcd.print(temperatura);
    lcd.print(" - ");
    lcd.print(humedad); 
    }
    
  for (int i=0;i<20;i++)
    {
    if (temperatura>25)
      encender_aire();
    else
      encender_bombilla();
    delay(1000);
    temperatura-=0.1;
    lcd.setCursor(0, 0);
    lcd.print(temperatura);
    lcd.print(" - ");
    lcd.print(humedad);
    }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("FIN SIMULACION");
  delay(1000);
  lcd.clear();
  }

void loop() 
  {
  push_lee = digitalRead(SIMULADOR);
  
  if (push_lee == 1) 
    {
      //Estaba encendido, lo apago
      digitalWrite(LED,LOW);
      //ALMACENO QUE ESTA APAGADO
      estadoLed=false;
      simulacion();
      //Estaba apagado, lo enciendo  
      digitalWrite(LED,HIGH);
  }
  real(); 
  }
