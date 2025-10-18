Neural: main.cpp
	g++  main.cpp -std=c++20 -Wall -Wextra -I"./include" -L"./lib" -lraylib -o Neural 

clean:
	rm Neural 
