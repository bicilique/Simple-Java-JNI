package org.example;

public class HelloWorldJNI {
    static {
        System.load(HelloWorldJNI.class.getResource("/SampleHelloWorld/native.dll").getPath());
    }
    public static void main(String[] args) {
        new HelloWorldJNI().sayHello();
    }
    public native void sayHello();

}
