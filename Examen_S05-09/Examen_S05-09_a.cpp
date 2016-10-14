#include <glrv/Utilidades.h>
void myInit(){
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHT0);
	//****** Pregunta 1.
	glMaterialfv(GL_FRONT,GL_SPECULAR,BLANCO);
	glMaterialf(GL_FRONT,GL_SHININESS,100);
	//*******/
}
void display(){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//******** Pregunta 2
	static GLfloat posluz[] = {0,1,0,0};
	glLightfv(GL_LIGHT0, GL_POSITION, posluz);
	//**********/
	gluLookAt(3,0,0,0,0,0,0,1,0);
	glutSolidSphere(1.0, 60, 60);
	glutSwapBuffers();
}
void reshape(int w, int h){
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60,(float)w / h,1,10);
}
void main(int argc, char** argv){
  glutInit(&argc, argv);							
  glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
  glutInitWindowSize(800,800);
  glutCreateWindow("EX_S05-09");						
  glutDisplayFunc(display);							
  glutReshapeFunc(reshape);	
  myInit();
  glutMainLoop();
}