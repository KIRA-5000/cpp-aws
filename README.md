# cpp-aws

## Build
 - Download and install latest cmake
 - Fork the main repo
 - Clone the forked repo under a folder say c:\cpp-rct
 - From inside a VS 2019 x64 command prompt:

```sh
git clone https://github.com/microsoft/vcpkg.git
.\vcpkg\bootstrap-vcpkg.bat
.\vcpkg\vcpkg install gflags:x64-windows
.\vcpkg\vcpkg install aws-sdk-cpp[s3] --recurse
.\vcpkg\vcpkg integrate install

cd c:\cpp-aws
md build
cd build
cmake ..
cmake --build . --config Release
```