#include "mesh.h"

void meshPrepare() {
	srand((unsigned int)time(0));
	remove("mesh.csv");
}

mesh* meshInit(int rows, int columns) {
	mesh* m = (mesh*)malloc(sizeof(mesh));
	meshAssign(m, rows, columns);
	return m;
}

void meshFree(mesh* m) {
	for (int i = 0; i < m->rows; i++)
		free(m->matrix[i]);

	free(m->matrix);
	free(m);
}

int meshAssign(mesh* m, int rows, int columns) {
	int i, j;
	int maxSize = MESH_MAX_SIZE;

	if (rows == 0) {
		rows = MESH_DEFAULT_SIZE;
	}
	else if (columns == 0) {
		columns = MESH_DEFAULT_SIZE;
	}
	else if (rows > maxSize || columns > maxSize) {
		return 1;
	}

	m->rows = rows;
	m->columns = columns;

	m->matrix = malloc(sizeof(double*) * rows);
	for (i = 0; i < rows; i++)
		m->matrix[i] = malloc(sizeof(double) * columns);

	for (i = 0; i < rows; i++)
		for (j = 0; j < columns; j++)
			m->matrix[i][j] = 0;

	return 0;
}

int meshAssignDefault(mesh* m) {
	int defaultSize = MESH_DEFAULT_SIZE;
	return meshAssign(m, defaultSize, defaultSize);
}

void meshPrint(mesh* m) {
	printf("Matrix %dx%d: \n", m->rows, m->columns);

	for (int i = 0; i < m->rows; i++) {
		for (int j = 0; j < m->columns; j++) {
			printf("%lf ", m->matrix[i][j]);
		}
		printf("\n");
	}
}

void meshExport(mesh* m) {
	FILE* arq;
	fopen_s(&arq, "mesh.csv", "a");

	fprintf(arq, "Matrix %dx%d:\n", m->rows, m->columns);
	for (int i = 0; i < m->rows; i++) {
		for (int j = 0; j < m->columns; j++) {
			fprintf(arq, "%f;", m->matrix[i][j]);
		}

		fprintf(arq, "\n");
	}

	fprintf(arq, "\n\n");
	fclose(arq);
}

void meshFill(mesh* m, double value) {
	for (int i = 0; i < (m->rows); i++)
		for (int j = 0; j < (m->columns); j++)
			m->matrix[i][j] = value;
}

void meshRandomize(mesh* m) {
	for (int i = 0; i < (m->rows); i++)
		for (int j = 0; j < (m->columns); j++)
			m->matrix[i][j] = (rand() % 5 - 1) * 0.02245328;
}