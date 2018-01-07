TARGET = apbuilder
CC = gcc

BIN_DIR  = bin

CC_FLAGS = -Wall -Werror

SOURCES = src/*.c


all: prepare build

clean:
	@rm -rf $(BIN_DIR)/$(TARGET)

prepare:
	mkdir -p $(BIN_DIR)
	rm -f $(BIN_DIR)/$(TARGET)

build:
	$(CC) -o $(BIN_DIR)/$(TARGET) $(CC_FLAGS) $(SOURCES)

install:
	install -m 755 $(BIN_DIR)/$(TARGET) /usr/local/bin

uninstall:
	rm -f  /usr/local/bin/$(TARGET)


