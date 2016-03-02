#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include <vector>
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
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

    class Camera{
    public:
        glm::mat4 model_view_matrix(){
            glm::mat4 Projection = glm::perspective(
                glm::radians(45.0f),
                    float(w)/float(h),
                0.1f,
                100.0f
                );
            
            glm::mat4 View = glm::lookAt(
                glm::vec3(0,0,2), // Camera
                glm::vec3(0,0,0), // Origin
                glm::vec3(0,1,0)  // Vertical axis
                );

            glm::mat4 Translate = glm::translate(glm::vec3(x,y,z));
            glm::mat4 Scale = glm::scale(glm::vec3(scale,scale,scale));
            glm::mat4 Model = glm::mat4(1.0f);
            
            glm::vec3 rotation_axis(1.0f,0.0f,0.0f);
            glm::mat4 Rotation = glm::rotate(angle, rotation_axis);
            glm::mat4 mvp =
                Projection * View * Translate * Rotation * Scale * Model;
            
            return mvp;
        }
        int w,h;
        float angle = 0;
        float scale = 0;
        float x = 0;
        float y = 0;
        float z = 0;
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
       
       The code of this class has to be seriously reviewed if user input is not
       trusted.
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
                    file >> v2[0] >> v2[1];
                    uvs.push_back(v2);
                }
                else if(s.substr(0,2) == "vn"){
                    file >> v3[0] >> v3[1] >> v3[2];
                    normals.push_back(v3);
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

                    if(tempfaceint.size() == 12){
                        cout << "Please triangulate your model." << endl;
                    }
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
        
        void create_buffers(){
            size_t numpoints = faces3.size() * 3;
            vertex_num = numpoints;
            vertex_buffer_data = new GLfloat[numpoints * 3];
            normal_buffer_data = new GLfloat[numpoints * 3];
            uv_buffer_data = new GLfloat[numpoints * 2];

            typedef vector<face3>::iterator faceit;
            
            int vi = 0; // vertex index
            int uvi = 0; // uv index
            for(faceit it = faces3.begin(); it != faces3.end();++it){
                // for each face:
                // add vertex coords to buffer data
                // add normal coords to buffer data
                // add uv coords to buffer data
                vertex_buffer_data[i+0] = vertices[(*it)[0]-1][0];
                vertex_buffer_data[i+1] = vertices[(*it)[0]-1][1];
                vertex_buffer_data[i+2] = vertices[(*it)[0]-1][2];
                vertex_buffer_data[i+3] = vertices[(*it)[3]-1][0];
                vertex_buffer_data[i+4] = vertices[(*it)[3]-1][1];
                vertex_buffer_data[i+5] = vertices[(*it)[3]-1][2];
                vertex_buffer_data[i+6] = vertices[(*it)[6]-1][0];
                vertex_buffer_data[i+7] = vertices[(*it)[6]-1][1];
                vertex_buffer_data[i+8] = vertices[(*it)[6]-1][2];

                // normals
                if(normals.size() != 0){
                    normal_buffer_data[i+0] = normals[(*it)[2]-1][0];
                    normal_buffer_data[i+1] = normals[(*it)[2]-1][1];
                    normal_buffer_data[i+2] = normals[(*it)[2]-1][2];
                    normal_buffer_data[i+3] = normals[(*it)[5]-1][0];
                    normal_buffer_data[i+4] = normals[(*it)[5]-1][1];
                    normal_buffer_data[i+5] = normals[(*it)[5]-1][2];
                    normal_buffer_data[i+6] = normals[(*it)[8]-1][0];
                    normal_buffer_data[i+7] = normals[(*it)[8]-1][1];
                    normal_buffer_data[i+8] = normals[(*it)[8]-1][2];
                }

                // uvs
                if(uvs.size() != 0){
                    uv_buffer_data[uvi+0] = uvs[(*it)[1]-1][0];
                    uv_buffer_data[uvi+1] = uvs[(*it)[1]-1][1];
                    uv_buffer_data[uvi+2] = uvs[(*it)[4]-1][0];
                    uv_buffer_data[uvi+3] = uvs[(*it)[4]-1][1];
                    uv_buffer_data[uvi+4] = uvs[(*it)[7]-1][0];
                    uv_buffer_data[uvi+5] = uvs[(*it)[7]-1][1];
                }
                
                i += 9;
                uvi += 6;
            }

            glGenBuffers(1, &vertex_buffer);
            glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
            glBufferData(
                GL_ARRAY_BUFFER,
                sizeof(GLfloat) * numpoints * 3,
                vertex_buffer_data,
                GL_STATIC_DRAW);

            glGenBuffers(1, &uv_buffer);
            glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
            glBufferData(
                GL_ARRAY_BUFFER,
                sizeof(GLfloat) * numpoints * 2,
                uv_buffer_data,
                GL_STATIC_DRAW);
            
            glGenBuffers(1, &normal_buffer);
            glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
            glBufferData(
                GL_ARRAY_BUFFER,
                sizeof(GLfloat) * numpoints * 3,
                normal_buffer_data,
                GL_STATIC_DRAW);
            
        }
        void render(){
            // Vertex data
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
            glVertexAttribPointer(
                0, // layout
                3, // xyz
                GL_FLOAT, // type
                GL_FALSE, // normalized?
                0, // stride
                (void*)0 // array buffer offset
                );
            
            glDrawArrays(GL_TRIANGLES, 0, vertex_num);
            glDisableVertexAttribArray(0);
            
            // UV data
            glEnableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
            glVertexAttribPointer(
                1, // layout
                2, // uv
                GL_FLOAT, // type
                GL_FALSE, // normalized?
                0, // stride
                (void*)0 // array buffer offset
                );

            // normal data
            glEnableVertexAttribArray(2);
            glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
            glVertexAttribPointer(
                2, // layout
                3, // normal
                GL_FLOAT, // type
                GL_FALSE, // normalized?
                0, // stride
                (void*)0 // array buffer offset
                );
        }
        ~Model(){
            delete vertex_buffer_data;
            delete uv_buffer_data;
        }

        GLfloat * vertex_buffer_data;
        GLfloat * normal_buffer_data;
        GLfloat * uv_buffer_data;

    private:
        vector <vec3> vertices;
        vector <vec2> uvs;
        vector <vec3> normals;
        vector <face3> faces3;
        size_t vertex_num;
        GLuint vertex_buffer;
        GLuint normal_buffer;
        GLuint uv_buffer;
    };
    
    Model m;
    Shader shader;
    Camera camera;
    
    void start(int * argc, char ** argv){
        glutInit(argc,argv);
        glClearColor(1.0f,1.0f,0.0f,0.0f);
        glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);        
        glutInitWindowSize(Settings::w,Settings::h);

        camera.w = Settings::w;
        camera.h = Settings::h;
        
        //m.load("models/test_3d.obj");
        //m.load("models/cube.obj");
        //m.load("models/world.obj");
        //m.load("models/building.obj");
        //m.load("models/test_3d_2.obj");
        m.load("models/itu.obj");
        
        auto Resize = [](int w,int h){
            camera.w = w;
            camera.h = h;
        };

        auto Render = [](){
            i++;
            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

            camera.scale = 0.015;
            camera.angle = 0.30;
            camera.x = 0.00;
            camera.y = -0.2;
            camera.z = 0;
            m.render();

            glm::mat4 mvp = camera.model_view_matrix();
            
            GLuint MatrixID =
            glGetUniformLocation(shader.programID, "MVP");
            
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
            
            glFlush();
            glutSwapBuffers();
            usleep(2000);
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

        m.create_buffers();

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        
        glutDisplayFunc(Render);
        glutIdleFunc(Render);

        Image img;
        
        shader.load("vertex.glsl","fragment.glsl");
        shader.bind();
        
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
