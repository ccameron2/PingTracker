mkdir "PingPlotter/vendor/SDL/build"
cd "PingPlotter/vendor/SDL/build"
cmake -DSDL_STATIC=ON -DSDL_SHARED=OFF -DSDL_STATIC_VCRT=ON ..
cd "../../../../"

mkdir "PingPlotter/vendor/libpng/zlib/build"
cd "PingPlotter/vendor/libpng/zlib/build"
cmake -DZLIB_BUILD_EXAMPLES=OFF ..
cd "../../../../../"

xcopy "PingPlotter\vendor\libpng\zlib\build\zconf.h" "PingPlotter\vendor\libpng\zlib\"

mkdir "PingPlotter/vendor/libpng/build"
cd "PingPlotter/vendor/libpng/build"
cmake -DPNG_STATIC=ON -DPNG_SHARED=OFF -DPNG_BUILD_ZLIB=OFF -DZLIB_ROOT=../../zlib/ -DZLIB_INCLUDE_DIR=../zlib -DZLIB_LIBRARY_DEBUG=../../zlib/build/Debug -DZLIB_LIBRARY_RELEASE=../../zlib/build/Debug ..
cd "../../../../"

call vendor\bin\premake\premake5.exe vs2022
PAUSE