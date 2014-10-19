CXX=g++
CPPFLAGS=-std=c++11 -IC:\SFML-2.1\include
LDFLAGS=-LC:\SFML-2.1\lib
LDLIBS=-lsfml-system-2 -lsfml-window-2 -lsfml-graphics-2 -lsfml-audio-2 
STORABLESOURCES=storablecontroller.cpp storabletilemap.cpp
TOOLBARSOURCES=toolbarbutton.cpp toolbarbuttonlist.cpp toolbaritem.cpp toolbarnumber.cpp
SOURCES=car.cpp configuration.cpp editor.cpp functions.cpp game.cpp main.cpp menu.cpp race.cpp \
				renderfunctions.cpp track.cpp $(TOOLBARSOURCES) $(STORABLESOURCES)
OBJECTS=$(SOURCES:.cpp=.o)

CatandMouse: $(OBJECTS)
	$(CXX) -o CatandMouse.exe $(OBJECTS) $(LDFLAGS) $(LDLIBS)

car.o:

configuration.o:

editor.o:

functions.o:

game.o:

main.o:

menu.o:

race.o:

renderfunctions.o:

track.o:

$(TOOLBARSOURCES) : 

$(STORABLESOURCES) :

clean:
	rm -rf *.o 
	


