FROM alpine:latest as builder
RUN apk update && apk add --no-cache \
    clang \
    boost-dev \
    protobuf-dev \
    protoc \
    cmake \
    make \
    g++ \
    git \
    ninja
RUN git clone https://github.com/protocolbuffers/protobuf /protobuf --recursive --depth=1 && cd /protobuf && CC=clang CXX=clang++ cmake . -GNinja -GNinja && cmake --build .
RUN cd /protobuf && cmake --install .
COPY . /app
WORKDIR /app/build
RUN CC=clang CXX=clang++ cmake .. -GNinja -DCMAKE_UNITY_BUILD=ON
RUN cmake --build .
FROM alpine:latest
ENV ADDRESS=Please_Set_me
ENV RESEED=knvnfuid67t6l7sgh42rrcscjoypxmqcosdc3dxbrlacks4ryaua.b32.i2p:6002
ENV PROXY=127.0.0.1:4447
RUN apk update && apk add --no-cache \
    boost-program_options boost-system \
    protobuf-dev
COPY --from=builder /app/build/ChessRouter/ChessRouter /usr/local/bin/ChessRouter

CMD ChessRouter --address $ADDRESS --reseed $RESEED --proxy $PROXY
