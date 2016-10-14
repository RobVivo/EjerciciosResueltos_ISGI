/***************************************************
ISGI::Variacion del material
Roberto Vivo', 2013 (v1.0)

Dibujo de 16 esferas con diferente material

Dependencias: 
+GLUT
***************************************************/ 
#define PROYECTO "ISGI::S7E05::Variacoon del material"

#include <iostream>									// Biblioteca de entrada salida
#include <gl\freeglut.h>							// Biblioteca grafica

void init()
// Funcion de inicializacion propia
{
	// Mensajes por consola
	std::cout << PROYECTO << " running" << std::endl;

	glClearColor(0,0,0,1);							// Color de fondo

	// Inicialización de luces
	GLfloat Al0[]={0.2,0.2,0.2,1.0};				// Color ambiental de la luz
	GLfloat Dl0[]={1.0,1.0,1.0,1.0};				// Color difuso de la luz			
	GLfloat Sl0[]={1.0,1.0,1.0,1.0};				// Color especular de la luz
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

  gluLookAt(0,0,5,0,0,0,0,1,0);						// Situa la camara

  GLfloat posicion[] = {1,1,1,0};					// Situa la luz direccional
  glLightfv(GL_LIGHT0, GL_POSITION, posicion);

  // Dibuja 16 esferas

  glTranslatef(-1.5,-1.5,0);
  for(int i=0; i<4; i++){
	  GLfloat difuso[] = {0.2*i,0.2*i,0.1*i,1};
	  glMaterialfv(GL_FRONT, GL_DIFFUSE, difuso);
	  for(int j=0; j<4; j++){
	      GLfloat especular[] = {0.3*j,0.3*j,0.3*j,1};
	      glMaterialfv(GL_FRONT, GL_SPECULAR, especular);
		  glMaterialf(GL_FRONT, GL_SHININESS, 100);
		  glPushMatrix();
		  glTranslatef(i,j,0);
		  glutSolidSphere(0.5,200,200);				// Dibuja una esfera
		  glPopMatrix();
	  }
  }

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
	if(w<h)
		glOrtho(-2,2,-2/razon,2/razon, 0,10);
	else
		glOrtho(-2*razon,2*razon,-2,2, 0,10);
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
  init();											// Funcion propia de inicializacion
  glutMainLoop();									// Puesta en marcha del programa
}