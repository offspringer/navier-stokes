#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "graphics.h"

const GLfloat light_ambient[] = { 0.2f, 0.6f, 1.0f, 0.5f };
const GLfloat light_diffuse[] = { 0.2f, 0.6f, 1.0f, 0.5f };
const GLfloat light0_pos[] = { 1.0f, 1.0f, -1.5f, 0.0f };
const GLfloat light1_pos[] = { 45.0, 0.0, 15.0, 0.0f };

const GLfloat blue[] = { 0.0, 0.8f, 0.8f, 0.8 };
const GLfloat red[] = { 0.8f, 0.0, 0.0, 0.8 };
const GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
const GLfloat polished[] = { 100.0 };

void init(void) {
	gfxPrintProjectInfo();

	glClearColor(0.0, 0.0, 0.0, 0.0); // Clear screen
	glShadeModel(GL_FLAT); // Flat Shading
	glEnable(GL_DEPTH_TEST); // Enable ZBuffer

	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);

	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_NORMALIZE);

	zoom = 15.0;
	height = 5.0;
	winWidth = 800;
	winHeight = 600;
	rotateAngle = 0;

	isFullScreen = FALSE;
	isWireFrame = TRUE;
	isPaused = FALSE;

	fpsFrame = fpsTime = fpsTimeBase = 0;

	meshSizeX = 128;
	meshSizeY = 128;
	current = nsInit(meshSizeX, meshSizeY);

	nsSetValues(current, 0.1, 0.001003, 1.0, 1.0, 1.0);

	meshFill(current->previous, 0);
	meshFill(current->previous, 0);
	nsSetCallBack(current, gfxDrawMesh);

	textureNumber = -1;
	textureNumber = gfxLoadTexture("water_texture.bmp");
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(250.0, 0.0, 20.0 * height, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);

	glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	if (useTexture) {
		if (textureNumber == -1) {
			printf("Couldn't initialize the texture.");
			exit(1);
		}

		glEnable(GL_TEXTURE_2D);
	}
	else
		glDisable(GL_TEXTURE_2D);

	glTranslatef(0.0, 0.0, 0.0);
	glRotatef(rotateAngle, 0.0, 0.0, 1.0);
	glTranslatef(-30.0, -39.0, 0.0);
	nsCalculate(current);
	glPopMatrix();
	glDisable(GL_BLEND);

	glDrawProgramInfo();

	glFlush();
	glutSwapBuffers();
}

void idle(void) {
	fpsFrame++;
	fpsTime = glutGet(GLUT_ELAPSED_TIME);

	if (fpsTime - fpsTimeBase > 1000) {
		fpsTimeBase = fpsTime;
		fpsFrame = 0;
	}

	if (!isPaused)
		glutPostRedisplay();
}

void reshape(int w, int h) {
	winWidth = w;
	winHeight = h;

	glViewport(0, 0, w, h);
	projection();
}

void keyboardHandler(unsigned char key, int x, int y) {
	interactionHandler(key);
}

void cameraHandler(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		zoom /= 1.2f;
		projection();
		break;
	case GLUT_KEY_DOWN:
		zoom *= 1.2f;
		projection();
		break;
	case GLUT_KEY_LEFT:
		rotateAngle += 10.0;
		break;
	case GLUT_KEY_RIGHT:
		rotateAngle -= 10.0;
		break;
	case GLUT_KEY_PAGE_UP:
		height += 1.0;
		break;
	case GLUT_KEY_PAGE_DOWN:
		height -= 1.0;
		break;
	default:
		break;
	}
}

void mouseHandler(int button, int state, int x, int y) {
	if (button == GLUT_RIGHT_BUTTON)
		if (state == GLUT_DOWN)
			createMenu();

	glutPostRedisplay();
}

void createMenu() {
	int m, sm1, sm2, sm3, sm4, sm5;

	sm1 = glutCreateMenu(actionMenu);
	glutAddMenuEntry("G - Generate drop in the center of the mesh", 0);
	glutAddMenuEntry("A - Generate random drop on the mesh", 1);
	glutAddMenuEntry("R - Generate random state for the mesh", 2);
	glutAddMenuEntry("Z - Reset mesh", 3);

	sm2 = glutCreateMenu(modeMenu);
	glutAddMenuEntry("X - Wireframe/Surface", 0);

	sm3 = glutCreateMenu(textureMenu);
	glutAddMenuEntry("T - Enable/Disable", 0);
	glutAddMenuEntry("ADD", 1);
	glutAddMenuEntry("BLEND", 2);
	glutAddMenuEntry("DECAL", 3);
	glutAddMenuEntry("MODULATE", 4);
	glutAddMenuEntry("REPLACE", 5);

	sm4 = glutCreateMenu(screenMenu);
	glutAddMenuEntry("F - Fullscren/Window", 0);
	glutAddMenuEntry("P - Pause", 1);

	sm5 = glutCreateMenu(valueMenu);
	glutAddMenuEntry("Time", 0);
	glutAddMenuEntry("Viscosity", 1);
	glutAddMenuEntry("Speed", 2);
	glutAddMenuEntry("Distance", 3);
	glutAddMenuEntry("Force", 4);
	glutAddMenuEntry("Reset values", 5);


	m = glutCreateMenu(mainMenu);
	glutAddSubMenu("Actions", sm1);
	glutAddSubMenu("Mode", sm2);
	glutAddSubMenu("Texture", sm3);
	glutAddSubMenu("Screen", sm4);
	glutAddSubMenu("Values", sm5);
	glutAddMenuEntry("ESC - Exit", 0);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void mainMenu(int op) {
	switch (op) {
	case 0:
		interactionHandler(27);
		break;
	}
}

void actionMenu(int op) {
	switch (op) {
	case 0:
		interactionHandler('g');
		break;
	case 1:
		interactionHandler('a');
		break;
	case 2:
		interactionHandler('r');
		break;
	case 3:
		interactionHandler('z');
		break;
	}
}

void modeMenu(int op) {
	switch (op) {
	case 0:
		interactionHandler('x');
		break;
	}
}

void textureMenu(int op) {
	switch (op) {
	case 0:
		interactionHandler('t');
		break;
	case 1:
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
		break;
	case 2:
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
		break;
	case 3:
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		break;
	case 4:
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		break;
	case 5:
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		break;
	}
}

void screenMenu(int op) {
	switch (op) {
	case 0:
		interactionHandler('f');
		break;
	case 1:
		interactionHandler('p');
		break;
	}
}

void valueMenu(int op) {
	double aux = 0.0;

	switch (op) {
	case 0:
		printf("\n Enter the new value for time: ");
		scanf("%lf", &aux);
		nsSetTime(current, aux);
		aux = 0.0;
		break;
	case 1:
		printf("\n Enter the new value for viscosity: ");
		scanf("%lf", &aux);
		nsSetViscosity(current, aux);
		aux = 0.0;
		break;
	case 2:
		printf("\n Enter the new value for speed: ");
		scanf("%lf", &aux);
		nsSetSpeed(current, aux);
		aux = 0.0;
		break;
	case 3:
		printf("\n Enter the new value for distance: ");
		scanf("%lf", &aux);
		nsSetDistance(current, aux);
		aux = 0.0;
		break;
	case 4:
		printf("\n Enter the new value for force: ");
		scanf("%lf", &aux);
		nsSetForce(current, aux);
		aux = 0.0;
		break;
	case 5:
		nsSetValues(current, 0.1, 0.001003, 1.0, 1.0, 1.0);
		break;
	}
}

void projection(void) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(zoom, (GLfloat)winWidth / (GLfloat)winHeight, 1.0, 500.0);
}

void writeString(float x, float y, void* font, char* string) {
	char* c;
	glRasterPos2f(x, y);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}

void interactionHandler(unsigned char command) {
	int random_x = 0;
	int random_y = 0;

	switch (command) {
	case 27: // ESC
		if (isFullScreen) {
			glutReshapeWindow(800, 600);
			glutPositionWindow(100, 100);
			isFullScreen = FALSE;
		}
		else
		{
			printf("\n\n");
			printf(" Exiting... \n\n");
			exit(0);
		}
		break;
	case 'G':
	case 'g':
		current->previous->matrix[meshSizeX / 2][meshSizeY / 2] -= 0.8;
		break;
	case 'A':
	case 'a':
		srand(clock() * 0.2);
		random_x = rand() % meshSizeX;
		random_y = rand() % meshSizeY;
		current->previous->matrix[random_x][random_y] -= 0.5;
		break;
	case 'Z':
	case 'z':
		meshFill(current->previous, 0.0);
		meshFill(current->current, 0.0);
		break;
	case 'R':
	case 'r':
		meshRandomize(current->previous);
		meshRandomize(current->current);
		break;
	case 'X':
	case 'x':
		if (isWireFrame) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			isWireFrame = FALSE;
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			isWireFrame = TRUE;
		}
		break;
	case 'T':
	case 't':
		if (useTexture) {
			useTexture = FALSE;
		}
		else {
			useTexture = TRUE;
		}
		break;
	case 'P':
	case 'p':
		if (isPaused) {
			isPaused = FALSE;
		}
		else {
			isPaused = TRUE;
		}
		break;
	case 'F':
	case 'f':
		if (isFullScreen) {
			glutReshapeWindow(800, 600);
			glutPositionWindow(100, 100);
			isFullScreen = FALSE;
		}
		else {
			glutFullScreen();
			isFullScreen = TRUE;
		}
		break;
	case 'W':
	case 'w':
		glutReshapeWindow(800, 600);
		break;
	default:
		break;
	}
}

void glEnter2DMode(void) {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, winWidth, 0, winHeight);

	glDisable(GL_LIGHTING);
	if (useTexture) glDisable(GL_TEXTURE_2D);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
}

void glLeave2DMode(void) {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glEnable(GL_LIGHTING);
	if (useTexture) glEnable(GL_TEXTURE_2D);

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void glDrawProgramInfo(void) {
	char param_dist[80], param_force[80], param_speed[80], param_time[80], param_visc[80];
	char param_mesh[80], param_fps[20];

	glEnter2DMode();
	sprintf(param_time, "%s%.3f", "Time: ", current->time);
	sprintf(param_visc, "%s%.8f", "Viscosity: ", current->viscosity);
	sprintf(param_speed, "%s%.3f", "Speed: ", current->speed);
	sprintf(param_dist, "%s%.3f", "Distance: ", current->distance);
	sprintf(param_force, "%s%.3f", "Force: ", current->force);
	sprintf(param_mesh, "%s%d%s%d", "Matrix: ", meshSizeX, "x", meshSizeY);
	sprintf(param_fps, "%4.2f fps", fpsFrame * 1000.0 / (fpsTime - fpsTimeBase));

	glColor3f(0.5, 0.5, 1);
	writeString(5, winHeight - 18, GLUT_BITMAP_HELVETICA_18, "Navier Stokes Simulator");
	writeString(winWidth - 55, winHeight - 12, GLUT_BITMAP_HELVETICA_10, param_fps);

	glColor3f(1, 1, 1);
	writeString(5, winHeight - (12 * 3), GLUT_BITMAP_HELVETICA_10, param_mesh);
	writeString(5, winHeight - (12 * 4), GLUT_BITMAP_HELVETICA_10, param_dist);
	writeString(5, winHeight - (12 * 5), GLUT_BITMAP_HELVETICA_10, param_force);
	writeString(5, winHeight - (12 * 6), GLUT_BITMAP_HELVETICA_10, param_speed);
	writeString(5, winHeight - (12 * 7), GLUT_BITMAP_HELVETICA_10, param_time);
	writeString(5, winHeight - (12 * 8), GLUT_BITMAP_HELVETICA_10, param_visc);
	glLeave2DMode();
}

int gfxLoadTexture(char* filename) {
	int i = 0;
	FILE* l_file;
	BITMAPINFOHEADER infoheader;
	char temp;
	char* bitmap;

	textureNumber++;

	if ((l_file = fopen(filename, "rb")) == NULL)
		return (-1);

	fseek(l_file, 18, SEEK_CUR);  // start reading width & height
	fread(&infoheader.biWidth, sizeof(int), 1, l_file);
	fread(&infoheader.biHeight, sizeof(int), 1, l_file);
	fread(&infoheader.biPlanes, sizeof(short int), 1, l_file);
	if (infoheader.biPlanes != 1)
		return -1;

	fread(&infoheader.biBitCount, sizeof(unsigned short int), 1, l_file);
	if (infoheader.biBitCount != 24)
		return -1;

	fseek(l_file, 24, SEEK_CUR); // skip to data section
	bitmap = (char*)malloc(infoheader.biWidth * infoheader.biHeight * 3);
	if (bitmap == NULL)
		return -1;

	if ((i = ((int)fread(bitmap, infoheader.biWidth * infoheader.biHeight * 3, 1, l_file))) != 1)
		return -1;

	// convert color from BGR to RGB
	for (i = 0; i < (infoheader.biWidth * infoheader.biHeight * 3); i += 3) {
		temp = bitmap[i];
		bitmap[i] = bitmap[i + 2];
		bitmap[i + 2] = temp;
	}
	fclose(l_file); // closes the file stream

	// configure the texture
	glBindTexture(GL_TEXTURE_2D, textureNumber);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); // to match color of the texture with the surface

	glTexImage2D(GL_TEXTURE_2D, 0, 3, infoheader.biWidth, infoheader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmap);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, infoheader.biWidth, infoheader.biHeight, GL_RGB, GL_UNSIGNED_BYTE, bitmap);
	free(bitmap);

	return textureNumber;
}


void gfxPrintProjectInfo() {
	printf("\n +------------------------------------------------------------+");
	printf("\n | Fluid Simulator based on Navier Stokes                     |");
	printf("\n |                                                            |");
	printf("\n | Oriented by: Luciano Silva                                 |");
	printf("\n | Developed by: Roger Santos and Thiago Baptista             |");
	printf("\n +------------------------------------------------------------+");
	printf("\n\n");
}

void gfxCalculateVectorNormal(float fVert1[], float fVert2[], float fVert3[], float* fNormalX, float* fNormalY, float* fNormalZ) {
	float Qx, Qy, Qz, Px, Py, Pz;

	Qx = fVert2[0] - fVert1[0];
	Qy = fVert2[1] - fVert1[1];
	Qz = fVert2[2] - fVert1[2];
	Px = fVert3[0] - fVert1[0];
	Py = fVert3[1] - fVert1[1];
	Pz = fVert3[2] - fVert1[2];

	*fNormalX = Py * Qz - Pz * Qy;
	*fNormalY = Pz * Qx - Px * Qz;
	*fNormalZ = Px * Qy - Py * Qx;
}

void gfxDrawMesh(mesh* m) {
	int i, j, count = 0;
	GLfloat glfPosX = 0.0, glfPosY = 0.0;
	float nx, ny, nz;
	float v1[3];
	float v2[3];
	float v3[3];
	float v4[3];
	double dist;

	// draw the lines
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialfv(GL_FRONT, GL_SHININESS, polished);

	for (i = 0; i < m->rows - 1; i++) {
		for (j = 0; j < m->columns - 1; j++) {
			dist = current->distance;

			// square
			v1[0] = glfPosX;
			v1[1] = glfPosY;
			v1[2] = m->matrix[i][j];

			v2[0] = glfPosX + dist;
			v2[1] = glfPosY;
			v2[2] = m->matrix[i + 1][j];

			v3[0] = glfPosX + dist;
			v3[1] = glfPosY + dist;
			v3[2] = m->matrix[i + 1][j + 1];

			v4[0] = glfPosX;
			v4[1] = glfPosY + dist;
			v4[2] = m->matrix[i][j + 1];

			glfPosX = (GLfloat)(i * dist);
			glfPosY = (GLfloat)(j * dist);

			glBegin(GL_TRIANGLES); // triangle 1
			gfxCalculateVectorNormal(v1, v2, v3, &nx, &ny, &nz);
			glNormal3f(nx, ny, nz);
			glTexCoord2f((float)i / m->columns, (float)j / m->rows);
			glVertex3f(glfPosX, glfPosY, m->matrix[i][j]);
			glTexCoord2f((float)i / m->columns, (float)j / m->rows);
			glVertex3f(glfPosX + dist, glfPosY, m->matrix[i + 1][j]);
			glTexCoord2f((float)i / m->columns, (float)j / m->rows);
			glVertex3f(glfPosX + dist, glfPosY + dist, m->matrix[i + 1][j + 1]);
			glEnd();

			glBegin(GL_TRIANGLES); // triangle 2			
			gfxCalculateVectorNormal(v1, v3, v4, &nx, &ny, &nz);
			glNormal3f(nx, ny, nz);
			glTexCoord2f((float)i / m->rows, (float)j / m->columns);
			glVertex3f(glfPosX, glfPosY, m->matrix[i][j]);
			glTexCoord2f((float)i / m->rows, (float)j / m->columns);
			glVertex3f(glfPosX + dist, glfPosY + dist, m->matrix[i + 1][j + 1]);
			glTexCoord2f((float)i / m->rows, (float)j / m->columns);
			glVertex3f(glfPosX, glfPosY + dist, m->matrix[i][j + 1]);
			glEnd();
		}
	}
}