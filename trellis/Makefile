CC = g++

all : compile
    
clean : 
	rm -f simulator

run : simulator
	simulator

compile : src/main.cpp
	$(CC) -o simulator -I/usr/local/boost_1_40_0 $<

