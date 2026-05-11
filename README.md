# Huffman Text Compression Tool

A command-line text compression tool implementing Huffman coding with user authentication, file I/O, and compression statistics.

## Features

- Lossless text file compression and decompression via Huffman coding with bit-packing
- User authentication — signup, login, guest mode — with salted SHA-256 password hashing
- Compression statistics: original size, compressed size, reduction percentage
- No external dependencies — SHA-256 implemented as a self-contained header

## Project Structure

```
TEXT_COMPRESSOR_FINAL/
├── include/
│   ├── authenticationManager.hpp
│   ├── compressionTool.hpp
│   ├── fileManager.hpp
│   ├── hash.hpp                  # Self-contained SHA-256 (no external deps)
│   ├── huffmanCompressor.hpp
│   ├── huffmanDecompressor.hpp
│   ├── huffmanNode.hpp           # Shared Node type (decouples compressor/decompressor)
│   └── statistics.hpp
│
├── src/
│   ├── main.cpp
│   ├── compressionTool.cpp
│   ├── fileManager.cpp
│   ├── huffmanCompressor.cpp
│   ├── huffmanDecompressor.cpp
│   ├── authenticationManager.cpp
│   └── statistics.cpp
│
├── input/                        # Test input files
├── output/                       # Compressed/decompressed output
├── obj/                          # Build artifacts
├── Makefile
├── .gitignore
├── LICENSE
└── README.md
```

## Architecture

```
CompressionTool        — orchestrates all other classes, owns the menu loop
├── AuthenticationManager  — signup/login/logout, salted SHA-256 hashing
├── FileManager            — file read/write/exists abstraction
├── HuffmanCompressor      — encodes text to compressed binary blob
├── HuffmanDecompressor    — decodes blob back to original text
└── Statistics             — records and displays compression results
```

**Key design decisions:**

- `huffmanNode.hpp` extracts the shared `Node` struct so neither the compressor nor decompressor depends on each other's header.
- `unique_ptr<Node>` replaces raw pointers throughout — no manual `deleteTree()` needed.
- `handleDecompression()` takes an optional `preloadedPath` parameter, eliminating a previously duplicated flow.
- `Statistics` is the single place compression ratios are calculated — no inline math scattered across `compressionTool.cpp`.
- `getChoice()` wraps `std::stoi` in try/catch — invalid menu input returns `-1` instead of crashing.
- `AuthenticationManager` is the single source of truth for the current user.

## How It Works

### Compression

1. Count character frequencies in the input text
2. Build a min-heap priority queue — lowest frequency has highest priority
3. Merge nodes bottom-up until one root remains (the Huffman tree)
4. Walk the tree to generate binary codes — common characters get short codes, rare ones get long codes
5. Encode the text by replacing each character with its code
6. Pack bits into bytes (8 bits per byte); store padding count in the first byte
7. Serialize the frequency table and write: `freq_table|||packed_bits`

### Decompression

1. Split the compressed blob at `|||`
2. Deserialize the frequency table
3. Rebuild the identical Huffman tree
4. Unpack bytes back to bits, stripping padding
5. Walk the tree bit-by-bit to decode each character

### Compressed File Format

```
h|1:e|1:l|3:o|2:w|1: |||[packed binary data]
```

## Security

Passwords are never stored in plain text. The storage format is:

```
username:salt:sha256hash
```

- **Salt** — a unique 32-character random hex string generated per user at signup
- **Hash** — `SHA-256(salt + password)`, computed by `hash.hpp` with no external libraries
- **Login** — re-hashes the typed password with the stored salt and compares; the original password is never stored or compared directly

## Building

Requires C++17 or later.

```bash
# Using Make
make clean
make
make run

# Or manually
g++ -std=c++17 -Iinclude -o huffman_compress src/*.cpp
```

## Usage

```bash
./huffman_compress
```

The tool walks through authentication then presents a menu:

```
1. Compress   2. Decompress   3. Statistics   4. Logout   5. Exit
```

Output files go to `output/` by default. Compressed files use the `.huff` extension.

## Performance (Example)

| Input type        | Original  | Compressed | Reduction |
|-------------------|-----------|------------|-----------|
| English text      | 1206.5 KB | 711.0 KB   | 41.1%     |
| Repetitive text   | 65 KB     | 28 KB      | 56.9%     |
| Random characters | 500 KB    | 498 KB     | ~0%       |

Compression effectiveness depends on character frequency variation. Highly repetitive text compresses well; random or already-compressed data does not.

## Algorithm

Huffman coding assigns shorter bit sequences to more frequent characters and longer ones to rarer characters, so the encoded output takes fewer bits than fixed-width encoding. The compressor builds a frequency table, constructs a min-heap, and merges nodes bottom-up into a binary tree. Each leaf's path from the root becomes its code. The encoded bit stream is packed into bytes and stored alongside the serialized frequency table, which the decompressor uses to reconstruct the same tree and reverse the process exactly.

**Time complexity:** O(n) to count frequencies, O(k log k) to build the tree (k = unique characters), O(n) to encode. Overall O(n log k).

## Known Limitations

- Text files only — not designed for binary formats, images, or video
- No concurrent file access — `users.txt` is not thread-safe
- Single user session per program run

## Possible Improvements

- LZ77 or arithmetic coding for better compression on natural language text
- CRC32 checksum to detect corrupted compressed files
- Compression progress indicator for large files
- Parallel encoding for multi-core performance

## Notes

- Delete `users.txt` if migrating from a version that stored passwords in plain text — the old format will not parse
- The `output/` directory must exist before running; it is not created automatically
- `users.txt` and `output/` are excluded from version control via `.gitignore`

## References

- Huffman, D. A. (1952). "A Method for the Construction of Minimum-Redundancy Codes"
- [Wikipedia: Huffman Coding](https://en.wikipedia.org/wiki/Huffman_coding)
- [cppreference.com](https://en.cppreference.com)

## License

MIT — see [LICENSE] for details.