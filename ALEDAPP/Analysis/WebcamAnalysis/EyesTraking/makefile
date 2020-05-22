all: EyesTraking
	
EyesTraking : EyesTraking.hpp
	g++ EyesTraking.cpp -o EyesTraking -Wall `pkg-config --cflags --libs opencv`


clean: 
	rm -f EyesTraking
