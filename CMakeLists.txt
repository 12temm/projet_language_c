cmake_minimum_required(VERSION 3.15)
project(MySDLProject C)

set(SDL2_PATH "C:/Users/bedia/projet_language_C/SDL2")

add_executable(MySDLProject main.c)

target_include_directories(MySDLProject PRIVATE "${SDL2_PATH}/include")

target_link_libraries(MySDLProject
        PRIVATE
        mingw32
        "${SDL2_PATH}/lib/libSDL2main.a"
        "${SDL2_PATH}/lib/libSDL2.a"
        "${SDL2_PATH}/lib/libSDL2_mixer.a"
        "${SDL2_PATH}/lib/libSDL2_ttf.a"
        winmm
        setupapi
        user32
        gdi32
        ole32
        shell32
        imm32
        version
        rpcrt4
)

add_custom_command(TARGET MySDLProject POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SDL2_PATH}/bin/SDL2.dll"
        $<TARGET_FILE_DIR:MySDLProject>)
