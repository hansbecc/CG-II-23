#define MACOSX

#ifdef MACOSX
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void mydisplay() 
{
  

  glVertex3f(1.0,1.0,1.0); 
 
  glBegin(GL_POLYGON);        
  glVertex2f(-0.5, -0.5);        
  glVertex2f(-0.5, 0.5);        
  glVertex2f(0.5, 0.5);        
  glVertex2f(0.5, -0.5);    
  
  glEnd();
  
  glFlush(); 
}
 
int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutCreateWindow("simple");     
  glutDisplayFunc(mydisplay);    	   
  glClearColor(0.0,0.5,0.0,1.0); 
  glutMainLoop();
}
