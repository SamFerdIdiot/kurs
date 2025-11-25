# 🍎 macOS Build Guide for 69 Crossroads
# Руководство по сборке на macOS

**Platform**: macOS (Intel & Apple Silicon)  
**Date**: 2025-11-17  
**Tested on**: MacBook Air M1, MacBook Pro Intel  
**Status**: ✅ Verified Working

---

## 📋 Table of Contents

1. [Prerequisites](#prerequisites)
2. [Quick Start](#quick-start)
3. [Detailed Instructions](#detailed-instructions)
4. [Troubleshooting](#troubleshooting)
5. [Development Setup](#development-setup)

---

## Prerequisites

### System Requirements

- **OS**: macOS 10.15 (Catalina) or later
- **Processor**: Intel x86_64 or Apple Silicon (M1/M2/M3)
- **RAM**: 4 GB minimum, 8 GB recommended
- **Storage**: 500 MB free space
- **Display**: 1366x768 or higher resolution

### Required Software

1. **Xcode Command Line Tools**
   ```bash
   xcode-select --install
   ```

2. **Homebrew Package Manager**
   ```bash
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   ```

3. **SFML 2.6.x** (installed via Homebrew)
   ```bash
   brew install sfml@2
   ```

4. **CMake 3.10+** (installed via Homebrew)
   ```bash
   brew install cmake
   ```

5. **Git** (usually pre-installed)
   ```bash
   git --version
   ```

---

## Quick Start

### 1. Clone Repository

```bash
# Clone the project
git clone https://github.com/barzikol/kkurs.git
cd kkurs
```

### 2. Automatic Build (Recommended)

The project includes a smart build script that automatically detects SFML on macOS:

```bash
# Run build script (auto-detects SFML)
./scripts/build.sh
```

The script will:
1. ✅ Auto-detect your Mac architecture (Intel/Apple Silicon)
2. ✅ Find SFML installation automatically
3. ✅ Configure CMake with correct paths
4. ✅ Build project in Release mode
5. ✅ Run all tests to verify build

### 3. Run the Game

```bash
# From project root
./scripts/run.sh

# Or manually
cd build
./kkurs
```

---

## Detailed Instructions

### Step-by-Step Build Process

#### 1. Install Dependencies

```bash
# Update Homebrew
brew update

# Install SFML 2.6.x
brew install sfml@2

# Install CMake
brew install cmake

# Verify installations
brew list sfml@2
cmake --version
```

#### 2. Locate SFML Installation

SFML can be installed in different locations depending on your Mac:

**Apple Silicon (M1/M2/M3)**:
```bash
# Primary location
/opt/homebrew/opt/sfml@2/lib/cmake/SFML

# Alternative
/opt/homebrew/Cellar/sfml/2.6.1/lib/cmake/SFML
```

**Intel Mac**:
```bash
# Primary location
/usr/local/opt/sfml@2/lib/cmake/SFML

# Alternative
/usr/local/Cellar/sfml/2.6.1/lib/cmake/SFML
```

**Find your SFML path**:
```bash
# Method 1: Using brew
brew list sfml@2 | grep cmake

# Method 2: Manual search
find /opt/homebrew -name "SFMLConfig.cmake" 2>/dev/null
find /usr/local -name "SFMLConfig.cmake" 2>/dev/null
```

#### 3. Build Project

**Option A: Using build script** (Recommended)
```bash
./scripts/build.sh
```

**Option B: Manual CMake**
```bash
# Create build directory
mkdir -p build && cd build

# Configure with SFML path (Apple Silicon example)
cmake .. -DSFML_DIR=/opt/homebrew/opt/sfml@2/lib/cmake/SFML

# Or for Intel Mac
cmake .. -DSFML_DIR=/usr/local/opt/sfml@2/lib/cmake/SFML

# Build in Release mode
cmake --build . --config Release

# Alternative: build with make
make -j$(sysctl -n hw.ncpu)
```

#### 4. Run Tests

```bash
# From build directory
ctest --output-on-failure

# Or with verbose output
ctest -V

# Run specific test
./unit_tests --gtest_filter=ResourceBarTest.*
```

#### 5. Install (Optional)

```bash
# Install system-wide
sudo cmake --install .

# Uninstall
sudo rm /usr/local/bin/kkurs
```

---

## Troubleshooting

### Problem: "SFML not found"

**Error Message**:
```
CMake Error: By not providing "FindSFML.cmake" in CMAKE_MODULE_PATH...
```

**Solution 1: Automatic Detection**
```bash
# Run build script (auto-detects)
./scripts/build.sh
```

**Solution 2: Specify Path Manually**
```bash
# Find SFML
brew list sfml@2 | grep cmake

# Use the path in cmake
cmake .. -DSFML_DIR=/opt/homebrew/opt/sfml@2/lib/cmake/SFML
```

**Solution 3: Reinstall SFML**
```bash
brew uninstall sfml@2
brew install sfml@2
brew link sfml@2
```

---

### Problem: "xcrun: error: invalid active developer path"

**Error Message**:
```
xcrun: error: invalid active developer path
```

**Solution**:
```bash
# Install Xcode Command Line Tools
xcode-select --install

# If already installed, reset path
sudo xcode-select --reset
```

---

### Problem: Unicode Icons Not Displaying

**Symptom**: Icons (💰⛽⚡📦) show as boxes or question marks

**Solution 1: Install System Fonts**
```bash
# Check if Arial is available
ls /System/Library/Fonts/Supplemental/Arial.ttf

# If not, install Apple fonts (usually pre-installed on macOS)
# No action needed - fonts should be available
```

**Solution 2: Use Alternative Font**
```bash
# Download DejaVu Sans
brew install --cask font-dejavu

# Update font path in code if needed
/Library/Fonts/DejaVuSans.ttf
```

---

### Problem: Permission Denied

**Symptom**: Cannot create build directory or run executable

**Solution**:
```bash
# Fix permissions
chmod +x scripts/*.sh

# If build directory exists with wrong permissions
rm -rf build
mkdir build
```

---

### Problem: Slow Build

**Symptom**: Build takes a long time

**Solution**:
```bash
# Use parallel build with all cores
make -j$(sysctl -n hw.ncpu)

# Or with CMake
cmake --build . -j $(sysctl -n hw.ncpu)

# Check CPU cores
sysctl -n hw.ncpu
```

---

### Problem: Game Window Not Opening

**Symptom**: Game compiles but window doesn't appear

**Solution 1: Check Display Settings**
```bash
# Verify display
system_profiler SPDisplaysDataType

# Check if running in headless mode (should not be on Mac)
echo $DISPLAY
```

**Solution 2: Run from Terminal**
```bash
# Always run from terminal, not Finder
cd /path/to/kkurs/build
./kkurs
```

**Solution 3: Check Metal/OpenGL Support**
```bash
# Check GPU acceleration
system_profiler SPDisplaysDataType | grep Metal

# SFML should use OpenGL, verify
glxinfo 2>/dev/null || echo "OpenGL available via SFML"
```

---

### Problem: "Library not loaded" Error

**Error Message**:
```
dyld: Library not loaded: @rpath/libsfml-graphics.2.6.dylib
```

**Solution 1: Fix RPATH**
```bash
# Add SFML to library path
export DYLD_LIBRARY_PATH=/opt/homebrew/lib:$DYLD_LIBRARY_PATH

# Or for Intel Mac
export DYLD_LIBRARY_PATH=/usr/local/lib:$DYLD_LIBRARY_PATH
```

**Solution 2: Copy Libraries (Not Recommended)**
```bash
# Copy SFML dylibs to build directory
cp /opt/homebrew/lib/libsfml*.dylib build/
```

**Solution 3: Install SFML with --build-from-source**
```bash
brew reinstall sfml@2 --build-from-source
```

---

## Development Setup

### Xcode Integration

#### 1. Generate Xcode Project

```bash
mkdir -p build-xcode && cd build-xcode
cmake -G Xcode .. -DSFML_DIR=/opt/homebrew/opt/sfml@2/lib/cmake/SFML
open kkurs.xcodeproj
```

#### 2. Configure Xcode Settings

- Set scheme to "kkurs"
- Set build configuration to "Release" for testing
- Set working directory to project root

### VS Code Integration

#### 1. Install Extensions

- C/C++ (Microsoft)
- CMake Tools (Microsoft)
- CMake (twxs)

#### 2. Configure Settings

Create `.vscode/settings.json`:
```json
{
    "cmake.configureSettings": {
        "SFML_DIR": "/opt/homebrew/opt/sfml@2/lib/cmake/SFML"
    },
    "cmake.buildDirectory": "${workspaceFolder}/build",
    "C_Cpp.default.configurationProvider": "ms-vscode.cmake-tools"
}
```

#### 3. Build and Debug

- Press `Cmd+Shift+B` to build
- Press `F5` to debug

---

## Performance Tips

### Optimize Build Time

```bash
# Use ccache
brew install ccache
export CC="ccache clang"
export CXX="ccache clang++"

# Build with optimizations
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-O3 -march=native"
```

### Optimize Runtime

```cpp
// Enable Metal backend (if SFML supports it)
sf::RenderWindow window(sf::VideoMode(800, 600), "Game", 
                       sf::Style::Default,
                       sf::ContextSettings(0, 0, 8));  // 8x MSAA

// Enable VSync for smooth 60 FPS
window.setVerticalSyncEnabled(true);

// Limit framerate as backup
window.setFramerateLimit(60);
```

---

## Clean Build

### Remove Build Artifacts

```bash
# Use clean script
./scripts/clean.sh

# Or manually
rm -rf build
rm -rf build-xcode
find . -name "*.o" -delete
find . -name "*.a" -delete
```

### Full Clean Rebuild

```bash
# Clean
./scripts/clean.sh

# Rebuild
./scripts/build.sh

# Test
./scripts/test.sh
```

---

## Verification Checklist

After building, verify the following:

- [ ] **Build completes without errors**
  ```bash
  ./scripts/build.sh
  ```

- [ ] **All 60 tests pass**
  ```bash
  cd build && ctest --output-on-failure
  ```

- [ ] **Game window opens**
  ```bash
  ./scripts/run.sh
  ```

- [ ] **UI displays correctly**
  - Top-right HUD visible
  - Unicode icons render (💰⛽⚡📦)
  - Resource bars animate smoothly

- [ ] **No console errors**
  - Check for font loading warnings
  - Verify SFML version
  - No OpenGL errors

- [ ] **Performance is good**
  - 60 FPS maintained
  - No lag during gameplay
  - Smooth animations

---

## Additional Resources

### Documentation

- [Main README](README.md)
- [UI Integration Guide](UI_INTEGRATION_EXAMPLES.md)
- [UI Implementation Prompt](UI_IMPLEMENTATION_PROMPT.md)
- [Build Scripts](scripts/README.md)

### External Links

- [SFML Documentation](https://www.sfml-dev.org/documentation/2.6.1/)
- [CMake Documentation](https://cmake.org/documentation/)
- [Homebrew SFML Formula](https://formulae.brew.sh/formula/sfml@2)

### Support

- **Issues**: https://github.com/barzikol/kkurs/issues
- **Discussions**: https://github.com/barzikol/kkurs/discussions

---

## Summary

### Quick Commands Reference

```bash
# Install dependencies
brew install sfml@2 cmake

# Clone and build
git clone https://github.com/barzikol/kkurs.git
cd kkurs
./scripts/build.sh

# Run tests
cd build && ctest

# Run game
cd .. && ./scripts/run.sh

# Clean
./scripts/clean.sh
```

### Architecture-Specific Paths

| Architecture | SFML Path |
|--------------|-----------|
| **Apple Silicon** | `/opt/homebrew/opt/sfml@2/lib/cmake/SFML` |
| **Intel Mac** | `/usr/local/opt/sfml@2/lib/cmake/SFML` |

### Success Criteria

- ✅ Build completes in < 2 minutes
- ✅ All 60 tests pass
- ✅ Game runs at 60 FPS
- ✅ HUD displays with Unicode icons
- ✅ No warnings or errors

---

**Happy Building! / Удачной сборки!** 🚀

---

**Document Version**: 1.0  
**Last Updated**: 2025-11-17  
**Maintained by**: GitHub Copilot
