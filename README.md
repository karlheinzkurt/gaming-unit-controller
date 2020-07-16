![build](https://github.com/karlheinzkurt/zock-zilla/workflows/build/badge.svg)

# ZockZilla
Daemon to monitor runtime of processes. When limits get exceeded, 
processes and childs will be terminated. 

Collected information can be written into InfluxDB via cpprestsdk to be
able to use Grafana to create nice dashboards and to send notification e.g.

# Requirements
* c++14 enabled compiler
* cmake (>= 3.1.2)
* log4cxx (>= 0.10.0, there is no conan package)
* python3 as a requirement for conan
* conan package manager (See: https://docs.conan.io/en/latest/installation.html)

(boost, cpprestsdk, gtest gets installed via conan)

```
apt-get -y install gcc cmake make liblog4cxx-dev python3-pip python3-setuptools python3-wheel ... maybe something more
pip3 install conan
```

# Build Instructions
```
mkdir build
pushd build

# Install dependencies
#
conan install --build missing ../
#
# OR maybe when you get linker errors and you are using gcc
conan install --build=missing ../ -s compiler.libcxx=libstdc++11

# Build lib, executable and tests
#
cmake -DCMAKE_BUILD_TYPE=Release ../source
cmake --build . --config Release -- -j

# Run tests
#
bin/gsclib.test

popd
```

# Installation instructions for Debian like systems
```
pushd build

# Create package and check content
#
cpack
dpkg --contents gsc*.deb

# Install package
#
sudo dpkg -i gsc*.deb

popd

# Enable and start daemon
#
sudo systemctl --system daemon-reload
sudo systemctl enable gsc
sudo systemctl start gsc
```
