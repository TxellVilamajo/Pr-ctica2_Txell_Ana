#include <Arduino.h>

#define LED_PIN 2               // Pin del LED
#define BUTTON_UP_PIN 4         // Pin del botó per augmentar la freqüència
#define BUTTON_DOWN_PIN 5       // Pin del botó per disminuir la freqüència

hw_timer_t *timer = NULL;       // Temporitzador de maquinari
volatile int blinkInterval = 500; // Interval inicial de parpelleig (ms)
volatile bool ledState = false;  // Estat del LED

// Variables per al filtratge de rebots dels botons
volatile unsigned long lastButtonPressUp = 0;
volatile unsigned long lastButtonPressDown = 0;
const unsigned long debounceDelay = 50; // Temps de debounce en ms

void IRAM_ATTR onTimer() {
    unsigned long currentTime = millis();

    // Filtratge de rebots del botó UP
    if (digitalRead(BUTTON_UP_PIN) == LOW && (currentTime - lastButtonPressUp) > debounceDelay) {
        lastButtonPressUp = currentTime;
        blinkInterval = max(100, blinkInterval - 100); // Redueix l'interval (augmenta la freqüència)
    }

    // Filtratge de rebots del botó DOWN
    if (digitalRead(BUTTON_DOWN_PIN) == LOW && (currentTime - lastButtonPressDown) > debounceDelay) {
        lastButtonPressDown = currentTime;
        blinkInterval = min(1000, blinkInterval + 100); // Augmenta l'interval (disminueix la freqüència)
    }

    // Canvi d'estat del LED
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
    
    // Reinicialitza el temporitzador amb el nou interval
    timerAlarmWrite(timer, blinkInterval * 1000, true);
}

void setup() {
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
    pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);

    // Configuració del temporitzador
    timer = timerBegin(0, 80, true); // Timer 0, prescaler 80 → 1 tick = 1µs
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmWrite(timer, blinkInterval * 1000, true);
    timerAlarmEnable(timer);
}

void loop() {
    // No cal fer res en el loop, tot està controlat pel temporitzador
}
