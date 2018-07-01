# gaming-unit-controller
Tool to monitor runtime of processes (games) and to terminate them when 
defined limits get exceeded.

# Requirements
* cmake version   > 2.8.1  
* gmock/gtest will be downloaded and build automatically
* boost version   > 1.58.0 (maybe earlier versions, but not tested)
* log4cxx version > 0.10.0 (maybe earlier versions, but not tested)

# Build Instructions
Binaries are put to "bin" folder.
Steps to generate makefiles/solutions/workspaces and build the projects:
```
mkdir build
pushd build
   cmake ../Source
popd
cmake --build build/... --config Debug
```

Run the following command to get a list of generators available 
for your platform. Place matching generator argument when calling 
cmake in line 3 above.
```
cmake -G
```
