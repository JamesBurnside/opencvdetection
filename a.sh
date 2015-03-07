#!/bin/bash
gcc main.cpp -Ofast -ffast-math -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_ml -lopencv_video -lopencv_objdetect -lstdc++ -std=c++11 &&
./a.out
