#include "navier.h"

navier* nsInit(int rows, int columns) {
	meshPrepare();

	navier* aux = (navier*)malloc(sizeof(navier));
	aux->previous = meshInit(rows, columns);
	aux->current = meshInit(rows, columns);
	aux->time = 0.1;
	aux->viscosity = 0.001003;
	aux->speed = 1.0;
	aux->distance = 1.0;
	aux->force = 1.0;
	aux->hasCallback = 0;

	return aux;
}

void nsFree(navier* n) {
	meshFree(n->previous);
	meshFree(n->current);
	free(n);
}

int nsAssignMeshes(navier* n, mesh* m1, mesh* m2) {
	if (n == NULL)
		return -1;

	n->previous = m1;
	n->current = m2;

	return 0;
}

void nsSwapMeshes(navier* n) {
	mesh* aux;
	aux = n->previous;
	n->previous = n->current;
	n->current = aux;
}

int nsCalculate(navier* n) {
	double t, c, d, u, z, aux;
	int rows, columns;

	if (n != NULL) {
		t = n->time;
		c = n->speed;
		d = n->distance;
		u = n->viscosity * n->force;

		rows = n->current->rows;
		columns = n->current->columns;

		// this is where the formula magic happens!
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				aux = 0; z = 0;
				z = ((4 - ((8 * pow(c, 2) * pow(t, 2)) / pow(d, 2))) / (u * t + 2)) * n->current->matrix[i][j];
				z += ((u * t - 2) / (u * t + 2)) * n->previous->matrix[i][j];
				aux = ((i + 1 >= rows) ? 0 : n->current->matrix[i + 1][j]) +
					((i - 1 < 0) ? 0 : n->current->matrix[i - 1][j]) +
					((j + 1 >= columns) ? 0 : n->current->matrix[i][j + 1]) +
					((j - 1 < 0) ? 0 : n->current->matrix[i][j - 1]);
				z += (((2 * pow(c, 2) * pow(t, 2)) / pow(d, 2)) / (u * t + 2)) * aux;
				n->previous->matrix[i][j] = z;
			}
		}

		if (n->hasCallback)
			n->callback(n->previous);

		nsSwapMeshes(n);
		return 0;
	}

	return -1;
}

int nsCalculateMany(navier* n, int iterations) {
	if (n != NULL && iterations > 0) {
		for (int i = 0; i < iterations; i++)
			nsCalculate(n);

		return 0;
	}

	return -1;
}


void nsSetValues(navier* n, double t, double v, double s, double d, double f) {
	n->time = t;
	n->viscosity = v;
	n->speed = s;
	n->distance = d;
	n->force = f;
}

void nsSetTime(navier* n, double t) {
	n->time = t;
}

void nsSetViscosity(navier* n, double u) {
	n->viscosity = u;
}

void nsSetSpeed(navier* n, double c) {
	n->speed = c;
}

void nsSetDistance(navier* n, double d) {
	n->distance = d;
}

void nsSetForce(navier* n, double f) {
	n->force = f;
}

void nsSetCallBack(navier* n, void (*callback)(mesh*)) {
	n->callback = callback;
	n->hasCallback = 1;
}