#define GLM_FORCE_RADIANS

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
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
#include <cstdio>

#include "Model.h"
#include "Shader.h"
#include "Matrix.h"
#include "Camera.h"
#include "Image.h"

#include "js_functions.h"

/*
  For other JS versions: look at
  https://developer.mozilla.org/en-US/docs/Mozilla/Projects/SpiderMonkey/How_to_embed_the_JavaScript_engine#Original_Document_Information
*/

using namespace std;

namespace OglApp{    
    int w = 100;
    int h = 100;
    int i = 0;
    
    JSContext * cx = NULL;
    // global object
    JS::RootedObject * gl;

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

    static void resize(int rhs_w, int rhs_h){
        w = rhs_w;
        h = rhs_h;
        camera.mat.resize(w,h);
    }

    static void render(){
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        glLoadIdentity();
        gluPerspective(
                       80,
                       float(w)/float(h),
                       1,
                       100
                       );

        gluLookAt(0.0f,0.0f,2.0f,
                  0.0f,0.0f,0.0f,
                  0.0f,1.0f,0.0f);

        glTranslatef(-0.4,-1,0);
        glScalef(0.1,0.1,0.1);
        
        JS::RootedValue rval(cx);
        JS::AutoValueVector argv(cx);
        
        JS_CallFunctionName(
                            cx,
                            *gl,
                            "render",
                            0,
                            argv.begin(),
                            rval.address()
                            );

        glFlush();
        glutSwapBuffers();
    }

    static void keyboard(unsigned char key, int x, int y){
        cout << "key " << key
             << " x: " << x
             << " y: " << y
             << endl;
    }

    static void apploop(){
        glutInit(&argc,argv);
        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
        glutInitWindowSize(w,h);

        
        glutCreateWindow("Hey");

        http://gamedev.stackexchange.com/questions/22785/
        GLenum err = glewInit();
        if (err != GLEW_OK){
            cout << "GLEW error: " << err << endl;
            cout <<  glewGetErrorString(err) << endl;
            exit(1);
        }

        glutKeyboardFunc((*keyboard));

        glEnable(GL_DEPTH_TEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);

        glutReshapeFunc(resize);
        glutDisplayFunc(render);
        glutIdleFunc(render);

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

    static int initJavascript(void (*after_run_callback)(void)){
        JSRuntime *rt = JS_NewRuntime(8L * 1024 * 1024, JS_USE_HELPER_THREADS);
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

                static JSFunctionSpec my_functions[] = {
                    JS_FN("plus", jsfn::plus, 2, 0),
                    JS_FN("translate", jsfn::translate, 3, 0),
                    JS_FN("rotate", jsfn::rotate, 4, 0),
                    JS_FN("triangle_strip", jsfn::triangle_strip, 1, 0),
                    JS_FN("color", jsfn::color, 4, 0),
                    JS_FN("model_test", jsfn::model_test, 0, 0),
                    JS_FN("scale", jsfn::scale, 3, 0),
                    JS_FN("divide", jsfn::divide, 2, 0),
                    JS_FN("log", jsfn::log, 1, 0),
                    JS_FN("pushMatrix", jsfn::pushMatrix, 0, 0),
                    JS_FN("popMatrix", jsfn::popMatrix, 0, 0),
                    JS_FS_END
                };

                bool ok = JS_DefineFunctions(cx, global, my_functions);
                if(!ok){
                    cout << "Function definition error" << endl;
                    return 1;
                }

                run_file("jslib/main.js",&rval);
                run_file("world/main.js",&rval);

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

        JS_DestroyContext(cx);
        JS_DestroyRuntime(rt);
        JS_ShutDown();
    }

    static void start(int _argc, char ** _argv){
        argc = _argc;
        argv = _argv;
        initJavascript(apploop);
    }
}
