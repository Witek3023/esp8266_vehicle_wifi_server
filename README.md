# ESP8266 Vehicle WiFi Server

Control a vehicle from your phone over WiFi. The ESP8266 runs as a **WiFi Access Point**, serves a mobile-friendly web UI, and maps each button to an HTTP endpoint that moves the steering or drive servo.

Control panel on a phone

## Web UI overview


| Button     | Action               |
| ---------- | -------------------- |
| **LEWO**   | Steer left → 150°    |
| **PROSTO** | Straight → 90°       |
| **PRAWO**  | Steer right → 30°    |
| **PRZÓD**  | Drive forward → 180° |
| **TYL**    | Drive backward → 0°  |
| **STOP**   | Neutral / stop → 90° |


The header shows live **Servo** (steering angle) and **Motor** (drive servo position). Values update after each tap (full page reload).

## Hardware

- **ESP8266**
- RC Chassis with:
    - **2× servo**
        - Steering → **GPIO 13** (NodeMCU **D7**)
        - Drive → **GPIO 12** (NodeMCU **D6**)

## Software

- [Arduino IDE](https://www.arduino.cc/en/software)
- **NodeMCU board package** 
- Libraries:
    - **ESP8266WiFi**
    - **ESP8266WebServer**
    - **Servo** (ESP8266Servo)

## Quick start

### 1. Flash firmware

1. Open this folder in Arduino IDE.
2. Open `servo_wifi.ino`.
3. **Tools → Board** →  *NodeMCU 1.0*.
4. **Tools → Port** → your serial port.
5. Click **Upload**.

### 2. Connect and drive

1. Open **Serial Monitor** at **9600** baud — note the AP IP (usually `192.168.4.1`).
2. On your phone, join WiFi:
    - **SSID:** `autko-wifi`
    - **Password:** `12345678`
3. In the browser open: **[http://192.168.4.1](http://192.168.4.1)** *(IP may vary)*

## HTTP API


| Method | Path        | Description       |
| ------ | ----------- | ----------------- |
| GET    | `/`         | Main control page |
| GET    | `/left`     | Steer left        |
| GET    | `/right`    | Steer right       |
| GET    | `/center`   | Straight          |
| GET    | `/forward`  | Forward           |
| GET    | `/backward` | Reverse           |
| GET    | `/stop`     | Stop              |


Each handler returns **200 OK** and full HTML with current state.

## Project files


| File               | Purpose                                       |
| ------------------ | --------------------------------------------- |
| `servo_wifi.ino`   | WiFi AP, HTTP server, servo control           |
| `webpage.h`        | HTML embedded in flash (`PROGMEM`) for ESP    |
| `web.html`         | Same UI for local editing and browser preview |
| `embed_webpage.py` | Regenerates `webpage.h` from `web.html`       |
| `webui.png`        | Screenshot for this README                    |


