#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "navier.h"

#ifndef GRAPHICS_H
#define GRAPHICS_H

BOOL isFullScreen, isWireFrame, useTexture, isPaused;
GLdouble zoom, height;
GLint winWidth, winHeight;
GLfloat rotateAngle;
int fpsFrame, fpsTime, fpsTimeBase;
int textureNumber;
int meshSizeX, meshSizeY;
navier* current;

void init(void);
void display(void);
void idle(void);
void reshape(int w, int h);
void keyboardHandler(unsigned char key, int x, int y);
void cameraHandler(int key, int x, int y);
void mouseHandler(int button, int state, int x, int y);
void createMenu();
void mainMenu(int op);
void actionMenu(int op);
void modeMenu(int op);
void textureMenu(int op);
void screenMenu(int op);
void valueMenu(int op);
void projection(void);
void writeString(float x, float y, void* font, char* string);
void interactionHandler(unsigned char);

void glEnter2DMode(void);
void glLeave2DMode(void);
void glDrawProgramInfo(void);

/* Load bitmap texture and indexes it by the returned number */
int gfxLoadTexture(char*);

/* Print project info on console */
void gfxPrintProjectInfo();

/* Calculate a vector normal */
void gfxCalculateVectorNormal(float fVert1[], float fVert2[], float fVert3[], float* fNormalX, float* fNormalY, float* fNormalZ);

/* Draw the mesh */
void gfxDrawMesh(mesh*);

#endif