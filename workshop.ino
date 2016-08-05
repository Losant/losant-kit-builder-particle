/**
 * Losant Particle workshop source code for Photon.
 *
 * Copyright (c) 2016 Losant IoT. All rights reserved.
 * https://www.losant.com
 */

int buttonState = LOW;
int ledState = LOW;

/**
 * Reads the value of the photoresistor and
 * publishes the value to the particle cloud.
 */
void readPhotoResistor() {
    
    // Read the analog input.
    int value = analogRead(A0);
    
    // Publish the value to particle.
    Particle.publish("light-level", String(value));
    
    // Right it serial for debugging.
    Serial.println(value);
}

Timer timer(5000, readPhotoResistor);

/**
 * Sets the state of the LED.
 */
void setLEDState(int state) {
    digitalWrite(D0, state);
    ledState = state;
}

/**
 * Handles an event from particle cloud.
 */
void handleEvent(const char *event, const char *data) {
    
    String command = String(event);
    if(command.compareTo(String("led-on")) == 0) {
        setLEDState(HIGH);
    }
    else if(command.compareTo(String("led-off")) == 0) {
        setLEDState(LOW);
    }
    else if(command.compareTo(String("led-toggle")) == 0) {
        setLEDState(ledState == HIGH ? LOW : HIGH);
    }
}

/**
 * Called automatically by Photon once when the device
 * is powered on.
 */
void setup() {
    
    Serial.begin(9600);
    timer.start();
    
    // Button input.
    pinMode(D1, INPUT_PULLDOWN);
    
    // Power to photoresistor.
    pinMode(A4, OUTPUT);
    
    // Power to LED.
    pinMode(D0, OUTPUT);
    
    // Send power to the photoresistor.
    digitalWrite(A4, HIGH);
    
    // Subscribe to events that control the LED.
    Particle.subscribe("led-on", handleEvent);
    Particle.subscribe("led-off", handleEvent);
    Particle.subscribe("led-toggle", handleEvent);
}

/**
 * Checks for whether or not the button was pressed
 * and publishes button-pressed event to particle cloud.
 */
void checkButton() {
    int currentRead = digitalRead(D1);

    if(currentRead != buttonState) {
        buttonState = currentRead;
        if(buttonState) {
          Serial.println("Button Pressed!");
          Particle.publish("button-pressed");
        }
    }
}

/**
 * Automatically called over and over again by the Photon.
 */
void loop() {
    checkButton();
    delay(100);
}
