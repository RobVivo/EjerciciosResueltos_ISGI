/***************************************************
ISGI::Menus de popup de GLUT
Roberto Vivo', 2013 (v1.0)

Muestra un menu de piezas y colores para seleccionar
al pulsar el boton derecho del raton

Dependencias: 
+GLUT
***************************************************/ 
#define PROYECTO "ISGI::S6E04::Menus GLUT"

#include <iostream>									// Biblioteca de entrada salida
#include <gl\freeglut.h>							// Biblioteca grafica
using namespace std;

//Variables globales

static GLuint ejes;									// Identificador de los ejes
static int xantes,yantes;							// Valor del pixel anterior
static float girox=0,giroy=0;						// Valor del giro a acumular
static float escalado=1;							// Valor del escalado acumulado
static enum Interaccion {GIRO,ESCALADO,NADA} accion;		// Tipo de acción de inspección
static enum Piezas {ESFERA,CUBO,TETERA} pieza = ESFERA;		// Pieza a dibujar
static enum Colores {ROJO,VERDE,AZUL} color;				// Opcion de color

void init()
// Funcion propia de inicializacion
{
	// Mensajes por consola
	cout << PROYECTO << " running" << endl;
	cout << "Version: OpenGL " <<  glGetString(GL_VERSION) << endl;
	cout << "Arrastre con boton izquierdo: Gira la pieza" << endl;
	cout << "Arrastre con boton central: Escala la pieza" << endl;
	cout << "Boton derecho: Menu de piezas y colores" << endl;

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
}

void display()
// Funcion de atencion al dibujo
{
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);	// Borra la pantalla
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  gluLookAt(0,0,5,0,0,0,0,1,0);						// Situa la camara
  glCallList(ejes);									// Dibuja los ejes fijos

  // La inspeccion se realiza en el sistema de coordenadas relativo
  // al objeto. Por eso los giros son respecto a los ejes X,Y del
  // modelo. VIEW*RX*RY*S
  glPushMatrix();
  glRotatef(girox,1,0,0);							// Giro en x
  glRotatef(giroy,0,1,0);							// Giro en y
  glScalef(escalado,escalado,escalado);				// Escalado 
  
  // Seleccion del color
  if(color == ROJO)
	  glColor3f(1,0,0);
  else if(color == VERDE)
	  glColor3f(0,1,0);
  else if(color == AZUL)
	  glColor3f(0,0,1);

  // Seleccion de la pieza a dibujar
  if(pieza == ESFERA)
	glutWireSphere(1.0,20,20);
  else if(pieza == CUBO)
	  glutWireCube(1.0);
  else if(pieza == TETERA)
	  glutWireTeapot(1.0);
  glPopMatrix();

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
	// Si ha levantado el boton pasamos a hacer NADA
	if(state == GLUT_UP){
		accion = NADA;
		return;
	}

	//La inspección puede ser girando la pieza o variando el tamaño
	switch (button)
	{
		case GLUT_LEFT_BUTTON:					// Girar la pieza
			accion= GIRO;
			xantes=x;							// Guarda el valor del pixel picado
			yantes=y;
			break;
		case GLUT_MIDDLE_BUTTON:				// Escalar la pieza
			accion= ESCALADO;
			yantes=y;							// La escala se maneja con mvto. vertical del ratón
			break;
		case GLUT_RIGHT_BUTTON:					// Por aqui no pasa pues se asocio al menu
			accion = NADA;
	}
}

void onMotion(int x, int y)
// Funcion de atencion al raton con el boton pulsado
// x,y: coordenadas del cursor referidas al pixel superior izquierdo(0,0)
{
	static const float pix2deg = 1.0;		// Factor de conversión pixel a grados
	static const float pix2fac = 0.01;		// Factor de conversión pixel a escalado

	switch(accion){							// La accion la determina el boton pulsado
	case GIRO:								// La acumulación del giro se produce en la MODELVIEW
		girox+= (y - yantes) * pix2deg;		// y crece hacia abajo. giro antihorario en x
		giroy+= (x - xantes) * pix2deg;		// x crece hacia derecha. giro antihorario en y
		yantes=y;
		xantes=x;
		break;
	case ESCALADO:							// La acumulación del escalado se lleva en "escalado"
		escalado+= (yantes - y) * pix2fac;	// y crece hacia abajo. escalado crece hacia arriba
		yantes=y;
	case NADA:
		;
	}
	glutPostRedisplay();
}

void onMenu(int opcion)
// Funcion de atencion al menu de popup
{
	switch(opcion){
	case 0: 
		pieza = ESFERA;
		break;
	case 1:
		pieza = CUBO;
		break;
	case 2:
		pieza = TETERA;
		break;
	case 3:
		color = ROJO;
		break;
	case 4:
		color = VERDE;
		break;
	case 5:
		color = AZUL;
	}
	glutPostRedisplay();
}

void initMenu()
// Construye el menu de popup
{
	int menucolores = glutCreateMenu(onMenu);
	glutAddMenuEntry("ROJO",3);
	glutAddMenuEntry("VERDE",4);
	glutAddMenuEntry("AZUL",5);

	glutCreateMenu(onMenu);
	glutAddMenuEntry("ESFERA", 0);
	glutAddMenuEntry("CUBO", 1);
	glutAddMenuEntry("TETERA",2);
	glutAddSubMenu("Color",menucolores);
	glutAttachMenu(GLUT_RIGHT_BUTTON);			// Ya no pasa por onClick
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
  initMenu();										// Construye menus
  init();											// Inicializacion propia
  glutMainLoop();									// Puesta en marcha del programa
}