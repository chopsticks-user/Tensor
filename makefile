# Makefile for x86_64 Intel Assembly

Application: Application.cpp
	mkdir -p build
	rm -f build/Application

	g++ -std=c++17 -Wall -S -fverbose-asm -g -o build/Application.asm Application.cpp
	g++ -std=c++17 -g -o build/Application Application.cpp

