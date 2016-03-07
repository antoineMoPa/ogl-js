namespace OglApp{
    /*
      The matrix
    */
    class Matrix{
    public:
        Matrix(){
            Matrix(100,100);
        }
        
        Matrix(int rhs_w, int rhs_h){
            resize(rhs_w,rhs_h);
        }
        
        void resize(int rhs_w, int rhs_h){
            w = rhs_w;
            h = rhs_h;
            
            Projection = glm::perspective(
                glm::radians(80.0f),
                float(w)/float(h),
                1.0f,
                100.0f
                );
            
            View = glm::lookAt(
                glm::vec3(0.0f,0.0f,2.0f), // Camera
                glm::vec3(0.0f,0.0f,0.0f), // Origin
                glm::vec3(0.0f,1.0f,0.0f)  // Vertical axis
                );
        }
        
        void operator=(Matrix rhs){
            Model = rhs.Model;
        }
        
        void clear_model(){
            Model = glm::mat4(1.0f);
        }
        
        void translate(float x, float y, float z){
            Model = glm::translate(Model,glm::vec3(x,y,z));
        }
        void scale(float x, float y, float z){
            Model = glm::scale(Model,glm::vec3(x,y,z));
        }
        
        void rotate(float angle,float x,float y, float z){
            glm::vec3 rotation_axis = glm::vec3(x,y,z);
            Model =
                glm::rotate(Model,angle,rotation_axis);
        }
        
        glm::mat4 model_view_matrix(){
            return
                Projection * View * Model;
        }
        
        glm::mat4 Projection;
        glm::mat4 View;
        glm::mat4 Model = glm::mat4(1.0f);
        
        int w,h;
    };
}
