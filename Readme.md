# IO_IMAGE: C++ Computer Vision Library

[![Language](https://img.shields.io/badge/Language-C%2B%2B17-blue.svg)](https://isocpp.org/)
[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)

**IO_IMAGE** is an image processing and computer vision library written entirely in C++17. Its key feature is having zero dependencies on third-party imaging libraries (like libpng or OpenCV) for PNG file handling. It implements its own PNG reader and writer from scratch, along with a collection of computer vision algorithms.

This project aims to provide a deep understanding of the low-level mechanics of image processing, from file format management to the implementation of fundamental algorithms.

## Table of Contents

1.  [Features](#-features)
2.  [Prerequisites](#-prerequisites)
3.  [Building from Source](#-building-from-source)
    *   [On Linux / macOS](#on-linux--macos)
    *   [On Windows](#on-windows)
4.  [Usage](#-usage)
5.  [Project Structure](#-project-structure)
6.  [Author](#-author)
7.  [License](#-license)

## ✨ Features

The IO_IMAGE library offers a diverse set of tools for image manipulation.

#### PNG File Handling
- **Read and write PNG files** without external dependencies.
- Handling of critical (IHDR, IDAT, IEND) and ancillary (pHYs) chunks.
- Implementation of filter algorithms (Sub, Up, Average, Paeth) and compression/decompression via the bundled `zlib`.

#### Color and Pixel Manipulation
- Color space conversion (RGB, HSL, HSV).
- Color mode conversion (RGB to Grayscale, RGB to Binary).
- Color channel extraction (Red, Green, Blue).
- Colorization via Look-Up Table (LUT).

#### Image Processing Algorithms
- **Image segmentation** (Thresholding and Otsu's method).
- **Dominant color detection** (Histogram-based and K-Means clustering).
- **Blur filters** (Mean blur and Gaussian blur).
- Edge detection.

#### Image Analysis
- Histogram computation and plotting.
- Partial multi-threading support for performance optimization.

## 📋 Prerequisites

Before building the project, make sure you have the following tools and libraries installed on your system.

- A **C++17 compliant compiler** (GCC, Clang, or MinGW for Windows).
- **Make** (for Unix-like systems).
- Development libraries for:
  - `zlib`
  - `OpenGL`
  - `GLUT`

#### Dependency Installation Instructions

- **On Debian / Ubuntu:**
  ```sh
  sudo apt-get update
  sudo apt-get install build-essential libz-dev libglu1-mesa-dev freeglut3-dev
  ```
- **On macOS (with [Homebrew](https://brew.sh/)):**
  ```sh
  brew install gcc zlib freeglut
  ```
- **On Windows:**
  The easiest environment is using **MinGW-w64**. You can install it via [MSYS2](https://www.msys2.org/) and add the necessary packages. Ensure the MinGW `bin` directory is added to your system's `PATH`.

## ⚙️ Building from Source

First, clone the repository:
```sh
git clone https://github.com/IgorGreenIGM/IO_IMAGE
cd IO_IMAGE
```

### On Linux / macOS

The project can be easily built using the provided `Makefile`.

1.  Open a terminal in the project's root directory.
2.  Run the `make` command to build the project:
    ```sh
    make
    ```
3.  The executable will be generated at `bin/output`.

To clean up object files and the executable, you can use:
```sh
make clean    # Removes object files (*.o)
make mrproper # Removes object files and the executable
```

### On Windows

The `compile.bat` and `link.bat` batch scripts are provided for 32-bit compilation with a MinGW-like toolchain.

1.  Open a Command Prompt (cmd) or PowerShell terminal in the project's root directory.
2.  **Step 1: Compile source files**
    Run the `compile.bat` script. It will compile the `.cpp` source files into object files (`.o`) and move them to the `bin/link` directory.
    ```cmd
    compile.bat
    ```
3.  **Step 2: Link object files**
    Run the `link.bat` script. It will link the object files to create the final executable.
    ```cmd
    link.bat
    ```
4.  The executable will be generated at `bin\output.exe`.

## 🚀 Usage

The main executable is a demo program that applies a few filters to a source image.

1.  **Prepare the input image:**
    Place a PNG image named `origin.png` in the project's root directory. The program will perform its operations on this image.

2.  **Run the executable:**
    - **On Linux / macOS:**
      ```sh
      ./bin/output
      ```
    - **On Windows:**
      ```cmd
      .\bin\output.exe
      ```

3.  **Check the results:**
    The program will generate two new images in the root directory:
    - `gray.png`: A grayscale version of the original image.
    - `blurred.png`: A blurred version of the original image.

## 📁 Project Structure

```
.
├── bin/                # Binary and object files
├── include/            # Library header files
│   ├── PNG/
│   └── PixelsManager/
├── lib/                # External libraries (zlib, glut)
├── src/                # Source (.cpp) files
│   ├── PNG/
│   └── PixelsManager/
├── compile.bat         # Windows compilation script
├── link.bat            # Windows linking script
├── Makefile            # Makefile for Unix-like systems
├── origin.png          # Input image (must be provided by the user)
└── Readme.md           # This file
```

## ✍️ Author

-   **[@igorgreenIGM](https://github.com/igorgreenIGM)** - Idea & Initial work

## 📜 License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.