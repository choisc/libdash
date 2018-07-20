REM @ECHO OFF
mkdir build-vs2017
cd build-vs2017 || exit
IF EXIST CMakeCache.txt del CMakeCache.txt
IF EXIST CMakeFiles rmdir /S/Q CMakeFiles
IF EXIST build.bat del build.bat
copy ..\build.bat build.bat
cmake ..\..\libdash\libdash\ -G"Visual Studio 15 2017 Win64"
cmake --build . --config Debug -- /m
cmake --build . --config Release -- /m

IF NOT EXIST ..\..\Lib\Windows\Debug\VS2017 md ..\..\Lib\Windows\Debug\VS2017
copy Debug\dash*.* ..\..\Lib\Windows\Debug\VS2017\
IF NOT EXIST ..\..\Lib\Windows\Release\VS2017 md ..\..\Lib\Windows\Release\VS2017
copy Release\dash*.* ..\..\Lib\Windows\Release\VS2017\
cd ..

