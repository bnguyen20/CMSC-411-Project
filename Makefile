CXX = g++
CXXFLAGS = 
simulator: simulator.cpp
	$(CXX) $(CXXFLAGS) simulator.cpp -o simulator
run:
	./simulator
clean:
	-rm simulator 
val:
	valgrind ./simulator
