# Simple portable C++ Makefile
# Put this file at the project root (your current file).
CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -MMD -MP
LDFLAGS :=

# Target binary name (change as needed)
TARGET := main

# Collect sources (root and src/ directory)
SRCS := $(wildcard *.cpp) $(wildcard src/*.cpp)
OBJS := $(SRCS:.cpp=.o)
DEPS := $(OBJS:.o=.d)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@

# Generic rule to build .o from .cpp
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Include auto-generated dependency files (if any)
-include $(DEPS)

clean:
	-rm -f $(TARGET) $(OBJS) $(DEPS)

run: all
	./$(TARGET)