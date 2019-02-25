# C/C++ compilation involves two stages:
# - compiling: convert each source (*.cpp) file indepdendently into binary object (.o) files
# - linking: assemble all the object files into a single executable
#
# Since any C++ project contains multiple source files, the above
# process involved firing multiple commands. Makefiles help us
# write all the compilation stages in a single file and run them
# as one big script.
#
# See http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
# and many many other tutorials for makefiles


# some variables
BUILD_DIR  :=_build
EXECUTABLE := vor
OUTPUT := Output
POINTS_FILE := points.txt
EDGES_FILE := edges.txt
PYT := python3 
DRAW_FILE := draw.py 
NUMB_POINTS := 10

#contain a list of test (blank for handout)
TESTS = test1

# the C++ compiler/linker to be used. define here so that we can change
# it easily if needed
CXX := g++ -Wall -std=c++11

# ------------------------------------------------------------------------------

# 'make' or 'make all' runs the default target 'all' which requires that
# the EXECUTABLE must be available; if the EXECUTABLE is already available
# then nothing happens. see the rules for EXECUTABLE

all: $(EXECUTABLE)

# ------------------------------------------------------------------------------

# 'make run' runs the target 'run' which calls for running the executable
# if the executable is not available, then the rules for creating it are run
# this is a superset of the target 'all' because it creates the executable
# and also runs it

run: $(EXECUTABLE)
	./$(EXECUTABLE) $(NUMB_POINTS) $(POINTS_FILE) $(EDGES_FILE)

# ------------------------------------------------------------------------------

# 'make clean' runs the target 'clean' which in turn removes the
# intermediate .o files and the executable

clean:
	rm -rf $(BUILD_DIR) $(EXECUTABLE) $(OUTPUT) $(TESTS)


# ------------------------------------------------------------------------------

# make tests' compiles the test cases
tests: $(TESTS)


draw: run
	$(PYT) $(DRAW_FILE) $(EDGES_FILE) $(POINTS_FILE)

# ------------------------------------------------------------------------------

# rule for creating the executable: this "links" the .o files using the g++ linker.
# If .o files are not available, then the rules for creating .o files are run.

$(EXECUTABLE): $(BUILD_DIR)/main.o $(BUILD_DIR)/Utils.o 
	$(CXX) $(BUILD_DIR)/main.o $(BUILD_DIR)/Utils.o -o $(EXECUTABLE)
	mkdir -p $(OUTPUT)


# ------------------------------------------------------------------------------

# rules for creating the .o files:  compile each of the .cpp files and create a
# corresponding .o file. Each .o depends upon the corresponding .cpp and other .h files

$(BUILD_DIR)/%.o: %.cpp 
	mkdir -p $(BUILD_DIR)
	$(CXX) -c $< -o $@


# ------------------------------------------------------------------------------

# rule for creating the tests: this "links" the .o files using the g++ linker.
# It crates each test .o file and links the created to all the other ones.

$(TESTS): $(BUILD_DIR)/Utils.o 
	$(CXX) -c $(@).cpp -o $(BUILD_DIR)/$@.o
	$(CXX) $(BUILD_DIR)/$(@).o $(BUILD_DIR)/Utils.o -o $(@)
	mkdir -p $(OUTPUT)
