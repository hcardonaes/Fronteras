/*
 Name:		Fronteras.ino
 Created:	06/04/2024 19:32:48
 Author:	Ofel
*/

#include <Arduino.h>

struct Point {
	float x, y;
};

struct Motor {
	int pin1, pin2, pin3, pin4;
	unsigned long stepInterval = 1100; // Intervalo entre pasos en microsegundos
	int stepsRemainingInicial; // Pasos a dar hasta la proyección
	int stepsRemainingFinal; // Pasos a dar desde la proyección hasta el destino
	int stepsRemainingTotal; // Pasos totales a dar
	bool direction; // true para adelante, false para atrás
	unsigned long lastStepTime; // Último momento en que se dio un paso
	int stepCount; // Controlar la posición actual en la secuencia de medio paso
	int esquina;
	double  mmAbase; // Distancia en mm a la base
	double  mmAdestino; // Distancia en mm al destino
	Motor(int p1, int p2, int p3, int p4, int esq) : pin1(p1), pin2(p2), pin3(p3), pin4(p4), esquina(esq) {}

};

const int motorSteps[8][4] = {
  {HIGH, LOW, LOW, LOW}, {HIGH, HIGH, LOW, LOW}, {LOW, HIGH, LOW, LOW}, {LOW, HIGH, HIGH, LOW},
  {LOW, LOW, HIGH, LOW}, {LOW, LOW, HIGH, HIGH}, {LOW, LOW, LOW, HIGH}, {HIGH, LOW, LOW, HIGH}
};

Motor motor1 = { 8, 9, 10, 11, 0 };
Motor motor2 = { 4, 5, 6, 7, 1 };
Motor motor3 = { 24,26,28,30, 2 };
Motor motor4 = { 40,42,44, 46, 3 };

Motor* motors[4] = { &motor1, &motor2, &motor3, &motor4 };

Point posActual;
Point posDestino;
Point esquinas[4]{ (0,0), (0,560), (560,560), (560,0) };

enum Orientacion { NORTE, NORESTE, ESTE, SURESTE, SUR, SUROESTE, OESTE, NOROESTE };
const int ladoCasilla = 70;

void setupMotor(Motor& motor) {
	pinMode(motor.pin1, OUTPUT);
	pinMode(motor.pin2, OUTPUT);
	pinMode(motor.pin3, OUTPUT);
	pinMode(motor.pin4, OUTPUT);
}

// Función para calcular la distancia
float calcularDistancia(float centroX, float centroY, float esquinaX, float esquinaY) {
	return sqrt(pow(centroX - esquinaX, 2) + pow(centroY - esquinaY, 2));
}

float medirDistancia(Point centro, Point esquina) {
	Point cartesianCenter = { centro.x * 70 + 35, centro.y * 70 + 35 };
	///////////////////////////////////////////////////////////////////////////////////////////
	return sqrt(pow(centro.x - esquina.x, 2) + pow(centro.y - esquina.y, 2));
}

int determinarOrientacion(Point posicionActual, Point posicionDestino) {
	int difCol = posicionDestino.x - posicionActual.x;
	int difFila = posicionDestino.y - posicionActual.y;

	// Determinar la orientación basada en las diferencias
	if (difFila > 0 && difCol == 0) return 1;
	if (difFila > 0 && difCol > 0) return 2;
	if (difFila == 0 && difCol > 0) return 3;
	if (difFila < 0 && difCol > 0) return 4;
	if (difFila < 0 && difCol == 0) return 5;
	if (difFila < 0 && difCol < 0) return 6;
	if (difFila == 0 && difCol < 0) return 7;
	if (difFila > 0 && difCol < 0) return 8;
}

void sacaDeSusCasillas(Point posicionActual) {
	int sacar = 35;

}

void setup() {
	Serial.begin(9600);
	Serial.println("Iniciando...");
	for (int i = 0; i < 4; i++) {
		setupMotor(*motors[i]);
	}
}

void loop() {
	if (Serial.available() > 0)
	{
		String command = Serial.readStringUntil('\n');
		command.trim();
		Serial.println("Comando recibido: " + String(command));

		posActual.x = command.charAt(0) - 'a';
		posActual.y = command.charAt(1) - '0';

		posDestino.x = command.charAt(2) - 'a';
		posDestino.y = command.charAt(3) - '0';


		int orientacion = determinarOrientacion(posActual, posDestino);
		switch (orientacion-1)
		{
		case NORTE:
			Serial.println("Norte");
			for (int i = 0; i < 4; i++)
			{
				bool bordeIzq = posActual.x < 4 ? 1 : 0;
				// calcula diferencia de longitudes entre centro de la casilla y el borde izquierdo para cada motor
				motors[i]->mmAbase = (4 - posActual.x) * ladoCasilla + bordeIzq * (ladoCasilla / 2);
			}
			break;
		case NORESTE:
			Serial.println("Noreste");
			break;
		case ESTE:
			Serial.println("Este");
			break;
		case SURESTE:
			Serial.println("Sureste");
			break;
		case SUR:
			Serial.println("Sur");
			break;
		case SUROESTE:
			Serial.println("Suroeste");
			break;
		case OESTE:
			Serial.println("Oeste");
			break;
		case NOROESTE:
			Serial.println("Noroeste");
			break;

		default:
			break;
		}
	}
}


