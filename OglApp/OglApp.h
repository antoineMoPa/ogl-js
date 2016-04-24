#define GLM_FORCE_RADIANS
#define GLM_SWIZZLE

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include <map>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GL/glut.h>
#include <cstdio>
#include <ctime>
#include <unistd.h>
#include <algorithm>
#include <jsapi.h>

#include "platform.h"
#include "Shader.h"

namespace OglApp{
    Shader * current_shader = nullptr;
    Shader post_process_shader;
    int frame_count = 0;
}

#include "Image.h"

namespace OglApp{
    // The texture that we can post process
    // (and render on the quad)
    using TextureMap = std::map<std::string,Image>;
    TextureMap js_textures;

    /*
      Contains data about an opengl framebuffer
      And the texture it gets rendered to
    */
    class FrameBuffer{
    public:
        Image * rendered_tex;
        GLuint fb_id;
        GLuint depth_buf;
    };

    const int pass_total = 3;
    FrameBuffer fbs [pass_total + 1];
}

#include "Model.h"
#include "Matrix.h"
#include "Camera.h"

using namespace std;

namespace OglApp{
    // Default app path
    string app_path = "world/";

    using ModelMap = std::map<std::string,Model>;
    ModelMap models;

    using ShaderMap = std::map<std::string,Shader>;
    ShaderMap shaders;

    void compute_matrix()
    {
        glm::mat4 mvp = camera.mat.model_view_matrix();

        // Update MVP in all shaders

        typedef ShaderMap::iterator map_it;
        for( map_it it = shaders.begin();
             it != shaders.end();
             it++ ){
            Shader * shader = &it->second;
            GLuint MatrixID =
                glGetUniformLocation(shader->ProgramID, "MVP");
            glUniformMatrix4fv(MatrixID,1,GL_FALSE,&mvp[0][0]);
        }
    }

    // Window width
    int w = 100;
    // Window height
    int h = 100;
}

#include "js_functions.h"

/*
  For other JS versions: look at
  https://developer.mozilla.org/en-US/docs/Mozilla/Projects/SpiderMonkey/How_to_embed_the_JavaScript_engine#Original_Document_Information
*/
namespace OglApp{
    // The depth buffer
    GLuint depth_buf;
    FrameBuffer fb;
    
    // The data of the render-to-texture quad
    GLuint quad_vertexbuffer;

    JSContext * cx = NULL;
    // global object
    JS::RootedObject * gl;
    JSRuntime *rt;

    int argc;
    char ** argv;

    /* The class of the global object. */
    static JSClass global_class = {
        "global",
        JSCLASS_GLOBAL_FLAGS,
        JS_PropertyStub,
        JS_DeletePropertyStub,
        JS_PropertyStub,
        JS_StrictPropertyStub,
        JS_EnumerateStub,
        JS_ResolveStub,
        JS_ConvertStub
    };

    /**
       Window resize callback
     */
    static void resize(int rhs_w, int rhs_h){
        w = rhs_w;
        h = rhs_h;
        camera.mat.resize(w,h);
    }

    /**
       Things to do when we kill JS
     */
    static void stop(){
        JS_DestroyContext(cx);
        JS_DestroyRuntime(rt);
        JS_ShutDown();
    }

    void main_render(){
        glViewport(0,0,w,h);
        
        // Actual rendering
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        // Reset camera transforms
        camera.mat.clear_model();
        
        current_shader = &shaders[string("default")];
        current_shader->bind();
        fbs[0].rendered_tex->bind(3,"rendered_tex");
        
        // return value and empty arg
        JS::RootedValue rval(cx);
        JS::AutoValueVector argv(cx);

        // Call javascript "render" function
        // defined in the app's main.js
        JSBool ok = JS_CallFunctionName(
            cx,
            *gl,
            "render",
            0,
            argv.begin(),
            rval.address()
            );

        glFlush();        
    }

    void post_process_render(int pass){
        glViewport(0,0,w,h);
        
        // Add timestamp
        GLuint loc = post_process_shader
            .get_uniform_location("time");
        
        // Bind timestamp to variable
        glUniform1i(loc,get_timestamp());

        // Add render pass number
        loc = post_process_shader
            .get_uniform_location("pass");
        
        // Bind pass number
        glUniform1i(loc,pass);

        // Add frame count
        loc = post_process_shader
            .get_uniform_location("frame_count");

        glUniform1i(loc,frame_count);
            
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        // Render the plane
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDisableVertexAttribArray(0);

        glFlush();
    }
    
    static void render(){
        // Prepare to render to a texture
        glBindFramebuffer(GL_FRAMEBUFFER, fbs[0].fb_id);
        main_render();

        int pass;
        GLuint pps = post_process_shader.get_id();
        int last_fb = 0;
        int curr_fb;

        post_process_shader.bind();
        
        /*
          Render each pass
          Bind texture of last pass to pass_0/pass_1/etc.
          pass_0 is the main rendered scene
         */
        for(pass = 1; pass <= pass_total; pass++){
            curr_fb = pass;
            
            string num = "0";
            num[0] += pass - 1;
            string pass_name("pass_");
            pass_name += num;

            // bind last texture
            fbs[last_fb]
                .rendered_tex->bind(pps,3,"last_pass");

            fbs[last_fb]
                .rendered_tex->bind(pps,3 + pass,pass_name.c_str());

            if(pass != pass_total){
                // Render texture on a plane
                glBindFramebuffer( GL_FRAMEBUFFER,
                                   fbs[curr_fb].fb_id );
            } else {
                // Last pass: render to screen
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
            }
            
            post_process_render(pass);
            last_fb = curr_fb;
        }

        glutSwapBuffers();
        frame_count++;
    }
    
    static void keyboard(unsigned char key, int x, int y){
        cout << "key " << key
             << " x: " << x
             << " y: " << y
             << endl;
    }

    static void load_default_shaders(){
        // default shaders
        char * vertex_path =
            strdup("./vertex.glsl");
        char * frag_path =
            strdup("./fragment.glsl");

        Shader s;
        cout << frag_path;
        cout << vertex_path;
        if(!s.load(vertex_path,frag_path)){
            cout << "No default vertex & fragment shader found." << endl;
            exit(0);
            return;
        }
        s.bind();

        using new_el = ShaderMap::value_type;

        shaders.insert(new_el("default",s));
    }

    static bool run_file(const char * filename, JS::RootedValue * rval){
        int lineno = 0;

        ifstream file;

        file.open(filename);

        // Take the content of the file
        // and put it in a string
        stringstream strStream;
        strStream << file.rdbuf();
        string str = strStream.str();
        const char * script = str.c_str();

        bool ok = JS_EvaluateScript
            (
             cx,
             *gl,
             script,
             strlen(script),
             filename,
             lineno,
             rval->address()
             );

        return ok;
    }
    
    static FrameBuffer init_render_buffers(){
        FrameBuffer new_fb;
        
        // Create framebuffer
        glGenFramebuffers(1, &new_fb.fb_id);
        glGenRenderbuffers(1, &new_fb.depth_buf);
        glBindFramebuffer(GL_FRAMEBUFFER,new_fb.fb_id);

        new_fb.rendered_tex = new Image(w,h);

        // Poor filtering. Needed !
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glFramebufferTexture2D(
                               GL_FRAMEBUFFER,
                               GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D,
                               new_fb.rendered_tex->get_id(),0);

        GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
        glDrawBuffers(1, DrawBuffers);

        glBindRenderbuffer(GL_RENDERBUFFER, new_fb.depth_buf);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);

        glFramebufferRenderbuffer(
                                  GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, new_fb.depth_buf
            );

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
            cerr << "Framebuffer setup error" << endl;
        }

        return new_fb;
    }

    /**
       Creates the plane that will be used to render everything on
     */
    static void create_render_quad(){
        GLuint quad_vertex_array_id;
        // Create a quad
        glGenVertexArrays(1, &quad_vertex_array_id);
        glBindVertexArray(quad_vertex_array_id);

        static const GLfloat quad_vertex_buffer_data[] = {
            -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            -1.0f,  -1.0f, 0.0f,
            1.0f,  -1.0f, 0.0f
        };

        // Put the data in buffers
        glGenBuffers(1, &quad_vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(quad_vertex_buffer_data),
                     quad_vertex_buffer_data,
                     GL_STATIC_DRAW);
    }

    static void apploop(){
        glutInit(&argc,argv);
        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
        glutInitWindowSize(w,h);

        glutCreateWindow("ogl-js");

        // http://gamedev.stackexchange.com/questions/22785/
        GLenum err = glewInit();
        if (err != GLEW_OK){
            cout << "GLEW error: " << err << endl;
            cout <<  glewGetErrorString(err) << endl;
        }

        load_default_shaders();

        // Listen to the keyboard
        glutKeyboardFunc((*keyboard));

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        // Alpha mixing setup
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);

        // Load post processing shaders
        post_process_shader.load(
            (app_path+"post-vertex.glsl").c_str(),
            (app_path+"post-fragment.glsl").c_str());

        // Setup javascript global context
        // Load main.js containing console.log()
        JS::RootedValue rval(cx);
        run_file((app_path+"main.js").c_str(),&rval);

        // Assign callbacks
        glutReshapeFunc(resize);
        glutDisplayFunc(render);
        glutIdleFunc(render);

        // Create the plane for render-to-texture
        create_render_quad();
        
        // Init buffers for render-to-texture
        // and post process
        for(int i = 0; i < pass_total + 1; i++){
            fbs[i] = init_render_buffers();
        }
        
        // The app becomes alive here
        glutMainLoop();
    }

    static void dispatchError(
                              JSContext* cx,
                              const char* message,
                              JSErrorReport* report) {
        cout << "Javascript error at line "
             << report->lineno << " column "
             << report->column << endl
             << message << endl
             << "'" << report->linebuf << "'" << endl;
    }

    static int initJavascript(void (*after_run_callback)(void)){
        rt = JS_NewRuntime(8L * 1024 * 1024, JS_USE_HELPER_THREADS);
        if (!rt)
            return 1;

        cx = JS_NewContext(rt, 8192);

        JS_SetErrorReporter(cx, &OglApp::dispatchError);

        if (!cx)
            return 1;

        JS::RootedValue rval(cx);

        {
            // Scope for our various stack objects
            // (JSAutoRequest, RootedObject), so they all go
            // out of scope before we JS_DestroyContext.
            JSAutoRequest ar(cx);
            // In practice, you would want to exit this any
            // time you're spinning the event loop

            JS::RootedObject global(cx, JS_NewGlobalObject(cx, &global_class, nullptr));

            gl = &global;

            if (!global)
                return 1;

            // Scope for JSAutoCompartment
            {
                JSAutoCompartment ac(cx, global);
                JS_InitStandardClasses(cx, global);
                /*
                  function name in js,
                  c++ function,
                  argument count,
                  flags
                 */
                static JSFunctionSpec my_functions[] = {
                    JS_FN("plus", jsfn::plus, 2, 0),
                    JS_FN("translate", jsfn::translate, 3, 0),
                    JS_FN("rotate", jsfn::rotate, 4, 0),
                    JS_FN("triangle_strip", jsfn::triangle_strip, 1, 0),
                    JS_FN("color", jsfn::color, 4, 0),
                    JS_FN("render_model", jsfn::render_model, 1, 0),
                    JS_FN("load_shaders", jsfn::load_shaders, 3, 0),
                    JS_FN("bind_shaders", jsfn::bind_shaders, 1, 0),
                    JS_FN("create_triangle_array", jsfn::create_triangle_array, 4, 0),
                    JS_FN("render_triangle_array", jsfn::render_triangle_array, 1, 0),
                    JS_FN("scale", jsfn::scale, 3, 0),
                    JS_FN("divide", jsfn::divide, 2, 0),
                    JS_FN("log", jsfn::log, 1, 0),
                    JS_FN("push_matrix", jsfn::push_matrix, 0, 0),
                    JS_FN("pop_matrix", jsfn::pop_matrix, 0, 0),
                    JS_FN("create_texture",jsfn::create_texture,3,0),
                    JS_FN("window_width",jsfn::window_width,3,0),
                    JS_FN("window_height",jsfn::window_height,3,0),
                    JS_FS_END
                };
                
                bool ok = JS_DefineFunctions(cx, global, my_functions);
                if(!ok){
                    cout << "Function definition error" << endl;
                    return 1;
                }

                run_file("jslib/main.js",&rval);

                // Now we can call functions from
                // the script
                after_run_callback();

                if(!ok){
                    return 1;
                }
            }
            if(rval.isString()){
                JSString *str = rval.toString();
                char * str2 = JS_EncodeString(cx, str);
                printf("%s\n", str2);
            }
        }

        stop();
    }

    static void start(int _argc, char ** _argv){
        argc = _argc;
        argv = _argv;
        if(argc >= 2){
            app_path = argv[1];
            if(app_path[app_path.size()-1] != '/'){
                app_path += '/';
            }
        }
        initJavascript(apploop);
    }
}
