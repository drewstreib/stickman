# Stickman Walking Animation

A simple C program that displays an animated stick figure walking across your terminal.

## Description

This program displays a looping animation of a stick figure walking, using terminal control sequences to update the display without flickering. The animation consists of 7 frames that are loaded from text files, making it easy to customize the animation.

## Features

- Smooth animation using terminal control sequences (no screen clearing)
- Customizable animation frames stored as text files
- Supports up to 20 animation frames
- Designed for 80x24 terminal size
- Hidden cursor during animation for better viewing experience

## Quick Start

### Run with Docker (Easiest)

No installation required! Just run directly from Docker Hub:

```bash
docker run -it drewstreib/stickman
```

This works on any system with Docker installed (Linux, macOS, Windows) and supports both x86_64 and ARM64 architectures.

Press `Ctrl+C` to stop the animation.

## Building from Source

### Requirements

- GCC compiler
- POSIX-compliant system (macOS, Linux)

### Compilation

```bash
make
```

To clean build artifacts:
```bash
make clean
```

## Running

```bash
make run
```

Or directly:
```bash
./stickman
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

Available for multiple architectures:
- `linux/amd64` (Intel/AMD x86_64)
- `linux/arm64` (ARM64/Apple Silicon)

### Building the Docker Image Locally

```bash
docker build -t drewstreib/stickman .
```

### Multi-architecture Build

For building multi-architecture images (amd64 and arm64):

```bash
docker buildx create --use
docker buildx build --platform linux/amd64,linux/arm64 -t drewstreib/stickman .
```

To push to Docker Hub:
```bash
docker buildx build --platform linux/amd64,linux/arm64 -t drewstreib/stickman --push .
```

## Technical Details

- Uses ANSI escape sequences for terminal control
- Implements differential rendering (only updates changed characters)
- Animation delay: 100ms between frames
- Automatically sorts animation files alphabetically
- Handles up to 20 frames maximum

## File Structure

```
stickman/
├── stickman.c       # Main program source
├── Makefile         # Build configuration
├── README.md        # This file
├── Dockerfile       # Container configuration
└── anim/           # Animation frames directory
    ├── frame01.txt
    ├── frame02.txt
    ├── frame03.txt
    ├── frame04.txt
    ├── frame05.txt
    ├── frame06.txt
    └── frame07.txt
```

## Author

Created by drewstreib