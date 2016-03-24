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
        GLuint buffer_id;
        int size;
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
       2 js args: name, data
       name is used to keep track of the data in
       float_arrays
       
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
        // data
        if(!args[1].isObject()){
            return false;
        }

        // fetch array
        JSObject * arr = &args[1].toObject();

        // verify if really an array
        if(!JS_IsArrayObject(cx,arr)){
            cerr << "second argument is not an array" << endl;
            return false;
        }

        // Used to fetch length
        uint32_t length;
        
        // Get length
        JS_GetArrayLength(cx, arr, &length);
        
        cout << "Creating array" << endl;

        // Temporary buffer data
        GLfloat * buffer_data;

        buffer_data = new GLfloat[length];
        
        // Current element in loop
        JS::Value element;
        
        for(int i = 0; i < length; i++){
            JS_GetElement(cx, arr, i, &element);
            buffer_data[i] = element.toNumber();
        }
        
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

        FloatArrayTracker * tracker = new FloatArrayTracker();
        float_arrays[index] = tracker;
        tracker->size = length;
        
        glGenBuffers(1, &tracker->buffer_id);
        glBindBuffer(GL_ARRAY_BUFFER, tracker->buffer_id);
        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(GLfloat) * length,
            buffer_data,
            GL_STATIC_DRAW
        );

        return true;
    }

    /**
       Argument:
       the index of the shader (string)
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

        GLuint type = GL_TRIANGLES;
        GLuint slot = VERTEX_SLOT;

        // Todo: unhardcode this
        GLuint id = desired->second->buffer_id;
        int size = desired->second->size;
        int indices = 3;
        
        glEnableVertexAttribArray(slot);
        
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glVertexAttribPointer(
            slot, // layout
            indices,
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0, // stride
            (void*)0 // array buffer offset
            );
        glDrawArrays(type, 0, size);
        glDisableVertexAttribArray(VERTEX_SLOT);
        
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
