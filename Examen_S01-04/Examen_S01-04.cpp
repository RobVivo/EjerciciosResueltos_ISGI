/***************************************************
ISGI::Examen Seminarios 1 al 4
Roberto Vivo', 2013 (v1.0)

Examen sobre geometría, transformaciones y cámara

Dependencias: 
+GLUT
***************************************************/ 
#define PROYECTO "ISGI::EX01::ExamenS01-04"

#include <iostream>									// Biblioteca de entrada salida
#include <glrv/Utilidades.h>						// Biblioteca grafica

static GLuint tetera;

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

  	GLfloat posicion[] = {-1,1,1,0};
	tetera = glGenLists( 1 );
	glNewList( tetera, GL_COMPILE );
	glColor3fv( BLANCO );
	glLightfv(GL_LIGHT0,GL_POSITION,posicion);
	glutSolidTeapot( 0.5 );
	glEndList();

}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);						
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  //gluLookAt(0,2,0,0,0,0,-1,0,0);
  //ejes();
  glRotatef(90,1,0,0);	// glRotatef(-90,0,0,1);  // glRotatef(-90,0,1,0);
  glRotatef(-90,0,1,0);   // glRotatef(90,1,0,0);   // glRotatef(-90,0,0,1);
  glCallList( tetera );							
  glFlush();										
}
void reshape(GLint w, GLint h)
{
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1,1,-(float)h/w,(float)h/w,-4,4);
}

void main(int argc, char** argv)
// Programa principal
{
  glutInit(&argc, argv);							// Inicializacion de GLUT
  glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_DEPTH);		// Alta de buffers a usar
  glutInitWindowSize(800,800);						// Tamanyo inicial de la ventana
  glutCreateWindow(PROYECTO);						// Creacion de la ventana con su titulo
  std::cout << PROYECTO << " running" << std::endl;	// Mensaje por consola
  glutDisplayFunc(display);							// Alta de la funcion de atencion a display
  glutReshapeFunc(reshape);							// Alta de la funcion de atencion a reshape
  init();
  glutMainLoop();									// Puesta en marcha del programa
}
