#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include <vector>
#include <GL/glut.h>
#include <cstdio>
#include <ctime>
#include <unistd.h>
#include <algorithm>
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
    time_t timev;
    
    /*
      I feel so hipster
    */
    typedef std::array<float,3> vec3;
    typedef std::array<float,3> vec2;
    typedef std::array<float,9> face3;
    
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
            faces3.clear();
            
            ifstream file;
            char c;
            string s;
            // temp vars
            float x;
            float y;
            float z;
            vec3 v3;
            vec2 v2;
            face3 f3;
            string face;
            vector<int> tempfaceint;
            
            file.open(filename);

            if(!file.is_open()){
                return;
            }
            
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
                else if(s.substr(0,1) == "f"){
                    tempfaceint.clear();
                    // skip character
                    file.get();
                    getline(file,face);
                    // http://stackoverflow.com/questions/2896600
                    replace(face.begin(),face.end(),'/',' ');
                    stringstream f(face);
                    int tempint;
                    while(f){
                        f >> tempint;
                        tempfaceint.push_back(tempint);                       
                    }
                    // remove last read number
                    tempfaceint.pop_back();
                    
                    if(tempfaceint.size() == 9){
                        for(vector<int>::iterator it = tempfaceint.begin();
                            it!= tempfaceint.end();
                            ){
                            
                            for(int i = 0; i < 9; i++,++it){
                                f3[i] = *it;
                            }
                            faces3.push_back(f3);
                        }
                    } else if (tempfaceint.size() == 6){
                        for(vector<int>::iterator it = tempfaceint.begin();
                            it!= tempfaceint.end();
                            ){
                            for(int i = 0; i < 9; i++){
                                if(i % 3 == 0){
                                    f3[i] = *it;
                                    ++it;
                                    ++it;
                                    // TODO: verify vertex actually exists
                                } else {
                                    f3[i] = 0;
                                }
                            }
                            faces3.push_back(f3);
                        }
                    } else if (tempfaceint.size() == 3){
                        for(vector<int>::iterator it = tempfaceint.begin();
                            it!= tempfaceint.end();
                            ){
                            for(int i = 0; i < 9; i++){
                                if(i % 3 == 0){
                                    f3[i] = *it;
                                    ++it;
                                    // TODO: verify vertex actually exists
                                } else {
                                    f3[i] = 0;
                                }
                            }
                            faces3.push_back(f3);
                        }
                    }
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

            glBegin(GL_TRIANGLES);
            for(vector<face3>::iterator it = faces3.begin();
                it != faces3.end();
                ++it){
                
                glVertex3f(
                    vertices[(*it)[0]-1][0],
                    vertices[(*it)[0]-1][1],
                    vertices[(*it)[0]-1][2]
                    );
                glVertex3f(
                    vertices[(*it)[3]-1][0],
                    vertices[(*it)[3]-1][1],
                    vertices[(*it)[3]-1][2]
                    );
                glVertex3f(
                    vertices[(*it)[6]-1][0],
                    vertices[(*it)[6]-1][1],
                    vertices[(*it)[6]-1][2]
                    );

            }
            glEnd();

        }
        vector <vec3> vertices;
        vector <vec2> uvs;
        vector <vec3> normals;
        vector <face3> faces3;
    };

    Model m;
    
    void start(int * argc, char ** argv){
        glutInit(argc,argv);
        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
        glutInitWindowSize(Settings::w,Settings::h);
        
        //m.load("models/test_3d.obj");
        //m.load("models/cube.obj");
        //m.load("models/world.obj");
        m.load("models/building.obj");
        
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
            
            glTranslatef(0,0,0);
            glScalef(0.3,0.3,0.3);
            glColor3f(0.6,0.3,1);
            glRotatef(i/3,1,1,0);
            
            m.render();
            usleep(2000);
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
