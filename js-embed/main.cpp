/*
  From: https://developer.mozilla.org/en-US/docs/Mozilla/Projects/SpiderMonkey/How_to_embed_the_JavaScript_engine
*/
#include "jsapi.h"

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

        { // Scope for JSAutoCompartment
            JSAutoCompartment ac(cx, global);
            JS_InitStandardClasses(cx, global);

            const char *script = "'hello'+'world, it is '+new Date()";
            const char *filename = "noname";
            int lineno = 1;

            bool ok = JS_EvaluateScript(cx, global, script, strlen(script), filename, lineno, rval.address());
            if (!ok)
                return 1;
        }

        JSString *str = rval.toString();
        printf("%s\n", JS_EncodeString(cx, str));
    }

    JS_DestroyContext(cx);
    JS_DestroyRuntime(rt);
    JS_ShutDown();
    return 0;
}
