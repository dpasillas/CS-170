main = main
headers = grid.h agent.h
sources = $(main).cpp $(headers:.h=.cpp)
objects = $(sources:.cpp=.o)
n = 4
m = 2
CPP_FLAGS = -std=c++0x

run: $(main).out
	./$(main).out $(n) $(m)

$(main).out: $(objects)
	g++ $(CPP_FLAGS) $(objects) -o $(main).out

.cpp.o:
	g++ $(CPP_FLAGS) -c $<

clean:
	rm $(objects) $(main).out
