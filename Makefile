CXX=g++
CPPFLAGS=-std=c++11 -IC:\SFML-2.1\include -O3 -Wall -pedantic
LDFLAGS=-LC:\SFML-2.1\lib
LDLIBS=-lsfml-system-2 -lsfml-window-2 -lsfml-graphics-2 -lsfml-audio-2 
SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)

OUT=CatandMouse

$(OUT): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) $(LDLIBS) -o $@

%.o: %.cpp
	$(CXX) $(CPPFLAGS) -c $< -o $@ 
	


clean:
	rm -rf *.o 
	


