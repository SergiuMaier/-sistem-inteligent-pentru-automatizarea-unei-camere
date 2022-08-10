## Sistem inteligent pentru automatizarea unei camere

Sistemul este bazat pe un modul Arduino Uno si urmatorii senzori:

- Senzor cu ultrasunete pentru deschiderea usii
- Sesnzor de prezenta (PIR) pentru aprinderea luminii
- Senzor de temperatura (TMP 36) pentru determinarea temperaturii.

In situatia in care senzorul de prezenta sesizeaza o miscare in aria sa de acoperire va genera aprinderea becului. Daca distanta fata de senzorul de temperatura scade sub 50cm se va deschide o usa (se va comanda un servomotor, care se va deplasa in intervalul 0-90 grade). La fiecare 30 de secunde se monitorizeaza temperatura. Daca temperatura scade sub 25 de grade se inchide usa (daca nu e deja inchisa) prin comanda aceluiasi servomotor. Toate informatiile despre senzori se afiseaza pe un LCD.
