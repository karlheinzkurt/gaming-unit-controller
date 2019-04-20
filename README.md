# zockZilla
Daemon to monitor runtime of processes. When limits get exceeded, 
processes and childs will be terminated.

# Requirements
* cmake version   > 3.1.2  
* conan 
* log4cxx version > 0.10.0 (unfortunately not available via conan)

# Build Instructions
Binaries are put to "bin" folder.
```
mkdir build
cd build
conan install --build missing ../
cmake -DCMAKE_BUILD_TYPE=Release ../Source
cmake --build .
cpack
```
