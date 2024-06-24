mkdir "PingPlotter/vendor/SDL/build"
cd "PingPlotter/vendor/SDL/build"
cmake -DSDL_STATIC=ON -DSDL_SHARED=OFF -DSDL_STATIC_VCRT=ON ..
cd "../../../../"

call vendor\bin\premake\premake5.exe vs2022
PAUSE