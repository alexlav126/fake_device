TARGET = client
BUILD_DIR = ../build

# LIB = $(BUILD_DIR)/libproto.a

CFLAGS = -Wall -g -std=c++17
LDFLAGS = -Wall -g -std=c++17 -lpthread

COMPILE = g++ $(CFLAGS) -c
LINK = g++ $(LDFLAGS) -o


BUILD_DIR_OBJ = $(BUILD_DIR)/$(TARGET)_obj

SOURCES = \
client.cpp

OBJECTS = $(addprefix $(BUILD_DIR_OBJ)/,$(notdir $(SOURCES:.cpp=.o)))
vpath %.cpp $(sort $(dir $(SOURCES)))

BIN = $(BUILD_DIR)/$(TARGET)

all: $(BIN)

$(BIN): $(OBJECTS) | $(BUILD_DIR)
	$(LINK) $(BIN) $(OBJECTS)
#	$(LINK) $(BIN) $(OBJECTS) $(LIB)

$(BUILD_DIR_OBJ)/%.o: %.cpp | $(BUILD_DIR_OBJ)
	$(COMPILE) $< -o $@

$(BUILD_DIR_OBJ):
	mkdir -p $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -Rf $(BUILD_DIR_OBJ)
	rm -f $(BIN)