FROM python:3.11-rc-slim AS builder

RUN for name in 'ctypes' 'pickle' 'test' 'cffi'; do \
        find /usr/local/lib/python3.11/ -name "*${name}*" -exec rm -rf '{}' '+'; \
    done

FROM debian:bookworm-slim

# Install required packages including build tools
RUN apt-get update && apt-get install -y --no-install-recommends \
    coreutils \
    build-essential \ 
    libseccomp-dev \
    gcc \
    make \
    && rm -rf /var/lib/apt/lists/*

# Copy source files and build preload.so and flag
COPY build/Makefile build/preload.c build/flag.c /build/
WORKDIR /build
RUN make all

# Set up chroot environment
RUN mkdir -p /chroot/lib /chroot/usr/lib /chroot/lib64 /chroot/bin /chroot/usr/local/lib && \
    cp -a /lib/* /chroot/lib/ && \
    cp -a /usr/lib/* /chroot/usr/lib/ && \
    if [ -d "/lib64" ]; then \
        cp -r /lib64/* /chroot/lib64/ || true; \
    fi

# Copy Python and its dependencies
COPY --from=builder /usr/local/lib/python3.11/ /chroot/usr/local/lib/python3.11/
COPY --from=builder /usr/local/lib/libpython3.11.so.1.0 /chroot/usr/lib/libpython3.11.so.1.0
COPY --from=builder /usr/local/bin/python3.11 /chroot/bin/python3.11

# Copy bash
COPY --from=builder /bin/bash /chroot/bin/bash

# Copy our compiled binaries to challenge directory
RUN mkdir -p /chroot/challenge && \
    cp /build/preload.so /chroot/challenge/ && \
    cp /build/flag /chroot/challenge/

COPY balloon.py /chroot/challenge/

RUN chmod 111 /chroot/challenge/flag \
    && chmod 555 /chroot/challenge/preload.so \
    && chmod 444 /chroot/challenge/balloon.py \
    && chmod 755 /chroot/bin/python3.11

COPY entrypoint.sh /tmp/entrypoint.sh
RUN chmod 555 /tmp/entrypoint.sh

ENTRYPOINT [ "/tmp/entrypoint.sh" ]
