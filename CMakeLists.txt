cmake_minimum_required(VERSION 3.12)
project(PingSight)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set architecture
set(ARCHITECTURE "x64")

# Define output directories
set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME}-${ARCHITECTURE}/${PROJECT_NAME}")

# Set output directories for different types of output
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR})

# Define include directories
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/PingSight)
set(SDL_INCLUDE_DIR ${INCLUDE_DIR}/vendor/SDL/include)
set(IMGUI_INCLUDE_DIR ${INCLUDE_DIR}/vendor/ImGui)
set(IMPLOT_INCLUDE_DIR ${INCLUDE_DIR}/vendor/ImPlot)
set(ICMPLIB_INCLUDE_DIR ${INCLUDE_DIR}/vendor/icmplib)
set(PLF_NANOTIMER_INCLUDE_DIR ${INCLUDE_DIR}/vendor/plf_nanotimer)
set(STB_INCLUDE_DIR ${INCLUDE_DIR}/vendor/stb)

add_subdirectory(${CMAKE_SOURCE_DIR}/PingSight/vendor/SDL)

# Add executable
add_executable(PingSight WIN32)

# Add source files
target_sources(PingSight PRIVATE
    ${CMAKE_SOURCE_DIR}/PingSight/src/Source.cpp
    ${CMAKE_SOURCE_DIR}/PingSight/src/App/App.h
    ${CMAKE_SOURCE_DIR}/PingSight/src/App/App.cpp
    ${CMAKE_SOURCE_DIR}/PingSight/src/App/AppColours.h
    ${CMAKE_SOURCE_DIR}/PingSight/src/App/AppColours.cpp
    ${CMAKE_SOURCE_DIR}/PingSight/src/App/AppIcon.h
    ${CMAKE_SOURCE_DIR}/PingSight/src/App/AppIcon.cpp
    ${CMAKE_SOURCE_DIR}/PingSight/src/App/AppSettings.h
    ${CMAKE_SOURCE_DIR}/PingSight/src/App/AppSettings.cpp
    ${CMAKE_SOURCE_DIR}/PingSight/src/PingSight/MultithreadingWorker.cpp
    ${CMAKE_SOURCE_DIR}/PingSight/src/PingSight/MultithreadingWorker.h
    ${CMAKE_SOURCE_DIR}/PingSight/src/PingSight/PingSight.cpp
    ${CMAKE_SOURCE_DIR}/PingSight/src/PingSight/PingSight.h
    ${CMAKE_SOURCE_DIR}/PingSight/src/PingSight/PingSightSettings.h
    ${CMAKE_SOURCE_DIR}/PingSight/vendor/imgui/imconfig.h
    ${CMAKE_SOURCE_DIR}/PingSight/vendor/imgui/imgui.h
    ${CMAKE_SOURCE_DIR}/PingSight/vendor/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/PingSight/vendor/imgui/imgui_demo.cpp
    ${CMAKE_SOURCE_DIR}/PingSight/vendor/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/PingSight/vendor/imgui/imgui_internal.h
    ${CMAKE_SOURCE_DIR}/PingSight/vendor/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/PingSight/vendor/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/PingSight/vendor/imgui/imstb_rectpack.h
    ${CMAKE_SOURCE_DIR}/PingSight/vendor/imgui/imstb_textedit.h
    ${CMAKE_SOURCE_DIR}/PingSight/vendor/imgui/imstb_truetype.h
    ${CMAKE_SOURCE_DIR}/PingSight/vendor/imgui/backends/imgui_impl_sdlrenderer3.cpp
    ${CMAKE_SOURCE_DIR}/PingSight/vendor/imgui/backends/imgui_impl_sdlrenderer3.h
    ${CMAKE_SOURCE_DIR}/PingSight/vendor/imgui/backends/imgui_impl_sdl3.cpp
    ${CMAKE_SOURCE_DIR}/PingSight/vendor/imgui/backends/imgui_impl_sdl3.h
    ${CMAKE_SOURCE_DIR}/PingSight/vendor/implot/implot.h
    ${CMAKE_SOURCE_DIR}/PingSight/vendor/implot/implot.cpp
    ${CMAKE_SOURCE_DIR}/PingSight/vendor/implot/implot_demo.cpp
    ${CMAKE_SOURCE_DIR}/PingSight/vendor/implot/implot_internal.h
    ${CMAKE_SOURCE_DIR}/PingSight/vendor/implot/implot_items.cpp
    ${CMAKE_SOURCE_DIR}/PingSight/vendor/stb/stb_image.h
)

# Set include directories
target_include_directories(PingSight PRIVATE
    ${INCLUDE_DIR}/src
    ${SDL_INCLUDE_DIR}
    ${IMGUI_INCLUDE_DIR}
    ${IMPLOT_INCLUDE_DIR}
    ${ICMPLIB_INCLUDE_DIR}
    ${PLF_NANOTIMER_INCLUDE_DIR}
    ${STB_INCLUDE_DIR}
)

# Link libraries
target_link_libraries(PingSight PRIVATE SDL3-static)

# Platform-specific settings
if(WIN32)
    target_link_libraries(PingSight PRIVATE setupapi winmm version)

    target_compile_definitions(PingSight PRIVATE WINDOWS)
    
    # Add resource files
    target_sources(PingSight PRIVATE ${CMAKE_SOURCE_DIR}/PingSight/PingSight.rc)
    target_sources(PingSight PRIVATE
        ${CMAKE_SOURCE_DIR}/PingSight/src/App/Icon.h
        ${CMAKE_SOURCE_DIR}/PingSight/src/App/Icon.cpp
    )

    # Post-build commands
    add_custom_command(TARGET PingSight POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy 
            ${CMAKE_SOURCE_DIR}/PingSight/src/PingSightConfig.ini
            $<TARGET_FILE_DIR:PingSight>
        COMMAND ${CMAKE_COMMAND} -E copy 
            ${CMAKE_SOURCE_DIR}/PingSight/Roboto.ttf
            $<TARGET_FILE_DIR:PingSight>
    )
elseif(APPLE)
    set_target_properties(PingSight PROPERTIES
        MACOSX_BUNDLE TRUE
    )
elseif(UNIX)
    # Linux-specific settings if needed
endif()

# Configuration-specific settings
set(CMAKE_CONFIGURATION_TYPES Debug Release)

# Set startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT PingSight)

# Set DPI awareness
if(WIN32)
    set_property(TARGET PingSight PROPERTY VS_DPI_AWARE "PerMonitor")
endif()
