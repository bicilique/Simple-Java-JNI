#!/bin/bash

# Check if both file paths are passed as arguments
if [ -z "$1" ] || [ -z "$2" ]; then
    echo "Usage: $0 <javaFilePath> <cppFilePath>"
    exit 1
fi

# Assign the Java and C++ file paths
JAVA_FILE_PATH=$1
CPP_FILE_PATH=$2

# Extract file names (without extension) from the file paths
JAVA_FILE_NAME=$(basename "$JAVA_FILE_PATH" .java)
CPP_FILE_NAME=$(basename "$CPP_FILE_PATH" .cpp)

# Step 1: Compile Java class and generate header file
echo "Compiling Java class and generating header file..."
javac -h . "$JAVA_FILE_PATH"

# Move the generated files (both .class and .h) into the C++ directory
HEADER_FILE="org_example_${JAVA_FILE_NAME}.h"
CLASS_FILE="${JAVA_FILE_NAME}.class"

if [ -f "$HEADER_FILE" ]; then
    mv "$HEADER_FILE" "$(dirname "$CPP_FILE_PATH")/"
    echo "Moved header file to C++ directory."
else
    echo "Header file not found!"
    echo "$HEADER_FILE"
    exit 1
fi

if [ -f "$CLASS_FILE" ]; then
    mv "$CLASS_FILE" "$(dirname "$CPP_FILE_PATH")/"
    echo "Moved class file to C++ directory."
else
    echo "Class file not found!"
    exit 1
fi

# Function to handle platform-specific compilation and linking
compile_and_link() {
    local platform=$1
    local cpp_file=$2
    local include_paths=$3
    local libraries=$4
    local output_file=$5

    # Compilation step
    g++ -c -I"$JAVA_HOME/include" $include_paths "$cpp_file" -o "$output_file.o"

    if [[ $? -ne 0 ]]; then
        echo "Error: Compilation failed for $platform."
        exit 1
    fi

    # Linking step to create the shared library
    g++ -shared -o "$output_file" "$output_file.o" $libraries

    if [[ $? -ne 0 ]]; then
        echo "Error: Failed to create the shared library for $platform."
        exit 1
    fi

    echo "Shared library created successfully for $platform: $output_file"

    # Clean up object file
    rm "$output_file.o"
}

# Step 2: Compile and link based on platform
echo "Compiling and linking C++ code for platform: $OSTYPE"

if [[ "$OSTYPE" == "msys"* ]]; then
    # For Windows (DLL)
    compile_and_link "Windows" "$CPP_FILE_PATH" "-I$JAVA_HOME/include/win32" "-lssl -lcrypto" "${JAVA_FILE_NAME}.dll"
elif [[ "$OSTYPE" == "darwin"* ]]; then
    # For macOS (dylib)
    compile_and_link "macOS" "$CPP_FILE_PATH" "-I$JAVA_HOME/include/darwin" "-lssl -lcrypto" "$(dirname "$CPP_FILE_PATH")/${CPP_FILE_NAME}.dll"
else
    # For Linux (so)
    compile_and_link "Linux" "$CPP_FILE_PATH" "-I$JAVA_HOME/include/linux" "-lssl -lcrypto" "lib${JAVA_FILE_NAME}.so"
fi
