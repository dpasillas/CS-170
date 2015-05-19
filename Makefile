main = main
headers = grid.h
sources = $(main).cpp $(headers:.h=.cpp)
objects = $(sources:.cpp=.o)

run: $(main).out
	./$(main).out

$(main).out: $(objects)
	g++ $(objects) -o $(main).out

.cpp.o:
	g++ -c $<

clean:
	rm $(objects) $(main).out
