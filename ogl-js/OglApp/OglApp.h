#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include <GL/glut.h>
#include <jsapi.h>
#include <cstdio>
#include "js_functions.h"

/*
  For other JS versions: look at
  https://developer.mozilla.org/en-US/docs/Mozilla/Projects/SpiderMonkey/How_to_embed_the_JavaScript_engine#Original_Document_Information
*/

using namespace std;

namespace OglApp{

    int w = 0;
    int h = 0;
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

    static void Resize(int w, int h){

    }

    static void Render(){
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


    static void apploop(){
        glutInit(&argc,argv);
        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
        glutInitWindowSize(w,h);
        
        glutReshapeFunc(Resize);
        glutCreateWindow("Hey");

        glEnable(GL_DEPTH_TEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        
        glutDisplayFunc(Render);
        glutIdleFunc(Render);
        
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
                    JS_FN("scale", jsfn::scale, 3, 0),
                    JS_FN("divide", jsfn::divide, 2, 0),
                    JS_FN("log", jsfn::log, 1, 0),
                    JS_FS_END
                };

                bool ok = JS_DefineFunctions(cx, global, my_functions);
                if(!ok){
                    cout << "Function definition error" << endl;
                    return 1;
                }
                
                const char *filename = "world/main.js";
                int lineno = 0;

                ifstream file;

                file.open(filename);

                // Take the content of the file
                // and put it in a string
                stringstream strStream;
                strStream << file.rdbuf();
                string str = strStream.str();
                const char * script = str.c_str();

                ok = JS_EvaluateScript
                    (
                     cx,
                     global,
                     script,
                     strlen(script),
                     filename,
                     lineno,
                     rval.address()
                     );

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
