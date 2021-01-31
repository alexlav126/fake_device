TARGET = libproto

CFLAGS = -Wall -g -std=c++17

COMPILE = g++ $(CFLAGS) -c
ARCHIVE = ar rvs

BUILD_DIR = ../build
BUILD_DIR_OBJ = $(BUILD_DIR)/$(TARGET)_obj

SOURCES = \
proto.cpp \
command.cpp \
command_start_measure.cpp \
command_stop_measure.cpp \
command_set_range.cpp \
command_get_status.cpp \
command_get_result.cpp

OBJECTS = $(addprefix $(BUILD_DIR_OBJ)/,$(notdir $(SOURCES:.cpp=.o)))
vpath %.cpp $(sort $(dir $(SOURCES)))

BIN = $(BUILD_DIR)/$(TARGET).a

all: $(BIN)

$(BIN): $(OBJECTS) | $(BUILD_DIR)
	$(ARCHIVE) $(BIN) $(OBJECTS)

$(BUILD_DIR_OBJ)/%.o: %.cpp | $(BUILD_DIR_OBJ)
	$(COMPILE) $< -o $@

$(BUILD_DIR_OBJ):
	mkdir -p $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -Rf $(BUILD_DIR_OBJ)
	rm -f $(BIN)