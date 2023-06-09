CC = g++
CFLAGS = -Wall -Wextra -pedantic -std=c++17
CFLAGS = -std=c++17
INCDIRS = /Users/macbookpro/or-tools_x86_64_macOS-13.2.1_cpp_v9.6.2534/include/
LIBDIRS = /Users/macbookpro/or-tools_x86_64_macOS-13.2.1_cpp_v9.6.2534/lib/
SRC = taxi_assignment_instance.cpp taxi_assignment_solution.cpp checker.cpp greedy_solver.cpp taxi_assignment_batching_solver.cpp main.cpp auxiliares.cpp
OBJ = $(SRC:.cpp=.o)
EXEC = taxi_batching

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -L$(LIBDIRS) -lortools -o $(EXEC)

%.o: %.cpp
	$(CC) $(CFLAGS) -I$(INCDIRS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)