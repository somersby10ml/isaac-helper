@echo off
cls
pushd %~dp0

cd asmtk
mkdir build
cd build
cmake .. -A Win32 -DASMJIT_STATIC=TRUE -DASMTK_STATIC=TRUE
cmake --build . --config Debug
copy Debug\asmtk.lib "%~dp0\lib\asmtkD.lib"

cmake --build . --config Release
copy Release\asmtk.lib "%~dp0\lib\asmtkR.lib"

popd
@echo on