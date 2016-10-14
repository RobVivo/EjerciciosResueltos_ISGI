/***************************************************
ISGI::Tetera GLUT
Roberto Vivo', 2013 (v1.0)

Dibujo basico de una tetera GLUT en OpenGL

Dependencias: 
+GLUT
***************************************************/ 
#define PROYECTO "ISGI::S2E01::Tetera GLUT"

#include <iostream>									// Biblioteca de entrada salida
#include <gl\freeglut.h>							// Biblioteca grafica

void display()
// Funcion de atencion al dibujo
{
  glClear(GL_COLOR_BUFFER_BIT);						// Borra la pantalla
  //glutWireTeapot(0.5);							// Dibujo de una tetera en alambrico
  glutSolidTeapot(0.5);								// Dibujo de una tetera solida
  glFlush();										// Finaliza el dibujo
}

void reshape(GLint w, GLint h)
// Funcion de atencion al redimensionamiento
{
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
  glutMainLoop();									// Puesta en marcha del programa
}
