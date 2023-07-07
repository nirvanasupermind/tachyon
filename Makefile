ifeq ($(OS),Windows_NT)
    SOURCE := src\*.cpp
    TARGET := C:\Program Files\tachyonc
else
    SOURCE := src/*.cpp
    TARGET := /usr/local/bin/tachyonc
endif

default:
	clang++ $(SOURCE) -o $(TARGET) -Wno-return-type -std=c++11