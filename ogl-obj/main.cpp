#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include <vector>
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>
#include <GL/glew.h>
#include <GL/glut.h>
#include <cstdio>
#include <ctime>
#include <unistd.h>
#include <algorithm>
#include <cstdio>
#include "math.h"
#include "shader.h"

/*
 Thanks to http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/
 */

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

    class Shader{
    public:
        bool load(const char * vertex, const char * fragment){
            programID = LoadShaders(vertex, fragment);
        } 
        bool bind(){
            glUseProgram(programID);
            return true;
        }
        GLuint programID;
    };
    
    /*
      Thanks to
      http://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/
    */
    class Image{
    public:
        bool load(const char * filename){
            FILE * file = fopen(filename,"rb");
            if(!file){
                cout << "Unable to read file." << endl;
                return false;
            }
            
            if(fread(header,1,54,file) != 54){
                cout << "Bad file header." << endl;
                return false;
            }
            
            if(header[0] != 'B' || header[1] != 'M'){
                cout << "Bad file header." << endl;
                return false;
            }

            dataPos = *(int*)&(header[0x0A]);
            imageSize = *(int*)&(header[0x22]);
            width = *(int*)&(header[0x12]);
            height = *(int*)&(header[0x16]);
            
            if(imageSize == 0){
                imageSize = width*height*3;
            }

            if(dataPos == 0){
                dataPos = 54;
            }
            
            data = new unsigned char [imageSize];
            fread(data,1,imageSize,file);
            fclose(file);

            return true;
        }
        bool bind(){
            glGenTextures(1, &textureID);
            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(
                GL_TEXTURE_2D,
                0, // level (for mipmap stuff)
                GL_RGB, // Internal format
                width,
                height,
                0, // Doc says:  "This value must be 0." ...
                GL_BGR, // Format
                GL_UNSIGNED_BYTE,
                data
                );

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

            // Mipmap
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        ~Image(){
            delete data;
        }
        GLuint textureID;
        unsigned char header[54];
        unsigned int dataPos;
        unsigned int width, height;
        unsigned int imageSize; // width*height*3
        unsigned char * data;
    };
    
    
    typedef array<float,3> vec3;
    typedef array<float,3> vec2;
    typedef array<float,9> face3;
    
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
        
    private:
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
            
            m.render();
            
            usleep(2000);
            glFlush();
            glutSwapBuffers();
        };
        
        glutReshapeFunc(Resize);
        glutCreateWindow("Hey");

        // http://gamedev.stackexchange.com/questions/22785/
        GLenum err = glewInit();
        if (err != GLEW_OK){
            cout << "GLEW error: " << err << endl;
            cout <<  glewGetErrorString(err) << endl;
            exit(1);
        }
        
        glutDisplayFunc(Render);
        glutIdleFunc(Render);

        Image img;
        Shader shader;
        
        glm::mat4 Projection = glm::perspective(
            glm::radians(45.0f),
            float(Settings::w)/float(Settings::h),
            0.1f,
            100.0f
            );

        glm::mat4 View = glm::lookAt(
            glm::vec3(0,0,2), // Camera
            glm::vec3(0,0,0), // Origin
            glm::vec3(0,1,0)  // Vertical axis
            );

        glm::mat4 Model = glm::mat4(1.0f);
        glm::mat4 mvp = Projection * View * Model;

        shader.load("vertex.glsl","fragment.glsl");
        shader.bind();
        GLuint MatrixID = glGetUniformLocation(shader.programID, "MVP");
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
                
        if(img.load("images/lava.bmp")){
            img.bind();
        }
        
                
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
