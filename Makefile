main = main
headers = grid.h agent.h
sources = $(main).cpp $(headers:.h=.cpp)
objects = $(sources:.cpp=.o)
n = 4
m = 1
CPP_FLAGS = -std=c++0x
CPP = g++
run: $(main).out
	./$(main).out $(n) $(m)

$(main).out: $(objects)
	$(CPP) $(CPP_FLAGS) $(objects) -o $(main).out

.cpp.o:
	$(CPP) $(CPP_FLAGS) -c $<

clean:
	rm $(objects) $(main).out
