CC = g++
CFLAGS = -Wall -Wextra
CPPFLAGS = -I src -MP -MMD

APP_NAME = sort
LIB_NAME = libsort
TEST_NAME = test

BIN_DIR = bin
OBJ_DIR = obj
SRC_DIR = src
TEST_DIR = test

APP_PATH = $(APP_NAME)
LIB_PATH = $(OBJ_DIR)/$(SRC_DIR)/$(LIB_NAME)/$(LIB_NAME).a
TEST_PATH = $(BIN_DIR)/$(TEST_NAME)

SRC_EXT = cpp

APP_SOURCES = $(shell find $(SRC_DIR)/$(APP_NAME) -name '*.$(SRC_EXT)')
APP_OBJECTS = $(APP_SOURCES:$(SRC_DIR)/%.$(SRC_EXT)=$(OBJ_DIR)/$(SRC_DIR)/%.o)

LIB_SOURCES = $(shell find $(SRC_DIR)/$(LIB_NAME) -name '*.$(SRC_EXT)')
LIB_OBJECTS = $(LIB_SOURCES:$(SRC_DIR)/%.$(SRC_EXT)=$(OBJ_DIR)/$(SRC_DIR)/%.o)

TEST_SOURCE = $(shell find $(TEST_DIR) -name '*.$(SRC_EXT)')
TEST_OBJECTS = $(TEST_SOURCE:$(TEST_DIR)/%.$(SRC_EXT)=$(OBJ_DIR)/$(TEST_DIR)/%.o)

DEPS = $(APP_OBJECTS:.o=.h) $(LIB_OBJECTS:.o=.h)

all: $(APP_PATH)

-include $(DEPS)


$(APP_PATH): $(APP_OBJECTS) $(LIB_PATH)
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@

$(LIB_PATH): $(LIB_OBJECTS)
	ar rcs $@ $^

$(OBJ_DIR)/$(SRC_DIR)/sort/%.o: $(SRC_DIR)/sort/%.cpp
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@


.PHONY: run clean test

run:
	./sort --help



clean:
	$(RM) obj/src/libsort/*.o
	$(RM) obj/src/sort/*.o
	$(RM) obj/src/libsort/*.d
	$(RM) obj/src/sort/*.d
	$(RM) obj/src/libsort/*.a
	$(RM) obj/src/sort/*.a
	$(RM) obj/src/sort/*.a
	$(RM) obj/test/*.o
	$(RM) obj/test/*.d
	$(RM) obj/test/*.a
	$(RM) bin/*.exe
	$(RM) sort