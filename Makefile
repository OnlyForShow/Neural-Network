Neural: main.cpp
	g++  main.cpp -o1 -std=c++20 -Wall -Wextra -I"./include" -L"./lib" -lraylib -o Neural 


.PHONY: test
test: xor_example test2

xor_example: xor_example.cpp
	g++  xor_example.cpp -g -std=c++20 -Wall -Wextra -I"./include" -L"./lib" -lraylib -o xor_example 

test2: test2.cpp
	g++  test2.cpp -g -std=c++20 -Wall -Wextra -I"./include" -L"./lib" -lraylib -o test2 


clean:
	rm Neural 
	rm xor_example 
	rm test2
