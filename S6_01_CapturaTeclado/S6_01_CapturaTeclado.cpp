/***************************************************
ISGI::Captura de teclado
Roberto Vivo', 2013 (v1.0)

Captura el pulsado de teclas. Cona "a" dibuja en alambrico
y con "s" en solido. Muestra la posicion del cursor en el
titulo de la ventana.

Dependencias: 
+GLUT
***************************************************/ 
#define PROYECTO "ISGI::S6E01::Catura del teclado"

#include <iostream>									// Biblioteca de entrada salida
#include <sstream>									// Biblioteca de manejo de strings
#include <cmath>									// Biblioteca matematica de C
#include <gl\freeglut.h>							// Biblioteca grafica
using namespace std;

static enum ModoDibujo {ALAMBRICO,SOLIDO} modo = ALAMBRICO;

void init()
// Funcion propia de inicializacion
{
	cout << "Version: OpenGL " <<  glGetString(GL_VERSION) << endl;

	glClearColor(1.0,1.0,1.0,1.0);					// Color de fondo a blanco

	glEnable(GL_DEPTH_TEST);						// Habilita visibilidad
}

void display()
// Funcion de atencion al dibujo
{
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);	// Borra la pantalla
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  gluLookAt(0,0,5,0,0,0,0,1,0);						// Situa la camara

  glColor3f(0,0,1);									// Color de dibujo a azul
  // Dibuja la teter segun el modo de dibujo
  if(modo == ALAMBRICO)
	  glutWireTeapot(1.0);
  else if(modo == SOLIDO)
	  glutSolidTeapot(1.0);							// Dibuja una tetera en el origen

  glutSwapBuffers();								// Intercambia los buffers
}

void reshape(GLint w, GLint h)
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

void onKey(unsigned char tecla, int x, int y)
// Funcion de atencion al teclado
{
	stringstream titulo;
	titulo<<x<<","<<y;
	glutSetWindowTitle(titulo.str().c_str());		// Pone el pixel en el titulo

	switch(tecla){
	case 'a': 
		modo = ALAMBRICO;
		break;
	case 's':
		modo = SOLIDO;
		break;
	case 27:										// Puso escape
		exit(0);
	}
	glutPostRedisplay();
}

void main(int argc, char** argv)
// Programa principal
{
  glutInit(&argc, argv);							// Inicializacion de GLUT
  glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);		// Alta de buffers a usar
  glutInitWindowSize(400,400);						// Tamanyo inicial de la ventana
  glutCreateWindow(PROYECTO);						// Creacion de la ventana con su titulo
  std::cout << PROYECTO << " running" << std::endl;	// Mensaje por consola
  glutDisplayFunc(display);							// Alta de la funcion de atencion a display
  glutReshapeFunc(reshape);							// Alta de la funcion de atencion a reshape
  glutKeyboardFunc(onKey);							// Alta de la funcion de atencion al teclado
  init();											// Inicializacion propia
  glutMainLoop();									// Puesta en marcha del programa
}