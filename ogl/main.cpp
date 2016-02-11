#include <iostream>
#include <GL/glut.h>

class Settings{
public:
    static int w;
    static int h;
};

int Settings::w = 0;
int Settings::h = 0;

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
            glDisable(GL_DEPTH_TEST);

            Settings s;
            
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
            
            glTranslatef(-0.4,-0.3,0.8);
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
            glVertex3f(0.0f,1.0f,0.9);
            glVertex3f(1.0f,1.0f,0.9);
            glVertex3f(1.0f,0.0f,0.9);
            glVertex3f(3.0f,1.0f,0.9);
            glVertex3f(3.0f,0.0f,0.9);
            glVertex3f(4.0f,1.0f,0.9);
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
