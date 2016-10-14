/***************************************************
ISGI::Texturas automáticas
Roberto Vivo', 2013 (v1.0)

Dibuja una tetera generando las normales automaticamente

Dependencias: 
+GLUT +glext +FreeGlut
***************************************************/ 
#define PROYECTO "ISGI::S8E04::Texturas automáticas"

#include <iostream>									// Biblioteca de entrada salida
#include <gl/freeglut.h>							// Biblioteca grafica
#include <GL/glext.h>								// Biblioteca de extensiones de GL
#include <freeimage/FreeImage.h>					// Biblioteca de gestion de imagenes
using namespace std;

//Variables globales

static int xantes,yantes;							// Valor del pixel anterior
static float girox=0,giroy=0;						// Valor del giro a acumular
static float escalado=1;							// Valor del escalado acumulado
static enum Interaccion {GIRO,ESCALADO} accion;		// Tipo de acción de inspección
static 	GLuint tex[1];								// Ids de texturas

void saveScreenshot(char* nombre, int ancho, int alto)
// Utiliza FreeImage para grabar un png
// nombre: Nombre del fichero con extensión p.e. salida.png
// ancho: Ancho del viewport en pixeles
// alto: Alto del viewport en pixeles
{	
	int pix = ancho * alto;
	BYTE *pixels = new BYTE[3*pix];	
	glReadBuffer(GL_FRONT);
	glReadPixels(0,0,ancho,alto,GL_BGR,GL_UNSIGNED_BYTE, pixels);
	FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, ancho, alto, ancho*3, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);
	FreeImage_Save(FIF_PNG, img, nombre, 0);
	cout << "Captura de ventana realizada en " << nombre << endl;
	delete pixels;
}

void loadImageFile(char* nombre)
// Uso de FreeImage para cargar la imagen en cualquier formato
// nombre: nombre del fichero con extensión en el mismo directorio que el proyecto
//         o con su path completo
{
	// Detección del formato, lectura y conversion a BGRA
    FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(nombre,0);
	FIBITMAP* imagen = FreeImage_Load(formato, nombre); 
	FIBITMAP* imagen32b = FreeImage_ConvertTo32Bits(imagen);

	// Lectura de dimensiones y colores
	int w = FreeImage_GetWidth(imagen32b);
	int h = FreeImage_GetHeight(imagen32b);
	GLubyte* texeles = FreeImage_GetBits(imagen32b);

	// Carga como textura actual
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, texeles);

	// Liberar recursos
	FreeImage_Unload(imagen);
	FreeImage_Unload(imagen32b);
}

void loadTexture()
// Funcion de carga de texturas e inicializacion
{    
	//1b. Activar el objeto textura
	glBindTexture(GL_TEXTURE_2D,tex[0]);

	//1c. Cargar la imagen que servira de textura
	loadImageFile("marmol.jpg");

	//1d. Habilitar las texturas
	glEnable(GL_TEXTURE_2D);

	//2a. Seleccionar el objeto textura
	glBindTexture(GL_TEXTURE_2D,tex[0]);

	//2b. Definir como se aplicará la textura en ese objeto
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);		//Texel menor que pixel
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		//Texel mayor que pixel
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);			//La textura se repite en abcisas
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);			//La textura se repite en ordenadas

	//2c. Definir la forma de combinar
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);		

	// Generacion automática de texturas
	//glEnable(GL_TEXTURE_GEN_S);
	//glEnable(GL_TEXTURE_GEN_T);

	// Alineacion de la textura
	GLfloat planoS[] = {1,0,0,0};
	GLfloat planoT[] = {0,1,0,0};
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_S, GL_OBJECT_PLANE, planoS);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_T, GL_OBJECT_PLANE, planoT);
}

void loadLight()
{
	//Luces
	glEnable(GL_LIGHT0);

	//Materiales
	GLfloat mat_diffuse[] = {0.8,0.8,0.8,1.0};			//Kd
	GLfloat mat_specular[] = {1,1,1,1.0};				//Ks	
	GLfloat mat_shininess[] = {100.0};					//n
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);  
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);  
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	//Habilita la iluminación
	glEnable(GL_LIGHTING);
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

	glClearColor(1.0,1.0,1.0,1.0);					// Color de fondo a blanco
	glEnable(GL_DEPTH_TEST);						// Habilita visibilidad

	loadLight();
	loadTexture();

}

void display()
// Funcion de atencion al dibujo
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);	// Borra la pantalla
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0,0,5,0,0,0,0,1,0);						// Situa la camara

	// La inspeccion se realiza en el sistema de coordenadas relativo
	// al objeto. Por eso los giros son respecto a los ejes X,Y del
	// modelo. VIEW*RX*RY*S

	glRotatef(girox,1,0,0);							// Giro en x
	glRotatef(giroy,0,1,0);							// Giro en y
	glScalef(escalado,escalado,escalado);			// Escalado 

	// Brillos separados de la textura
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

	// Dibujar el objeto
	glPushMatrix();
	glutSolidTeapot(1.5);
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
		saveScreenshot("captura.png", vport[2], vport[3]); // CUIDADO. Viewports mayores de 520 dan error en heap
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