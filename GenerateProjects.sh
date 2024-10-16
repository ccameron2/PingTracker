mkdir -p "PingTracer/vendor/SDL/build"
cd "PingTracer/vendor/SDL/build"

cmake -DSDL_STATIC=ON -DSDL_SHARED=OFF -DSDL_STATIC_VCRT=ON -DSDL_CAMERA=OFF -DSDL_AUDIO=OFF -DSDL_DISKAUDIO=OFF -DSDL_HAPTIC=OFF -DSDL_JOYSTICK=OFF -DSDL_OPENGL=ON -DSDL_OPENGLES=OFF -DSDL_RENDER_D3D=OFF -DSDL_RENDER_D3D11=OFF -DSDL_RENDER_D3D12=OFF -DSDL_RENDER_METAL=OFF -DSDL_RENDER_VULKAN=OFF -DSDL_SENSOR=OFF -DSDL_TEST_LIBRARY=OFF -DSDL_VIDEO=ON -DSDL_VIRTUAL_JOYSTICK=OFF -DSDL_VULKAN=OFF -DSDL_DIRECTX=OFF -DSDL_WASAPI=OFF -DSDL_X11=ON ..

if [ ! -f "../Debug/libSDL3.a" ]; then
    cmake --build . --target SDL3-static --config Debug
fi

if [ ! -f "../Release/libSDL3.a" ]; then
    cmake --build . --target SDL3-static --config Release
fi

cd "../../../../"
premake5 gmake2

make -j6 all

echo Press enter to continue; read dummy;
