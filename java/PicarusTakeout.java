public class PicarusTakeout
{
    static {
        System.loadLibrary("picarus_takeout_jni");
    }
    public native void process(String jsonConfig);
    public static void main(String[] argv)
        {
            String retval = null;
            PicarusTakeout pt = new PicarusTakeout();//, byte [] input
            pt.process("{\"name\": \"picarus.ImagePreprocessor\", \"kw\": {\"method\": \"max_side\", \"size\": 320, \"compression\": \"jpg\"}}");
            //System.out.println("Invocation returned " + retval);  // retval = 
        }
}
