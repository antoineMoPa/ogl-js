#include <iostream>
#include <GL/glut.h>
#include <cstdio>
#include <v8.h>
#include "math.h"

class Settings{
public:
    static int w;
    static int h;
    static int i;
};

int Settings::w = 0;
int Settings::h = 0;
int Settings::i = 0;

class OglApp{
public:
    OglApp(int * argc, char ** argv){
        glutInit(argc,argv);
        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
        glutInitWindowSize(Settings::w,Settings::h);
        
        auto Resize = [](int w,int h){
            
        };
        
        auto Render = [](){
            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
            
            glLoadIdentity();
            gluPerspective(
                           80,
                           float(Settings::w)/float(Settings::h),
                           1,
                           100
                           );

            gluLookAt(0.0f,0.0f,2.0f,
                      0.0f,0.0f,0.0f,
                      0.0f,1.0f,0.0f);
            
            glTranslatef(-0.4,-1,0);
            glScalef(0.1,0.1,0.1);

            glColor3f(0.6,0.3,1);
            
            glBegin(GL_TRIANGLE_STRIP);
            glVertex3f(0.0f,1.0f,0.0f);
            glVertex3f(1.0f,1.0f,0.0);
            glVertex3f(1.0f,0.0f,0.0);
            glVertex3f(3.0f,1.0f,0.0);
            glVertex3f(3.0f,0.0f,0.0);
            glVertex3f(4.0f,1.0f,0.0);
            glEnd();
                
            glFlush();
            glutSwapBuffers();
        };
        
        glutReshapeFunc(Resize);
        glutCreateWindow("Hey");
        
        glutDisplayFunc(Render);
        glutIdleFunc(Render);
       
        glutMainLoop();
    }
private:
    int i = 0;
};
    
int main(int argc, char **argv){
    Settings::w = 640;
    Settings::h = 480;
    OglApp app(&argc,argv);

    
    return 0;
}
