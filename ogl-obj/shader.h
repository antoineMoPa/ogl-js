#include <GL/gl.h>
#include <GL/glut.h>

class Shader{
public:
    /**
       shader type is either:
       GL_VERTEX_SHADER
       GL_FRAGMENT_SHADER
    */
    GLuint load_shader(const char * file_path,GLenum shaderType){
        // Create the shaders
        GLuint ShaderID = glCreateShader(shaderType);
        
        std::string ShaderCode;
        std::ifstream ShaderStream(file_path, std::ios::in);
        if(ShaderStream.is_open()){
            std::string Line = "";
            while(getline(ShaderStream, Line))
                ShaderCode += "\n" + Line;
            ShaderStream.close();
        }else{
            printf("Impossible to open %s.\n", file_path);
            getchar();
            return 0;
        }
        
        GLint Result = GL_FALSE;
        int InfoLogLength;

        printf("Compiling shader : %s\n", file_path);
        
        if(shaderType == GL_FRAGMENT_SHADER){
            char const * FragmentSourcePointer = ShaderCode.c_str();
            glShaderSource(ShaderID, 1, &FragmentSourcePointer, NULL);
        }
        else if (shaderType == GL_VERTEX_SHADER){
            char const * VertexSourcePointer = ShaderCode.c_str();
            glShaderSource(ShaderID, 1, &VertexSourcePointer, NULL);
        }
        
        glCompileShader(ShaderID);
        
        glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

        if ( InfoLogLength > 0 ){
            std::vector<char> err_msg(InfoLogLength+1);
            glGetShaderInfoLog(ShaderID, InfoLogLength, NULL, &err_msg[0]);
            printf("%s\n", &err_msg[0]);
        }
        return ShaderID;
    }

    bool load(const char * vertex_path, const char * fragment_path){
        if(used){
            std::cout << "This shader has already been used." << std::endl;
            std::cout << "Create a new one." << std::endl;
            return false;
        }
        
        GLuint VertexShaderID = load_shader(vertex_path,GL_VERTEX_SHADER);
        GLuint FragmentShaderID = load_shader(fragment_path,GL_FRAGMENT_SHADER);
        
        // Link the program
        printf("Linking program\n");
        ProgramID = glCreateProgram();
        glAttachShader(ProgramID, VertexShaderID);
        glAttachShader(ProgramID, FragmentShaderID);
        glLinkProgram(ProgramID);
        
        GLint Result = GL_FALSE;
        int InfoLogLength;

        // Check the program
        glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
        glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        
        if ( InfoLogLength > 0 ){
            std::vector<char> prog_err_msg(InfoLogLength+1);
            glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &prog_err_msg[0]);
            printf("%s\n", &prog_err_msg[0]);
        }
        
        glDetachShader(ProgramID, VertexShaderID);
        glDetachShader(ProgramID, FragmentShaderID);
        glDeleteShader(VertexShaderID);
        glDeleteShader(FragmentShaderID);
        
        used = true;

        return ProgramID;
    } 

    bool bind(){
        glUseProgram(ProgramID);
        return true;
    }

    bool used = false;
    GLuint ProgramID = 0;
};
