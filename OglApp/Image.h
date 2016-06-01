using namespace std;

/*
  Thanks to
  http://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/
*/
class Image{
public:
    Image(){

    }
    
    /* generate empty image */
    Image(int w, int h){
        init(w,h);
    }

    void init(int w, int h){
        // Create framebuffer texture
        glGenTextures(1, &textureID);

        resize(w, h);
    }

    void delete_ressources(){
        glDeleteTextures(1, &textureID);
    }

    void resize(int w, int h){
        glBindTexture(GL_TEXTURE_2D, textureID);
        
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            w,h,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            0);
    }
    
    GLuint get_id(){
        return textureID;
    }

    /* Loads a bitmap (.bmp) file 
       Todo: Load other formats (like png)
     */
    bool load(const char * filename){
        FILE * file = fopen(filename,"rb");
        if(!file){
            cout << "Unable to read file '" << filename << "'."<< endl;
            return false;
        }
        
        if(fread(header,1,54,file) != 54){
            cout << "Bad file header." << endl;
            fclose(file);
            return false;
        }
        
        if(header[0] != 'B' || header[1] != 'M'){
            cout << "Bad file header." << endl;
            fclose(file);
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

        generate();
        
        return true;
    }
    
    void generate(){
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(
            GL_TEXTURE_2D,
            0, // level (for mipmap stuff)
            GL_RGB, // Internal format
            width,
            height,
            0, // Doc says:  "This value must be 0." ...
            GL_RGB;, // Format
            GL_UNSIGNED_BYTE,
            data);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        
        // Mipmap
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    void bind(){
        glBindTexture(GL_TEXTURE_2D, textureID);
    }

    void bind(GLuint index,const char * name){
        GLuint shader_id = OglApp::current_shader->get_id();
        bind(shader_id, index, name);
    }
    
    void bind(GLuint shader_id, GLuint index,const char * name){
        GLuint loc = glGetUniformLocation(shader_id, name);
        glActiveTexture(GL_TEXTURE0 + index);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glUniform1i(loc,index);
    }
    
    ~Image(){
        if(data != nullptr){
            delete data;
        }
        delete_ressources();
    }
    
    GLuint textureID;
    unsigned char header[54];
    unsigned int dataPos;
    unsigned int width, height;
    unsigned int imageSize; // width*height*3
    unsigned char * data = nullptr;
};
