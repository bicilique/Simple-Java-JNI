##  This is step by step how to work on this first sample

### 1. Create your java class

### 2. Compile your java class 
``javac -h . HelloWorld.java`` 
#### Your output file will be "HelloWorld.class" and "org_example_HelloWorldJNI.h"

### 3. Create your C++ that has same method with your signature as in [org_example_HelloWorldJNI.h](org_example_HelloWorldJNI.h)”

### 4. Compile your C++ code
``g++ -c -I”%JAVA_HOME%\include” -I”%JAVA_HOME%\include\win32" HelloWorldJNI.cpp``
##### notes : please make sure your JAVA_HOME location since in the different OS it will be different path so make sure you are not wrong on this step

### 5. Create shared library base on your output file
``g++ -shared -o native.dll HelloWorldJNI.o`` 

### 6. Finally you can try run HelloWorld.Java

#### This step or tutorial is based on this tutorial
[Guide to JNI (Java Native Interface)](https://medium.com/nerd-for-tech/guide-to-jni-java-native-interface-5b63fea01828)

#### Thanks to the writer I can practice and try this simple JNI example


    

