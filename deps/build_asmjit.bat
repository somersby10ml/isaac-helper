@echo off
cls
pushd %~dp0

cd asmjit
mkdir build
cd build
cmake .. -AWin32 -DASMJIT_STATIC=TRUE
cmake --build . --config Debug
copy Debug\asmjit.lib "%~dp0\lib\asmjitD.lib"
cmake --build . --config Release
copy Release\asmjit.lib "%~dp0\lib\asmjitR.lib"
rem msbuild asmjit.sln -m /p:Configuration=Release /p:Platform=Win32

popd
@echo on