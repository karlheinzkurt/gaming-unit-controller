# zockZilla
Daemon to monitor runtime of processes. When limits get exceeded, 
processes and childs will be terminated.

# Requirements
* c++17 enabled compiler
* cmake (>= 3.1.2)
* log4cxx (>= 0.10.0)
* python3 as a requirement for conan
* conan package manager (See: https://docs.conan.io/en/latest/installation.html)

```
sudo apt install gcc make cmake python3 liblog4cxx-dev ... maybe something more

python3 -m pip install conan
```

# Build Instructions
```
mkdir build
pushd build

# Install dependencies
#
conan install --build missing ../

# Build lib, executable and tests
#
cmake -DCMAKE_BUILD_TYPE=Release ../source
cmake --build .

# Run tests
#
bin/gsclib.test

popd
```

# Installation instructions for Debian like systems
```
# Create package and check content
#
cpack
dpkg --contents gsc*.deb

# Install package
#
sudo dpkg -i build/gsc*.deb

# Enable and start daemon
#
sudo systemctl --system daemon-reload
sudo systemctl enable gsc
sudo systemctl start gsc
```
