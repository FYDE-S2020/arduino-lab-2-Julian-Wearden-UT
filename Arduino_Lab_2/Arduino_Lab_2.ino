/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/
#define LED_BUILTIN 2
#define LED 4

#include <Blynk.h>
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>


BlynkTimer timer;

const int freq = 5000;
const int ledChannel = 0;
const int resolution = 10;

char auth[] = "eeCWpgqqEdUeJNEt-zi1KdcHXa9cTHoB";
char ssid[32] = "utguest";
char pass[32] = "";

int time_count = 0; // timer counter global variable
String content = "";  // null string constant ( an empty string )

void myTimerEvent() // Every 10 ms
{
    if (time_count == 100){
        Blynk.virtualWrite(V0, millis() / 1000); // Do thing that needs to happen every 1 second
        time_count = 0; // reset time counter
    }
    else {
        // Send serial data to Blynk terminal
        char character;
        while(Serial.available()) { // Check if serial is available every 10 ms
            character = Serial.read();
            content.concat(character);
        }
        if (content != "") {
            Blynk.virtualWrite(V5, content);
            content = ""; // Clear String
        }  
    }
    time_count += 1; // Increment on every tick
}


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  pinMode(LED_BUILTIN, OUTPUT);
  
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(LED, ledChannel);

  timer.setInterval(10L, myTimerEvent);
}

// the loop function runs over and over again forever
void loop() {
    Blynk.run();
    timer.run();
}

BLYNK_WRITE(V1)
{

    // param is a member variable of the Blynk ADT. It is exposed so you can read it.
    int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable

    // Because V1 is a button, pinValue will be a 0 or a 1.
    if (pinValue == 0) {
          digitalWrite(LED_BUILTIN, LOW);    // turn LED off
    }
    else {
         digitalWrite(LED_BUILTIN, HIGH); // turn LED on
    }
}

BLYNK_WRITE(V2)
{

    // param is a member variable of the Blynk ADT. It is exposed so you can read it.
    int val = param.asInt(); // assigning incoming value from pin V1 to a variable
    ledcWrite(ledChannel, val);
}
