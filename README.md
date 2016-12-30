# gaming-unit-controller
Tool to monitor runtime of processes (games) and terminate them when defined limits get exceeded

# Requirements
* cmake version   > 2.8.1  
* gmock/gtest will be downloaded and build automatically
* boost version   > 1.58.0 (maybe earlier versions, but not tested)
* log4cxx version > 0.10.0 (maybe earlier versions, but not tested)

# Build Instructions
Binaries are put to "bin" folder.
Build steps to generate plain makefiles and build the projects:
```
mkdir build
pushd build
   cmake ../Source
popd
cmake --build build --config Debug
```

Run the following command to get a list of generators available for your platform.
```
cmake -G
```

E.g. for CodeLite replace step 4 by:
```sh
cmake -G"CodeLite - Unix Makefiles" ../Source
```

On Windows for VS2012 replace step 4 by:
```sh
cmake -G"Visual Studio 11 2012" ..\Source
```

For Eclipse 4.4 with CDT4 replace step 4 by:
```sh
cmake -G"Eclipse CDT4 - Unix Makefiles" -D_ECLIPSE_VERSION=4.4 ../Source
```
