CPPFLAGS=-std=c++11 -Wall -Wconversion -Wsign-compare -Wreorder -Wold-style-cast -Werror #-Wpedantic #TODO: make compiler as strict as possible
RM=rm -f
SRC_DIR=src/
BUILD_DIR=build/
TEST_DIR=test/
BIN_DIR=bin/
UNIT_TESTS ?= no
BUILD_MODE ?= normal

ifeq ($(BUILD_MODE), dbg)
CPPFLAGS+=-DDBG
BUILD_DIR=build-dbg/
endif

all: directories 
	$(MAKE) $(BIN_DIR)hexibit_app 

.PHONY directories: $(BIN_DIR) $(BUILD_DIR)
	mkdir -p $(BIN_DIR) $(BUILD_DIR)

clean:
	$(RM) -rf $(BIN_DIR) $(BUILD_DIR)

define depends_on 
    $(addsuffix .h, $(addprefix $(SRC_DIR), $(1))) $(addsuffix .cpp, $(addprefix $(SRC_DIR), $(1))) $(SRC_DIR)/UtilDefs.h
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
$(BIN_DIR)hexibit_app: $(BUILD_DIR)Config.o $(BUILD_DIR)Buffer.o $(BUILD_DIR)hexibit_main.o
	g++ $(BUILD_DIR)Config.o $(BUILD_DIR)Buffer.o $(BUILD_DIR)hexibit_main.o -o $(BIN_DIR)hexibit_app

#Compiling
$(BUILD_DIR)hexibit_main.o: $(SRC_DIR)hexibit_main.cpp
	$(call compilef, hexibit_main)

$(BUILD_DIR)Config.o: $(call depends_on, config/Config)
	$(call compilef, Config, config)

$(BUILD_DIR)Buffer.o: $(call depends_on, Buffer)
	$(call compilef, buffer)
