CXX := g++
CXXFLAGS := -O3 -march=native -std=c++17

.PHONY: all clean

all: Main Run

Main: Main.cpp ObjectiveFunction.cpp Job.cpp
	$(CXX) $(CXXFLAGS) -o Main Main.cpp ObjectiveFunction.cpp Job.cpp

Run: Run.cpp
	$(CXX) $(CXXFLAGS) -o Run Run.cpp

clean:
	rm -f Main Run