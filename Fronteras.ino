/*
 Name:		Fronteras.ino
 Created:	06/04/2024 19:32:48
 Author:	Ofel
*/

#include <Arduino.h>

struct Point {
	float x, y;
};

Point posActual;
Point posDestino;

enum Orientacion { NORTE, NORESTE, ESTE, SURESTE, SUR, SUROESTE, OESTE, NOROESTE };

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


		//determinarOrientacion(posActual, posDestino);{
		int orientacion = determinarOrientacion(posActual, posDestino);
		//Serial.println("Orientacion: " + String(orientacion));
		//}
		switch (orientacion-1)
		{
		case NORTE:
			Serial.println("Norte");
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


