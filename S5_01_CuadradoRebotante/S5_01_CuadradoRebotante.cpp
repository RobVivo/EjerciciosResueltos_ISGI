/***************************************************
ISGI::Cuadrado rebotante
Roberto Vivo', 2013 (v1.0)

Anima un cuadrado en pantalla que rebota al llegar a
los limites de la ventana sin control de tiempos

Dependencias: 
+GLUT
***************************************************/ 
#define PROYECTO "ISGI::S5E01::Cuadrado rebotante"

#include <iostream>									// Biblioteca de entrada salida
#include <cmath>									// Biblioteca matematica de C
#include <gl\freeglut.h>							// Biblioteca grafica
using namespace std;

static float posicionx(0.0), posiciony(0.0);		// Posicion del cuadrado
static GLuint cuadrado;								// Id del cuadrado
static const GLfloat semilado(0.1);					// Tamanyo del cuadrado
static GLfloat semiancho, semialto;					// Tamaño actual de la ventana del MR


void init()
// Funcion propia de inicializacion
{
	cout << "Version: OpenGL " <<  glGetString(GL_VERSION) << endl;

	glClearColor(1.0,1.0,1.0,1.0);					// Color de fondo a blanco
	glColor3f(0,0,0);								// Color de dibujo a negro

	// Crea un cuadrado de lado x lado
	cuadrado = glGenLists(1);
	glNewList(cuadrado,GL_COMPILE);
	glBegin(GL_POLYGON);
	  glVertex3f(-semilado,-semilado,0.0);
	  glVertex3f(semilado,-semilado,0.0);
	  glVertex3f(semilado,semilado,0.0);
	  glVertex3f(-semilado,semilado,0.0);
	glEnd();
	glEndList();
}

void display()
// Funcion de atencion al dibujo
{
  glClear(GL_COLOR_BUFFER_BIT);						// Borra la pantalla
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  gluLookAt(0,0,5,0,0,0,0,1,0);						// Situa la camara

  glTranslatef(posicionx, posiciony, 0);			// Situa al cuadrado
  glCallList(cuadrado);								// y lo dibuja

  glFlush();										// Finaliza el dibujo
}

void reshape(GLint w, GLint h)
// Funcion de atencion al redimensionamiento
{
	// Tamanyo objetivo de la ventana del mundo real
	static const GLfloat ANCHO(2.0),ALTO(2.0);

	// Usamos toda el area de dibujo
	glViewport(0,0,w,h);

	// Definimos la camara (matriz de proyeccion)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	float razon = (float) w / h;

	/* CAMARA ORTOGRAFICA */
	// Ajustamos la vista a la dimenson más pequenya del viewport para
	// poder ver la totalidad de la ventana del mundo real (2x2)
	if(w<h){
		semiancho = ANCHO/2;
		semialto = ALTO/2/razon;
	}
	else{
		semiancho = ANCHO*razon/2;
		semialto = ALTO/2;
	}
	glOrtho(-semiancho,semiancho, -semialto,semialto, 0,10);
}

void onIdle()
// Funcion de atencion al evento idle
{
	// Vector velocidad del cuadrado en unidades/frame en ambas direcciones
	static float uperframe[] = {0.001,0.001};

	// posicion = posicion anterior + velocidad x (1 frame)
	posicionx+= uperframe[0];
	posiciony+= uperframe[1];

	// La ventana mide ancho x alto
	// Detección de colisiones
	if(posicionx+semilado>=semiancho || posicionx-semilado<=-semiancho) uperframe[0] *= -1;
	if(posiciony+semilado>=semialto  || posiciony-semilado<=-semialto)  uperframe[1] *= -1;

	// Puede suceder que al variar el marco, la posicion actual quede fuera de la ventana
	// con lo que el cuadrado desaparece.
	// Se puede mejorar, pero por simplicidad escogemos hacer un reset al centro

	if(abs(posicionx)>semiancho || abs(posiciony)>semialto){
		posicionx = 0.0;
		posiciony = 0.0;
	};

	glutPostRedisplay();
}
void main(int argc, char** argv)
// Programa principal
{
  glutInit(&argc, argv);							// Inicializacion de GLUT
  glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);		// Alta de buffers a usar
  glutInitWindowSize(400,400);						// Tamanyo inicial de la ventana
  glutCreateWindow(PROYECTO);						// Creacion de la ventana con su titulo
  std::cout << PROYECTO << " running" << std::endl;	// Mensaje por consola
  glutDisplayFunc(display);							// Alta de la funcion de atencion a display
  glutReshapeFunc(reshape);							// Alta de la funcion de atencion a reshape
  glutIdleFunc(onIdle);								// Alta de la funcion de atencion a idle
  init();											// Inicializacion propia
  glutMainLoop();									// Puesta en marcha del programa
}