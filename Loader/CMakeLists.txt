cmake_minimum_required(VERSION 3.5)

file(GLOB SRC_FILES src/*.cpp src/*.h)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library(Loader 
SHARED ${SRC_FILES}
)

target_compile_options(Loader PRIVATE /W4 /WX)
target_compile_definitions(Loader PRIVATE DLL_EXPORT)

set_target_properties(Loader PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(Loader PROPERTIES CXX_STANDARD 20)
set_target_properties(Loader PROPERTIES OUTPUT_NAME "DINPUT8")

target_link_libraries(Loader PRIVATE B3L)
