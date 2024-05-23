FROM alpine:3.19
RUN apk update && \
    apk add --no-cache \
        build-base make gdb
