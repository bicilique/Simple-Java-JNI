#include<iostream>
#include<jni.h>
#include "org_example_HelloWorldJNI.h"
void sayHello(){
  std::cout << "Hello from C++ !!" << std::endl;
}
JNIEXPORT void JNICALL Java_org_example_HelloWorldJNI_sayHello
  (JNIEnv* env, jobject thisObject) {
    sayHello();
}
