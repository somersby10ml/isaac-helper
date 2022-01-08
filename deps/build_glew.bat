@echo off
cls
pushd %~dp0

cd glew
cd build/cmake
mkdir build
cd build
cmake -AWin32 ..

cmake --build . --config Debug
copy lib\Debug\glew32d.lib "%~dp0\lib\glew32D.lib"
copy lib\Debug\libglew32d.lib "%~dp0\lib\libglew32D.lib"
copy lib\Debug\libglew32d.pdb "%~dp0\lib\"

cmake --build . --config Release
copy lib\Release\glew32.lib "%~dp0\lib\glew32R.lib"
copy lib\Release\libglew32.lib "%~dp0\lib\libglew32R.lib"

popd
@echo on