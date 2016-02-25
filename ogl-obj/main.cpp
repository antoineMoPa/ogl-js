#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
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

namespace OglApp{
    int i = 0;
    
    /*
      I feel so hipster
    */
    typedef std::array<float,3> vec3;
    typedef std::array<float,3> vec2;
    
    /** 
        Code + shameless inspiration
        http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
    */
    class Model{
    public:
        void load(const char * filename){
            vertices.clear();
            uvs.clear();
            normals.clear();
            
            ifstream file;
            char c;
            string s;
            float x;
            float y;
            float z;
            vec3 v3;
            vec2 v2;
            
            file.open(filename);
            
            while(!file.eof()){
                // first part of string
                file >> s;
                if(s.substr(0,2) == "vt"){
                    file >> x >> y;
                }
                else if(s.substr(0,2) == "vn"){
                    file >> x >> y >> z;
                }
                else if(s.substr(0,1) == "v"){
                    file >> v3[0] >> v3[1] >> v3[2];
                    vertices.push_back(v3);
                }
                else{
                    getline(file,s);
                }
            }
            file.close();
        }

        void render(){
            glBegin(GL_POINTS);
            for(vector<vec3>::iterator it = vertices.begin();
                it != vertices.end();
                ++it
                ){
                glVertex3f((*it)[0],(*it)[1],(*it)[2]);
            }
            glEnd();
        }
        vector <vec3> vertices;
        vector <vec2> uvs;
        vector <vec3> normals;
    };

    Model m;
    
    void start(int * argc, char ** argv){
        glutInit(argc,argv);
        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
        glutInitWindowSize(Settings::w,Settings::h);
        
        m.load("models/test_3d.obj");
        
        auto Resize = [](int w,int h){
            
        };
        
        auto Render = [](){
            i++;
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
            glRotatef(i,0,0,1);
            glColor3f(0.6,0.3,1);
            
            m.render();
            
            glFlush();
            glutSwapBuffers();
        };
        
        glutReshapeFunc(Resize);
        glutCreateWindow("Hey");
        
        glutDisplayFunc(Render);
        glutIdleFunc(Render);

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        
        glutMainLoop();
    }
};

int main(int argc, char **argv){
    Settings::w = 640;
    Settings::h = 480;
    OglApp::start(&argc,argv);

    
    return 0;
}
