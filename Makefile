CC = clang
CFLAGS = -Wall -Werror -Wextra -pedantic
BUILD_DIR = build
DIST_DIR = dist
BIN = logoon
BINDIR = /usr/local/bin

build: src/*.c
	mkdir -p "$(BUILD_DIR)"
	"$(CC)" -o "$(BUILD_DIR)/$(BIN)" $(CFLAGS) $^
	chmod +x "$(BUILD_DIR)/$(BIN)"

clean:
	-rm -r "$(BUILD_DIR)" "$(DIST_DIR)"

.PHONY: run
run:
	make build
	"./$(BUILD_DIR)/$(BIN)"

release: src/*.c
	mkdir -p "$(DIST_DIR)"
	"$(CC)" $(CFLAGS) -s -o "$(DIST_DIR)/$(BIN)" $^
	chmod +x "$(DIST_DIR)/$(BIN)"

install:
	make release
	install -m0755 "$(DIST_DIR)/$(BIN)" "$(BINDIR)/$(BIN)"
