#!/bin/bash

# Script to build multi-architecture Docker images

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

echo "Building multi-architecture Docker image for stickman..."

# Check if buildx is available
if ! docker buildx version &> /dev/null; then
    echo -e "${RED}Error: Docker buildx is not available${NC}"
    echo "Please update Docker to a version that includes buildx"
    exit 1
fi

# Create and use a new builder instance
echo "Setting up buildx builder..."
docker buildx create --name stickman-builder --use 2>/dev/null || docker buildx use stickman-builder

# Build for multiple architectures
echo "Building for linux/amd64 and linux/arm64..."
docker buildx build \
    --platform linux/amd64,linux/arm64 \
    -t drewstreib/stickman:latest \
    .

if [ $? -eq 0 ]; then
    echo -e "${GREEN}Build successful!${NC}"
    echo ""
    echo "To push to Docker Hub, run:"
    echo "  docker buildx build --platform linux/amd64,linux/arm64 -t drewstreib/stickman:latest --push ."
else
    echo -e "${RED}Build failed!${NC}"
    exit 1
fi