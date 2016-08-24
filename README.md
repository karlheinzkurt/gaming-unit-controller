# gaming-unit-controller
Tool to monitor runtime of processes (games) and terminate them when defined limits get exceeded

# Requirements
cmake version   > 2.8.1  
gmock will be downloaded and build automatically
boost version   > 1.58.0 (maybe earlier versions are running as well, but not tested)
log4cxx version > 0.10.0 (maybe earlier versions are running as well, but not tested)

# Build Instructions
Binaries are put to "bin" folder.
Build steps to generate makefiles:
```
mkdir build
cd build
cmake ../Source
make
```

For CodeLite replace step 4 by:
```sh
cmake -G"CodeLite - Unix Makefiles" ../Source
```

On Windows for VS2012 replace step 4 by:
```sh
cmake -G"Visual Studio 12" ..\Source
```

On mac for XCode replace step 4 by:
```sh
cmake -G"XCode" ../Source
```

For Eclipse 4.4 with CDT4 replace step 4 by:
```sh
cmake -G"Eclipse CDT4 - Unix Makefiles" -D_ECLIPSE_VERSION=4.4 ../Source
```

For CodeBlocks replace step 4 by:
```sh
cmake -G"CodeBlocks - Unix Makefiles" ../Source
```
