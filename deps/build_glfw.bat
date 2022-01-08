@echo off
cls
pushd %~dp0

cd glfw
mkdir build
cd build
cmake -AWin32 -DGLFW_BUILD_DOCS=OFF -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF ..

cmake --build . --config Debug
copy src\Debug\glfw3.lib "%~dp0\lib\glfw3D.lib"

cmake --build . --config Release
copy src\Release\glfw3.lib "%~dp0\lib\glfw3R.lib"


popd
@echo on