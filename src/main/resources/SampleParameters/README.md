##  This is my second sample of working on JNI

### In this sample we will try to pass a paramater to our C++ code and then we try to get the return value

### Here is the step by step tutorial

### 1. Create your java class

### 2. Compile your java class 
``javac -h . ExampleParameters.java`` 
#### Your output file will be "ExampleParameters.class" and "org_example_ExampleParameters.h"

### 3. Create your C++ that has same method with your signature as in [org_example_ExampleParametersJNI.h](org_example_ExampleParametersJNI.h)”

### 4. Compile your C++ code
``g++ -c -I”%JAVA_HOME%\include” -I”%JAVA_HOME%\include\win32" ExampleParameters.cpp``
##### notes : please make sure your JAVA_HOME location since in the different OS it will be different path so make sure you are not wrong on this step

### 5. Create shared library base on your output file
``g++ -shared -o nativeWithParamaters.dll ExampleParameters.o`` 

### 6. Finally you can try run ExampleParameters.Java

#### This step or tutorial is based on this tutorial
[Guide to JNI (Java Native Interface)](https://www.baeldung.com/jni)

#### Thanks to the writer I can practice and try this simple JNI example


    

