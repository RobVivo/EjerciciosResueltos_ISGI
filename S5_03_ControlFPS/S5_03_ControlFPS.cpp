/***************************************************
ISGI::Control de los FPS
Roberto Vivo', 2013 (v1.0)

Anima la camara en un travelling circular controlando
la frecuencia de dibujo para mantener los FPS constantes
y manteneindo la velocidad de la animacion constante

Dependencias: 
+GLUT
***************************************************/ 
#define PROYECTO "ISGI::S5E02::Control del tiempo"

#include <iostream>									// Biblioteca de entrada salida
#include <sstream>									// Biblioteca de manejo de strings
#include <cmath>									// Biblioteca matematica de C
#include <gl\freeglut.h>							// Biblioteca grafica
using namespace std;

static const float radio = 5.0;						// Radio de giro de la camara
static float angulo = 0.0;							// Angulo de travelling inicial de la camara
static float ojo[] = {0,0,radio};					// Posicion inicial de la camara
static const float velocidad = 24.0*3.1415926/180;	// radianes/segundo
static const int tasaFPS = 40;						// tasa que se quiere como maximo

void muestraFPS()
// Calcula la frecuencia y la muestra en el título de la ventana
// cada segundo
{
	int ahora, tiempo_transcurrido;
	static int antes = 0;
	static int FPS = 0;
	stringstream titulo;

	//Cuenta de llamadas a esta función en el último segundo
	FPS++;
	ahora = glutGet(GLUT_ELAPSED_TIME);				//Tiempo transcurrido desde el inicio
	tiempo_transcurrido = ahora - antes;			//Tiempo transcurrido desde antes
	if(tiempo_transcurrido>1000){					//Acaba de rebasar el segundo
		titulo << "FPS: " << FPS;					//Se muestra una vez por segundo
		glutSetWindowTitle(titulo.str().c_str());
		antes = ahora;								//Ahora ya es antes	
		FPS = 0;									//Reset del conteo
	}
}

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

  gluLookAt(ojo[0],ojo[1],ojo[2],0,0,0,0,1,0);		// Situa la camara

  glColor3f(0,0,1);									// Color de dibujo a azul
  glutSolidTeapot(1.0);								// Dibuja una tetera en el origen

  glPushMatrix();
  glTranslatef(0,0,-2);
  glColor3f(1,0,0);									// Color de dibujo a rojo
  glutSolidTeapot(0.5);								// Dibuja una tetera detras
  glPopMatrix();

  muestraFPS();										// Hemos dibujado un frame

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

//void onIdle()
void onTimer(int valor)
// Funcion de atencion al timer periodico
{
	//Calculamos el tiempo transcurrido desde la última vez
	static int antes=0;
	int ahora, tiempo_transcurrido;

	ahora= glutGet(GLUT_ELAPSED_TIME);			//Tiempo transcurrido desde el inicio
	tiempo_transcurrido= ahora - antes;			//Tiempo transcurrido desde antes en msg.

	// angulo = angulo anterior + velocidad x tiempo
	angulo += velocidad*tiempo_transcurrido/1000.0;
	ojo[0] = radio * sin(angulo);
	ojo[2] = radio * cos(angulo);

	antes = ahora;

	glutTimerFunc(1000/tasaFPS,onTimer,tasaFPS);	// Se encola un nuevo timer
	glutPostRedisplay();							// Se manda el dibujo
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
  //glutIdleFunc(onIdle);
  glutTimerFunc(1000/tasaFPS,onTimer,tasaFPS);		// Alta de la funcion de atencion al timer
  init();											// Inicializacion propia
  glutMainLoop();									// Puesta en marcha del programa
}