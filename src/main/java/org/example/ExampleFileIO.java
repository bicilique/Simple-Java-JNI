package org.example;

public class ExampleFileIO {

    static {
        System.load(ExampleFileIO.class.getResource("/lib/ExampleFileIO.dll").getPath());
    }


    public static void main(String[] args) {
        ExampleFileIO fileIO = new ExampleFileIO();

        // Example of reading a large file with a custom buffer size of 512KB
        byte[] fileContent = fileIO.readLargeFile("src/main/resources/files/10mb.pdf", 512 * 1024);

        // Example of writing processed byte data with a custom buffer size of 512KB
        fileIO.writeProcessedData("src/main/resources/files/output.pdf", fileContent, 512 * 1024);
    }


    public native byte[] readLargeFile(String filename, int bufferSize);

    public native void writeProcessedData(String filename, byte[] data, int bufferSize);
}
