#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define RST_PIN 9
#define SS_PIN 10
#define LED_PIN_VERDE 2
#define LED_PIN_ROJO 3
//ID TARJETA: CE 02 36 02
//ID LLAVERO: 73 39 9B 13

//Listado de IDs registrada
const byte listado_UID[][4] = {
  {0xCE, 0x02, 0x36, 0x02} //ID Tarjeta
};

//Listado de nombres 
const char* nombresTarjetas[] = {
  "Carol"
};

//se dividen las filas por la cantidad de bytes de la tarjet
//para encontrar la posición en el listado de nombres y asociarlas a su ID 
const int cantidad_ID = sizeof(listado_UID) / 4;

//Crea objeto del modulo
MFRC522 mfrc522(SS_PIN, RST_PIN);

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  //Inicia comunicación
  Serial.begin(9600);
  //Inicia bus SPI
  SPI.begin();
  //Comunicación con el lector
  mfrc522.PCD_Init();

  //LED
  pinMode(LED_PIN_VERDE, OUTPUT);
  pinMode(LED_PIN_ROJO, OUTPUT);

  //Asegura que el led esté apagado
  digitalWrite(LED_PIN_VERDE, LOW);
  digitalWrite(LED_PIN_ROJO, LOW);

  lcd.init();
  lcd.backlight();

  
  modoEspera();
}

void loop() {
  // Condición para verificar que la tarjeta está presentes
  if ( !mfrc522.PICC_IsNewCardPresent() ){
    //apaga los led si no hay tarjeta
    digitalWrite(LED_PIN_VERDE, LOW);
    digitalWrite(LED_PIN_ROJO, LOW);
    return;
  }
    
  // Condición para extraer datos de tarjeta
  if ( !mfrc522.PICC_ReadCardSerial() ) {
    digitalWrite(LED_PIN_VERDE, LOW);
    digitalWrite(LED_PIN_ROJO, LOW);
    return;
  }
    
  //Verifica que la tarjeta esté registrada:
  bool estaRegistrada = false;
  const char* nombre = "";

  for (int i = 0; i < cantidad_ID; i++) {
    bool coincide = true;
    for (byte x = 0; x < mfrc522.uid.size; x++) {
      if (mfrc522.uid.uidByte[x] != listado_UID[i][x]) {
        coincide = false;
        break;
      }
    }

    if (coincide) {
      estaRegistrada = true;
      nombre = nombresTarjetas[i];
      break;
    }

  }

  if (estaRegistrada) {
    digitalWrite(LED_PIN_VERDE, HIGH);
    digitalWrite(LED_PIN_ROJO, LOW);
    hola();
    modoEspera();

  } else {
    digitalWrite(LED_PIN_VERDE, LOW);
    digitalWrite(LED_PIN_ROJO, HIGH);
    adios();
    modoEspera();
  }

  delay(1000);
  mfrc522.PICC_HaltA();
    
  
}

void hola(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Welcome :D");
  lcd.setCursor(0, 1);
  lcd.print("Hola");
  delay(1000);
}

void adios(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Adios ");
  lcd.setCursor(0, 1);
  lcd.print(":(");
  delay(1000);
  
}

void modoEspera(){
  delay(1000);
  lcd.setCursor(0, 0);
  lcd.print("Bienvenido/a");

  lcd.setCursor(0, 1);
  lcd.print("a CITT PALTO");
}

