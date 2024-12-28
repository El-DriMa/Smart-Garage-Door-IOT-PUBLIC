#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <Servo.h>

// Firebase configuration
#define FIREBASE_HOST ""
#define FIREBASE_AUTH ""

// WiFi credentials
const char* ssid = "";
const char* password = "";

// Create Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Create servo motor object
Servo doorServo;

// Last known door status to prevent unnecessary updates
String lastStatus = "";
bool initialRead = true;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  // Wait for Wi-Fi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Firebase configuration
  config.api_key = FIREBASE_AUTH;
  config.database_url = FIREBASE_HOST;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Firebase authentication
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Firebase authentication successful!");
  } else {
    Serial.print("Authentication error: ");
    Serial.println(config.signer.tokens.error.message.c_str());
  }

  // Attach the servo to pin 5 (adjust pin and servo range if needed)
  doorServo.attach(5, 500, 2500);

  // Start listening to Firebase changes (door status)
  Firebase.beginStream(fbdo, "/doorStatus"); // Start stream for door status

}

void loop() {
  // Check for changes in door status from Firebase

  if (Firebase.readStream(fbdo)) {
    if (fbdo.streamAvailable()) {
      String newStatus = fbdo.stringData();
    
       if (initialRead) {
          // Skip the first read to avoid unnecessary actions
          initialRead = false;
          lastStatus = newStatus;
          
          Serial.println("Initial door status: " + newStatus);
        

          // Set the initial position of the servo based on the last known state
          if (newStatus == "OPEN") {
            doorServo.write(120);  // Set to open position without moving
          } else if (newStatus == "CLOSED") {
            doorServo.write(0);    // Set to closed position without moving
          }
        } else if (newStatus != lastStatus) {
          Serial.println("Door status updated: " + newStatus);
          lastStatus = newStatus;
        // Take action based on the new status
        if (newStatus == "OPEN") {
          openDoor();
          //openAndCloseThreeTimes();
        } else if (newStatus == "CLOSED") {
          closeDoor();
        }

        
      }
    }
  }
}

void openAndCloseThreeTimes() {
  for (int i = 0; i < 3; i++) {
    openDoor();
    delay(1000); // Adjust the delay as needed
    closeDoor();
    delay(1000); // Adjust the delay as needed
  }
}
// Function to open the door
void openDoor() {
 // doorServo.write(120);  // Open the door
  for(int i=0;i<=120;i+=1){
    doorServo.write(i);
    delay(15);
  }

  Serial.println("Vrata su otvorena");
  // Update Firebase with door status
  Firebase.setString(fbdo, "/doorStatus", "OPEN");
}


// Function to close the door
void closeDoor() {
  //doorServo.write(0);    // Close the door
  for(int i=120;i>=0;i-=1){
    doorServo.write(i);
    delay(15);
  }
  Serial.println("Vrata su zatvorena");
  // Update Firebase with door status
  Firebase.setString(fbdo, "/doorStatus", "CLOSED");
}