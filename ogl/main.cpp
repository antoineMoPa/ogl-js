#include <iostream>
#include <GL/glut.h>

GLvoid Render(){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
}

GLvoid Resize(GLint width,GLint height){
    
}

int main(int argc, char **argv){
    std::cout << "Hey!" << std::endl;
    
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);

    glutCreateWindow("Hey");
    
    glutDisplayFunc(Render);
    glutReshapeFunc(Resize);
    

    
    glutMainLoop();
    return 0;
}
