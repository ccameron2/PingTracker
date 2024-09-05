mkdir "PingSight/vendor/SDL/build"
cd "PingSight/vendor/SDL/build"
cmake -DSDL_STATIC=ON -DSDL_SHARED=OFF -DSDL_STATIC_VCRT=ON -DSDL_CAMERA=OFF -DSDL_AUDIO=OFF -DSDL_DISKAUDIO=OFF -DSDL_HAPTIC=OFF -DSDL_JOYSTICK=OFF -DSDL_OPENGL=ON -DSDL_OPENGLES=OFF -DSDL_RENDER_D3D=OFF -DSDL_RENDER_D3D11=OFF -DSDL_RENDER_D3D12=OFF -DSDL_RENDER_METAL=OFF -DSDL_RENDER_VULKAN=OFF -DSDL_SENSOR=OFF -DSDL_TEST_LIBRARY=OFF -DSDL_VIDEO=ON -DSDL_VIRTUAL_JOYSTICK=OFF -DSDL_VULKAN=OFF -DSDL_DIRECTX=OFF -DSDL_WASAPI=OFF ..

if not exist ../Debug/SDL3-static.lib (
    cmake --build . --target SDL3-static --config Debug
)

if not exist ../Release/SDL3-static.lib (
    cmake --build . --target SDL3-static --config Release
)

cd "../../../../"

call vendor\bin\premake\premake5.exe vs2022
pause