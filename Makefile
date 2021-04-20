#
# Variables
#
CC         = clang++
TARGET     = obss
CFLAGS     = -g -Wall -Iinclude
LINKER     = clang++ -pthreads
SOURCE_DIR = src
HEADER_DIR = header
OBJECT_DIR = build/obj
BINARY_DIR = build/bin
# Find all source files in the $(SOURCE_DIR)
SOURCES = $(wildcard $(SOURCE_DIR)/*.cpp)
#use substitution $(var:pattern=replacement) to replace all *.cpp files to *.o files to use as dependency
OBJECTS = $(SOURCES:$(SOURCE_DIR)/%.cpp=$(OBJECT_DIR)/%.o)

#
# Targets
#

#Default goal is to build the library
.DEFAULT_GOAL := library
library: clean $(BINARY_DIR)/$(TARGET)

#Compile all src/*.cpp files into objects
$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	@echo 'Compiling $< to object file $@'
	$(CC) $(CFLAGS) -c $< -o $@

#Archive all obj/* objects into a static library
$(BINARY_DIR)/$(TARGET): $(OBJECTS)
	@echo 'Linking $^ to $@'
	$(LINKER) $^ -o $(BINARY_DIR)/$(TARGET)

clean:
	@echo 'Cleaning $(OBJECT_DIR)/* and $(BINARY_DIR)/*'
	@rm -rf ./$(OBJECT_DIR)/* ./$(BINARY_DIR)/*
	@echo 'Creating temp directories $(OBJECT_DIR)/* and $(BINARY_DIR)/*'
	@mkdir -p ./$(OBJECT_DIR)
	@mkdir -p ./$(BINARY_DIR)