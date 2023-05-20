
// LIBRAIRIES
#include <Arduino.h>
#include <DHT.h>

// PIN
#define DHTPIN1 2     // Digital pin connected to the DHT sensor int
#define DHTPIN2 3     // Digital pin connected to the DHT sensor ext
#define DHTTYPE DHT22 // DHT 22 (AM2302)
#define VENTILOPIN 4  // Digital pin du ventilo
#define HUMIDPIN 5    // Digital pin de l'humidificateur
DHT dhtint(DHTPIN1, DHTTYPE);
DHT dhtext(DHTPIN2, DHTTYPE);

// DEFINITION DES CONSTANTES
const int DELAIS_MIN = 2000;
const int NB_CAPTEUR = 2;
unsigned long baud = 115200;
int etatVentilateur = 0;
int etatHumidificateur = 0;
String commandeRaspberry;

// DEFINITION DES FONCTIONS
void readSerialPort()
{
  commandeRaspberry = "";
  if (Serial.available())
  {
    delay(10);
    while (Serial.available() > 0)
    {
      commandeRaspberry += (char)Serial.read();
    }
    Serial.flush();
  }
}

void sendData()
{
  float humiditeint = dhtint.readHumidity();
  float humiditeext = dhtext.readHumidity();
  float temperatureint = dhtint.readTemperature();
  float temperatureext = dhtext.readTemperature();
  if (digitalRead(VENTILOPIN) == HIGH)
  {
    etatVentilateur = 1;
  }
  else
  {
    etatVentilateur = 0;
  }
  if (digitalRead(HUMIDPIN) == HIGH)
  {
    etatHumidificateur = 1;
  }
  else
  {
    etatHumidificateur = 0;
  }

  Serial.print(String(humiditeext) + ';');
  Serial.print(String(humiditeint) + ';');
  Serial.print(String(temperatureext) + ';');
  Serial.print(String(temperatureint) + ';');
  Serial.print(String(etatVentilateur) + ';');
  Serial.println(String(etatHumidificateur));

  delay(DELAIS_MIN);
  Serial.flush();
}

// MAIN
void setup()
{
  Serial.begin(baud);
  dhtext.begin();
  dhtint.begin();
  pinMode(VENTILOPIN, OUTPUT);
  pinMode(HUMIDPIN, OUTPUT);
}
void loop()
{
  readSerialPort();

  if (commandeRaspberry == "data")
  {
    sendData();
  }
  else if (commandeRaspberry == "HumidON")
  {
    digitalWrite(HUMIDPIN, HIGH);
    Serial.print("Humidificateur ON !");
    delay(DELAIS_MIN);
    Serial.flush();
  }
  else if (commandeRaspberry == "HumidOFF")
  {
    digitalWrite(HUMIDPIN, LOW);
    Serial.print("Humidificateur OFF !");
    delay(DELAIS_MIN);
    Serial.flush();
  }
  else if (commandeRaspberry == "VentilON")
  {
    digitalWrite(VENTILOPIN, HIGH);
    Serial.print("Ventilateur ON !");
    delay(DELAIS_MIN);
    Serial.flush();
  }
  else if (commandeRaspberry == "VentilOFF")
  {
    digitalWrite(VENTILOPIN, LOW);
    Serial.print("Ventilateur OFF !");
    delay(DELAIS_MIN);
    Serial.flush();
  }
}
