/***************************************************
ISGI::Isotropia de la vista
Roberto Vivo', 2013 (v1.0)

Dibujo de una esfera donde la vista se ajusta al marco

Dependencias: 
+GLUT
***************************************************/ 
#define PROYECTO "ISGI::S4E01::Isotropia Vista"

#include <iostream>									// Biblioteca de entrada salida
#include <gl\freeglut.h>							// Biblioteca grafica

void display()
// Funcion de atencion al dibujo
{
  glClear(GL_COLOR_BUFFER_BIT);						// Borra la pantalla
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  gluLookAt(0,0,5,0,0,0,0,1,0);						// Situa la camara

  glutWireSphere(1.0,20,20);						// Dibuja la esfera

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

	/* CAMARA ORTOGRAFICA 
	// Ajustamos la vista a la dimenson más pequenya del viewport para
	// poder ver la totalidad de la ventana del mundo real (2x2)
	if(w<h)
		glOrtho(-1,1,-1/razon,1/razon, 0,10);
	else
		glOrtho(-1*razon,1*razon,-1,1, 0,10); */

	/* CAMARA PERSPECTIVA */
	// La razon de aspecto se pasa directamente a la camara perspectiva
	// Como damos fijo el angulovertical, el tamanyo del dibujo solo se
	// modifica cuando variamos la altura del viewport
	gluPerspective(45,razon,1,10); 
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