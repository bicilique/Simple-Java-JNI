package org.example;

public class ExampleCSVParser {


    static {
        System.load(ExampleCSVParser.class.getResource("/lib/ExampleCSVParser.dll").getPath());
    }

    public static void main(String[] args) {
        ExampleCSVParser exampleCSVParser = new ExampleCSVParser();

        String[][] data = exampleCSVParser.parseCSVFile("src/main/resources/files/customers-10000.csv");

        // Display the parsed data
        for (String[] row : data) {
            for (String cell : row) {
                System.out.print(cell + "\t");
            }
            System.out.println();
        }
    }

    public native String[][] parseCSVFile(String filename);
}
