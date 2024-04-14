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
	int stepsRemainingHastaBorde; // Pasos a dar hasta el borde de la casilla
	int stepsRemainingDesdeBorde; // Pasos a dar desde el borde de la casilla hasta el centro
	int stepsRemainingAbcisaa; // Pasos a dar en la abscisa
	int stepsRemainingOrdenada; // Pasos a dar en la ordenada
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


float medirDistancia(Point centro, Point esquina) {
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
		Point cartesianCenter = { posActual.x * 70 + 35, posActual.y * 70 + 35 };
		Point bordeSup = { cartesianCenter.x, cartesianCenter.y + 35 };
		Point bordeInf = { cartesianCenter.x, cartesianCenter.y - 35 };
		Point bordeDer = { cartesianCenter.x + 35, cartesianCenter.y };
		Point bordeIzq = { cartesianCenter.x - 35, cartesianCenter.y };

		switch (orientacion-1)
		{
		case NORTE:
			Seri
				al.println("Norte");

			for (int i = 0; i < 4; i++)
			{
				float distCentro = medirDistancia(cartesianCenter, esquinas[i]);
				float distBorde = medirDistancia(bordeSup, esquinas[i]);
				motors[i]->stepsRemainingHastaBorde = distCentro - distBorde;
				Serial.println("Distancia centro: " + String(distCentro) + 
							  " Distancia borde: " + String(distBorde));
				Serial.println("Pasos iniciales: " + String(motors[i]->stepsRemainingHastaBorde));
				float abcisaEntreCasillas = medirDistancia(bordeSup, bordeInf);////////////////////////////
				float ordenadaEntreCasillas = posDestino.y - posActual.y;
				motors[i]->stepsRemainingAbcisaa = abcisaEntreCasillas * 70;
				motors[i]->stepsRemainingOrdenada = ordenadaEntreCasillas * 70;

			}
			break;

		case NORESTE:
			Serial.println("Noreste");
			for (int i = 0; i < 4; i++)
			{
				float distCentro = medirDistancia(cartesianCenter, esquinas[i]);
				float distBorde = medirDistancia(bordeDer, esquinas[i]);
				motors[i]->stepsRemainingHastaBorde = distCentro - distBorde;
				Serial.println("Distancia centro: " + String(distCentro) +
							  " Distancia borde: " + String(distBorde));
				Serial.println("Pasos iniciales: " + String(motors[i]->stepsRemainingHastaBorde));
			}
			break;

		case ESTE:
			Serial.println("Este");
			for (int i = 0; i < 4; i++)
			{
				int distCentro = medirDistancia(cartesianCenter, esquinas[i]);
				int distBorde = medirDistancia(bordeDer, esquinas[i]);
				motors[i]->stepsRemainingHastaBorde = distCentro - distBorde;
				Serial.println("Distancia centro: " + String(distCentro) +
							  " Distancia borde: " + String(distBorde));
				Serial.println("Pasos iniciales: " + String(motors[i]->stepsRemainingHastaBorde));
			}
			break;

		case SURESTE:
			Serial.println("Sureste");
			for (int i = 0; i < 4; i++)
			{
				int distCentro = medirDistancia(cartesianCenter, esquinas[i]);
				int distBorde = medirDistancia(bordeDer, esquinas[i]);
				motors[i]->stepsRemainingHastaBorde = distCentro - distBorde;
				Serial.println("Distancia centro: " + String(distCentro) +
							  " Distancia borde: " + String(distBorde));
				Serial.println("Pasos iniciales: " + String(motors[i]->stepsRemainingHastaBorde));
			}
			break;

		case SUR:
			Serial.println("Sur");
			for (int i = 0; i < 4; i++)
			{
				int distCentro = medirDistancia(cartesianCenter, esquinas[i]);
				int distBorde = medirDistancia(bordeInf, esquinas[i]);
				motors[i]->stepsRemainingHastaBorde = distCentro - distBorde;
				Serial.println("Distancia centro: " + String(distCentro) +
							  " Distancia borde: " + String(distBorde));
				Serial.println("Pasos iniciales: " + String(motors[i]->stepsRemainingHastaBorde));
			}
			break;

		case SUROESTE:
			Serial.println("Suroeste");
			for (int i = 0; i < 4; i++)
			{
				int distCentro = medirDistancia(cartesianCenter, esquinas[i]);
				int distBorde = medirDistancia(bordeIzq, esquinas[i]);
				motors[i]->stepsRemainingHastaBorde = distCentro - distBorde;
				Serial.println("Distancia centro: " + String(distCentro) +
							  " Distancia borde: " + String(distBorde));
				Serial.println("Pasos iniciales: " + String(motors[i]->stepsRemainingHastaBorde));
			}
			break;

		case OESTE:
			Serial.println("Oeste");
			for (int i = 0; i < 4; i++)
			{
				int distCentro = medirDistancia(cartesianCenter, esquinas[i]);
				int distBorde = medirDistancia(bordeIzq, esquinas[i]);
				motors[i]->stepsRemainingHastaBorde = distCentro - distBorde;
				Serial.println("Distancia centro: " + String(distCentro) +
							  " Distancia borde: " + String(distBorde));
				Serial.println("Pasos iniciales: " + String(motors[i]->stepsRemainingHastaBorde));
			}
			break;

		case NOROESTE:
			Serial.println("Noroeste");
			for (int i = 0; i < 4; i++)
			{
				int distCentro = medirDistancia(cartesianCenter, esquinas[i]);
				int distBorde = medirDistancia(bordeIzq, esquinas[i]);
				motors[i]->stepsRemainingHastaBorde = distCentro - distBorde;
				Serial.println("Distancia centro: " + String(distCentro) +
							  " Distancia borde: " + String(distBorde));
				Serial.println("Pasos iniciales: " + String(motors[i]->stepsRemainingHastaBorde));
			}
			break;

		default:
			Serial.println("Orientación no válida");
			break;
		}
	}
}


