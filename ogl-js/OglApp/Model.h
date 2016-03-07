#include <array>
#include <vector>
#include <algorithm>
#include "math.h"

namespace OglApp{    
    using namespace std;
    
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
                        // Todo: this is so wrong
                        // We should verify if we
                        // have normals or uvs
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
            if(buffers_created){
                return;
            }
            size_t numpoints = faces3.size() * 3;
            vertex_num = numpoints;
            vertex_buffer_data = new GLfloat[numpoints * 3];
            normal_buffer_data = new GLfloat[numpoints * 3];
            uv_buffer_data = new GLfloat[numpoints * 2];
            
            typedef vector<face3>::iterator faceit;

            int i = 0;
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

            buffers_created = true;
        }
        void render(){
            if(!buffers_created){
                return;
            }
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
            if(vertex_buffer_data == NULL){
                delete vertex_buffer_data;
            }
            if(normal_buffer_data == NULL){
                delete normal_buffer_data;
            }
            if(uv_buffer_data == NULL){
                delete uv_buffer_data;
            }
        }

        GLfloat * vertex_buffer_data = NULL;
        GLfloat * normal_buffer_data = NULL;
        GLfloat * uv_buffer_data = NULL;

    private:
        bool buffers_created = false;
        vector <vec3> vertices;
        vector <vec2> uvs;
        vector <vec3> normals;
        vector <face3> faces3;
        size_t vertex_num;
        GLuint vertex_buffer;
        GLuint normal_buffer;
        GLuint uv_buffer;
    };
}
