#include <iostream>
#include <fstream>
#include <vector>
#include <GL/glut.h>
#include <cstdio>
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

using namespace std;

class OglApp{
public:

    /*
      I feel so hipster
    */
    typedef float vec3[3];
    typedef float vec2[2];
    
    /** 
        Code + shameless inspiration
        http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
    */
    class Model{
    public:
        void load(const char * filename){
            ifstream file;
            char c;
            string s;
            file.open(filename);

            while(!file.eof()){
                getline(file,s);
                if(s.substr(0,1) == "#"){
                    // ignore
                }
                else if(s.substr(0,2) == "vt"){
                    cout << "vt" << endl;
                }
            }
            file.close();
        }
        vector <vec3> vertices;
        vector <vec2> uvs;
        vector <vec3> normals;
    };
        
    OglApp(int * argc, char ** argv){
        glutInit(argc,argv);
        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
        glutInitWindowSize(Settings::w,Settings::h);

        m.load("models/test_3d.obj");
        
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
    Model m;
};
    
int main(int argc, char **argv){
    Settings::w = 640;
    Settings::h = 480;
    OglApp app(&argc,argv);

    
    return 0;
}
