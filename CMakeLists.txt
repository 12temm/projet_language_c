cmake_minimum_required(VERSION 3.15)
project(MySDLProject C)

# Ton chemin spécifique
set(SDL2_PATH "C:/Users/Benoit/CLionProjects/untitled1/SDL2")

# 1. Liste des fichiers sources (.c)
# On inclut le main et tous les fichiers du dossier include.c
add_executable(MySDLProject
        main.c
        include.c/init.c
        include.c/input.c
        include.c/draw.c
        include.c/Utils.c
        include.c/settings.c
        include.c/button.c
)

# 2. Dossiers d'entête (.h)
# CMake va regarder dans "include.h" automatiquement
target_include_directories(MySDLProject PRIVATE
        "${SDL2_PATH}/include"
        "${CMAKE_SOURCE_DIR}/include.h"
)

# 3. Lier les bibliothèques SDL2
target_link_libraries(MySDLProject
        PRIVATE
        mingw32
        "${SDL2_PATH}/lib/libSDL2main.a"
        "${SDL2_PATH}/lib/libSDL2.a"
        "${SDL2_PATH}/lib/libSDL2_mixer.a"
        "${SDL2_PATH}/lib/libSDL2_ttf.a"
        "${SDL2_PATH}/lib/libSDL2_image.a"
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

# 4. Copier la DLL SDL2 à côté de l'exécutable
add_custom_command(TARGET MySDLProject POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SDL2_PATH}/bin/SDL2.dll"
        $<TARGET_FILE_DIR:MySDLProject>)