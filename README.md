## Sistem inteligent pentru automatizarea unei camere

Sistemul este bazat pe un modul Arduino Uno și următorii senzori:

- Senzor cu ultrasunete pentru deschiderea ușii
- Senzor de prezenta (PIR) pentru aprinderea luminii
- Senzor de temperatura (TMP 36) pentru determinarea temperaturii

În situatia în care senzorul de prezență sesizează o mișcare în aria sa de acoperire, va genera aprinderea becului. Dacă distanța față de senzorul de temperatură scade sub 50cm se va deschide o ușa (este comandat un servomotor, care se va deplasa în intervalul 0-90 grade). La fiecare 30 de secunde se monitorizeaza temperatura. Dacă temperatura scade sub 25 de grade se închide ușa (dacă nu e deja închisă) prin comanda aceluiași servomotor. Toate informațiile despre senzori sunt afișate pe un LCD.
