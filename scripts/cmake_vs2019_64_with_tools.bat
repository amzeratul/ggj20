set PreferredToolArchitecture=x64
cd ..
mkdir build
cd build
cmake -G "Visual Studio 16 2019" ^
    -A x64 ^
    -DHALLEY_PATH=../halley ^
    -DCMAKE_INCLUDE_PATH="lib\include;C:\dev\include;C:\dev\vcpkg\installed\x64-windows\include" ^
    -DCMAKE_LIBRARY_PATH="lib\windows64;C:\dev\lib64;C:\dev\vcpkg\installed\x64-windows\lib;C:\dev\vcpkg\installed\x64-windows\lib\manual-link" ^
    -DBOOST_LIBRARYDIR="c:\Boost\lib" ^
    -DBOOST_INCLUDEDIR="c:\Boost\include\boost-1_66" ^
    -DBoost_USE_STATIC_LIBS=1 ^
    -DBUILD_HALLEY_TOOLS=1 ^
    -DBUILD_HALLEY_TESTS=1 ^
    -DUSE_PCH=1 ^
    ..
pause