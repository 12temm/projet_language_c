cmake_minimum_required(VERSION 3.15)
project(MySDLProject C)

set(SDL2_PATH "C:/Users/Benoit/CLionProjects/untitled1/SDL2")

add_executable(MySDLProject
        main.c
        include.c/init.c
        include.c/input.c
        include.c/draw.c
        include.c/Utils.c
        include.c/settings.c
        include.c/button.c
)

target_include_directories(MySDLProject PRIVATE
        "${SDL2_PATH}/include"
        "${CMAKE_SOURCE_DIR}/include.h"
)

target_link_libraries(MySDLProject PRIVATE mingw32 "${SDL2_PATH}/lib/libSDL2main.a" "${SDL2_PATH}/lib/libSDL2.a" "${SDL2_PATH}/lib/libSDL2_mixer.a" "${SDL2_PATH}/lib/libSDL2_ttf.a" "${SDL2_PATH}/lib/libSDL2_image.a" winmm setupapi user32 gdi32 ole32 shell32 imm32 version rpcrt4)

add_custom_command(TARGET MySDLProject POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SDL2_PATH}/bin/SDL2.dll" $<TARGET_FILE_DIR:MySDLProject>)