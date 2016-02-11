#include <iostream>
#include <GL/glut.h>

class OglApp{
public:
    OglApp(int * argc, char ** argv){
        glutInit(argc,argv);
        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
        glutInitWindowSize(600,600);
        glutReshapeFunc(Resize);
        glutCreateWindow("Hey");
        
        
        glutDisplayFunc(Render);
        glutIdleFunc(Render);
        
        glutMainLoop();
    }
    static GLvoid Render(){
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glutSolidTeapot(30.0);
        glDisable(GL_DEPTH_TEST);

        glLoadIdentity();
        
        gluLookAt(0.0f,0.0f,1.0f,
                  0.0f,0.0f,0.0f,
                  0.0f,1.0f,0.0f);
        
        
        
        glScalef(0.2,0.2,0.2);
        
        glBegin(GL_TRIANGLE_STRIP);
        glVertex3f(0.0f,1.0f,0.0f);
        glVertex3f(1.0f,1.0f,0.0);
        glVertex3f(1.0f,0.0f,0.0);
        glVertex3f(3.0f,1.0f,0.0);
        glVertex3f(3.0f,0.0f,0.0);
        glVertex3f(4.0f,1.0f,0.0);
        glEnd();

        glBegin(GL_TRIANGLE_STRIP);
        glVertex3f(0.0f,1.0f,0.1);
        glVertex3f(1.0f,1.0f,0.1);
        glVertex3f(1.0f,0.0f,0.1);
        glVertex3f(3.0f,1.0f,0.1);
        glVertex3f(3.0f,0.0f,0.1);
        glVertex3f(4.0f,1.0f,0.1);
        glEnd();

        glFlush();
        glutSwapBuffers();
    }
    
    static GLvoid Resize(GLint width,GLint height){
        
    }
private:
    int i = 0;
};

int main(int argc, char **argv){
    OglApp app(&argc,argv);
    
    return 0;
}
