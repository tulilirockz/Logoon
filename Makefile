CC = clang
CFLAGS = -Wall -Werror
BUILD_DIR = build
DIST_DIR = dist
BIN = logoon

build: src/*.c
	mkdir -p "$(BUILD_DIR)"
	"$(CC)" -o "$(BUILD_DIR)/$(BIN)" $(CFLAGS) $^
	chmod +x "$(BUILD_DIR)/$(BIN)"

clean:
	-rm "$(BUILD_DIR)/*" "$(DIST_DIR)/*"

.PHONY: run
run:
	make build
	"./$(BUILD_DIR)/$(BIN)"

release: src/*.c
	mkdir -p "$(DIST_DIR)"
	"$(CC)" $(CFLAGS) -O3 -o "$(DIST_DIR)/$(BIN)" $^
	strip "$(DIST_DIR)/$(BIN)"
	chmod +x "$(DIST_DIR)/$(BIN)"

install:
	make release
	sudo install -m0755 "$(DIST_DIR)/$(BIN)" "/usr/local/bin/$(BIN)"
