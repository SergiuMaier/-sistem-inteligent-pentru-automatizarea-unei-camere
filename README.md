# Intelligent Room Automation System

💡 The system is based on an Arduino Uno module and the following sensors:

- **Ultrasonic sensor** for door opening  
- **PIR motion sensor** for turning on the light  
- **TMP 36 temperature sensor** for measuring temperature  

---

### Functionality

- When the **PIR motion sensor** detects movement within its coverage area, it triggers the light to turn on.  
- If the distance to the **ultrasonic sensor** decreases below 50 cm, a door is opened by controlling a servo motor, which moves within a range of 0-90 degrees.  
- Every **30 seconds**, the system monitors the temperature:  
  - If the temperature drops below **25°C**, the door is closed (if it isn’t already) by commanding the same servo motor.  

All sensor data is displayed on an **LCD screen**.  

