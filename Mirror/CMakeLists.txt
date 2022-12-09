cmake_minimum_required(VERSION 3.5)

file(GLOB SRC_FILES src/*.cpp src/*.h)

add_library(MirrorMod
    SHARED ${SRC_FILES}
)

set(MIRROR_TARGET_GAME "ELDENRING" CACHE STRING "Target game to build mirror mod for")
set_property(CACHE MIRROR_TARGET_GAME PROPERTY STRINGS ELDENRING SEKIRO DS3 DS2SOTFS DSR)

add_compile_definitions("BUILD_${MIRROR_TARGET_GAME}")

target_compile_options(MirrorMod PRIVATE /W4 /WX)

set_target_properties(MirrorMod PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(MirrorMod PROPERTIES CXX_STANDARD 20)

target_link_libraries(MirrorMod PRIVATE B3L)

target_include_directories(MirrorMod PRIVATE ../thirdparty/json/include)