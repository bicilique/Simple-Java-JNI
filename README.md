
## **Step-by-Step Guide to JNI (Java Native Interface)**

This tutorial will guide you through the process of using JNI to call native C++ methods from Java. Follow each step carefully to set up the environment and compile both Java and C++ code.

---

### **1. Create Your Java Class**

Begin by writing your Java class, which will contain the native methods you want to call. The class should look like this:

```java
public class HelloWorld {
    // Declare the native method
    public native void sayHello();

    // Load the native library
    static {
        System.loadLibrary("helloWorld");
    }

    // Main method to run the program
    public static void main(String[] args) {
        new HelloWorld().sayHello();
    }
}
```

---

### **2. Compile Your Java Class**

Use the `javac` command to compile your Java code and generate the header file for the native methods:

```bash
javac -h . HelloWorld.java
```

This will produce two files:

- `HelloWorld.class` — the compiled Java bytecode.
- `org_example_HelloWorldJNI.h` — the header file with the native method signature.

---

### **3. Create Your C++ File**

Next, create the corresponding C++ file that implements the `sayHello()` method declared in the header file `org_example_HelloWorldJNI.h`. Here's an example:

```cpp
#include <iostream>
#include "org_example_HelloWorldJNI.h"

JNIEXPORT void JNICALL Java_HelloWorld_sayHello(JNIEnv *env, jobject obj) {
    std::cout << "Hello from C++!" << std::endl;
}
```

---

### **4. Compile Your C++ Code**

Now, compile the C++ code to generate an object file. Be sure to replace the `JAVA_HOME` path based on your system configuration:

For Windows :
```bash
g++ -c -I"%JAVA_HOME%/include" -I"%JAVA_HOME%/include/win32" HelloWorldJNI.cpp
```

For MacOS :
```bash
g++ -c -I"$JAVA_HOME/include" -I"$JAVA_HOME/include/darwin" HelloWorldJNI.cpp
```

For Linux :
```bash
g++ -c -I"$JAVA_HOME/include" -I"$JAVA_HOME/include/linux" HelloWorldJNI.cpp
```

**Note:** The `JAVA_HOME` path may differ based on your operating system. Ensure it's correctly set to avoid errors.

---

### **5. Create the Shared Library**

Next, create the shared library (DLL on Windows) using the following command:

```bash
g++ -shared -o helloWorld.dll HelloWorldJNI.o
```

This will generate the `helloWorld.dll` file, which contains the native implementation of your method.

---

### **6. Run Your Java Program**

Finally, you can now run your Java program. If everything is set up correctly, you should see the output:

```bash
Hello from C++!
```

Execute the Java program as follows:

```bash
java HelloWorld
```

---

### **Reference**

This tutorial is based on the guide provided by [Guide to JNI (Java Native Interface)](https://medium.com/nerd-for-tech/guide-to-jni-java-native-interface-5b63fea01828).

---

### **Acknowledgments**

A special thanks to the original writer for the inspiration and detailed explanation, which allowed me to explore and practice JNI with this simple example.
