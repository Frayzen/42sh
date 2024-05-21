# 42sh - A POSIX-Compliant Shell

## Overview
**42sh** is a POSIX-compliant shell created as part of an EPITA project. The shell aims to replicate the functionality of `bash --posix`, providing a robust and efficient command-line interface. This project adheres strictly to POSIX standards and SCL (Software Configuration Library) documentation.

## Features
- **POSIX Compliance**: 42sh follows POSIX rules to ensure compatibility and standard behavior across different UNIX-like systems.
- **Bash-like Interface**: The shell is designed to work exactly like `bash --posix`, providing a familiar environment for users.
- **Built-in Commands**: Supports a range of built-in commands similar to those found in bash.
- **Scripting**: Allows the execution of shell scripts, adhering to POSIX scripting conventions.
- **Customizable**: Users can extend functionality and customize their environment.

## Getting Started

### Prerequisites
Ensure you have the following tools installed on your system:
- GNU Autotools (Autoconf, Automake, Libtool)
- Make
- GCC (GNU Compiler Collection)

### Installation

Clone the repository:
```sh
git clone https://github.com/Frayzen/42sh.git
cd 42sh
```

Generate the configuration files using Autotools:
```sh
autoreconf -i
```

Configure the project:
```sh
./configure
```

Compile the project:
```sh
make
```

### Usage

To create the shell executable:
```sh
make create
```

This will generate the `./src/42sh` executable.

To execute functional tests:
```sh
make funct
```

### Running the Shell

After compilation, you can run the shell using:
```sh
./src/42sh
```

## Acknowledgements

This project is developed as part of the EPITA curriculum. We thank our instructors and peers for their support and guidance.

### Google Slide Presentation for Defense
We have created a Google Slide presentation to support our defense of the 42sh project. The presentation includes:
[https://docs.google.com/presentation/d/1x9QjPQar7kT1-aK1lZbPzEg8iUtT91ix0EM9TU9YInw/edit?usp=sharing](The link is available here)

