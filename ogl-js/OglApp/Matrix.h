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
            clear_model();
        }
        
        void resize(int rhs_w, int rhs_h){
            w = rhs_w;
            h = rhs_h;
            
            Projection = glm::perspective(
                glm::radians(45.0f),
                float(w)/float(h),
                0.1f,
                100.0f
                );
            
            View = glm::lookAt(
                glm::vec3(0,0,2), // Camera
                glm::vec3(0,0,0), // Origin
                glm::vec3(0,1,0)  // Vertical axis
                );
        }
        
        Matrix operator=(Matrix rhs){
            Model = rhs.Model;
            return *this;
        }
        
        void clear_model(){
            Model = glm::mat4(1.0f);
        }
        
        void translate(float x, float y, float z){
            glm::mat4 Translate = glm::translate(glm::vec3(x,y,z));
            Model = Translate * Model;
        }
        void scale(float x, float y, float z){
            glm::mat4 Scale = glm::scale(glm::vec3(x,y,z));
            Model = Scale * Model;
        }
        
        void rotate(float angle,float x,float y, float z){
            glm::vec3 rotation_axis(x,y,z);
            glm::mat4 Rotation = glm::rotate(angle, rotation_axis);
            Model = Rotation * Model;
        }
        
        glm::mat4 model_view_matrix(){
            return
                Projection * View * Model;
        }
        
        glm::mat4 Projection;
        glm::mat4 View;
        glm::mat4 Model;
        
        int w,h;
    };
}
