#include <Adafruit_Fingerprint.h> // Library for fingerprint sensor
#include <Wire.h>                 // Library for I2C communication
#include <LiquidCrystal_I2C.h>    // Library for I2C LCD
#include <Servo.h>                // Library for servo motor

// Initialize LCD (I2C address, columns, rows)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Initialize servo motor
Servo doorLock;

// Initialize fingerprint sensor
SoftwareSerial mySerial(2, 3); // RX, TX
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup() {
  // Start serial communication
  Serial.begin(9600);

  // Initialize LCD
  lcd.begin();
  lcd.backlight();
  lcd.print("Hello Engineer Joe"); // Initial greeting message
  delay(2000); // Display for 2 seconds
  lcd.clear();
  lcd.print("Fingerprint Door");
  lcd.setCursor(0, 1);
  lcd.print("   Lock System   ");
  delay(2000);
  lcd.clear();

  // Initialize servo motor
  doorLock.attach(9);
  doorLock.write(0); // Lock the door initially

  // Initialize fingerprint sensor
  finger.begin(57600);
  if (finger.verifyPassword()) {
    lcd.print("Fingerprint OK");
    delay(1000);
    lcd.clear();
  } else {
    lcd.print("Sensor Error");
    while (1);
  }
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("Place Finger");

  // Check for fingerprint
  int fingerprintID = getFingerprintID();
  if (fingerprintID != -1) {
    lcd.clear();
    lcd.print("Access Granted");
    lcd.setCursor(0, 1);
    lcd.print("Welcome Eng. Joe"); // Welcome message
    unlockDoor();
    delay(3000); // Keep door unlocked for 3 seconds
    lockDoor();
    lcd.clear();
  } else {
    lcd.clear();
    lcd.print("Access Denied");
    delay(2000);
    lcd.clear();
  }
}

// Function to unlock the door
void unlockDoor() {
  doorLock.write(90); // Rotate servo to 90 degrees (unlock)
}

// Function to lock the door
void lockDoor() {
  doorLock.write(0); // Rotate servo to 0 degrees (lock)
}

// Function to get fingerprint ID
int getFingerprintID() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) return -1;

  return finger.fingerID; // Return the fingerprint ID
}