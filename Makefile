CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -O2 -I./include
LDFLAGS = -Wl,--subsystem,console

SRC_DIR = src
OBJ_DIR = obj
TARGET = huffman_compress

SOURCES = $(SRC_DIR)/main.cpp \
          $(SRC_DIR)/fileManager.cpp \
          $(SRC_DIR)/huffmanCompressor.cpp \
          $(SRC_DIR)/huffmanDecompressor.cpp \
          $(SRC_DIR)/authenticationManager.cpp \
          $(SRC_DIR)/statistics.cpp \
          $(SRC_DIR)/compressionTool.cpp

OBJECTS = $(OBJ_DIR)/main.o \
          $(OBJ_DIR)/fileManager.o \
          $(OBJ_DIR)/huffmanCompressor.o \
          $(OBJ_DIR)/huffmanDecompressor.o \
          $(OBJ_DIR)/authenticationManager.o \
          $(OBJ_DIR)/statistics.o \
          $(OBJ_DIR)/compressionTool.o

.PHONY: all run clean rebuild

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET)
	@echo "Build successful!"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(TARGET)

rebuild: clean all