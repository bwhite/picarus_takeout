#include <stdio.h>
#include "PicarusTakeout.h"
JNIEXPORT void JNICALL Java_PicarusTakeout_process
(JNIEnv *env, jobject thisobject, jstring js)

{
    const char *nativeString = (*env)->GetStringUTFChars(env, js, 0);
    printf("%s\n", nativeString);
    (*env)->ReleaseStringUTFChars(env, js, nativeString);
    //return js;
}
