#include <Arduino.h>
#include <Servo.h>

// Ultrasonic pins
const int TRIG_PIN = 2;
const int ECHO_PIN = 7;
const int SERVO_PIN = 8;

// PID konstante
double kp = 8.6;
double ki = 1.1;
double kd = 6.3;

// PID spremenljivke
double integral = 0;
double previousError = 0;
unsigned long lastTime = 0;

// Razdalja k jo uporabnik nastav
double setpoint = 0;

Servo servo;

double computePID(double error, double dt);
float measureDistance();
double readSetpoint();

void setup()
{
    Serial.begin(9600);

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    // TUKAJ se nastavi pozicijo palce da je rauna
    servo.attach(SERVO_PIN);
    servo.write(90);
    delay(1000);

    setpoint = readSetpoint();

    lastTime = millis();
}

void loop()
{
    // Doloci dt
    unsigned long now = millis();
    double dt = (now - lastTime) / 1000.0;
    lastTime = now;

    // Izmeri taenutno razdaljo (cm)
    float currentDistance = measureDistance();

    // Tuki se doloca napaka za PID loop
    double error = setpoint - currentDistance;

    // PID funkcija
    double pidOutput = computePID(error, dt);

    // Uporabimo deadband: če je napaka zelo majhna, prisilimo PID izhod na 0
    // (resetiramo tudi integral), da servo ostane na 90°.
    if (fabs(error) < 0.3)
    { // prag lahko prilagodiš po potrebi
        pidOutput = 0;
        integral = 0;
    }

    // Preslikava PID izhoda na kot servo motorja:
    //  - Pri pidOutput = 0 je kot 90° (palica ravna).
    //  - Če je napaka pozitivna (avtomobilček je preblizu ali pa je meritev manjša od setpointa),
    //    se pidOutput > 0 in izraz 90 - pidOutput < 90, kar pomeni, da se palica spusti.
    //  - Če je napaka negativna (avtomobilček je predaleč), se 90 - pidOutput > 90, torej se palica dvigne.
    int servoAngle = 90 - (int)pidOutput;

    // Nastavi servo kot med 0 in 180 stopinj
    servoAngle = constrain(servoAngle, 0, 180);

    // pramakne servo motor
    servo.write(servoAngle);

    Serial.print("Setpoint: ");
    Serial.print(setpoint);
    Serial.print(" cm, Meritev: ");
    Serial.print(currentDistance);
    Serial.print(" cm, Napaka: ");
    Serial.print(error);
    Serial.print(", PID izhod: ");
    Serial.print(pidOutput);
    Serial.print(", Servo kot: ");
    Serial.println(servoAngle);

    delay(50);
}

double computePID(double error, double dt)
{
    // Proporcija
    double P = kp * error;

    // Integral
    integral += error * dt;
    double I = ki * integral;

    // Diferantial
    double D = kd * (error - previousError) / dt;
    previousError = error;

    return P + I + D;
}

float measureDistance()
{
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Impulzivno merjenje (30 milisekund tajmouta)
    long duration = pulseIn(ECHO_PIN, HIGH, 30000);

    // Pretvorimo čas (zvoka k potuje) v razdaljo v cm (faktor 58.2 temelji na hitrsoti zvoka v zraku)
    float distance = duration / 58.2;

    // Omejimo razdaljo na dolzino palice
    if (distance > 40)
        distance = 40;

    return distance;
}

double readSetpoint()
{
    double targetDistance = 0;

    Serial.println("Vnesi zeljeno razdaljo v cm:");
    while (Serial.available() == 0)
    {
    }

    String input = Serial.readStringUntil('\n');
    targetDistance = input.toFloat();
    // Omejimo razdaljo na dolzino palice
    if (targetDistance < 0)
        targetDistance = 0;
    if (targetDistance > 42)
        targetDistance = 42;

    Serial.print("Nastavljena razdalja: ");
    Serial.print(targetDistance);
    Serial.println(" cm");

    return targetDistance;
}
