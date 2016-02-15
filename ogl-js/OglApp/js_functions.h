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
