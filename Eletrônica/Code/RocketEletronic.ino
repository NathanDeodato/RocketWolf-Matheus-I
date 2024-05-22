// Libs 
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>

// Consts
#define RELE_PIN 6
#define SD_CS_PIN 4
#define BUZZER 2

Adafruit_BMP085 bmp;
File dataFile;

// Config function
void setup() {
  
  // Arduino Serial Port Configuration
  Serial.begin(9600);

  // Sensor Configuration
  if (!bmp.begin()) {
    Serial.println("Erro ao inicializar o sensor BMP180.");
    return;
  }

  // Configuring the relay ports
  pinMode(RELE_PIN, OUTPUT);
  digitalWrite(RELE_PIN, LOW);

  // SD card configuration
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("Erro ao inicializar o cartão SD.");
    return;
  }

  // Configuring data storage on the SD card
  dataFile = SD.open("DADOS.txt", FILE_WRITE);
  if (!dataFile) {
    Serial.println("Erro ao abrir o arquivo.");
    return;
  }
}

// Process function
void loop() {
  
  // Sensor variables
  float temperature = bmp.readTemperature();
  float pressure = bmp.readPressure();
  float altitude = (bmp.readAltitude(101663.00 ));

  // Error Handling
  if (isnan(temperature) || isnan(pressure) || isnan(altitude)) {
    Serial.println("Erro ao ler os dados do sensor BMP180.");
    return;
  }

  // Relay activation
  if (altitude >= 15.00) {
    digitalWrite(RELE_PIN, HIGH);
    Serial.println("Relé ativado.");
    dataFile.println("Relé ativado.");

    // Buzzer config active
    delay(500);

    tone(BUZZER, 1500);
    delay(50);

    noTone(BUZZER);
    delay(50);
    
  } else {
    digitalWrite(RELE_PIN, LOW);
    Serial.println("Relé desativado.");
    dataFile.println("Relé desativado.");
  }

  // Sensor Reading and Update
  Serial.print("Temperatura: ");
  Serial.print(temperature);
  Serial.print("°C; Pressão: ");
  Serial.print(pressure);
  Serial.print(" Pa; Altitude: ");
  Serial.println(altitude);

  dataFile.print("Temperatura: ");
  dataFile.print(temperature);
  dataFile.print("°C; Pressão: ");
  dataFile.print(pressure);
  dataFile.print(" Pa; Altitude: ");
  dataFile.println(altitude);

  // Storage of readings
  dataFile.flush();

  // Reading range 
  delay(4000);
}
