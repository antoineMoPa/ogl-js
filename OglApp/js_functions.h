/*
  Boring TODO if this file becomes important:
  type checks for each function
  (ensure we don't crash if the parameter is not
  of the good type)
*/

using namespace std;
using namespace OglApp;

namespace jsfn{
    
    class FloatArrayTracker{
    public:
        ~FloatArrayTracker(){
            delete vertex_buffer_data;
            delete normal_buffer_data;
            delete uv_buffer_data;
        }
        GLuint vertex_id;
        GLuint normal_id;
        GLuint uv_id;
        int vertex_size;
        int normal_size;
        int uv_size;
        GLfloat * vertex_buffer_data;
        GLfloat * normal_buffer_data;
        GLfloat * uv_buffer_data;
    };
    
    using GLuintMap = std::map<std::string, FloatArrayTracker*>;
    GLuintMap float_arrays;
    
    static JSBool
        plus(JSContext *cx, unsigned argc, jsval *vp)
    {
        JS::CallArgs args = CallArgsFromVp(argc, vp);
        args.rval().setInt32(args[0].toInt32() + args[1].toInt32());
        return true;
    }
    
    static JSBool
        divide(JSContext *cx, unsigned argc, jsval *vp)
    {
        JS::CallArgs args = CallArgsFromVp(argc, vp);
        args.rval().setInt32(args[0].toInt32() / args[1].toInt32());
        return true;
    }

    static JSBool
        translate(JSContext *cx, unsigned argc, jsval *vp)
    {
        JS::CallArgs args = CallArgsFromVp(argc, vp);
        get_camera().mat.translate(
                     args[0].toNumber(),
                     args[1].toNumber(),
                     args[2].toNumber()
                     );
        compute_matrix();
        return true;
    }

    /**
       Pass a variable to the GLSL shader
     */
    static JSBool
        shader_var(JSContext *cx, unsigned argc, jsval *vp)
    {
        JS::CallArgs args = CallArgsFromVp(argc, vp);

        if(!args[0].isString()){
            return false;
        }

        const char * str =
            JS_EncodeString(cx,args[0].toString());

        GLuint loc = OglApp::post_process_shader
            .get_uniform_location(str);

        if(args[1].isInt32()){
            glUniform1i(loc,args[1].toInt32());
        } else if (args[1].isNumber()){
            glUniform1f(loc,args[1].toNumber());
        }
        

        return true;
    }

    /**
       Returns window width
     */
    static JSBool
        window_width(JSContext *cx, unsigned argc, jsval *vp)
    {
        JS::CallArgs args = CallArgsFromVp(argc, vp);
        args.rval().setInt32(OglApp::w);
        return true;
    }

    /**
       Returns window height
     */
    static JSBool
        window_height(JSContext *cx, unsigned argc, jsval *vp)
    {
        JS::CallArgs args = CallArgsFromVp(argc, vp);
        args.rval().setInt32(OglApp::h);
        return true;
    }

    /**
       Creates a texture for use in GLSL shaders
       
       Args: 
       
       name
       width
       height
     */
    static JSBool
        create_texture(JSContext *cx, unsigned argc, jsval *vp)
    {
        JS::CallArgs args = CallArgsFromVp(argc, vp);

        if(!args[0].isString()){
            return false;
        }

        int w = args[1].toInt32();
        int h = args[2].toInt32();
        
        const char * str =
            JS_EncodeString(cx,args[0].toString());
        
        string index(str);
        
        OglApp::js_textures[index].init(w,h);
        
        return true;
    }
    
    static JSBool
        rotate(JSContext *cx, unsigned argc, jsval *vp)
    {
        JS::CallArgs args = CallArgsFromVp(argc, vp);

        camera.mat.rotate(
                  args[0].toNumber(),
                  args[1].toNumber(),
                  args[2].toNumber(),
                  args[3].toNumber()
                  );
        compute_matrix();
        return true;
    }
    
    static JSBool
        scale(JSContext *cx, unsigned argc, jsval *vp)
    {
        JS::CallArgs args = CallArgsFromVp(argc, vp);
        camera.mat.scale(
                 args[0].toNumber(),
                 args[1].toNumber(),
                 args[2].toNumber()
                 );
        compute_matrix();
        return true;
    }

    static JSBool
        push_matrix(JSContext *cx, unsigned argc, jsval *vp)
    {
        camera.push();
        return true;
    }

    static JSBool
        pop_matrix(JSContext *cx, unsigned argc, jsval *vp)
    {
        camera.pop();
        return true;
    }
    
    /**
       OpenGL triangle strip
    */
    static JSBool
        triangle_strip(JSContext *cx, unsigned argc, jsval *vp)
    {
        JS::CallArgs args = CallArgsFromVp(argc, vp);

        // Will be the length of the array
        uint32_t len;
        if(args.length()<1 || !args[0].isObject())
        {
            cerr << "Arg is not an Object" << endl;
            return false;
        }
        JSObject * arr = args[0].toObjectOrNull();
        JSClass * cl = JS_GetClass(arr);
        if(!cl || 0 != strcmp("Array",cl->name))
        {
            cerr << "Arg is not an Array" << endl;
            return false;
        }
        JS_GetArrayLength(cx,arr,&len);

        // We will not use last elements
        // if they are not in a group of 3
        len = len - len % 3;
        
        // Will hold single element from array
        JS::Value el;

        // Will hold parameters
        float a,b,c;

        glBegin(GL_TRIANGLE_STRIP);
        for(uint32_t i = 0; i < len; i+=3){
            // get array elements
            JS_GetElement(cx, arr, i, &el);
            a = el.toNumber();
            JS_GetElement(cx, arr, i+1, &el);
            b = el.toNumber();
            JS_GetElement(cx, arr, i+2, &el);
            c = el.toNumber();
            glVertex3f(a,b,c);
        }

        glEnd();
        return true;
    }
    
    /**
       Set color
    */
    static JSBool
        color(JSContext *cx, unsigned argc, jsval *vp)
    {
        JS::CallArgs args = CallArgsFromVp(argc, vp);
        /*glColor4f(
                  args[0].toNumber(),
                  args[1].toNumber(),
                  args[2].toNumber(),
                  args[3].toNumber()
                  );*/
        return true;
    }

    /**
       Load .obj file and render it
       (Or render it if previously called)
    */
    static JSBool
        render_model(JSContext *cx, unsigned argc, jsval *vp)
    {
        JS::CallArgs args = CallArgsFromVp(argc, vp);
        
        if(!args[0].isString()){
            return false;
        }
        
        const char * str =
                JS_EncodeString(cx,args[0].toString());

        string index(str);
        
        if(models.find(index) == models.end()){
            // Load model
            Model * m = &models[index];
            m->load((app_path + index).c_str());
            
            using new_el = ModelMap::value_type;
            
            m->create_buffers();
        }
        
        Model * m = &models[index];
        m->render();
        
        return true;
    }

    /**
     */
    static JSBool
        load_shaders(JSContext *cx, unsigned argc, jsval *vp)
    {
        JS::CallArgs args = CallArgsFromVp(argc, vp);
        
        if(!args[0].isString()){
            return false;
        }
        if(!args[1].isString()){
            return false;
        }
        if(!args[2].isString()){
            return false;
        }

        const char * index_cstr =
            JS_EncodeString(cx,args[0].toString());
        
        const char * vert_cstr =
            JS_EncodeString(cx,args[1].toString());

        const char * frag_cstr =
            JS_EncodeString(cx,args[2].toString());

        string index(index_cstr);
        string vert_file(vert_cstr);
        string frag_file(frag_cstr);
        
        if(shaders.find(index) == shaders.end()){
            // Load shader
            Shader s;
            s.load(
                   (app_path + vert_cstr).c_str(),
                   (app_path + frag_cstr).c_str()
                   );
            
            using new_el = ShaderMap::value_type;
            OglApp::shaders.insert(new_el(index,s));
        } else {
            cerr << "Shader already loaded" << endl;
            return false;
        }
        return true;
    }

    /**
     */
    static JSBool
        bind_shaders(JSContext *cx, unsigned argc, jsval *vp)
    {
        JS::CallArgs args = CallArgsFromVp(argc, vp);

        if(!args[0].isString()){
            return false;
        }
        
        const char * index_cstr =
            JS_EncodeString(cx,args[0].toString());
        
        string index(index_cstr);
        
        ShaderMap::iterator desired = shaders.find(index);
        
        if(desired == shaders.end()){
            cerr << "No shader with index '"
                 << index << "'" << endl;
            return false;
        }
        
        desired->second.bind();
        OglApp::current_shader = &desired->second;
        
        compute_matrix();
        
        return true;
    }

    /**
       4 js args: name, data
       name is used to keep track of the data in float_arrays
       vertex position float
       normal floats
       uv floats
       
       TODO:
       
       - being able to delete object
       - verify if object already exists (before overwriting)
       - delete previous data if object already exists
       
     */
    static JSBool
        create_triangle_array(JSContext *cx, unsigned argc, jsval *vp)
    {
        JS::CallArgs args = CallArgsFromVp(argc, vp);

        // Verify arguments
        // string
        if(!args[0].isString()){
            return false;
        }
        // vertex data
        if(!args[1].isObject() ||
           !args[2].isObject() ||
           !args[3].isObject()){
            return false;
        }

        // Get name
        const char * index_cstr =
            JS_EncodeString(cx,args[0].toString());
        
        string index(index_cstr);
        
        GLuintMap::iterator desired = float_arrays.find(index);
        
        if(desired != float_arrays.end()){
            cerr << "Array with name '"
                 << index << "' already exist."
                 << endl;
            return false;
        }

        // Create tracker for ids and data
        FloatArrayTracker * tracker = new FloatArrayTracker();
        float_arrays[index] = tracker;

        // fetch arrays
        JSObject * vertex = &args[1].toObject();
        JSObject * normal = &args[2].toObject();
        JSObject * uv     = &args[3].toObject();
        
        // verify if really an array
        if(!JS_IsArrayObject(cx,vertex)){
            cerr << "vertex data is not an array" << endl;
            return false;
        }
        if(!JS_IsArrayObject(cx,normal)){
            cerr << "normal data is not an array" << endl;
            return false;
        }
        if(!JS_IsArrayObject(cx,uv)){
            cerr << "uv is not an array" << endl;
            return false;
        }

        // Used to fetch size
        uint32_t vertex_size;
        uint32_t normal_size;
        uint32_t uv_size;
        
        // Get size
        JS_GetArrayLength(cx, vertex, &vertex_size);
        JS_GetArrayLength(cx, normal, &normal_size);
        JS_GetArrayLength(cx, uv, &uv_size);

        cout << "Creating arrays" << endl;

        tracker->vertex_buffer_data = new GLfloat[vertex_size];
        tracker->normal_buffer_data = new GLfloat[normal_size];
        tracker->uv_buffer_data = new GLfloat[uv_size];
        
        // Current element in loop
        JS::Value element;
        
        for(int i = 0; i < vertex_size; i++){
            JS_GetElement(cx, vertex, i, &element);
            tracker->vertex_buffer_data[i] = element.toNumber();
        }
        for(int i = 0; i < normal_size; i++){
            JS_GetElement(cx, normal, i, &element);
            tracker->normal_buffer_data[i] = element.toNumber();
        }
        for(int i = 0; i < uv_size; i++){
            JS_GetElement(cx, uv, i, &element);
            tracker->uv_buffer_data[i] = element.toNumber();
        }
        
        tracker->vertex_size = vertex_size;
        tracker->normal_size = normal_size;
        tracker->uv_size = uv_size;
        
        glGenBuffers(1, &tracker->vertex_id);
        glBindBuffer(GL_ARRAY_BUFFER, tracker->vertex_id);
        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(GLfloat) * vertex_size,
            tracker->vertex_buffer_data,
            GL_STATIC_DRAW
        );

        glGenBuffers(1, &tracker->normal_id);
        glBindBuffer(GL_ARRAY_BUFFER, tracker->normal_id);
        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(GLfloat) * normal_size,
            tracker->normal_buffer_data,
            GL_STATIC_DRAW
        );

        glGenBuffers(1, &tracker->uv_id);
        glBindBuffer(GL_ARRAY_BUFFER, tracker->uv_id);
        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(GLfloat) * uv_size,
            tracker->uv_buffer_data,
            GL_STATIC_DRAW
        );

        return true;
    }

    /**
       Argument:
       the index of the buffer (string)
       TODO:
       this, but for quads and stuff
       this, but for normals
     */
    static JSBool
        render_triangle_array(JSContext *cx, unsigned argc, jsval *vp)
    {
        JS::CallArgs args = CallArgsFromVp(argc, vp);

        // Verify arguments
        // string
        if(!args[0].isString()){
            return false;
        }
        
        const char * index_cstr =
            JS_EncodeString(cx,args[0].toString());
        
        string index(index_cstr);
        
        GLuintMap::iterator desired = float_arrays.find(index);
        
        if(desired == float_arrays.end()){
            cerr << "Array with name '"
                 << index << "' not found."
                 << endl;
            return false;
        }

        GLuint vertex_id = desired->second->vertex_id;
        int vertex_size = desired->second->vertex_size;
        GLuint normal_id = desired->second->normal_id;
        int normal_size = desired->second->normal_size;
        GLuint uv_id = desired->second->uv_id;
        int uv_size = desired->second->uv_size;
        
        glEnableVertexAttribArray(UV_SLOT);
        glBindBuffer(GL_ARRAY_BUFFER, uv_id);
        glVertexAttribPointer(
            UV_SLOT, // layout
            2,
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0, // stride
            (void*)0 // array buffer offset
            );

        glEnableVertexAttribArray(NORMAL_SLOT);
        glBindBuffer(GL_ARRAY_BUFFER, normal_id);
        glVertexAttribPointer(
            NORMAL_SLOT, // layout
            3,
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0, // stride
            (void*)0 // array buffer offset
            );

        glEnableVertexAttribArray(VERTEX_SLOT);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_id);
        glVertexAttribPointer(
            VERTEX_SLOT, // layout
            3,
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0, // stride
            (void*)0 // array buffer offset
            );
        
        glDrawArrays(GL_TRIANGLES, 0, vertex_size/3);
        
        glDisableVertexAttribArray(VERTEX_SLOT);
        
        return true;
    }

    /**
       Enable 2 pass post process.
       
       In this mode, post-fragment.glsl shader will
       receive:
       
       uniform sampler2D last_pass;
       uniform sampler2D pass_0; // render result
       uniform sampler2D pass_1; // result of first pass
       uniform sampler2D pass_2; // result of second pass
       uniform int time;
       uniform int pass; // current pass
       [...]
       At every pass, the shader is run. 
       You have access to the last pass color in a texture.
       (You have to set color at each pass)
       
    */
    static JSBool
        enable_2_pass_pp(JSContext *cx, unsigned argc, jsval *vp)
    {

        OglApp::enable_2_pass_pp = true;
        cout << "2 pass post processing enabled." << endl;
        
        return true;
    }
    
    /**
       log strings
     */
    static JSBool
        log(JSContext *cx, unsigned argc, jsval *vp)
    {
        JS::CallArgs args = CallArgsFromVp(argc, vp);
        if(args[0].isString()){
            const char * str = JS_EncodeString(cx,args[0].toString());
            cout << str << endl;
        } else {
            cout << "[trying to log something that is not a string]"
                 << endl;
        }
        return true;
    }
}
