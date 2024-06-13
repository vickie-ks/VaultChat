#!/bin/bash

# Compile the program
g++ src/main.cpp src/ChatServer.cpp -o main -I/opt/homebrew/Cellar/cppcms/1.2.1_1/include -L/opt/homebrew/Cellar/cppcms/1.2.1_1/lib -lcppcms -lbooster

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful. Running the program..."
    ./main -c config.json
else
    echo "Compilation failed."
fi
