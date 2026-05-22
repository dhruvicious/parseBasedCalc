FROM ubuntu:24.04 as builder

RUN apt-get update && \
    apt-get install -y \
    g++ \
    make \
    libreadline-dev

WORKDIR /app

COPY . .

RUN make

FROM ubuntu:24.04

RUN apt-get update && \
    apt-get install -y \ 
    libreadline8 && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --from=builder /app/calculator .

CMD [ "./calculator" ]
