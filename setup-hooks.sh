#!/bin/bash
#
# Setup script to install git hooks for the stickman project
#

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${YELLOW}Setting up git hooks for stickman project...${NC}"

# Check if we're in a git repository
if [ ! -d ".git" ]; then
    echo -e "${RED}Error: Not in a git repository${NC}"
    exit 1
fi

# Check if we're in the right directory
if [ ! -f "Makefile" ] || [ ! -f "stickman.c" ]; then
    echo -e "${RED}Error: Not in stickman project root directory${NC}"
    exit 1
fi

# Create hooks directory if it doesn't exist
if [ ! -d ".git/hooks" ]; then
    mkdir -p .git/hooks
fi

# Install pre-commit hook
if [ -f ".githooks/pre-commit" ]; then
    cp .githooks/pre-commit .git/hooks/pre-commit
    chmod +x .git/hooks/pre-commit
    echo -e "${GREEN}✓ Pre-commit hook installed${NC}"
else
    echo -e "${RED}Error: .githooks/pre-commit not found${NC}"
    exit 1
fi

# Configure git to use our hooks directory (optional, for future hooks)
git config core.hooksPath .githooks

echo ""
echo -e "${GREEN}🎉 Git hooks setup complete!${NC}"
echo ""
echo "The pre-commit hook will now run automatically and check:"
echo "  • Code formatting (clang-format)"
echo "  • Static analysis (clang-tidy, cppcheck)"  
echo "  • Unit tests"
echo ""
echo "To manually run the same checks: make verify"
echo "To bypass the hook (not recommended): git commit --no-verify"