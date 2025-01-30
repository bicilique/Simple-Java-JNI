#include <jni.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "org_example_ExampleCSVParser.h"

// Helper function to split a CSV row considering quoted fields
std::vector<std::string> splitCSVRow(const std::string &line) {
    std::vector<std::string> result;
    std::string cell;
    bool insideQuotes = false;

    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];

        if (c == '"') {
            insideQuotes = !insideQuotes; // Toggle quote state
        } else if (c == ',' && !insideQuotes) {
            result.push_back(cell);
            cell.clear();
        } else {
            cell += c;
        }
    }

    // Add the last cell
    result.push_back(cell);

    return result;
}

// JNI Method to parse a CSV file
JNIEXPORT jobjectArray JNICALL Java_org_example_ExampleCSVParser_parseCSVFile(JNIEnv *env, jobject obj, jstring filename) {
    const char *fileName = env->GetStringUTFChars(filename, nullptr);
    if (fileName == nullptr) {
        std::cerr << "Failed to get filename string" << std::endl;
        return env->NewObjectArray(0, env->FindClass("java/lang/String"), nullptr); // Return an empty array
    }

    // Open the CSV file
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << fileName << std::endl;
        env->ReleaseStringUTFChars(filename, fileName);
        return env->NewObjectArray(0, env->FindClass("java/lang/String"), nullptr); // Return an empty array
    }

    // Data container for CSV content (rows and columns)
    std::vector<std::vector<std::string> > data; // Add space between >> for compatibility
    std::string line;

    // Read each line from the file
    while (std::getline(file, line)) {
        // Split the line into cells
        std::vector<std::string> row = splitCSVRow(line);
        data.push_back(row);
    }

    file.close();
    env->ReleaseStringUTFChars(filename, fileName);

    // If no data was read, return an empty array
    if (data.empty()) {
        std::cerr << "No data found in the CSV file" << std::endl;
        return env->NewObjectArray(0, env->FindClass("java/lang/String"), nullptr); // Return an empty array
    }

    // Prepare the result array
    jclass stringClass = env->FindClass("java/lang/String");
    jclass stringArrayClass = env->FindClass("[Ljava/lang/String;");
    jobjectArray resultArray = env->NewObjectArray(data.size(), stringArrayClass, nullptr);

    // Convert each row to a String array
    for (size_t i = 0; i < data.size(); ++i) {
        jsize rowSize = data[i].size();
        jobjectArray rowArray = env->NewObjectArray(rowSize, stringClass, nullptr);

        for (size_t j = 0; j < rowSize; ++j) {
            jstring cell = env->NewStringUTF(data[i][j].c_str());
            env->SetObjectArrayElement(rowArray, j, cell);
        }

        env->SetObjectArrayElement(resultArray, i, rowArray);
    }

    return resultArray;
}
