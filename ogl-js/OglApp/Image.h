using namespace std;

/*
  Thanks to
  http://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/
*/
class Image{
public:
    bool load(const char * filename){
        FILE * file = fopen(filename,"rb");
        if(!file){
            cout << "Unable to read file '" << filename << "'."<< endl;
            fclose(file);
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
        if(data != nullptr){
            delete data;
        }
    }
    
    GLuint textureID;
    unsigned char header[54];
    unsigned int dataPos;
    unsigned int width, height;
    unsigned int imageSize; // width*height*3
    unsigned char * data = nullptr;
};
