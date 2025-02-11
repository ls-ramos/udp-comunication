# UDP Client-Server Profile Management Demo

A simple demonstration project showcasing UDP socket programming in C. The project implements a basic client-server architecture that manages professional profiles, primarily focusing on demonstrating UDP communication patterns and basic data serialization.

## 🎯 Project Purpose

This project serves as an educational demonstration of:
- UDP socket programming in C
- Basic client-server communication
- Custom binary serialization
- File I/O operations in C
- Memory management in C

## 🌟 Key Features

- **UDP Communication**: Basic implementation of client-server communication using UDP sockets
- **Custom Serialization**: Simple binary serialization for data transfer
- **Basic Profile Operations**:
  - Create profiles
  - Add experiences
  - Search profiles
  - Remove profiles
- **File Storage**: Basic persistence using binary file storage

## 🛠️ Technical Implementation

### Core Components

- **UDP Sockets**: Implementation of datagram sockets for network communication
- **Serialization**: Basic binary serialization for struct data
- **File Operations**: Simple file-based storage system
- **Memory Management**: Basic malloc/free operations

### Build System

Simple Makefile with the following targets:
- `make all`: Builds both client and server
- `make client`: Builds only the client
- `make server`: Builds only the server
- `make clean`: Cleans build artifacts

## 🚀 Getting Started

1. Clone the repository
2. Build the project:
   ```bash
   make all
   ```
3. Start the server:
   ```bash
   ./server
   ```
4. Run the client (in a different terminal):
   ```bash
   ./client <server_ip>
   ```

## 🔄 Future Improvements

1. **Basic Reliability**:
   - Add basic error handling
   - Implement simple retry mechanism
   - Add input validation

2. **Code Structure**:
   - Improve code documentation
   - Add basic logging
   - Implement basic error messages

3. **Code Quality**:
   - Add basic error checking
   - Improve memory management
   - Add code comments

## 🏗️ Project Structure

```
.
├── client.c         # UDP client implementation
├── server.c         # UDP server implementation
├── serialize.c      # Basic serialization functions
├── serialize.h      # Serialization declarations
├── unp.h           # Common definitions
└── Makefile        # Build configuration
```

## 💻 Technical Details

- **Language**: C99
- **Network Protocol**: UDP
- **Build System**: Make
- **Storage**: Binary file

## 📝 Note

This is a demonstration project intended to show basic UDP socket programming concepts. It is not intended for production use and lacks many features necessary for a real-world application.

---

This project was developed as a learning exercise in network programming, specifically focusing on UDP socket programming in C.