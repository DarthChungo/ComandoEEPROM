CXX      := -c++
CXXFLAGS := -pedantic-errors -Wall -Wextra -std=c++20
LDFLAGS  := -L/usr/lib -lstdc++ -lm

BUILD    := ./build
OBJ_DIR  := $(BUILD)
BIN_DIR  := $(BUILD)/bin

TARGET   := maker
INCLUDE  := -Iinclude/

SRC      := $(shell find src/ -name "*.cpp")
OBJECTS  := $(SRC:%.cpp=$(OBJ_DIR)/%.o)

all: release run

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@ $(LDFLAGS)

$(BIN_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/$(TARGET) $^ $(LDFLAGS)

.PHONY: all clean debug release run

run:
	$(BIN_DIR)/$(TARGET)

build: $(BIN_DIR)/$(TARGET)
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(OBJ_DIR)

debug: CXXFLAGS += -DDEBUG -g
debug: build

release: CXXFLAGS += -O2
release: build

clean:
	-@rm -rvf $(BUILD)/*