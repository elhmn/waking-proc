# waking-proc

Unix process visualiser

## Architecture

The project is composed of 3 main components:

- a C library that provides an API to interact with the system's processes
- a server written in C that will implement a TCP server that will provide process data to clients
- a Rust client that will connect to the server and display the process visualisation

The choice of languages is solely based on my current learning goals.

```mermaid
flowchart LR
    A[Client or visualiser] --> B[Server]
    B --> C[Library]
    B -->  A
```

## How to run


### Library

The library is written in C and can be built using the provided Makefile.
running `make` will build the `libwaking-proc.a` library, that can be linked to any C or Rust program.

```sh
cd lib
make
```

### Server

The server has the `libwaking-proc.a` library as a dependency and can be built using the provided Makefile.

```sh
cd server
make
./waking-proc-server
```
### Client

The client is written in Rust and can be built using cargo.

```sh
cargo run
```
