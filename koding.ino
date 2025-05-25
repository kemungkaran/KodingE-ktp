#include <EEPROM.h>
#include <SPI.h>
#include <MFRC522.h>

#define LED1_PIN 6
#define LED2_PIN 7
#define RELAY_PIN 8
#define DELETE_PIN 3
#define BUZZER_PIN 2
#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);

boolean cocok = false;
boolean programMode = false;
int berhasil;
byte sementara[4];
byte baca[4];
byte master[4];

void setup() {
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(LED1_PIN, OUTPUT);
    pinMode(LED2_PIN, OUTPUT);
    pinMode(DELETE_PIN, INPUT_PULLUP);
    pinMode(RELAY_PIN, OUTPUT);

    SPI.begin();
    mfrc522.PCD_Init();
    ShowReaderDetails();

    if (digitalRead(DELETE_PIN) == LOW) {
        digitalWrite(LED1_PIN, HIGH);
        digitalWrite(LED2_PIN, HIGH);
        delay(5000);
        if (digitalRead(DELETE_PIN) == LOW) {
            for (int x = 0; x < EEPROM.length(); x++) {
                EEPROM.write(x, 0);
            }
            digitalWrite(BUZZER_PIN, HIGH);
            flashLED(LED1_PIN, LED2_PIN, 4, 200);
            digitalWrite(BUZZER_PIN, LOW);
        } else {
            digitalWrite(LED1_PIN, LOW);
            digitalWrite(LED2_PIN, LOW);
            digitalWrite(BUZZER_PIN, LOW);
        }
    }

    if (EEPROM.read(1) != 143) {
        do {
            berhasil = getID();
            buzzAndBlink(BUZZER_PIN, LED1_PIN, LED2_PIN, 200);
        } while (!berhasil);
        for (int j = 0; j < 4; j++) {
            EEPROM.write(2 + j, baca[j]);
        }
        EEPROM.write(1, 143);
        digitalWrite(LED1_PIN, LOW);
        digitalWrite(LED2_PIN, LOW);
    }

    for (int i = 0; i < 4; i++) {
        master[i] = EEPROM.read(2 + i);
    }
}

void loop() {
    do {
        berhasil = getID();
        if (programMode) {
            ledkedip();
        } else {
            normal();
        }
    } while (!berhasil);

    if (programMode) {
        if (isMaster(baca)) {
            programMode = false;
        } else {
            if (findID(baca)) {
                deleteID(baca);
            } else {
                writeID(baca);
            }
        }
    } else {
        if (isMaster(baca)) {
            programMode = true;
        } else {
            if (findID(baca)) {
                buzzAndBlink(BUZZER_PIN, BUZZER_PIN, BUZZER_PIN, 100);
                granted(300);
            } else {
                digitalWrite(BUZZER_PIN, HIGH);
                delay(1000);
                digitalWrite(BUZZER_PIN, LOW);
                denied();
            }
        }
    }
}

void denied() {
    flashLED(LED1_PIN, -1, 3, 1000);
}

void granted(int setDelay) {
    digitalWrite(LED2_PIN, HIGH);
    digitalWrite(RELAY_PIN, !digitalRead(RELAY_PIN));
    delay(setDelay);
}

void ledkedip() {
    flashLED(BUZZER_PIN, LED1_PIN, 1, 200);
}

void normal() {
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
}

int getID() {
    if (!mfrc522.PICC_IsNewCardPresent()) {
        return 0;
    }
    if (!mfrc522.PICC_ReadCardSerial()) {
        return 0;
    }
    for (int i = 0; i < 4; i++) {
        baca[i] = mfrc522.uid.uidByte[i];
    }
    mfrc522.PICC_HaltA();
    return 1;
}

boolean isMaster(byte test[]) {
    return checkTwo(test, master);
}

void writeID(byte a[]) {
    if (!findID(a)) {
        int num = EEPROM.read(0);
        int start = (num * 4) + 6;
        num++;
        EEPROM.write(0, num);
        for (int j = 0; j < 4; j++) {
            EEPROM.write(start + j, a[j]);
        }
        successWrite();
        buzzAndBlink(BUZZER_PIN, BUZZER_PIN, BUZZER_PIN, 100);
    } else {
        failedWrite();
        digitalWrite(BUZZER_PIN, HIGH);
        delay(1000);
        digitalWrite(BUZZER_PIN, LOW);
    }
}

void successWrite() {
    flashLED(LED1_PIN, LED2_PIN, 4, 200);
}

boolean findID(byte find[]) {
    int count = EEPROM.read(0);
    for (int i = 1; i <= count; i++) {
        readID(i);
        if (checkTwo(find, sementara)) {
            return true;
        }
    }
    return false;
}

void failedWrite() {
    flashLED(LED1_PIN, LED2_PIN, 3, 200);
}

void deleteID(byte a[]) {
    if (!findID(a)) {
        failedWrite();
    } else {
        int num = EEPROM.read(0);
        int slot = findIDSLOT(a);
        int start = (slot * 4) + 2;
        int looping = ((num - slot) * 4);
        num--;
        EEPROM.write(0, num);
        for (int j = 0; j < looping; j++) {
            EEPROM.write(start + j, EEPROM.read(start + 4 + j));
        }
        for (int k = 0; k < 4; k++) {
            EEPROM.write(start + looping + k, 0);
        }
        successDelete();
        buzzAndBlink(BUZZER_PIN, BUZZER_PIN, BUZZER_PIN, 1000);
    }
}

boolean checkTwo(byte a[], byte b[]) {
    for (int k = 0; k < 4; k++) {
        if (a[k] != b[k]) {
            return false;
        }
    }
    return true;
}

void readID(int number) {
    int start = (number * 4) + 2;
    for (int i = 0; i < 4; i++) {
        sementara[i] = EEPROM.read(start + i);
    }
}

int findIDSLOT(byte find[]) {
    int count = EEPROM.read(0);
    for (int i = 1; i <= count; i++) {
        readID(i);
        if (checkTwo(find, sementara)) {
            return i;
        }
    }
    return -1;
}

void successDelete() {
    flashLED(LED1_PIN, LED2_PIN, 3, 200);
}

void ShowReaderDetails() {
    byte v = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
    if ((v == 0x00) || (v == 0xFF)) {
        while (true);
    }
}

void flashLED(int led1, int led2, int times, int delayTime) {
    for (int i = 0; i < times; i++) {
        if (led1 != -1) digitalWrite(led1, HIGH);
        if (led2 != -1) digitalWrite(led2, HIGH);
        delay(delayTime);
        if (led1 != -1) digitalWrite(led1, LOW);
        if (led2 != -1) digitalWrite(led2, LOW);
        delay(delayTime);
    }
}

void buzzAndBlink(int buzzer, int led1, int led2, int delayTime) {
    digitalWrite(buzzer, HIGH);
    delay(delayTime);
    digitalWrite(buzzer, LOW);
    if (led1 != -1) {
        digitalWrite(led1, HIGH);
        delay(delayTime);
        digitalWrite(led1, LOW);
    }
    if (led2 != -1) {
        digitalWrite(led2, HIGH);
        delay(delayTime);
        digitalWrite(led2, LOW);
    }
}