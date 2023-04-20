CXX=g++
CXXFLAGS=-std=c++11
SRC=main.cpp
OBJ=$(SRC:.cpp=.o)
TARGET=program

all: $(TARGET)
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<


clean:
	rm -f $(OBJ) $(TARGET)
