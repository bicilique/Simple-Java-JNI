#include <iostream>
#include <string>
#include "org_example_ExampleParametersJNI.h"

JNIEXPORT jlong JNICALL Java_org_example_ExampleParametersJNI_sumIntegers (JNIEnv* env, jobject thisObject, jint first, jint second){
    std::cout << "C++: The numbers received are : " << first << " and " << second << std::endl;
    return (long)first + (long)second;
}

JNIEXPORT jstring JNICALL Java_org_example_ExampleParametersJNI_sayHelloToMe (JNIEnv* env, jobject thisObject, jstring name, jboolean isFemale){
    const char* nameCharPointer = env->GetStringUTFChars(name, NULL);
    std::cout << "C++: The string received is: " << nameCharPointer << std::endl;
    std::string title;
    if(isFemale){
        title = "Ms. ";
    }
    else{
        title = "Mr. ";
    }

    std::string fullName = title + nameCharPointer;
    return env->NewStringUTF(fullName.c_str());
}
