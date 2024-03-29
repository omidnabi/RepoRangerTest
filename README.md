# RepoRanger and Tester Documentation


RepoRanger is a Qt/C++ application designed for monitoring and managing repository updates. This document provides instructions on building and running RepoRanger, as well as using the accompanying Tester for Linux environments.

## Getting Started

These instructions will guide you through setting up RepoRanger on your local machine for development and testing purposes.

### Prerequisites

Before you begin, ensure you have the following installed:
- Git
- CMake (version 3.x or higher)
- A C++ compiler (GCC for Linux/macOS, MSVC for Windows)
- Qt SDK

### Cloning the Repository

Clone the RepoRanger repository using Git:

```sh
git clone https://github.com/omidnabi/RepoRangerTest.git
cd RepoRangerTest/RepoRanger
```

### Building RepoRanger

#### Unix-like Systems (Linux/macOS)

To build RepoRanger on Unix-like systems, execute the following:

```sh
./build.sh
```
Ensure `build.sh` is executable. If necessary, run `chmod +x build.sh`.

#### Windows

For Windows, utilize the CMake GUI or command line to configure and generate the build system, then compile with your compiler. Note: The Tester is designed for Linux and may not work on Windows. Use appropriate Windows testing tools.

### Running RepoRanger

After building, run RepoRanger from the build directory:

```sh
./build/RepoRanger
```

### Configuration

Configure RepoRanger using the `managerConfig.json` file provided in the `RepoRanger` directory. Ensure the configuration suits your setup before running the application.

## Testing with the Tester (Linux Only)

The Tester is intended for Linux environments to facilitate RepoRanger testing. Follow these steps to use the Tester:

1. Confirm RepoRanger is configured and running.
2. Start RepoRanger process
3. Navigate to the `Tester` directory within `RepoRangerTest`.
4. Start the testing process using `start.sh`:

```sh
./start.sh
```
Make `start.sh` executable with `chmod +x start.sh`.

5. Make changes to the tester repo to trigger an update. 



