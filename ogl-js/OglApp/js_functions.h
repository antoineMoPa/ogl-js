using namespace std;

namespace jsfn{
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

        glTranslatef(
                     args[0].toNumber(),
                     args[1].toNumber(),
                     args[2].toNumber()
                     );
        return true;
    }

    static JSBool
        scale(JSContext *cx, unsigned argc, jsval *vp)
    {
        JS::CallArgs args = CallArgsFromVp(argc, vp);
        
        glScalef(
                 args[0].toNumber(),
                 args[1].toNumber(),
                 args[2].toNumber()
                 );
        return true;
    }

    static JSBool
        triangle_strip(JSContext *cx, unsigned argc, jsval *vp)
    {
        JS::CallArgs args = CallArgsFromVp(argc, vp);
        
        uint32_t len;
        JSObject * arr = args[0].toObjectOrNull();
        JS_GetArrayLength(cx,arr,&len);
        
        // We will not use last elements
        // if they are not in a group of 3
        len = len - len % 3;
        
        JS::Value el;
        
        float a,b,c;
        
        glBegin(GL_TRIANGLE_STRIP);
        for(uint32_t i = 0; i < len; i+=3){
            // Get numbers
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
