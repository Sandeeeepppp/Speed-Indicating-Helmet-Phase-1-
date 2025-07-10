#include<SoftwareSerial.h>

SoftwareSerial SIM7670Serial (2, 3); // RX, TX

const int piezoPins [4] = {A0, A1, A2, A3};
const int ledPin = 10;
const int threshold = 1000; // Define the threshold value for impact detection
int speed;

void sendATCommand(const char* cmd, const char* expectedResponse, unsigned long timeout) {
    SIM7670Serial.println(cmd);
    String response = "";
    unsigned long startTime = millis();
    bool responseOK = false;

    while (millis() startTime < timeout) {
        while (SIM7670Serial.available() > 0) {
            char c = SIM7670Serial.read();
            response += c;
        }
        if (response.indexOf (expectedResponse) != -1) {
            responseOK = true;
            break; // found it 
        }
    }
Serial.println(response);

if (responseOK){
     Serial.println("Response OK");
}else{
    Serial.println("Timeout without expected Response");
}
}

if (Serial.available() > 0) {
    speed = Serial.parseInt();
    Serial.print("Speed: ");
    Serial.print(speed);
    Serial.println(" km/h");

if (speed >= 80 && speed < 100) {
    blinkLED(1, 1000); // Blink once every second
    sendSMS ("6360286938", "the speed is 80");
} else if (speed >= 100 && speed < 120) {
    blinkLED(2, 1000); // Blink twice every second 
    sendSMS("6360286938", "the speed is 100");
} else if (speed >= 120 && speed <= 140) {
    blinkLED(3, 1000); // Blink three times every second 
    sendSMS("6360286938", "the speed is 120");
} else if (speed > 140) {
    blinkLED(4, 1000); // Blink four times every second 
    sendSMS("6360286938", "the speed is 140");
}
}

            
if (SIM7670Serial.available()){
    String response = SIM7670Serial.readString();
    Serial.println(response);
    // Check if the response indicates a call has been connected
    if (response.indexOf("CONNECT") != -1) {
       // call connected
    Serial.println("Call Connected");
    // Add code here to perform actions during the call
    }
}

void blinkLED(int blinks, int duration) {
    for (int i = 0; i < blinks; i++) {
        digitalWrite(ledPin, HIGH);
        delay(duration / (2 * blinks));
        digitalWrite(ledPin, LOW);
        delay(duration / (2 * blinks));
    }
}


void setup(){
    Serial.begin(960000);
    Serial.begin(115200);

SIM7670Serial.begin(115200);
sendATCommand("AT+CGMM", "OK", 1000); // check communication
sendATCommand("AT+CMGF=1", "OK", 1000); // set SMS format to text
// Set the LED pin as output
pinMode(ledPin, OUTPUT);

// Display initial message on the serial monitor
Serial.println("Helmet Impact Detection System Initialized");
}

void sendSMS (String number, String message) {
    String cmd = "AT+CMGS=\"" + number + "\"\r\n";
    SIM7670Serial.print(cmd);
    delay(100);
    SIM7670Serial.println(message);
    delay(100);
    SIM7670Serial.write(0x1A); // send ctrl-z
    delay(100);
}


void loop() {
    for (int i = 0; i < 4; i++) {
        int sensorValue = analogRead(piezoPins[i]);
        if (sensorValue > threshold) { // Compare sensor value with the threshold
            Serial.print("Impact Detected on Sensor: ");
            Serial.println(i + 1);
            Serial.println(sensorValue);
            sendSMS ("6360286938", "Accident Detected");

SIM7670Serial.println("ATD +916360286938;");
delay(10000);
}
}
}