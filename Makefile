# Tool paths (adjust these for your system)
HOMEBREW_PREFIX = /opt/homebrew
LLVM_PATH = $(HOMEBREW_PREFIX)/opt/llvm/bin

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c11
CFLAGS += -Wformat=2 -Wconversion -Wundef -Wshadow -Wpointer-arith
CFLAGS += -Wcast-qual -Wcast-align -Wwrite-strings -Wstrict-prototypes
CFLAGS += -Wmissing-prototypes -Wmissing-declarations -Wredundant-decls
CFLAGS += -Wnested-externs -Winline -Wuninitialized -Wstrict-aliasing
LDFLAGS = 
DEBUGFLAGS = -g -O0 -DDEBUG
RELEASEFLAGS = -O2 -DNDEBUG

# Linting and formatting tools
CLANG_FORMAT = clang-format
CLANG_TIDY = $(LLVM_PATH)/clang-tidy
CPPCHECK = cppcheck

# Project structure
TARGET = stickman
SOURCES = stickman.c
OBJECTS = $(SOURCES:.c=.o)
DEPS = $(SOURCES:.c=.d)

# Default target
.PHONY: all
all: release

# Release build
.PHONY: release
release: CFLAGS += $(RELEASEFLAGS)
release: $(TARGET)

# Debug build
.PHONY: debug
debug: CFLAGS += $(DEBUGFLAGS)
debug: $(TARGET)

# Build executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Pattern rule for object files
%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# Include dependency files
-include $(DEPS)

# Run the program
.PHONY: run
run: $(TARGET)
	./$(TARGET)

# Format code
.PHONY: format
format:
	@if command -v $(CLANG_FORMAT) >/dev/null 2>&1; then \
		echo "Formatting code..."; \
		$(CLANG_FORMAT) -i $(SOURCES); \
	else \
		echo "clang-format not found. Install with: brew install clang-format"; \
	fi

# Lint with clang-tidy
.PHONY: lint
lint:
	@if command -v $(CLANG_TIDY) >/dev/null 2>&1; then \
		echo "Running clang-tidy..."; \
		$(CLANG_TIDY) $(SOURCES) -- $(CFLAGS); \
	else \
		echo "clang-tidy not found. Install with: brew install llvm"; \
	fi

# Static analysis with cppcheck
.PHONY: check
check:
	@if command -v $(CPPCHECK) >/dev/null 2>&1; then \
		echo "Running cppcheck..."; \
		$(CPPCHECK) --enable=all --suppress=missingIncludeSystem $(SOURCES); \
	else \
		echo "cppcheck not found. Install with: brew install cppcheck"; \
	fi

# Run all checks
.PHONY: verify
verify: format lint check

# Memory check with valgrind (if available on macOS)
.PHONY: memcheck
memcheck: debug
	@if command -v valgrind >/dev/null 2>&1; then \
		valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET); \
	else \
		echo "valgrind not available on macOS. Use leaks command instead."; \
		echo "Running leaks..."; \
		leaks --atExit -- ./$(TARGET); \
	fi

# Clean build artifacts
.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJECTS) $(DEPS)
	rm -rf *.dSYM

# Install (optional)
.PHONY: install
install: release
	@echo "Installing to /usr/local/bin (requires sudo)"
	@sudo cp $(TARGET) /usr/local/bin/

# Uninstall
.PHONY: uninstall
uninstall:
	@echo "Removing from /usr/local/bin (requires sudo)"
	@sudo rm -f /usr/local/bin/$(TARGET)

# Help
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  all      - Build release version (default)"
	@echo "  release  - Build optimized version"
	@echo "  debug    - Build debug version with symbols"
	@echo "  run      - Build and run the program"
	@echo "  format   - Format code with clang-format"
	@echo "  lint     - Run clang-tidy static analysis"
	@echo "  check    - Run cppcheck static analysis"
	@echo "  verify   - Run format, lint, and check"
	@echo "  memcheck - Check for memory leaks"
	@echo "  clean    - Remove build artifacts"
	@echo "  install  - Install to /usr/local/bin"
	@echo "  help     - Show this help message"