cmake_minimum_required(VERSION 3.15)
project(MySDLProject C)

set(SDL2_PATH "C:/projet_langage_C/SDL2")

add_executable(MySDLProject main.c
        include.h/structure.h
        include.c/init_global.c
        include.h/init_global.h
        include.c/menu.c
        include.h/menu.h
        include.c/menu_weapons.c
        include.h/menu_weapons.h
        include.c/background.c
        include.h/background.h
        include.c/entity.c
        include.h/leaderboard.h
        include.c/leaderboard.c
        include.h/victory.h
        include.c/victory.c
        include.h/game_logic.h
        include.c/game_logic.c
        include.h/config_loader.h
        include.c/config_loader.c
        include.h/utils.h
        include.c/utils.c
        include.h/init_assets.h
        include.c/init_asset.c
        include.h/cleanup.h
        include.c/cleanup.c
        include.h/draw.h
        include.c/draw.c
        include.h/settings_menu.h
        include.c/settings_menu.c
)

target_include_directories(MySDLProject PRIVATE
        "${SDL2_PATH}/include"
        "${CMAKE_SOURCE_DIR}/include.h"
)

target_link_libraries(MySDLProject PRIVATE
        mingw32
        "${SDL2_PATH}/lib/libSDL2main.a"
        "${SDL2_PATH}/lib/libSDL2.a"
        "${SDL2_PATH}/lib/libSDL2_mixer.a"
        "${SDL2_PATH}/lib/libSDL2_ttf.a"
        "${SDL2_PATH}/lib/libSDL2_image.a"
        "${CMAKE_SOURCE_DIR}/include.h"
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
