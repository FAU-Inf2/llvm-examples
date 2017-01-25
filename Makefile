#----------------------------------------------------------
# Makefile for building an example LLVM pass "out-of-tree"
#----------------------------------------------------------

# Run 'make VERBOSE=1' to output executed commands
ifdef VERBOSE
	VERB := 
else
	VERB := @
endif

# Run 'make LLVM_VERSION=<V>' to set LLVM version to <V>
LLVM_VERSION := "3.8"

# Set C++ compiler and compiler flags used to compile the project
CXX := g++
CXXFLAGS :=

# Set linker and linker flags used to link the project's object files
LD := g++
LDFLAGS :=

# Path to the llvm-config tool
# If this tool resides in a global path, 'llvm-config' should suffice
LLVM_CONFIG := llvm-config-$(LLVM_VERSION)

# Compiler and linker flags needed to link against the LLVM libraries
LLVM_CXXFLAGS := `$(LLVM_CONFIG) --cxxflags`
LLVM_LDFLAGS := `$(LLVM_CONFIG) --ldflags --system-libs`

# Additional include path to find LLVM headers
LLVM_INCLUDE := -I`$(LLVM_CONFIG) --includedir`

# Directories for the source and object files, as well as for the dependency files
DIR_SRC := src
DIR_OBJ := obj
DIR_DEP := dep

# Target file
DIR_BIN := bin
TARGET := $(DIR_BIN)/instruction_counter.so

# Get all source and object files
SRC_FILES := $(shell find $(DIR_SRC) -name "*.cpp")
OBJ_FILES := $(patsubst %.cpp, $(DIR_OBJ)/%.o, $(notdir $(SRC_FILES)))


#----------------------------------------------------------------------------
#----------------------------------------------------------------------------


# Print 'header' whenever make is issued
$(info --------------------------)
$(info Example LLVM analysis pass)
$(info --------------------------)
$(info )


#----------------------------------------------------------------------------
#----------------------------------------------------------------------------


.PHONY: all
all: $(TARGET)


$(TARGET): $(OBJ_FILES)
	$(VERB)mkdir -p $(DIR_BIN)
	@echo -e 'LD\t$@'
	$(VERB)$(CXX) -shared $(LDFLAGS) $^ $(LLVM_LDFLAGS) -o $@


-include $(patsubst %.cpp, $(DIR_DEP)/%.md, $(notdir $(SRC_FILES)))


$(DIR_OBJ)/%.o: $(DIR_SRC)/%.cpp
	$(VERB)mkdir -p $(DIR_OBJ)
	$(VERB)mkdir -p $(DIR_DEP)
	@echo -e 'DEP\t$<'
	$(VERB)$(CXX) $(LLVM_CXXFLAGS) $(CXXFLAGS) $(LLVM_INCLUDE) -MM -MP -MT $(DIR_OBJ)/$(*F).o -MT $(DIR_DEP)/$(*F).md $< > $(DIR_DEP)/$(*F).md
	@echo -e 'CMPL\t$<'
	$(VERB)$(CXX) $(LLVM_CXXFLAGS) $(CXXFLAGS) $(LLVM_INCLUDE) -c $< -o $@


.PHONY: clean
clean:
	@echo -e 'RM\t $(DIR_OBJ)'
	$(VERB)rm -rf $(DIR_OBJ)
	@echo -e 'RM\t $(DIR_BIN)'
	$(VERB)rm -rf $(DIR_BIN)
	@echo -e 'RM\t $(DIR_DEP)'
	$(VERB)rm -rf $(DIR_DEP)
