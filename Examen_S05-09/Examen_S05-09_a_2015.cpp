#include <glrv/Utilidades.h>
void myInit(){
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHT0);
	glMaterialfv(GL_FRONT,GL_SPECULAR,BLANCO);
	glMaterialf(GL_FRONT,GL_SHININESS,30);
}
void display(){
	glClearColor( 1,1,1,1 );
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(2,0,0,0,0,0,0,1,0);
	ejes();
	glutSolidSphere(0.5, 60, 60);
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