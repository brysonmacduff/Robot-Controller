# Robot Controller

## Build, Run, and Test Instructions

This document describes how to build the Robot‑Controller project, run the controller binary, and execute the full unit‑test suite using CMake and CTest.

### Build Instructions

#### 1. Create a build directory

```
mkdir -p build
cd build
```

#### 2. Configure the project

Debug build:

```
cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON ..
```

Release build:

```
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF ..
```

#### 3. Build the project

```
cmake --build .
```

### Run Instructions

```
cd build/src
./robot_controller
```

### Test Instructions

```
cd build
ctest
```