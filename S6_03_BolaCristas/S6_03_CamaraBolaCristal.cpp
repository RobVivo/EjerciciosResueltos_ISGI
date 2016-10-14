/***************************************************
ISGI::Interfaz de camara Bola de Cristal
Roberto Vivo', 2013 (v1.0)

Mueve la camara en la superficie de una bola de 
cristal

Dependencias: 
+GLUT
***************************************************/ 
#define PROYECTO "ISGI::S6E03::Interfaz Bola de Cristal"

#include <iostream>									// Biblioteca de entrada salida
#include <cmath>									// Biblioteca matemática
#include <gl\freeglut.h>							// Biblioteca grafica
using namespace std;

//Variables globales

static GLuint ejes;									// Identificador de los ejes
static int xantes,yantes;							// Valor del pixel anterior
static enum {GIRO,ZOOM} accion;						// Tipo de acción de movimiento de la camara

static GLfloat ojo[3];								// Posicion del ojo
static GLfloat vertical[3];							// Vertical de la camara
static GLfloat centro[3];							// Punto al que se esta mirando

void pvec(const GLfloat a[3], const GLfloat b[3], GLfloat *c)
// Calcula el producto vectorial c = a x b
{
	c[0] = a[1]*b[2]-a[2]*b[1];
	c[1] = a[2]*b[0]-a[0]*b[2];
	c[2] = a[0]*b[1]-a[1]*b[0];
}

void normalizar(GLfloat *v)
// Normaliza el vector
{
	float lon = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
	v[0] /= lon; v[1] /= lon; v[2] /= lon;
}

void mult(const GLfloat m[16], GLfloat *p)
// Aplica la matriz al punto por la derecha p = m*p
// m ordenada por columnas
{
	float x,y,z;
	x = p[0]*m[0] + p[1]*m[4] + p[2]*m[8] + m[12];
	y = p[0]*m[1] + p[1]*m[5] + p[2]*m[9] + m[13];
	z = p[0]*m[2] + p[1]*m[6] + p[2]*m[10] + m[14];
	p[0] = x; p[1] = y; p[2] = z;
}

void up(float grados)
// Mueve la camara hacia arriba en la Bola
// Giro alrededor de u
{ 
	// Construye el sistema de la camara u,v,w
	GLfloat w[3] = {ojo[0]-centro[0], ojo[1]-centro[1], ojo[2]-centro[2]};
	normalizar(w);
	GLfloat u[3];
	pvec(vertical,w,u);
	normalizar(u);

	// Calcula la matriz a aplicar al ojo
	GLfloat coef[16];		
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(centro[0],centro[1],centro[2]);			// 3.Devuelve centro a su posicion
	glRotatef(-grados,u[0],u[1],u[2]);						// 2.Gira respecto a u
	glTranslatef(-centro[0],-centro[1],-centro[2]);			// 1.Traslada el centro al origen
	glGetFloatv(GL_MODELVIEW_MATRIX, coef);
	glPopMatrix();

	// Nueva posicion del ojo
	mult(coef,ojo);

	// Nuevo eje w
	w[0] = ojo[0] - centro[0];
	w[1] = ojo[1] - centro[1];
	w[2] = ojo[2] - centro[2];
	normalizar(w);

	// Nuevo eje v (vertical)
	pvec(w,u,vertical);										// ||vertical||=1
}

void down(float grados)
// Mueve la camara hacia abajo
{
	up(-grados);											// up negativo
}

void right(float grados)
// Nueve la camara hacia la derecha de la Bola
// Giro alrededor de v
{
	// Construye el sistema de la camara u,v,w
	GLfloat w[3] = {ojo[0]-centro[0], ojo[1]-centro[1], ojo[2]-centro[2]};
	normalizar(w);

	// Construye la matriz de giro alrededor de v
	GLfloat coef[16];
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(centro[0],centro[1],centro[2]);			// 3.Devuelve centro a su posicion
	glRotatef(grados,vertical[0],vertical[1],vertical[2]);	// 2.Gira respecto a v
	glTranslatef(-centro[0],-centro[1],-centro[2]);			// 1.Traslada el centro al origen
	glGetFloatv(GL_MODELVIEW_MATRIX, coef);
	glPopMatrix();

	// Nueva posicion del ojo, la vertical es la misma
	mult(coef,ojo);
}

void left(float grados)
// Mueve la camara hacia la izquierda
{
		right(-grados);										// left negativo
}

void zoom(float d)
// Acerca o aleja la camara
{
	// Calcula w
	GLfloat w[3] = {ojo[0]-centro[0], ojo[1]-centro[1], ojo[2]-centro[2]};
	normalizar(w);

	// Nueva posicion del ojo, la vertical no cambia
	ojo[0] += d*w[0]; ojo[1] += d*w[1]; ojo[2] += d*w[2];
}

void init()
// Funcion propia de inicializacion
{
	cout << "Version: OpenGL " <<  glGetString(GL_VERSION) << endl;

	cout << "Arrastre con boton izquierdo: Mueve la camara en la Bola" << endl;
	cout << "Arrastre con boton derecho: Acerca o aleja la camara" << endl;
	cout << "q: Posicion inicial de la camara" << endl;
	cout << "esc: Salir" << endl;

	glClearColor(1.0,1.0,1.0,1.0);					// Color de fondo a blanco
	glEnable(GL_DEPTH_TEST);						// Habilita visibilidad

	// Crea una lista para dibujar los ejes
	ejes = glGenLists(1);
	glNewList(1,GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT|GL_LINE_BIT);
	glLineWidth(4.0);
	glBegin(GL_LINES);
	glColor3f(1.0,0.0,0.0);							// Eje X en rojo
	glVertex3f(0,0,0);
	glVertex3f(1,0,0);
	glColor3f(0,1,0);								// Eje Y en verde
	glVertex3f(0,0,0);
	glVertex3f(0,1,0);				
	glColor3f(0,0,1);								// Eje Z en azul
	glVertex3f(0,0,0);
	glVertex3f(0,0,1);
	glEnd();
	glColor3f(0.5,0.5,0.5);							// Origen en gris
	glutWireCube(0.1);
	glPopAttrib();
	glEndList();

	// Inicializa la camara
	centro[0] = 0; centro[1] = 0; centro[2] = 0;
	ojo[0] = 0; ojo[1] = 0; ojo[2] = 5;
	vertical[0] = 0; vertical[1] = 1; vertical[2] = 0;
}

void display()
// Funcion de atencion al dibujo
{
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);	// Borra la pantalla
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  gluLookAt(ojo[0],ojo[1],ojo[2],
			centro[0],centro[1],centro[2],
			vertical[0], vertical[1], vertical[2]);	// Situa la camara
  glCallList(ejes);									// Dibuja los ejes fijos
  glColor3f(1,0,1);									// Color de dibujo a magenta
  glutWireTeapot(1.0);

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

void onClick(int button,int state, int x, int y)
// Funcion de atencion al boton del raton
// button: GLUT_LEFT|MIDDLE|RIGHT_BUTTON
// state: GLUT_UP|DOWN
// x,y: pixel respecto a vertice superior izquierdo
{
	//La inspección puede ser girando la pieza o variando el tamaño
	switch (button)
	{
		case GLUT_LEFT_BUTTON:					// Girar la pieza
			accion= GIRO;
			xantes=x;							// Guarda el valor del pixel picado
			yantes=y;
			break;
		case GLUT_RIGHT_BUTTON:					// Escalar la pieza
			accion= ZOOM;
			yantes=y;							// La escala se maneja con mvto. vertical del ratón
			break;
	};
}

void onMotion(int x, int y)
// Funcion de atencion al raton con el boton pulsado
// x,y: coordenadas del cursor referidas al pixel superior izquierdo(0,0)
{
	static const float pix2deg = 1.0;		// Factor de conversión pixel a grados
	static const float pix2fac = 0.1;		// Factor de conversión pixel a zoom

	switch(accion){							// La accion la determina el boton pulsado
	case GIRO:								// La acumulación del giro se produce en la MODELVIEW
		down( (y - yantes) * pix2deg );		// y crece hacia abajo. giro antihorario en x
		right((x - xantes) * pix2deg );		// x crece hacia derecha. giro antihorario en y
		yantes = y;
		xantes = x;
	break;
	case ZOOM:							// La acumulación del escalado se lleva en "escalado"
		zoom( (yantes - y) * pix2fac);	// y crece hacia abajo. escalado crece hacia arriba
		yantes=y;
	break;
	};

	glutPostRedisplay();
}

void onKey(unsigned char tecla, int x, int y)
// Funcion de atencion al teclado
{
	switch(tecla){
	case 'q':								// Vuelve a la posicion original
		// Inicializa la camara
		centro[0] = 0; centro[1] = 1; centro[2] = 0;
		ojo[0] = 0; ojo[1] = 0; ojo[2] = 5;
		vertical[0] = 0; vertical[1] = 1; vertical[2] = 0;
		break;
	case 27:								// Salir de la aplicacion
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
  glutMouseFunc(onClick);							// Alta de la funcion de atencion al click del ratón
  glutMotionFunc(onMotion);							// Alta de la funcion de atencion al movimiento del ratón
  glutKeyboardFunc(onKey);							// Alta de la funcion de atencion al teclado
  init();											// Inicializacion propia
  glutMainLoop();									// Puesta en marcha del programa
}