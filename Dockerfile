FROM ubuntu:24.04 AS builder

RUN apt-get update && apt-get install -y --no-install-recommends \
    g++ \
    make \
    libreadline-dev \
    libcurl4-openssl-dev

WORKDIR /app

# Still keeping layer optimization by splitting COPY
COPY Makefile ./
COPY src/ ./src/

RUN make -j$(nproc)

FROM ubuntu:24.04

# Added ca-certificates to the runtime stage
RUN apt-get update && apt-get install -y --no-install-recommends \
    libreadline8 \
    libcurl4 \
    ca-certificates && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Standard copy and chown (no --link)
COPY --from=builder --chown=ubuntu:ubuntu /app/calculator .

USER ubuntu

ENTRYPOINT [ "./calculator" ]
