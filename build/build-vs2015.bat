REM @ECHO OFF
mkdir build-vs2015
cd build-vs2015 || exit
IF EXIST CMakeCache.txt del CMakeCache.txt
IF EXIST CMakeFiles rmdir /S/Q CMakeFiles
IF EXIST build.bat del build.bat
copy ..\build.bat build.bat
cmake ..\..\libdash\libdash\ -G"Visual Studio 14 2015 Win64"
cmake --build . --config Debug -- /m
cmake --build . --config Release -- /m

IF NOT EXIST ..\..\Lib\Windows\Debug\VS2015 md ..\..\Lib\Windows\Debug\VS2015
copy Debug\dash*.* ..\..\Lib\Windows\Debug\VS2015\
IF NOT EXIST ..\..\Lib\Windows\Release\VS2015 md ..\..\Lib\Windows\Release\VS2015
copy Release\dash*.* ..\..\Lib\Windows\Release\VS2015\
cd ..

