/*
  From: https://developer.mozilla.org/en-US/docs/Mozilla/Projects/SpiderMonkey/How_to_embed_the_JavaScript_engine
*/
#include "jsapi.h"
#include <iostream>

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

using namespace std;

#define DOIT_MINARGS 2

static JSBool jsfn_plus(JSContext *cx, unsigned argc, jsval *vp)
{
    JS::CallArgs args = CallArgsFromVp(argc, vp);
    args.rval().setInt32(args[0].toInt32() + args[1].toInt32());
    return true;
}

static JSBool jsfn_divide(JSContext *cx, unsigned argc, jsval *vp)
{
    JS::CallArgs args = CallArgsFromVp(argc, vp);
    args.rval().setInt32(args[0].toInt32() / args[1].toInt32());
    return true;
}


int main(int argc, const char *argv[])
{
    JSRuntime *rt = JS_NewRuntime(8L * 1024 * 1024, JS_USE_HELPER_THREADS);
    if (!rt)
        return 1;

        
    JSContext *cx = JS_NewContext(rt, 8192);
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
                JS_FN("plus", jsfn_plus, DOIT_MINARGS, 0),
                JS_FN("divide", jsfn_divide, DOIT_MINARGS, 0),
                JS_FS_END
            };
            
            bool ok = JS_DefineFunctions(cx, global, my_functions);            
            if(!ok){
                cout << "Function definition error" << endl;
                return 1;
            }
            const char *script = "String(plus(20,divide(20,3)))";
            const char *filename = "noname";
            int lineno = 1;

            ok = JS_EvaluateScript(cx, global, script, strlen(script), filename, lineno, rval.address());
            
            if(!ok)
                return 1;
        }
        
        JSString *str = rval.toString();
        char * str2 = JS_EncodeString(cx, str);
        printf("%s\n", str2);
    }
    
    JS_DestroyContext(cx);
    JS_DestroyRuntime(rt);
    JS_ShutDown();
    return 0;
}
