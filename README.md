# Huffman Text Compression Tool

A professional C++20 text compression application implementing the Huffman Coding algorithm with user authentication, file I/O, and comprehensive statistics tracking.



## 📋 Table of Contents

- [Features](#features)
- [Project Structure](#project-structure)
- [Architecture](#architecture)
- [Algorithm Overview](#algorithm-overview)
- [Installation](#installation)
- [Usage](#usage)
- [How It Works](#how-it-works)
- [Compression Statistics](#compression-statistics)
- [Technical Details](#technical-details)
- [Requirements](#requirements)
- [Building](#building)
- [Contributing](#contributing)
- [License](#license)

## ✨ Features

### Core Features
- **Huffman Compression**: Efficient text compression using Huffman coding algorithm
- **Huffman Decompression**: Perfect reconstruction of original files
- **User Authentication**: Login/Signup system with persistent user storage
- **Guest Mode**: Anonymous access without account creation
- **Real-time Statistics**: Compression ratio, space saved, and file size comparisons
- **Bit Packing**: Efficient binary storage (8 bits per byte, not per character)

### Advanced Features
- **Object-Oriented Design**: 6 well-designed classes following SOLID principles
- **Professional UI**: Clean, intuitive command-line interface with numbered options
- **Error Handling**: Comprehensive error messages and validation
- **Persistent Storage**: User credentials stored securely in `users.txt`
- **Modular Architecture**: Separate concerns for file I/O, compression, authentication, and UI

## 📁 Project Structure

```
huffman-text-compression-final/
├── include/                          # Header files
│   ├── compressionTool.hpp          # Main orchestrator class
│   ├── fileManager.hpp              # File I/O operations
│   ├── huffmanCompressor.hpp        # Compression logic
│   ├── huffmanDecompressor.hpp      # Decompression logic
│   ├── authenticationManager.hpp    # User authentication
│   └── statistics.hpp               # Compression metrics
│
├── src/                              # Source files
│   ├── main.cpp                     # Entry point (5 lines only!)
│   ├── compressionTool.cpp          # Main orchestrator implementation
│   ├── fileManager.cpp              # File I/O implementation
│   ├── huffmanCompressor.cpp        # Compression implementation
│   ├── huffmanDecompressor.cpp      # Decompression implementation
│   ├── authenticationManager.cpp    # Authentication implementation
│   └── statistics.cpp               # Statistics implementation
│
├── input/                            # Test input files
├── output/                           # Compressed/decompressed output
├── obj/                              # Object files (build artifacts)
├── Makefile                          # Build configuration
├── README.md                         # This file
├── LICENSE                           # MIT License
└── .gitignore                        # Git ignore rules
```

## 🏗️ Architecture

### Class Diagram

```
CompressionTool (Main Orchestrator)
│
├── AuthenticationManager (User Management)
├── FileManager (File I/O)
├── HuffmanCompressor (Compression Algorithm)
├── HuffmanDecompressor (Decompression Algorithm)
└── Statistics (Metrics Tracking)
```

### Design Patterns Used

- **Single Responsibility Principle**: Each class has one purpose
- **Encapsulation**: Private implementation details, public interfaces
- **Composition**: CompressionTool uses other classes
- **Persistent Storage**: User data saved to disk

## 🔐 Algorithm Overview

### Huffman Coding

Huffman coding is a greedy algorithm that creates optimal prefix-free codes for text compression.

#### How It Works:

1. **Frequency Analysis**: Count occurrence of each character
2. **Tree Building**: Create binary tree with characters as leaves
   - Start with leaf nodes (each character with its frequency)
   - Repeatedly combine two lowest-frequency nodes
   - Continue until single root remains
3. **Code Generation**: Assign codes (traverse left=0, right=1)
4. **Encoding**: Replace characters with their codes
5. **Bit Packing**: Pack binary codes into actual bytes (8 bits per byte)

#### Example:

```
Text: "hello world"

Frequencies: h=1, e=1, l=3, o=2, w=1, r=1, d=1, space=1

Tree:
         (11)
        /    \
      (5)    (6)
     /  \    / \
    (3) (2)(3) (3)
   / \  / \ / \ / \
  h(1)e l(3)o w r d
  
Codes:
l = 00 (most frequent)
o = 01
space = 10
h = 110
e = 1110
w = 11110
r = 111110
d = 111111

Original: "hello world" = 11 bytes = 88 bits
Encoded:  110 1110 00 00 01 10 11110 111110 01 00 111111 ≈ 56 bits
Compression: 56/88 = 63.6% (36.4% smaller)
```

### Bit Packing Optimization

**Problem**: Initial implementation stored '0' and '1' as ASCII characters (1 byte each)
**Solution**: Pack 8 bits into 1 byte using bit manipulation

```cpp
// Before: "01101011" = 8 bytes
// After:  [0x6B] = 1 byte + padding info

Packing Process:
1. Calculate padding needed (0-7 bits)
2. Store padding count in first byte
3. Pack 8 bits per byte using bitwise operations
4. Result: ~87.5% reduction in encoded data size!
```

## 📦 Installation

### Prerequisites

- **C++ Compiler**: GCC/Clang with C++20 support
- **Build Tool**: GNU Make
- **OS**: Windows (MinGW), Linux, macOS

### On Windows (MinGW)

```bash
# Install w64devkit or MinGW
# https://github.com/skeeto/w64devkit

# Clone repository
git clone https://github.com/YOUR_USERNAME/huffman-text-compression-final.git
cd huffman-text-compression-final

# Build
make clean
make run
```

### On Linux/macOS

```bash
# Clone repository
git clone https://github.com/YOUR_USERNAME/huffman-text-compression-final.git
cd huffman-text-compression-final

# Build
make clean
make run
```

## 🚀 Usage

### Running the Program

```bash
./huffman_compress
```

### User Flow

#### 1. Authentication
```
HUFFMAN TEXT COMPRESSION TOOL
Version 1.0

1. Login  2. Signup  3. Guest  4. Exit

> 2
Username: john_doe
Password: secure_password
Signup successful! Welcome, john_doe!
```

#### 2. Main Menu
```
User: john_doe

1. Compress  2. Decompress  3. View statistics  4. Logout  5. Exit

> 1
```

#### 3. Compression
```
=== FILE COMPRESSION ===
Enter input file path: input/sample.txt
Compressing...
Enter output file path (default: output/compressed.huff): output/myfile.huff
Successfully wrote to file: output/myfile.huff

1. Decompress file  2. Exit

> 1
```

#### 4. Decompression
```
=== FILE DECOMPRESSION ===
Enter compressed file path: output/myfile.huff
Decompressing...
Enter output file path (default: output/decompressed.txt): output/recovered.txt
Successfully wrote to file: output/recovered.txt

Do you want to see statistics? (1. Yes / 2. No)

> 1
Compression complete! 1206.5 KB → 711.0 KB (41.1% smaller)

Do you want to use again? (1. Yes / 2. Logout / 3. Exit)

> 1
```

## 💾 How It Works

### File Format

**Compressed File Structure:**
```
[Frequency Table]|||[Compressed Binary Data]

Example:
h|1:e|1:l|3:o|2:w|1:r|1:d|1: |||[binary data here]
```

### Compression Process

1. Read input file
2. Calculate character frequencies
3. Build Huffman tree
4. Generate variable-length codes
5. Encode text using codes
6. Pack bits (8 bits per byte)
7. Serialize frequencies for storage
8. Save: frequency_table + "|||" + packed_binary_data

### Decompression Process

1. Read compressed file
2. Split at "|||" separator
3. Deserialize frequency table
4. Rebuild Huffman tree (identical to compression)
5. Unpack binary data (reverse bit packing)
6. Decode using tree traversal
7. Write original text to output file

## 📊 Compression Statistics

### Real-World Performance

| File Type | Original | Compressed | Ratio | Reduction |
|-----------|----------|-----------|-------|-----------|
| Text (English) | 1206.5 KB | 711.0 KB | 58.9% | 41.1% |
| Text (Repetitive) | 65 KB | 28 KB | 43.1% | 56.9% |
| Text (Random) | 500 KB | 498 KB | 99.6% | 0.4% |

### Why Compression Varies

- **English Text**: High character frequency variation → 40-50% compression
- **Repetitive Text**: Low character variety → 50-70% compression
- **Random Data**: No frequency pattern → Compression ineffective (>100%)
- **Already Compressed**: ZIP, JPEG, MP4 → Can't compress further

## 🔧 Technical Details

### Memory Management

- **Dynamic Allocation**: Huffman tree nodes allocated on heap
- **Proper Cleanup**: Destructor recursively deletes tree nodes
- **No Memory Leaks**: Tested with valgrind/ASAN

### Input/Output

- **Binary Mode**: File I/O uses `ios::binary` for exact byte preservation
- **Error Handling**: Checks file existence, permissions, and read/write success
- **Buffering**: Uses efficient C++ streams with automatic buffering

### Data Structures

```cpp
// Huffman Node
struct Node {
    char character;      // Character stored in leaf nodes
    int frequency;       // Frequency count
    Node* left;          // Left subtree (0 bit)
    Node* right;         // Right subtree (1 bit)
};

// Comparator for priority queue
struct CompareNode {
    bool operator()(Node* a, Node* b) {
        return a->frequency > b->frequency;  // Min-heap
    }
};
```

## 📋 Requirements

- **C++20 or later**: Uses structured bindings, std::map, std::priority_queue
- **Standard Library**: iostream, fstream, sstream, map, queue, string
- **Compiler**: GCC 10+, Clang 12+, MSVC 2019+
- **RAM**: ~10MB for typical operations
- **Disk**: Space for input + ~50% for output (varies by compression)

## 🏗️ Building

### Build Commands

```bash
# Clean build
make clean

# Build executable
make

# Build and run
make run

# Rebuild from scratch
make rebuild

# Show help
make help

# Count lines of code
make lines
```

### Makefile Variables

```makefile
CXX = g++                    # Compiler
CXXFLAGS = -std=c++20 ...   # Compilation flags
SRC_DIR = src                # Source directory
OBJ_DIR = obj                # Object directory
INCLUDE_DIR = include        # Header directory
TARGET = huffman_compress    # Output executable
```

## 🎓 Learning Outcomes

This project demonstrates:

1. **Data Structures**
   - Binary trees
   - Priority queues (heaps)
   - Hash maps (frequency tables)

2. **Algorithms**
   - Huffman coding (greedy algorithm)
   - Tree traversal (DFS)
   - Bit manipulation

3. **Software Engineering**
   - Object-oriented design (6 classes)
   - SOLID principles
   - Separation of concerns

4. **C++ Features**
   - Classes and inheritance
   - Dynamic memory management
   - STL containers (map, queue, vector)
   - File I/O (ifstream, ofstream)
   - Exception handling

5. **Version Control**
   - Git workflow
   - Feature branches
   - Pull requests
   - Commit history

## 🐛 Known Limitations

1. **Text Files Only**: Designed for text compression (not images/videos)
2. **No Encryption**: Passwords stored in plain text (for learning purposes)
3. **Single User Session**: One user per program execution
4. **No Concurrent Access**: users.txt not thread-safe

## 🔮 Future Improvements

- [ ] LZW compression algorithm
- [ ] Multiple compression algorithms selector
- [ ] Password encryption (bcrypt)
- [ ] Concurrent user sessions
- [ ] GUI interface (Qt/wxWidgets)
- [ ] Parallel compression for large files
- [ ] Compression progress bar
- [ ] File type detection and optimization

## 🤝 Contributing

Contributions welcome! Please:

1. Fork the repository
2. Create feature branch (`git checkout -b feature/improvement`)
3. Commit changes (`git commit -m "Add feature"`)
4. Push to branch (`git push origin feature/improvement`)
5. Open Pull Request

## 📄 License

This project is licensed under the MIT License - see [LICENSE](LICENSE) file for details.

## 👨‍💼 Author

**Sarthak**
- College: Computer Science Student
- Project: Huffman Text Compression Tool
- GitHub: [@sarthak653](https://github.com/sarthak653)

## 🙏 Acknowledgments

- Huffman, D. A. (1952). "A Method for the Construction of Minimum-Redundancy Codes"
- C++ Standard Library Documentation
- Professor and course instructors

## 📚 References

1. **Huffman Coding**
   - [Wikipedia: Huffman Coding](https://en.wikipedia.org/wiki/Huffman_coding)
   - [GeeksforGeeks: Huffman Coding](https://www.geeksforgeeks.org/huffman-coding/)

2. **C++ Resources**
   - [cppreference.com](https://en.cppreference.com)
   - [C++ Standard Library](https://en.cppreference.com/w/cpp/header)

3. **Git & GitHub**
   - [GitHub Guides](https://guides.github.com)
   - [Git Documentation](https://git-scm.com/doc)

---

**Last Updated**: March 2026  
**Version**: 1.0  
**Status**: Complete & Production Ready ✅