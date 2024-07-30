CXX = g++
CXXFLAGS = -Wall -g -std=c++11  # Add the C++11 standard and other common flags

# Name of the executable
TARGET = MEGATRON

# All object files that will be generated
OBJS = main.o database.o schema_manager.o pesos.o data_manager.o global_variable.o

# Default rule
all: $(TARGET)

# How to link the program
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

# How to compile each source file
main.o: main.cpp database.h schema_manager.h pesos.h data_manager.h global_variable.h
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

database.o: database.cpp schema_manager.h database.h data_manager.h pesos.h global_variable.h
	$(CXX) $(CXXFLAGS) -c database.cpp -o database.o

schema_manager.o: schema_manager.cpp schema_manager.h pesos.h data_manager.h global_variable.h
	$(CXX) $(CXXFLAGS) -c schema_manager.cpp -o schema_manager.o

pesos.o: pesos.cpp pesos.h data_manager.h
	$(CXX) $(CXXFLAGS) -c pesos.cpp -o pesos.o

data_manager.o: data_manager.cpp data_manager.h
	$(CXX) $(CXXFLAGS) -c data_manager.cpp -o data_manager.o

global_variable.o: schema_manager.cpp schema_manager.h
	$(CXX) $(CXXFLAGS) -c global_variable.cpp -o global_variable.o

# Command to clean compiled files (optional)
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
