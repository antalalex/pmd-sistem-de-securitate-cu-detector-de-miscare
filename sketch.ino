#include <LiquidCrystal.h>

// LCD: RS, E, D4, D5, D6, D7
// Conform legăturilor tale: RS=D12, E=D11, D4=D5, D5=D4, D6=D3, D7=D6
LiquidCrystal lcd(12, 11, 5, 4, 3, 6);

// PIR + LED + Buzzer
const int pirPin = 2;      // PIR OUT -> D2
const int ledPin = 13;     // LED A -> D13, LED C -> GND
const int buzzerPin = 8;   // BZ1 2 -> D8, BZ1 1 -> GND (sau invers, dar așa e ok)

bool alarmaActiva = false;
unsigned long lastBeep = 0;
const unsigned long beepInterval = 700; // cât de des bipăie când e mișcare

void afisareFaraMiscare() {
  lcd.setCursor(0, 0);
  lcd.print("Sistem alarma! ");
  lcd.setCursor(0, 1);
  lcd.print("Fara miscare    ");
}

void afisareMiscare() {
  lcd.setCursor(0, 0);
  lcd.print("MOTION DETECTED ");
  lcd.setCursor(0, 1);
  lcd.print("ALARMA PORNITA  ");
}

void setup() {
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  lcd.begin(16, 2);
  lcd.clear();

  digitalWrite(ledPin, LOW);
  noTone(buzzerPin);

  afisareFaraMiscare();
}

void loop() {
  int miscare = digitalRead(pirPin);

  if (miscare == HIGH) {
    // mișcare detectată
    digitalWrite(ledPin, HIGH);
    if (!alarmaActiva) {
      alarmaActiva = true;
      afisareMiscare();
    }

    // bip periodic (ca să nu fie tone continuu)
    unsigned long now = millis();
    if (now - lastBeep >= beepInterval) {
      lastBeep = now;
      tone(buzzerPin, 1000, 200); // 1000 Hz, 200ms
    }

  } else {
    // fără mișcare
    digitalWrite(ledPin, LOW);
    noTone(buzzerPin);

    if (alarmaActiva) {
      alarmaActiva = false;
      afisareFaraMiscare();
    }
  }

  delay(50);
}
