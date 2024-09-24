# Compiler
CXX = g++

# Compiler Flags
CXXFLAGS = -std=c++17 -Wall -Wextra

# Linker Flags
LDFLAGS =

# Source Files
SRCS = cd.cpp echo.cpp fw_bw.cpp ls.cpp main.cpp pinfo.cpp pwd.cpp split_words.cpp tokenize.cpp search.cpp redirect.cpp history.cpp pipe.cpp

# Object Files
OBJS = $(SRCS:.cpp=.o)

# Executable Name
TARGET = a.out

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS)

# Compile the source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(TARGET) $(OBJS)

# Phony targets
.PHONY: all clean
