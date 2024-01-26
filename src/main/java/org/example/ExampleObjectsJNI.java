package org.example;


import org.example.model.UserData;

public class ExampleObjectsJNI {

    static {
        System.load(ExampleObjectsJNI.class.getResource("/SampleObjects/nativeWithObjects.dll").getPath());
    }


    public static void main(String[] args) {
        ExampleObjectsJNI instance = new ExampleObjectsJNI();
        UserData newUser = instance.createUser("John Doe", 450.67);
        instance.printUserData(newUser);
    }

    public native UserData createUser(String name, double balance);
    public native String printUserData(UserData user);
}
