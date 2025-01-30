#include <jni.h>
#include <iostream>
#include "org_example_ExampleMatrixJNI.h"  // Sesuaikan dengan header file yang dihasilkan oleh javac -h

JNIEXPORT jobjectArray JNICALL Java_org_example_ExampleMatrixJNI_multiplyMatrix(JNIEnv *env, jobject obj, jobjectArray matrix1, jobjectArray matrix2) {
    // Dapatkan ukuran matriks pertama
    jsize rows1 = env->GetArrayLength(matrix1);
    jsize cols1 = env->GetArrayLength((jobjectArray)env->GetObjectArrayElement(matrix1, 0));

    // Dapatkan ukuran matriks kedua
    jsize rows2 = env->GetArrayLength(matrix2);
    jsize cols2 = env->GetArrayLength((jobjectArray)env->GetObjectArrayElement(matrix2, 0));

    // Validasi: jumlah kolom matriks pertama harus sama dengan jumlah baris matriks kedua
    if (cols1 != rows2) {
        std::cerr << "Dimensi matriks tidak cocok untuk perkalian!" << std::endl;
        return NULL;
    }

    // Kelas array integer (jintArray)
    jclass intArrayClass = env->FindClass("[I");
    if (intArrayClass == NULL) {
        std::cerr << "Gagal menemukan kelas array integer!" << std::endl;
        return NULL;
    }

    // Buat matriks hasil (ukuran: rows1 x cols2)
    jobjectArray resultMatrix = env->NewObjectArray(rows1, intArrayClass, NULL);
    if (resultMatrix == NULL) {
        std::cerr << "Gagal mengalokasikan memori untuk matriks hasil!" << std::endl;
        return NULL;
    }

    // Lakukan perkalian matriks
    for (jsize i = 0; i < rows1; ++i) {
        jintArray resultRow = env->NewIntArray(cols2);
        if (resultRow == NULL) {
            std::cerr << "Gagal mengalokasikan memori untuk baris hasil!" << std::endl;
            return NULL;
        }

        jint* rowData = new jint[cols2]; // Alokasi manual untuk baris hasil sementara
        for (jsize j = 0; j < cols2; ++j) {
            jint sum = 0;

            for (jsize k = 0; k < cols1; ++k) {
                jintArray row1 = (jintArray)env->GetObjectArrayElement(matrix1, i);
                jint* array1 = env->GetIntArrayElements(row1, NULL);

                jintArray row2 = (jintArray)env->GetObjectArrayElement(matrix2, k);
                jint* array2 = env->GetIntArrayElements(row2, NULL);

                sum += array1[k] * array2[j];

                // Lepaskan memori elemen
                env->ReleaseIntArrayElements(row1, array1, JNI_ABORT);
                env->ReleaseIntArrayElements(row2, array2, JNI_ABORT);
                env->DeleteLocalRef(row1);
                env->DeleteLocalRef(row2);
            }
            rowData[j] = sum; // Simpan hasil untuk elemen ini
        }

        // Salin baris hasil ke JNI array
        env->SetIntArrayRegion(resultRow, 0, cols2, rowData);

        // Set baris hasil di matriks hasil
        env->SetObjectArrayElement(resultMatrix, i, resultRow);

        // Lepaskan memori JNI untuk resultRow
        env->DeleteLocalRef(resultRow);

        // Hapus alokasi manual
        delete[] rowData;
    }

    return resultMatrix;
}
