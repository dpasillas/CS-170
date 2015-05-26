main = main
headers = grid.h agent.h random.h
sources = $(main).cpp $(headers:.h=.cpp)
objects = $(sources:.cpp=.o)
n = 4
m = 1
CPP_FLAGS = -g
CPP = g++

$(main).out: $(objects)
	$(CPP) $(CPP_FLAGS) $(objects) -o $(main).out

run: $(main).out
	./$(main).out $(n) $(m)

.cpp.o:
	$(CPP) $(CPP_FLAGS) -c $<

clean:
	rm $(objects) $(main).out
