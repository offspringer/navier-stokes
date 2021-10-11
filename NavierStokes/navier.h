#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mesh.h"

#ifndef NAVIERSTK_H
#define NAVIERSTK_H

typedef struct {
	mesh* previous;
	mesh* current;
	double time, viscosity, speed, distance, force;
	void (*callback)(mesh*);
	int hasCallback;
} _navier;

typedef _navier navier;

/* Initialize Navier Stokes */
navier* nsInit(int, int);

/* Destroy the Navier Stokes */
void nsFree(navier*);

/* Assign meshes to the Navier Stokes */
int nsAssignMeshes(navier*, mesh*, mesh*);

/* Swap meshes from the Navier Stokes
In Depth:
 - current mesh becomes the previous mesh;
 - newly calculated mesh becomes the current mesh;
 - only two meshes are used to save memory (the next mesh will be calculated over the previous one)
*/
void nsSwapMeshes(navier*);

/* Calculate the Navier Stokes */
int nsCalculate(navier*);

/* Calculate the Navier Stokes many times, according to the informed amount */
int nsCalculateMany(navier*, int);

/* Define Navier Stokes values for time, viscosity, speed, distance and force at once */
void nsSetValues(navier*, double, double, double, double, double);

/* Define Navier Stokes values for time */
void nsSetTime(navier*, double);

/* Define Navier Stokes values for viscosity */
void nsSetViscosity(navier*, double);

/* Define Navier Stokes values for speed */
void nsSetSpeed(navier*, double);

/* Define Navier Stokes values for distance */
void nsSetDistance(navier*, double);

/* Define Navier Stokes values for force */
void nsSetForce(navier*, double);

/* Define a callback for every calculated mesh from Navier Stokes */
void nsSetCallBack(navier*, void (*callback)(mesh*));

#endif