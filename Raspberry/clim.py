
# LIBRAIRIES
import serial
import time
import datetime
import csv

# CONSTANTES CULTURE
TEMP_MAX = 22
HUM_MAX = 90
HUM_MIN = 40

# CONSTANTES PGM
PORT_ARDUINO = "/dev/ttyACM0"
BAUD = 115200
DELAIS_MESURE = 10
numéro = 1
ventilateur = 0
humidificateur = 0


# DEFINITION FONCTION


def write(no, date, heure, tint, text, hint, hext, vent, hum):
    with open(Titre + '.csv', 'a', newline='') as f:  # 'a' pour append (ajouter)
        writer = csv.writer(f)
        writer.writerow([no, date, heure, tint, text, hint, hext, vent, hum])

    f.close()


def commandeArduino(cmd):
    time.sleep(DELAIS_MESURE)
    arduino.write(str.encode(cmd))
    reponse = arduino.readline().decode()[:-2]
    print(reponse)


def Convert(string):
    li = list(string.split(";"))
    return li


# CREATION FICHIER CSV
RawDate = datetime.datetime.now() + datetime.timedelta(hours=1)
Titre = RawDate.strftime('%Y-%m-%d')
with open(Titre + '.csv', 'w', newline='') as f:  # 'w' pour write
    writer = csv.writer(f)
    writer.writerow(["No", "Date", "Heure", "Temperature Intérieure", "Temperature extérieure",
                    "Humidité Intérieure", "Humidité extérieure", "Ventilateur", "Humidificateur"])
    f.close()

# BOUCLE

with serial.Serial(PORT_ARDUINO, BAUD, timeout=2) as arduino:
    time.sleep(3)
    while True:
        arduino.write(str.encode('data'))
        RawSensor = arduino.readline().decode()[:-2]
        Data = Convert(RawSensor)
        tint = float(Data[2])
        text = float(Data[3])
        hint = float(Data[0])
        hext = float(Data[1])
        ventilateur = int(Data[4])
        humidificateur = int(Data[5])
        RawDate = datetime.datetime.now() + datetime.timedelta(hours=1)
        Jour = RawDate.strftime('%Y-%m-%d')
        Heure = RawDate.strftime('%H:%M:%S')

        if tint >= TEMP_MAX:
            commandeArduino("VentilON")
        elif tint < (TEMP_MAX - 2):
            commandeArduino("VentilOFF")
        else:
            time.sleep(DELAIS_MESURE)

        if hint >= HUM_MAX and ventilateur == 0:
            commandeArduino("VentilON")
        elif hint < (HUM_MAX - 15) and tint < (TEMP_MAX - 2):
            commandeArduino("VentilOFF")
        elif hint <= HUM_MIN and ventilateur == 0:
            commandeArduino("HumidON")
        elif hint > (HUM_MAX - 20):
            commandeArduino("HumidOFF")
        else:
            time.sleep(DELAIS_MESURE)

        write(numéro, Jour, Heure, tint, text, hint, hext, ventilateur, humidificateur)
        print(Jour, Heure)
        print(Data)

        numéro += 1
        time.sleep(DELAIS_MESURE)
