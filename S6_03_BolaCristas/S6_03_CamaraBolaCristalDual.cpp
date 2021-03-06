/***************************************************
ISGI::Camara Bola de Cristal Dual
Roberto Vivo', 2013 (v1.0)

Captura el movimiento del raton para mover la camara 
en la escena sobre una Bola de Cristal. El movimiento
se aplica a los objetos por dualidad, es decir con
signo contrario manteniendo la camara fija.

Dependencias: 
+GLUT
***************************************************/ 
#define PROYECTO "ISGI::S6E03::Bola de Cristal Dual"

#include <iostream>									// Biblioteca de entrada salida
#include <cmath>									// Biblioteca matematica de C
#include <gl\glut.h>								// Biblioteca grafica
using namespace std;

//Variables globales

static GLuint ejes;									// Identificador de los ejes
static int xantes,yantes;							// Valor del pixel anterior
static float girox=0,giroy=0;						// Valor del giro a acumular
static float escalado=1;							// Valor del escalado acumulado
static float coef[16];								// Matriz MODELVIEW
static enum Interaccion {GIRO,ESCALADO} accion;		// Tipo de acci�n de inspecci�n

void init()
// Funcion propia de inicializacion
{
	cout << PROYECTO << " running" << endl;			// Mensajes por consola
	cout << "Version: OpenGL " <<  glGetString(GL_VERSION) << endl;
	cout << "Arrastre con boton izquierdo: Mueve la camara en la Bola" << endl;
	cout << "Arrastre con boton derecho: Acerca o aleja la camara" << endl;
	cout << "q: Posicion inicial de la camara" << endl;
	cout << "esc: Salir" << endl;

	glClearColor(1.0,1.0,1.0,1.0);					// Color de fondo a blanco
	glEnable(GL_DEPTH_TEST);						// Habilita visibilidad
	glGetFloatv(GL_MODELVIEW_MATRIX, coef);			// Inicializa coef a la matriz identidad

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
	glEnd;
	glColor3f(0.5,0.5,0.5);							// Origen en gris
	glutWireCube(0.1);
	glPopAttrib();
	glEndList();
}

void display()
// Funcion de atencion al dibujo
{
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);	// Borra la pantalla
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Los giros se acumulan con signo contrario por la izquierda sobre el objeto girado,
  // as� siempre se gira sobre el sistema fijo. VIEW*R*Racumulado*zoom

  // Primero calculamos los giros R*Racumulado (signos contrarios)
  glRotatef(-girox,1,0,0);							// 3.Aplicamos el giro actual en X fijo
  glRotatef(-giroy,0,1,0);							// 2.Aplicamos el giro actual en Y fijo
  glMultMatrixf(coef);								// 1.Aplicamos el giro anterior
  glGetFloatv(GL_MODELVIEW_MATRIX, coef);			// Nos guardamos la orientacion para la proxima

  // Restablecemos la MODELVIEW y componemos en el orden correcto
  glLoadIdentity();
  gluLookAt(0,0,5,0,0,0,0,1,0);						// Situa la camara fija MV=VIEW
  glMultMatrixf(coef);								// Gira el acumulado MV=VIEW*R*Racum
  glScalef(escalado,escalado,escalado);				// Escala el modelo MV=VIEW*R*Racum*zoom

  // Dibuja la escena
  glCallList(ejes);									// Dibuja los ejes 
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
	//La inspecci�n puede ser girando la pieza o variando el tama�o
	switch (button)
	{
		case GLUT_LEFT_BUTTON:					// Girar la pieza
			accion= GIRO;
			xantes=x;							// Guarda el valor del pixel picado
			yantes=y;
			break;
		case GLUT_RIGHT_BUTTON:					// Escalar la pieza
			accion= ESCALADO;
			yantes=y;							// La escala se maneja con mvto. vertical del rat�n
			girox=0;							// Cuando se escala no se acumula ning�n giro
			giroy=0;
			break;
	};
}

void onMotion(int x, int y)
// Funcion de atencion al raton con el boton pulsado
// x,y: coordenadas del cursor referidas al pixel superior izquierdo(0,0)
{
	static const float pix2deg = 1.0;		// Factor de conversi�n pixel a grados
	static const float pix2fac = 0.01;		// Factor de conversi�n pixel a escalado

	switch(accion){							// La accion la determina el boton pulsado
	case GIRO:								// La acumulaci�n del giro se produce en la MODELVIEW
		girox= (y - yantes) * pix2deg;		// y crece hacia abajo. giro antihorario en x
		giroy= (x - xantes) * pix2deg;		// x crece hacia derecha. giro antihorario en y
		yantes=y;
		xantes=x;
	break;
	case ESCALADO:							// La acumulaci�n del escalado se lleva en "escalado"
		escalado+= (yantes - y) * pix2fac;	// y crece hacia abajo. escalado crece hacia arriba
		yantes=y;
	break;
	};

	glutPostRedisplay();
}

void onKey(unsigned char tecla, int x, int y)
// Funcion de atencion al teclado
{
	switch(tecla){
	case 'q':						// Vuelve a la posicion original
		girox = 0;
		giroy = 0;
		escalado = 1;
		glPushMatrix();
		glLoadIdentity();
		glGetFloatv(GL_MODELVIEW_MATRIX,coef);	// Inicializa coef a la identidad
		glPopMatrix();
		break;
	case 27:							// Salir de la aplicacion
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
  glutMouseFunc(onClick);							// Alta de la funcion de atencion al click del rat�n
  glutMotionFunc(onMotion);							// Alta de la funcion de atencion al movimiento del rat�n
  glutKeyboardFunc(onKey);							// Alta de la funcion de atencion al teclado
  init();											// Inicializacion propia
  glutMainLoop();									// Puesta en marcha del programa
}