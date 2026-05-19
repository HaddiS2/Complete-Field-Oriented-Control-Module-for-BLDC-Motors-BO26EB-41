#include <Wire.h>
#include <SparkFun_STUSB4500.h>

#define SDA_PIN 17
#define SCL_PIN 18

STUSB4500 usb;

// ================================================================
// TEST VOLTAGES - cycles through these every second
// ================================================================
const float voltages[] = {5.0, 9.0, 15.0, 20.0};
const int numVoltages = 4;
int currentIdx = 0;

unsigned long lastSwitch = 0;
const unsigned long switchInterval = 1000;  // 1 second

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("\nSTUSB4500 Voltage Test Bench");
    Serial.println("============================");

    // ========== Init I2C and STUSB4500 ==========
    Wire.begin(SDA_PIN, SCL_PIN);
    delay(500);

    if (!usb.begin()) {
        Serial.println("Could not find STUSB4500!");
        while (1);
    }
    Serial.println("STUSB4500 connected!");

    // ========== Configure 3 PDOs ==========
    // PDO1 is FIXED at 5V (USB-PD specification requirement)
    // PDO2 and PDO3 are programmable

    usb.setPdoNumber(3);    

    // PDO1: 5V (fixed by spec, but set current)
    usb.setCurrent(1, 3.0);

    // PDO2: 9V
    usb.setVoltage(2, 9.0);
    usb.setCurrent(2, 3.0);

    // PDO3: starts at 15V (we'll change between 15V and 20V at runtime)
    usb.setVoltage(3, 15.0);
    usb.setCurrent(3, 3.0);

    usb.write();   
    usb.softReset();
    delay(100);

    Serial.println("PDOs configured:");
    Serial.printf("  PDO1: %.1fV (fixed)\n", usb.getVoltage(1));
    Serial.printf("  PDO2: %.1fV\n",        usb.getVoltage(2));
    Serial.printf("  PDO3: %.1fV\n",        usb.getVoltage(3));
    Serial.println("\nStarting voltage cycle...\n");
}

void loop() {
    if (millis() - lastSwitch >= switchInterval) {
        float targetVoltage = voltages[currentIdx];

        // ========== Select correct PDO based on voltage ==========
        if (targetVoltage == 5.0) {
            usb.setPdoNumber(1);  // PDO1 = 5V
        }
        else if (targetVoltage == 9.0) {
            usb.setPdoNumber(2);  // PDO2 = 9V
        }
        else {
            // For 15V or 20V, reprogram PDO3 and select it
            usb.setVoltage(3, targetVoltage);
            usb.setPdoNumber(3);
        }
        // Trigger renegotiation
        usb.softReset();

        uint8_t activePDO = usb.getPdoNumber();

        Serial.printf("PDO %d : %.0fV\n", activePDO, targetVoltage);
        // Move to next voltage in cycle
        currentIdx = (currentIdx + 1) % numVoltages;
        lastSwitch = millis();
    }
}