
INC:=include
TEST:=test
BLD:=build

# general includes
INCLUDES= \
	-I $(INC) \

# GCC
# CPP=g++
# CLANG
CPP=clang++
CPP_FLAGS= \
	-Wall \
	-Wextra \
	-pedantic \
	-Wnon-virtual-dtor \
	-Wold-style-cast \
	-Wunused \
	-Woverloaded-virtual \
	-Wimplicit-fallthrough \
	-Wdocumentation \
	-std=c++20 \
	-g \
	$(INCLUDES)\

DEBUG ?= 0
ifeq ($(DEBUG), 1)
    CPP_FLAGS += -O0 -DDEBUG
    BLD:=$(BLD)/debug
else
    CPP_FLAGS += -O2 -march=native
endif

### DEFAULT ###
.PHONY: default
.DEFAULT_GOAL=default
default: test

### ALL ###
.PHONY: all
all: test doc

### CLEAN ##
.PHONY:clean
clean:
	rm -rf build/*
	rm -rf doc/*

### DOCUMENTATION ###
.PHONY: doc
doc:
	@mkdir -p doc
	doxygen Doxyfile.in

### TESTS ###

TEST_SOURCES = $(shell find $(TEST) -name '*.cpp')
TEST_DEPENDS = $(TEST_SOURCES:$(TEST)/%.cpp=$(BLD)/%.d)
TEST_BINARIES = $(TEST_SOURCES:$(TEST)/%.cpp=$(BLD)/%)
TESTS = $(filter $(BLD)/%, $(TEST_BINARIES))

.PHONY: test
test: $(TESTS);
	@# loop is exited after first failure
	@set -e; for T in $(TESTS); do echo "test: $$T"; $$T -l warning; done

$(TEST_DEPENDS): $(BLD)/%.d: $(TEST)/%.cpp
	@mkdir -p $(@D) # provide parent directory of target
	$(CPP) $(CPP_FLAGS) -MM -MQ $@ -o $@ $<

$(TESTS): $(BLD)/%: $(TEST)/%.cpp $(BLD)/%.d
	@mkdir -p $(@D) # provide parent directory of target
	$(CPP) $(CPP_FLAGS) -o $@ $<

# import dependencies for all target binaries
include $(TEST_DEPENDS)

ALL_SOURCE_FILES = $(shell find $(INC) $(TEST) -iname *.hpp -o -iname *.cpp)

### CLANG-TIDY ###
.PHONY: check_tidy
check_tidy:
	clang-tidy \
	--warnings-as-errors="*" \
	$(ALL_SOURCE_FILES) \
	-- \
	-std=c++20 \
	$(INCLUDES) \

 