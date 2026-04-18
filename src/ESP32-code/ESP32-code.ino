#include <esp_task_wdt.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <BleKeyboard.h>

/* ================= CONFIG ================= */

// WiFi credentials (set your own)
char ssid[] = "YOUR_WIFI_SSID";
char pass[] = "YOUR_WIFI_PASSWORD";

// Blynk configuration (set your own)
#define BLYNK_TEMPLATE_ID   "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "YOUR_TEMPLATE_NAME"
#define BLYNK_AUTH_TOKEN    "YOUR_AUTH_TOKEN"

#define BLYNK_PRINT Serial

// Watchdog configuration
const esp_task_wdt_config_t wdtConfig = {
    .timeout_ms = 10000,
    .trigger_panic = true
};

/* ================= HARDWARE ================= */

const int RELAY_PIN = 27;
const int HEADPHONES_PIN = 13;

/* ================= GLOBALS ================= */

BleKeyboard bleKeyboard;

bool headphonesState;
bool prevHeadphonesState = true;
bool stateChanged = false;

unsigned long changeTimestamp = 0;
unsigned long currentTime;

/* ================= BLYNK HANDLERS ================= */

// PC Power Control
BLYNK_WRITE(V0) {
    int value = param.asInt();
    Serial.printf("PC Power: %d\n", value);

    digitalWrite(RELAY_PIN, value ? LOW : HIGH);
}

// Audio Output Control
BLYNK_WRITE(V1) {
    int value = param.asInt();
    Serial.printf("Audio Control: %d\n", value);

    if (!bleKeyboard.isConnected()) {
        Serial.println("BLE not connected");
        return;
    }

    if (value) {
        // Headphones
        bleKeyboard.press(KEY_LEFT_CTRL);
        bleKeyboard.press(KEY_F7);
    } else {
        // Speakers
        bleKeyboard.press(KEY_LEFT_CTRL);
        bleKeyboard.press(KEY_F8);
    }

    bleKeyboard.releaseAll();
}

// Example system command (CTRL + ALT + DEL -> Power OFF)
BLYNK_WRITE(V2) {
    if (!bleKeyboard.isConnected()) {
        Serial.println("BLE not connected");
        return;
    }

    if (!param.asInt()) {
        bleKeyboard.press(KEY_LEFT_CTRL);
        bleKeyboard.press(KEY_LEFT_ALT);
        bleKeyboard.press(KEY_DELETE);
        bleKeyboard.releaseAll();

        delay(10);

        for (int i = 0; i < 7; i++) {
            bleKeyboard.write(KEY_TAB);
            delay(2);
        }

        bleKeyboard.write(KEY_NUM_ENTER);
    }
}

/* ================= SETUP ================= */

void setup() {
    Serial.begin(115200);

    pinMode(RELAY_PIN, OUTPUT_OPEN_DRAIN);
    pinMode(HEADPHONES_PIN, INPUT_PULLUP);

    digitalWrite(RELAY_PIN, HIGH);

    bleKeyboard.begin();
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);

    esp_task_wdt_init(&wdtConfig);
    esp_task_wdt_add(NULL);
}

/* ================= LOOP ================= */

void loop() {
    esp_task_wdt_reset();
    Blynk.run();

    currentTime = millis();
    headphonesState = digitalRead(HEADPHONES_PIN);

    // Detect state change
    if (headphonesState != prevHeadphonesState) {
        changeTimestamp = currentTime;
        stateChanged = true;
    }

    // Debounce / delay logic
    if ((currentTime - changeTimestamp > 500) && stateChanged) {
        if (bleKeyboard.isConnected()) {
            if (headphonesState) {
                // Headphones
                bleKeyboard.press(KEY_LEFT_CTRL);
                bleKeyboard.press(KEY_F7);
                Serial.println("Switched to headphones");
            } else {
                // Speakers
                bleKeyboard.press(KEY_LEFT_CTRL);
                bleKeyboard.press(KEY_F8);
                Serial.println("Switched to speakers");
            }

            bleKeyboard.releaseAll();
        } else {
            Serial.println("BLE not connected");
        }

        stateChanged = false;
    }

    prevHeadphonesState = headphonesState;
}