CPPFLAGS=-std=c++11 -Wall -Wconversion -Wsign-compare -Wreorder -Wold-style-cast -Werror #-Wpedantic #TODO: make compiler as strict as possible
RM=rm -f
SRC_DIR=src/
BUILD_DIR=build/
TEST_DIR=test/
BIN_DIR=bin/
UNIT_TESTS ?= no
BUILD_MODE ?= normal

ifeq ($(BUILD_MODE), dbg)
CPPFLAGS+=-DDBG -g
BUILD_DIR=build-dbg/
endif

all: directories 
	$(MAKE) $(BIN_DIR)hexibit_app 

.PHONY directories: $(BIN_DIR) $(BUILD_DIR)
	mkdir -p $(BIN_DIR) $(BUILD_DIR)

clean:
	$(RM) -rf $(BIN_DIR) $(BUILD_DIR)

define depends_on 
    $(addsuffix .h, $(addprefix $(SRC_DIR), $(1))) $(addsuffix .cpp, $(addprefix $(SRC_DIR), $(1))) $(SRC_DIR)/MainUtils.h
endef

#define compile $(addsuffix .h, $(addprefix $(SRC_DIR), $(1))) $(addsuffix .cpp, $(addprefix $(SRC_DIR), $(1))) 
define compilef
	g++ $(CPPFLAGS) -Isrc -o $(addsuffix .o, $(addprefix $(BUILD_DIR), $(1))) -c  $(addsuffix .cpp, $(addprefix $(SRC_DIR), $(addprefix $(2)/, $(1))))
endef

define compilef_test
	g++ $(CPPFLAGS) -Isrc -o $(addsuffix .o, $(addprefix $(BUILD_DIR), $(1))) -c  $(addsuffix .cpp, $(addprefix $(TEST_DIR), $(addprefix $(2)/, $(1))))
endef

##################### HEXIBIT APP ###################################################
#Linking
$(BIN_DIR)hexibit_app: $(BUILD_DIR)ByteOrder.o $(BUILD_DIR)ListPrinter.o $(BUILD_DIR)Interpretation.o $(BUILD_DIR)Interpreter.o $(BUILD_DIR)Config.o $(BUILD_DIR)Buffer.o $(BUILD_DIR)hexibit_main.o
	g++ $(BUILD_DIR)ByteOrder.o $(BUILD_DIR)ListPrinter.o $(BUILD_DIR)Interpretation.o $(BUILD_DIR)Interpreter.o $(BUILD_DIR)Config.o $(BUILD_DIR)Buffer.o $(BUILD_DIR)hexibit_main.o -o $(BIN_DIR)hexibit_app

#Compiling
$(BUILD_DIR)hexibit_main.o: $(SRC_DIR)hexibit_main.cpp
	$(call compilef, hexibit_main)

$(BUILD_DIR)Config.o: $(call depends_on, config/Config)
	$(call compilef, Config, config)

$(BUILD_DIR)Buffer.o: $(call depends_on, data/Buffer)
	$(call compilef, Buffer, data)

$(BUILD_DIR)ByteOrder.o: $(call depends_on, data/ByteOrder)
	$(call compilef, ByteOrder, data)

$(BUILD_DIR)Interpretation.o: $(call depends_on, parse/Interpretation)
	$(call compilef, Interpretation, parse)

$(BUILD_DIR)Interpreter.o: $(call depends_on, parse/Interpreter)
	$(call compilef, Interpreter, parse)

$(BUILD_DIR)ListPrinter.o: $(call depends_on, print/ListPrinter)
	$(call compilef, ListPrinter, print)

