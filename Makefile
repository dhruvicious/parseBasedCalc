CXX = g++
CXXFLAGS = -std=c++17 -Wall -Isrc/include -MMD -MP
LDFLAGS = -lreadline -lcurl

SRC = src/main.cpp \
      src/tokenizer.cpp \
      src/parser.cpp \
      src/converter.cpp \
      src/currency_fetcher.cpp

OBJ = $(SRC:.cpp=.o)
DEP = $(OBJ:.o=.d)

TARGET = calculator

.PHONY: all clean docker-build

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(DEP) $(TARGET)

docker-build:
	docker build -t calculator .
	docker image prune -f

-include $(DEP)
