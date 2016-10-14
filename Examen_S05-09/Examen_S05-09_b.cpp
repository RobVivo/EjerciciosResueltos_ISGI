/***************************************************
ISGI::Quad Texturado
Roberto Vivo', 2013 (v1.0)

Dibuja un quad con una textura repetida 6 veces

Dependencias: 
+GLUT +FreeGlut
***************************************************/ 
#define PROYECTO "EXAMEN_S05-09"
//#define GL_BGRA 0x80E1

#include <iostream>									// Biblioteca de entrada salida
#include <gl\freeglut.h>							// Biblioteca grafica
#include <freeimage/FreeImage.h>					// Biblioteca de gestion de imagenes
using namespace std;

//Variables globales

static int xantes,yantes;							// Valor del pixel anterior
static float girox=0,giroy=0;						// Valor del giro a acumular
static float escalado=1;							// Valor del escalado acumulado
static enum Interaccion {GIRO,ESCALADO} accion;		// Tipo de acción de inspección
void loadTexture()
// Funcion de carga de texturas e inicializacion
{    
	//1a. Generar un objeto textura
	GLuint tex0;
	glGenTextures(1,&tex0);

	//1b. Activar el objeto textura
	glBindTexture(GL_TEXTURE_2D,tex0);

	//1c. Cargar la imagen que servira de textura
	FREE_IMAGE_FORMAT formato = FreeImage_GetFileType("upv.jpg",0);		// Automatically detects the format(from over 20 formats!)
	FIBITMAP* imagen = FreeImage_Load(formato, "upv.jpg"); 
	if(imagen==NULL) cerr << endl << "NO SE ENCONTRO LA IMAGEN" << endl;
	FIBITMAP* imagen32b = FreeImage_ConvertTo32Bits(imagen);
 
	int w = FreeImage_GetWidth(imagen32b);
	int h = FreeImage_GetHeight(imagen32b);
	GLubyte* pixeles = FreeImage_GetBits(imagen32b);

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, pixeles); // FreeImage loads in BGR format, so you need GL_BGRA.
	FreeImage_Unload(imagen);
	FreeImage_Unload(imagen32b);

	//1d. Habilitar las texturas
	glEnable(GL_TEXTURE_2D);

	//2a. Seleccionar el objeto textura
	//glBindTexture(GL_TEXTURE_2D,tex0);

	//2b. Definir como se aplicará la textura en ese objeto
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);		//Texel menor que pixel
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		//Texel mayor que pixel
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);			//La textura se repite en abcisas
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);			//La textura se repite en ordenadas
	
	//2c. Definir la forma de combinar
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);				//Asigna solo el color de la textura al fragmento (solo tuberia fija)
}
void init()
// Funcion propia de inicializacion
{
	// Mensajes por consola
	cout << PROYECTO << " running" << endl;			// Mensaje por consola
	cout << "Version: OpenGL " <<  glGetString(GL_VERSION) << endl;

	cout << "Arrastre con boton izquierdo: Gira la pieza" << endl;
	cout << "Arrastre con boton derecho: Aumenta o disminuye" << endl;
	cout << "S: Captura la ventana en captura.png" << endl;

	glClearColor(0.0,0.0,0.0,1.0);					// Color de fondo a blanco
	glEnable(GL_DEPTH_TEST);						// Habilita visibilidad

	loadTexture();

}
void display()
{
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0,0,4,0,0,0,0,1,0);	
  glPushMatrix();
  glRotatef(10,1,0,0);	
  glRotatef(10,0,1,0);	
  glBegin(GL_POLYGON);
  //** solucion:   glTexCoord2f(0,0); //(0,1)
  glVertex3f(-1,-1,0);
  //** solucion:   glTexCoord2f(-1,0); //(0,0)
  glVertex3f(1,-1,0);
  //** solucion:   glTexCoord2f(-1,1); //(0,1)
  glVertex3f(1,1,0);
  //** solucion:   glTexCoord2f(0,1);  //(1,1)
  glVertex3f(-1,1,0);
  glEnd();
  glPopMatrix();
  glutSwapBuffers();
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
		girox+= (y - yantes) * pix2deg;		// y crece hacia abajo. giro antihorario en x
		giroy+= (x - xantes) * pix2deg;		// x crece hacia derecha. giro antihorario en y
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
	case 'S':										// Screenshot
		GLint vport[4];
		glGetIntegerv(GL_VIEWPORT, vport);			// Recupera el viewport corriente
		//saveScreenshot("captura.png", vport[2], vport[3]);
		break;
	case 27:										// Puso escape
		exit(0);
	}
}


void main(int argc, char** argv)
// Programa principal
{
  FreeImage_Initialise();							// Inicializacion de FreeImage
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
  FreeImage_DeInitialise();							// Cierre de FreeImage
}