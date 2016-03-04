namespace OglApp{
    class Camera{
    public:
        Camera(int w, int h){
            mat = Matrix(w,h);
        }
        void push(){
            matrix_stack.push_back(mat);
        }
        void pop(){
            if(!matrix_stack.empty()){
                mat = matrix_stack.back();
                matrix_stack.pop_back();
            }
        }
        Matrix mat;
    private:
        vector <Matrix> matrix_stack;
    };

    Camera camera(100,100);
    
    static Camera& get_camera(){
        return camera;
    }
}
