#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include <GL/glut.h>
#include <jsapi.h>
#include <cstdio>

/*
  For other JS versions: look at
  https://developer.mozilla.org/en-US/docs/Mozilla/Projects/SpiderMonkey/How_to_embed_the_JavaScript_engine#Original_Document_Information
 */

using namespace std;

class Settings{
public:
    static int w;
    static int h;
    static int i;
};

int Settings::w = 0;
int Settings::h = 0;
int Settings::i = 0;

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

class OglApp{
 public:   
    static JSBool
        jsfn_plus(JSContext *cx, unsigned argc, jsval *vp)
    {
        JS::CallArgs args = CallArgsFromVp(argc, vp);
        args.rval().setInt32(args[0].toInt32() + args[1].toInt32());
        return true;
    }
    
    static JSBool
        jsfn_divide(JSContext *cx, unsigned argc, jsval *vp)
    {
        JS::CallArgs args = CallArgsFromVp(argc, vp);
        args.rval().setInt32(args[0].toInt32() / args[1].toInt32());
        return true;
    }
    
    static JSBool
        jsfn_log(JSContext *cx, unsigned argc, jsval *vp)
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
    
    OglApp(int * argc, char ** argv){
        glutInit(argc,argv);
        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
        glutInitWindowSize(Settings::w,Settings::h);     
        
        auto Resize = [](int w,int h){
        };
        
        auto Render = [](){
            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
            
            glLoadIdentity();
            gluPerspective(
                           80,
                           float(Settings::w)/float(Settings::h),
                           1,
                           100
                           );
            
            gluLookAt(0.0f,0.0f,2.0f,
                      0.0f,0.0f,0.0f,
                      0.0f,1.0f,0.0f);
            
            glTranslatef(-0.4,-1,0);
            glScalef(0.1,0.1,0.1);
            
            glColor3f(0.6,0.3,1);
            
            glBegin(GL_TRIANGLE_STRIP);
            glVertex3f(0.0f,1.0f,0.0f);
            glVertex3f(1.0f,1.0f,0.0);
            glVertex3f(1.0f,0.0f,0.0);
            glVertex3f(3.0f,1.0f,0.0);
            glVertex3f(3.0f,0.0f,0.0);
            glVertex3f(4.0f,1.0f,0.0);
            glEnd();
            
            glFlush();
            glutSwapBuffers();
        };
        
        glutReshapeFunc(Resize);
        glutCreateWindow("Hey");
        
        glutDisplayFunc(Render);
        glutIdleFunc(Render);
        
        initJS();
        
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
    
    int initJS(){
        JSRuntime *rt = JS_NewRuntime(8L * 1024 * 1024, JS_USE_HELPER_THREADS);
        if (!rt)
            return 1;
        
        JSContext *cx = JS_NewContext(rt, 8192);
        
        JS_SetErrorReporter(cx, &OglApp::dispatchError);
        
        if (!cx)
            return 1;
        
        {
            // Scope for our various stack objects
            // (JSAutoRequest, RootedObject), so they all go
            // out of scope before we JS_DestroyContext.
            
            JSAutoRequest ar(cx);
            // In practice, you would want to exit this any
            // time you're spinning the event loop
            
            JS::RootedObject global(cx, JS_NewGlobalObject(cx, &global_class, nullptr));
            
            if (!global)
                return 1;
            
            JS::RootedValue rval(cx);
            
            // Scope for JSAutoCompartment
            { 
                JSAutoCompartment ac(cx, global);
                JS_InitStandardClasses(cx, global);
                
                static JSFunctionSpec my_functions[] = {
                    JS_FN("plus", jsfn_plus, 2, 0),
                    JS_FN("divide", jsfn_divide, 2, 0),
                    JS_FN("log", jsfn_log, 1, 0),
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
                
                // How can C++ call itself a high level language
                // If I can't even `inFile.c_str();`
                // to get the damn string...
                // http://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
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
 private:
    int i = 0;
};
