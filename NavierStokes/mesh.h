#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef MESH_H
#define MESH_H

typedef struct {
	int rows;
	int columns;
	double** matrix;
} _mesh;

typedef _mesh mesh;

#define MESH_MAX_SIZE 999999999
#define MESH_DEFAULT_SIZE 20

/* Initialize random seed and clear previously exported file */
void meshPrepare();

/* Initialize a mesh */
mesh* meshInit(int, int);

/* Destroy the mesh */
void meshFree(mesh*);

/* Allocates a new mesh with the informed size (rows and columns) */
int meshAssign(mesh*, int, int);

/* Allocates a new mesh with the default size */
int meshAssignDefault(mesh*);

/* Print the mesh */
void meshPrint(mesh*);

/* Export a .csv file of the mesh */
void meshExport(mesh*);

/* Fill the mesh with the informed value */
void meshFill(mesh*, double);

/* Fill the mesh with random values */
void meshRandomize(mesh*);

#endif