package org.example;

//This code is example how you passing parameters to your C++ code
public class ExampleParametersJNI {
    static {
        System.load(ExampleParametersJNI.class.getResource("/SampleParameters/nativeWithParamaters.dll").getPath());
    }

    public static void main(String[] args) {
        System.out.println("Java: My full name: " + new ExampleParametersJNI().sayHelloToMe("Bicilique", false));
        long sumFromNative = new ExampleParametersJNI().sumIntegers(455, 44);
        System.out.println("Java: The sum coming from native code is: " + sumFromNative);
    }

    // Declare another method sumIntegers that receives two integers and return a long with the sum
    public native long sumIntegers(int first, int second);

    // Declare another method sayHelloToMe that receives the name and gender and returns the proper salutation
    public native String sayHelloToMe(String name, boolean isFemale);

}
