
# Smart Door Control with ESP8266 and Firebase

A simple IoT project that uses an **ESP8266** microcontroller to control a door via a **Firebase Realtime Database**. The system allows real-time monitoring and control of the door status (`OPEN` or `CLOSED`) and moves a servo motor accordingly.

## How It Works
1. The **ESP8266** connects to a Wi-Fi network and listens for changes in the Firebase Realtime Database at the `/doorStatus` path.
2. When the door status in Firebase changes:
   - If `OPEN`, the servo motor rotates to open the door.
   - If `CLOSED`, the servo motor rotates to close the door.
3. The system updates the database to confirm the door's new status.

## Features
- Real-time door status updates via Firebase.
- Remote control of the door using a database.
- Simple Wi-Fi setup with ESP8266.
- Smooth operation of a servo motor.

