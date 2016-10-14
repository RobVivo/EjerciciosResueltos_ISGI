/***************************************************
ISGI::Luz de Minero
Roberto Vivo', 2013 (v1.0)

Mueve la camara en la interfaz de Bola de Cristal
llevando una luz focal adherida

Dependencias: 
+GLUT
***************************************************/ 
#define PROYECTO "ISGI::S7E04::Luz de Minero"

#include <iostream>									// Biblioteca de entrada salida
#include <gl\freeglut.h>							// Biblioteca grafica
using namespace std;

// Variables globales
static int xantes,yantes;							// Valor del pixel anterior
static float girox=0,giroy=0;						// Valor del giro a acumular
static float escalado=1;							// Valor del escalado acumulado
static float coef[16];								// Matriz MODELVIEW
static enum Interaccion {GIRO,ESCALADO} accion;		// Tipo de acción de inspección

void init()
// Funcion propia de inicializacion
{
	cout << PROYECTO << " running" << endl;			// Mensajes por consola
	cout << "Version: OpenGL " <<  glGetString(GL_VERSION) << endl;
	cout << "Arrastre con boton izquierdo: Mueve la camara en la Bola" << endl;
	cout << "Arrastre con boton derecho: Acerca o aleja la camara" << endl;
	cout << "q: Posicion inicial de la camara" << endl;
	cout << "esc: Salir" << endl;

	glClearColor(0.0,0.0,0.0,1.0);					// Color de fondo a blanco

	glGetFloatv(GL_MODELVIEW_MATRIX, coef);			// Inicializa coef a la matriz identidad

	// Inicializa luces
	// LIGHT2: FOCAL
	GLfloat Al2[]={0.2,0.2,0.2,1.0};				// Color ambiental de la luz
	GLfloat Dl2[]={1.0,1.0,0.0,1.0};				// Color difuso de la luz			
	GLfloat Sl2[]={1.0,1.0,0.0,1.0};				// Color especular de la luz
	glLightfv(GL_LIGHT2, GL_AMBIENT, Al2);			// Caracteristicas de LIGHT2
	glLightfv(GL_LIGHT2, GL_DIFFUSE, Dl2);
	glLightfv(GL_LIGHT2, GL_SPECULAR, Sl2);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 10.0);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT,50.0);
	glEnable(GL_LIGHT2);

	// Caracteristicas del render
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
}

void display()
// Funcion de atencion al dibujo
{
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);	// Borra la pantalla
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Para la luz de minero situamos la luz antes de situar la camara
  // asi no le afecta la VIEW y trabajamos en coordenadas del sistema de la camara
  GLfloat posicion[] = {0,0,0,1};
  glLightfv(GL_LIGHT2,GL_POSITION,posicion);		// Luz focal sobre la camara
  GLfloat dir_central[]={0.0, 0.0, -1.0};			// Direccion del foco la de la vista
  glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, dir_central);

  // Los giros se acumulan con signo contrario por la izquierda sobre el objeto girado,
  // así siempre se gira sobre el sistema fijo. VIEW*R*Racumulado*zoom

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
  glutSolidTorus(0.5,1.0,200,200);

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
			accion= ESCALADO;
			yantes=y;							// La escala se maneja con mvto. vertical del ratón
			girox=0;							// Cuando se escala no se acumula ningún giro
			giroy=0;
			break;
	};
}

void onMotion(int x, int y)
// Funcion de atencion al raton con el boton pulsado
// x,y: coordenadas del cursor referidas al pixel superior izquierdo(0,0)
{
	static const float pix2deg = 1.0;		// Factor de conversión pixel a grados
	static const float pix2fac = 0.01;		// Factor de conversión pixel a escalado

	switch(accion){							// La accion la determina el boton pulsado
	case GIRO:								// La acumulación del giro se produce en la MODELVIEW
		girox= (y - yantes) * pix2deg;		// y crece hacia abajo. giro antihorario en x
		giroy= (x - xantes) * pix2deg;		// x crece hacia derecha. giro antihorario en y
		yantes=y;
		xantes=x;
	break;
	case ESCALADO:							// La acumulación del escalado se lleva en "escalado"
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
  glutMouseFunc(onClick);							// Alta de la funcion de atencion al click del ratón
  glutMotionFunc(onMotion);							// Alta de la funcion de atencion al movimiento del ratón
  glutKeyboardFunc(onKey);							// Alta de la funcion de atencion al teclado
  init();											// Inicializacion propia
  glutMainLoop();									// Puesta en marcha del programa
}