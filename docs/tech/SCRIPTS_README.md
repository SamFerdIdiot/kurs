# Build Scripts

Helper scripts for building, testing, and running the kkurs game project.

## Available Scripts

### 🔨 build.sh
Quick build script with error handling.

**Usage:**
```bash
./scripts/build.sh
```

**What it does:**
- Creates build directory if needed
- Configures project with CMake
- Compiles with multi-core support
- Shows colored output for success/failure

---

### ▶️ run.sh
Build and run the game.

**Usage:**
```bash
./scripts/run.sh
```

**What it does:**
- Checks if executable exists
- Builds if needed
- Launches the game

---

### 🧹 clean.sh
Clean all build artifacts.

**Usage:**
```bash
./scripts/clean.sh
```

**What it does:**
- Removes build directory
- Removes CMake cache files
- Provides confirmation messages

---

### 🔄 rebuild.sh
Complete clean rebuild.

**Usage:**
```bash
./scripts/rebuild.sh
```

**What it does:**
- Runs clean.sh first
- Then runs build.sh
- Ensures fresh compilation

---

### ✅ test.sh
Comprehensive automated testing.

**Usage:**
```bash
./scripts/test.sh
```

**What it does:**
- Checks executable exists
- Verifies executable permissions
- Tests game launches successfully
- Checks required files are present
- Shows detailed test results

---

## Quick Reference

```bash
# First time setup
./scripts/build.sh

# Run the game
./scripts/run.sh

# After making changes
./scripts/rebuild.sh
./scripts/test.sh

# Clean up
./scripts/clean.sh
```

## Requirements

All scripts require:
- Bash shell
- CMake 3.10+
- C++17 compiler
- SFML 2.6 libraries

See `../DEVELOPMENT_GUIDE.md` for detailed setup instructions.

## Notes

- All scripts use color-coded output for clarity
- Scripts handle errors gracefully
- Safe to run from any directory
- Scripts are executable (chmod +x already applied)

## Troubleshooting

**Problem:** Script not executable
```bash
chmod +x scripts/*.sh
```

**Problem:** Build fails
```bash
# Check dependencies
cmake --version
g++ --version
pkg-config --modversion sfml-all

# See DEVELOPMENT_GUIDE.md for installation
```

**Problem:** Game won't launch
```bash
# Check if executable exists
ls -la build/kkurs

# Try rebuilding
./scripts/rebuild.sh
```

---

For more information, see:
- `../DEVELOPMENT_GUIDE.md` - Comprehensive development guide
- `../README.md` - Project overview
- `../TODO.md` - Task list
