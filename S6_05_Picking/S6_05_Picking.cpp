/***************************************************
ISGI::Seleccion de objetos
Roberto Vivo', 2013 (v1.0)

Permite picar sobre un objeto para ponerlo a girar.

Dependencias: 
+GLUT
***************************************************/ 
#define PROYECTO "ISGI::S6E02::Inspeccion SCM"

#include <iostream>
#include <GL/freeglut.h>
using namespace std;

static GLubyte selected[] = {0x00};			// Pixel leído donde se picó 
static bool onSelection;					// Activar/Desactivar la selección
static GLint pickx,picky;					// Localización del pixel en coordenadas del marco
static float giroT(0),giroS(0),giroC(0);	// Giro particular de cada objeto

void loadPickName(char name)
//Carga un identificativo para la selección
{
	//Simplemente se pinta de rojo 'name' todo lo que se dibuje hasta otra carga de nombre
	//cuando se está en modo selección
	if(onSelection)glColor3ub(name,0x00,0x00);
};

void init()
//Inicializaciones
{
	// Mensajes por consola
	cout << PROYECTO << " running" << endl;
	cout << "Version: OpenGL " <<  glGetString(GL_VERSION) << endl;
	cout << "Selecciona el objeto que gira con el boton izquierdo" << endl;

	glClearColor(1.0,1.0,1.0,1.0);			// Color de fondo
};
void onDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(1,2,3,0,0,0,0,1,0);			// Posiciona la camara


	if(onSelection){							// Render en backbuffer sin swap
		// Primer objeto
		glPushMatrix();
		glTranslatef(-2,0,0);
		glRotatef(giroT,0,1,0);
		loadPickName('T');
		glutSolidTeapot(0.5);					// Tetera 'T'
		glPopMatrix();
		// Segundo objeto
		glPushMatrix();
		glRotatef(giroS,0,1,0);
		loadPickName('S');						// Esfera 'S'
		glutSolidSphere(0.5,20,20);
		glPopMatrix();
		// Tercer objeto
		glPushMatrix();
		glTranslatef(1.5,0,0);
		glRotatef(giroC,0,1,0);
		loadPickName('C');						// Cubo 'C'
		glutSolidCube(0.5);
		glPopMatrix();

		//Se lee el canal rojo de un rectangulo de 1x1 pixel en pickx,picky (DC)
		glReadPixels(pickx,picky,1,1,GL_RED,GL_UNSIGNED_BYTE, selected);

		onSelection= false;
	}
	else{										// Render normal
		// Primer objeto
		glPushMatrix();
		glTranslatef(-2,0,0);
		glRotatef(giroT,0,1,0);
		glColor3f(1,0,0);
		glutWireTeapot(0.5);					
		glPopMatrix();
		// Segundo objeto
		glPushMatrix();
		glRotatef(giroS,0,1,0);
		glColor3f(0,1,0);
		glutWireSphere(0.5,20,20);
		glPopMatrix();
		// Tercer objeto
		glPushMatrix();
		glTranslatef(1.5,0,0);
		glRotatef(giroC,0,1,0);
		glColor3f(0,0,1);
		glutWireCube(0.5);
		glPopMatrix();

		glutSwapBuffers();						// Intercambia los buffers
	}
}

void onReshape(int w, int h)
// Funcion de atencion al redimensionamiento
{
	// Usamos toda el area de dibujo
	glViewport(0,0,w,h);

	// Definimos la camara (matriz de proyeccion)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	float razon = (float) w / h;

	/* CAMARA PERSPECTIVA */
	gluPerspective(60,razon,1,10);
}

void onClick(int button, int state, int x, int y)
//Callback de respuesta a presionar el botón del ratón
{

	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){		//Botón izquierdo abajo
		GLint vport[4];
		onSelection= true;					//Se activa el modo selección
		glGetIntegerv(GL_VIEWPORT, vport);	//Recupera el viewport corriente
		pickx=x; picky=vport[3]-y;			//Coordenadas en el sistema del marco
		glutPostRedisplay();				//Evento para procesar la selección
	}
}

void onIdle()
// Funcion de atencion al evento idle
{
	switch((char) selected[0]){
	case 'T':
		giroT += 0.1;
		glutPostRedisplay();
		break;
	case 'S':
		giroS += 0.1;
		glutPostRedisplay();
		break;
	case 'C':
		giroC += 0.1;
		glutPostRedisplay();
	}
}

void main(int argc, char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(400,300);
	glutInitWindowPosition(0,0);
	glutCreateWindow(PROYECTO);
	glutReshapeFunc(onReshape);
	glutDisplayFunc(onDisplay);
	glutMouseFunc(onClick);
	glutIdleFunc(onIdle);
	init();

	glutMainLoop();
};