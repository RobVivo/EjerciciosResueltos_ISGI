/***************************************************
ISGI::Luz Puntual vs Direccional
Roberto Vivo', 2013 (v1.0)

Dibujo de una esfera iluminada con diferentes luces

Dependencias: 
+GLUT
***************************************************/ 
#define PROYECTO "ISGI::S7E02::Puntual vs Direccional"

#include <iostream>									// Biblioteca de entrada salida
#include <gl\freeglut.h>							// Biblioteca grafica

static bool DIRECCIONAL(true);						// Tipo de luz

void init()
// Funcion de inicializacion propia
{
	// Mensajes por consola
	std::cout << PROYECTO << " running" << std::endl;
	std::cout << "d: Luz direccional" << std::endl;
	std::cout << "p: Luz puntual" << std::endl << std::endl;

	glClearColor(0,0,0,1);							// Color de fondo

	// Inicialización de luces
	GLfloat Al0[]={0.2,0.2,0.2,1.0};				// Color ambiental de la luz
	GLfloat Dl0[]={1.0,1.0,0.0,1.0};				// Color difuso de la luz			
	GLfloat Sl0[]={1.0,1.0,0.0,1.0};				// Color especular de la luz
	glLightfv(GL_LIGHT0, GL_AMBIENT, Al0);			// Caracteristicas de LIGHT0
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Dl0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Sl0);
	glEnable(GL_LIGHT0);

	// Caracteristicas del render
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}

void display()
// Funcion de atencion al dibujo
{
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);	// Borra la pantalla y el Z-buffer
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  gluLookAt(1,0,1,0,0,0,0,1,0);						// Situa la camara

  // Posiciona y define el tipo de luz
  GLfloat posicion[] = {0,1,0,0};
  if(DIRECCIONAL) posicion[3] = 0;					// Luz direccional
  else posicion[3] = 1;								// Luz puntual
  glLightfv(GL_LIGHT0,GL_POSITION,posicion);

  glColor3f(1,1,1);
  glutSolidSphere(0.5,20,20);						// Dibuja una esfera blanca

  glutSwapBuffers();								// Finaliza el dibujo
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
	gluPerspective(90,razon,0.1,10); 
}

void onKey(unsigned char tecla, int x, int y)
// Funcion de atencion al teclado
{
	switch(tecla){
	case 'p': 
		DIRECCIONAL = false;
		std::cout<<"PUNTUAL ON, direccional off"<<std::endl;
		break;
	case 'd':
		DIRECCIONAL = true;
		std::cout<<"DIRECCIONAL ON, puntual off"<<std::endl;
		break;
	case 27:										// Pulso escape
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
  glutDisplayFunc(display);							// Alta de la funcion de atencion a display
  glutReshapeFunc(reshape);							// Alta de la funcion de atencion a reshape
  glutKeyboardFunc(onKey);							// Alta de funcion de atencion al teclado
  init();											// Funcion propia de inicializacion
  glutMainLoop();									// Puesta en marcha del programa
}