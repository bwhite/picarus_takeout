rm -f *.so *.class
javac PicarusTakeout.java
javah -jni PicarusTakeout
gcc PicarusTakeoutJNI.c -I /usr/lib/jvm/java-6-sun/include -I /usr/lib/jvm/java-6-sun/include/linux -shared -Wl,-soname,picarus_takeout_jni.so -o libpicarus_takeout_jni.so -fPIC
#java -Djava.library.path=.  PicarusTakeout