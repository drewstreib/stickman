# Stickman Walking Animation

A simple C program that displays an animated stick figure walking across your terminal.

## Description

This program displays a looping animation of a stick figure walking, using terminal control sequences to update the display without flickering. The animation consists of 7 frames that are loaded from text files, making it easy to customize the animation.

## Features

- **Smooth animation** using terminal control sequences (no screen clearing)
- **Customizable animation frames** stored as text files
- **Cross-platform** support via Docker (Linux, macOS, Windows)
- **Multi-architecture** Docker images (x86_64 and ARM64)
- **Professional C codebase** with comprehensive tooling
- **Unit testing framework** with 7 test cases
- **Signal handling** for graceful exit (Ctrl+C)
- **Command line options** (-h, -v flags)

## Quick Start

### Run with Docker (Easiest)

No installation required! Just run directly from Docker Hub:

```bash
docker run -it drewstreib/stickman
```

**Version-specific tags available:**
```bash
docker run -it drewstreib/stickman:v1.0.0  # Specific version
docker run -it drewstreib/stickman:v1.0    # Minor version
docker run -it drewstreib/stickman:v1      # Major version  
docker run -it drewstreib/stickman:latest  # Latest release
```

This works on any system with Docker installed and supports both x86_64 and ARM64 architectures.

Press `Ctrl+C` to stop the animation.

## Building from Source

### Requirements

- **GCC compiler** with C11 support
- **POSIX-compliant system** (macOS, Linux)
- **Optional tools:** clang-format, clang-tidy, cppcheck (for development)

### Quick Build

```bash
make
```

### Development Build

```bash
make debug    # Build with debug symbols
make verify   # Run all quality checks (format, lint, test)
```

### Available Make Targets

```bash
make help     # Show all available targets
```

## Running

```bash
make run      # Build and run
./stickman    # Run directly
./stickman -h # Show help
./stickman -v # Show version
```

Press `Ctrl+C` to stop the animation.

## Customizing the Animation

The animation frames are stored as text files in the `anim/` directory. Each file represents one frame of the animation and should be designed for an 80x24 terminal.

To create your own animation:
1. Create text files in the `anim/` directory
2. Each file should contain your ASCII art (max 80 characters wide, 24 lines tall)
3. Name the files in alphabetical order (they will be played in sorted order)
4. The program will automatically load and play all files in the directory

## Docker

### Running from Docker Hub

The easiest way to run the animation:

```bash
docker run -it drewstreib/stickman
```

**Available architectures:**
- `linux/amd64` (Intel/AMD x86_64)
- `linux/arm64` (ARM64/Apple Silicon)

### Building the Docker Image Locally

```bash
docker build -t drewstreib/stickman .
```

### Multi-architecture Build

For building multi-architecture images:

```bash
./build-multiarch.sh
```

Or manually:
```bash
docker buildx create --use
docker buildx build --platform linux/amd64,linux/arm64 -t drewstreib/stickman .
```

## Testing

The project includes a comprehensive unit testing framework to ensure code quality.

### Running Tests

```bash
make test
```

### Test Coverage

Current tests cover:
- **Frame loading functionality** (basic, empty, nonexistent files)
- **Filename comparison and sorting** algorithms
- **Data structure constraints** and validation
- **Constants sanity checks** and bounds
- **Basic error handling** scenarios

**Note:** These tests are somewhat perfunctory for the current simple functionality, but establish good practices for code quality and provide a foundation for testing as the project expands.

### Development Workflow

```bash
make verify      # Run all quality checks (format, lint, test)
make format      # Format code with clang-format
make lint        # Run clang-tidy static analysis
make check       # Run cppcheck static analysis
make setup-hooks # Install git pre-commit hooks (recommended)
```

### Pre-commit Hooks

The project includes git pre-commit hooks that automatically run quality checks before each commit:

```bash
make setup-hooks    # Install pre-commit hooks
make test-hooks     # Test hooks manually
make remove-hooks   # Remove pre-commit hooks
```

The pre-commit hook will automatically check:
- **Code formatting** (clang-format)
- **Static analysis** (clang-tidy, cppcheck)
- **Unit tests** (all tests must pass)

If any check fails, the commit will be blocked until issues are fixed. To bypass (not recommended):
```bash
git commit --no-verify
```

## Technical Details

- **Language:** C11 with GNU extensions
- **Terminal control:** ANSI escape sequences
- **Rendering:** Differential updates (only changes characters that differ)
- **Animation timing:** 100ms between frames (configurable)
- **File handling:** Robust loading with multiple line ending support
- **Memory management:** Proper allocation/deallocation with cleanup
- **Signal handling:** Graceful shutdown on SIGINT/SIGTERM
- **Architecture:** Modular design with separation of concerns

## Project Structure

```
stickman/
├── stickman.c              # Main program source
├── stickman.h              # Public header
├── stickman_internal.h     # Internal functions (testing)
├── Makefile                # Comprehensive build system
├── README.md               # This documentation
├── LICENSE                 # MIT License
├── Dockerfile              # Multi-stage container build
├── .clang-format          # Code formatting rules
├── .gitignore             # Git ignore patterns
├── .dockerignore          # Docker ignore patterns
├── build-multiarch.sh     # Multi-architecture build script
├── tests/                 # Unit testing framework
│   └── test_stickman.c    # Test suite (7 test cases)
└── anim/                  # Animation frames directory
    ├── frame01.txt        # Walking animation frame 1
    ├── frame02.txt        # Walking animation frame 2
    ├── frame03.txt        # Walking animation frame 3
    ├── frame04.txt        # Walking animation frame 4
    ├── frame05.txt        # Walking animation frame 5
    ├── frame06.txt        # Walking animation frame 6
    └── frame07.txt        # Walking animation frame 7
```

## Contributing

This project follows modern C development practices:

1. **Code style:** Enforced by clang-format
2. **Static analysis:** clang-tidy and cppcheck integration
3. **Testing:** Unit tests with custom lightweight framework
4. **Pre-commit hooks:** Automatic quality checks before commits
5. **Documentation:** Comprehensive inline and external docs
6. **CI/CD:** Multi-architecture Docker builds

**Getting started with development:**
```bash
git clone https://github.com/drewstreib/stickman.git
cd stickman
make setup-hooks  # Install pre-commit hooks
make verify       # Run all quality checks
```

Pull requests welcome! Please ensure `make verify` passes and hooks are installed.

## License

MIT License - see [LICENSE](LICENSE) file for details.

## Version

Current version: **v1.0.0**

## Author

Created by [drewstreib](https://github.com/drewstreib)

---

**Repository:** https://github.com/drewstreib/stickman  
**Docker Hub:** https://hub.docker.com/r/drewstreib/stickman