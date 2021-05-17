CPPFLAGS=-std=c++11 -Wall -Wswitch -Wconversion -Wsign-compare -Wreorder -Wold-style-cast -Werror \
-Wundef -Wuninitialized -Wparentheses  -Wpointer-arith -Wunused -Wformat -Wreturn-type

RM=rm -f
SRC_DIR=src/
BUILD_DIR=build/
TEST_DIR=test/
BIN_DIR=bin/
BUILD_MODE ?= normal
CXX=g++

ifeq ($(BUILD_MODE), dbg)
  CPPFLAGS+=-DDBG -g
  BUILD_DIR=build-dbg/
  BIN_DIR=bin-dbg/
endif

all: directories 
	$(MAKE) $(BIN_DIR)hexibit 
	$(MAKE) $(BIN_DIR)hexibit_tests 

.PHONY directories: $(BIN_DIR) $(BUILD_DIR)
	mkdir -p $(BIN_DIR) $(BUILD_DIR)

clean:
	$(RM) -rf $(BIN_DIR) $(BUILD_DIR)

.PHONY run_tests: hexibit_tests
ifeq ($(UNIT_TESTS), yes)
	$(BIN_DIR)hexibit_tests
endif

##################### HEXIBIT APP ###################################################
src = $(wildcard src/*.cpp) \
      $(wildcard src/*/*.cpp)

obj = $(src:$(SRC_DIR)%.cpp=$(BUILD_DIR)%.o)

build_dirs = $(sort $(dir $(obj)))

$(build_dirs):
	mkdir -p $(build_dirs)

$(BUILD_DIR)%.o: src/%.cpp src/%.h
	$(CXX) -c $(CPPFLAGS) -Isrc -o $@ $<

$(BIN_DIR)hexibit: $(obj)
	$(CXX) $(obj) -o $@ 

##################### UNIT TESTS ###################################################
test_src = $(wildcard test/*.cpp)

test_obj = $(test_src:test/%.cpp=build/%.o)

no_main_obj = $(filter-out %main.o,$(obj))

$(BUILD_DIR)%.o: test/%.cpp
	$(CXX) -c $(CPPFLAGS) -Isrc -o $@ $<

$(BIN_DIR)hexibit_tests: $(no_main_obj) $(test_obj)
	$(CXX) $(no_main_obj) $(test_obj) -o $@ 
