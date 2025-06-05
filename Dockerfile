# Multi-stage build for smaller final image
FROM gcc:latest AS builder

WORKDIR /app

# Copy source files
COPY stickman.c Makefile ./

# Build the application
RUN make

# Final stage
FROM debian:bookworm-slim

# Install runtime dependencies
RUN apt-get update && apt-get install -y --no-install-recommends \
    libc6 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy the compiled binary and animation files
COPY --from=builder /app/stickman .
COPY anim ./anim

# Run the animation
CMD ["./stickman"]