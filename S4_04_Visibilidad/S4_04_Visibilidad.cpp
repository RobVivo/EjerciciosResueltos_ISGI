/***************************************************
ISGI::Visibilidad
Roberto Vivo', 2013 (v1.0)

Dibujo de una teter vista desde arriba

Dependencias: 
+GLUT
***************************************************/ 
#define PROYECTO "ISGI::S4E04::Visibilidad"

#include <iostream>									// Biblioteca de entrada salida
#include <gl\freeglut.h>							// Biblioteca grafica

void display()
// Funcion de atencion al dibujo
{
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);	// Borra la pantalla y el Z-buffer
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  gluLookAt(0,0,5,0,0,0,0,1,0);						// Situa la camara

  //glEnable(GL_DEPTH_TEST);

  glColor3f(1,0,0);
  glutWireTeapot(0.5);								// Dibuja la tetera cercana roja

  glTranslatef(1,0,-4);				
  glColor3f(0,0,1);
  glutSolidTeapot(0.5);

  glFlush();										// Finaliza el dibujo
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
	gluPerspective(45,razon,1,10); 
}

void main(int argc, char** argv)
// Programa principal
{
  glutInit(&argc, argv);							// Inicializacion de GLUT
  glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_DEPTH);		// Alta de buffers a usar
  glutInitWindowSize(400,400);						// Tamanyo inicial de la ventana
  glutCreateWindow(PROYECTO);						// Creacion de la ventana con su titulo
  std::cout << PROYECTO << " running" << std::endl;	// Mensaje por consola
  glutDisplayFunc(display);							// Alta de la funcion de atencion a display
  glutReshapeFunc(reshape);							// Alta de la funcion de atencion a reshape
  glutMainLoop();									// Puesta en marcha del programa
}