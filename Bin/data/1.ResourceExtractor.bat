set GamePath=C:\Users\root\Desktop\The.Binding.of.Isaac.Afterbirth.Plus.Update.1

@echo off
pushd %~dp0
cls

cd "%~dp0"
if not exist temp\ (
  %GamePath%\tools\ResourceExtractor\ResourceExtractor.exe "%GamePath%" "temp"
)

copy temp\resources\gfx\items\collectibles\* image\collectibles\*
copy temp\resources\gfx\items\trinkets\* image\trinkets\*
del image\collectibles\questionmark.png

rem trinkets
rem clean
set GamePath=

popd
@echo on
pause