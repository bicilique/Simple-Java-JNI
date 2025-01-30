package org.example;

public class ExampleMatrixJNI {


    static {
        System.load(ExampleMatrixJNI.class.getResource("/lib/matrixMulti.dll").getPath());
    }

    public static void main(String[] args) {
        // Example Matriks
        int[][] matrix1 = {
                {1, 2},
                {3, 4}
        };
        int[][] matrix2 = {
                {5, 6},
                {7, 8}
        };


        int[][] result =new ExampleMatrixJNI().multiplyMatrix(matrix1,matrix2);

        for (int i = 0; i < result.length; i++) {
            for (int j = 0; j < result[i].length; j++) {
                System.out.print(result[i][j] + " ");
            }
            System.out.println();
        }
    }

    public native int[][] multiplyMatrix(int[][] matrix1, int[][] matrix2);

}
