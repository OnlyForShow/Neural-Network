Neural: main.cpp Neural.h Neural.cpp Visual.h Visual.cpp
	mkdir -p build
	g++ Visual.cpp Neural.cpp main.cpp  -o1 -std=c++20 -Wall -Wextra -I"./include" -L"./lib" -lraylib -o build/Neural 


.PHONY: test
test: xor_example 

xor_example: xor_example.cpp
	mkdir -p build
	g++  xor_example.cpp -g -std=c++20 -Wall -Wextra -I"./include" -L"./lib" -lraylib -o build/xor_example 



clean:
	rm -rf build 
