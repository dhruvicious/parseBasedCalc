CXX = g++
CXXFLAGS = -std=c++17 -Wall
LDFLAGS = -lreadline

SRC = src/main.cpp \
      src/tokenizer.cpp \
      src/parser.cpp \
      src/evaluator.cpp

OBJ = $(SRC:.cpp=.o)

TARGET = calculator

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
