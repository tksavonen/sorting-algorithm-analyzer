CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2

TARGET := final

SRCS := $(shell find . -type f -name "*.cpp")

OBJS := $(patsubst ./%.cpp, build/%.o, $(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

build/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf build $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: 
