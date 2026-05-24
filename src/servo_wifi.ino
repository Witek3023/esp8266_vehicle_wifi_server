#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>
#include "webpage.h"

// GPIO pins (NodeMCU labels: D7 = GPIO 13, D6 = GPIO 12)
#define SERVO_PIN 13   // steering servo signal
#define MOTOR_PIN 12   // drive servo signal (ESC-style throttle)

// WiFi Access Point credentials (phone connects directly to ESP)
const char* ap_ssid = "autko-wifi";
const char* ap_password = "12345678";  // min. 8 characters for ESP8266 AP

ESP8266WebServer server(80);

Servo steeringServo;
Servo moveServo;

// Mirrored on the web UI after each request ({{SERVO_POS}}, {{MOTOR_PWM}})
int servoPos = 90;
int motorSpeed = 90;

String webpage();

// HTTP route handlers (must be declared before setup on some toolchains)
void handleRoot();
void handleLeft();
void handleRight();
void handleCenter();
void handleForward();
void handleBackward();
void handleStop();

void setup()
{
    Serial.begin(9600);

    steeringServo.attach(SERVO_PIN);
    steeringServo.write(90);

    moveServo.attach(MOTOR_PIN);
    moveServo.write(90);

    // ESP acts as its own WiFi hotspot; clients use http://192.168.4.1 (typical)
    WiFi.softAP(ap_ssid, ap_password);
    Serial.println(WiFi.softAPIP());

    server.on("/", handleRoot);
    server.on("/left", handleLeft);
    server.on("/right", handleRight);
    server.on("/center", handleCenter);
    server.on("/forward", handleForward);
    server.on("/backward", handleBackward);
    server.on("/stop", handleStop);

    server.begin();
    Serial.println("SERVER START");
}

void loop()
{
    // Processes one HTTP client per call
    server.handleClient();
}

// --- Actuators ---

void setServo(int angle)
{
    steeringServo.write(angle);
     servoPos = angle;
}

void setMotor(int speedValue)
{
    // Second servo used as throttle (0° / 90° stop / 180°)
    moveServo.write(speedValue);
    motorSpeed = speedValue;
}

// --- Steering handlers ---

void handleLeft()
{
    setServo(150);
    server.send(200, "text/html", webpage());
}

void handleRight()
{
    setServo(30);
    server.send(200, "text/html", webpage());
}

void handleCenter()
{
    setServo(90);
    server.send(200, "text/html", webpage());
}

// --- Drive handlers ---

void handleForward()
{
    setMotor(180);
    server.send(200, "text/html", webpage());
}

void handleBackward()
{
    setMotor(0);
    server.send(200, "text/html", webpage());
}

void handleStop()
{
    setMotor(90);
    server.send(200, "text/html", webpage());
}

void handleRoot()
{
    server.send(200, "text/html", webpage());
}

// --- Web UI ---

String webpage()
{
    // FPSTR reads WEBPAGE_HTML from flash (PROGMEM) instead of RAM
    String html = FPSTR(WEBPAGE_HTML);

    // Simple template placeholders filled with last known state
    html.replace("{{SERVO_POS}}", String(servoPos));
    html.replace("{{MOTOR_PWM}}", String(motorSpeed));

    return html;
}
