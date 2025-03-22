#include <Arduino.h>

#define LED_PIN 2               // Pin del LED (simulat)
#define BUTTON_UP 'u'           // Comanda per augmentar la freqüència
#define BUTTON_DOWN 'd'         // Comanda per disminuir la freqüència

hw_timer_t *timer = NULL;       // Temporitzador de maquinari
volatile int blinkInterval = 500; // Interval inicial de parpelleig (ms)
volatile bool ledState = false;  // Estat del LED

void IRAM_ATTR onTimer() {
    // Simula el canvi d'estat del LED
    ledState = !ledState;
    Serial.print("LED: ");
    Serial.println(ledState ? "ON" : "OFF");

    // Reinicialitza el temporitzador amb el nou interval
    timerAlarmWrite(timer, blinkInterval * 1000, true);
}

void setup() {
    Serial.begin(115200);  // Inicialitza el monitor sèrie
    Serial.println("Prem 'u' per augmentar la velocitat o 'd' per reduir-la");

    // Configuració del temporitzador
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmWrite(timer, blinkInterval * 1000, true);
    timerAlarmEnable(timer);
}

void loop() {
    if (Serial.available()) {  // Comprova si hi ha dades del monitor sèrie
        char input = Serial.read();  // Llegeix el caràcter introduït

        if (input == BUTTON_UP) {
            blinkInterval = max(100, blinkInterval - 100);
            Serial.print("Freqüència augmentada: ");
        } 
        else if (input == BUTTON_DOWN) {
            blinkInterval = min(1000, blinkInterval + 100);
            Serial.print("Freqüència reduïda: ");
        }

        Serial.print(blinkInterval);
        Serial.println(" ms");
    }
}
