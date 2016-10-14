/***************************************************
ISGI::Dos lunas
Roberto Vivo', 2013 (v1.0)

Representa dos lunas y un planeta vistos desde arriba

Dependencias: 
+GLUT
***************************************************/ 
#define PROYECTO "ISGI::S3E03::Planeta y lunas"

#include <iostream>									// Biblioteca de entrada salida
#include <cmath>									// Biblioteca matematica de C
#include <gl\freeglut.h>							// Biblioteca grafica

void display()
// Funcion de atencion al dibujo
{
  glClear(GL_COLOR_BUFFER_BIT);						// Borra la pantalla
  glMatrixMode(GL_MODELVIEW);						// Selecciona la modelview
  glLoadIdentity();									// Carga la matriz identidad

  glScalef(0.2,0.2,0.2);							// Escalado general

  // Dibuja el sol
  glutSolidSphere(1.0,10,10);						// El sol mide 1 de radio

  glPushMatrix();									// Salva el estado de la transformacion

  // Dibuja el planeta en coordenadas del sol

  /* Suponiendo que la orbita en rotacional */
  glRotatef(45,0,0,1);								// Giro alrededor del sol
  glTranslatef(3.0,0.0,0.0);						// Puesto en orbita

  /* Suponiendo la orbita traslacional 
  glTranslatef(	3.0*cos(45*3.1415926/180),
				3.0*sin(45*3.1415926/180),0);*/		// Posicion en orbita

  glutWireSphere(0.5,10,10);						// El planeta mide 0.5

  // Dibuja la primera luna en coordenadas del planeta
  glPushMatrix();									// Salva la transformacion del planeta
  glRotatef(100,0,0,1);								// Gira alrededor del planeta
  glTranslatef(1.0,0.0,0.0);						// Orbita de la primera luna
  glutWireSphere(0.1,5,5);							// La luna mide 0.1
  glPopMatrix();									// Restaura la transformacion del planeta

  // Dibuja la segunda luna en ccordenadas del planeta
  glPushMatrix();									// Salva la transformacion del planeta
  glRotatef(20,0,0,1);								// Gira alrededor del planeta
  glTranslatef(1.2,0.0,0.0);						// Orbita de la segunda luna
  glutWireSphere(0.1,5,5);							// La segunda luna mide 0.1
  glPopMatrix();									// Restaura la trasformacion del planeta

  // Aquí se pueden segur dibujando más lunas

  glPopMatrix();									// Restaura la trasformacion del sol

  // Aquí se pueden seguir dibujando más planetas

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
