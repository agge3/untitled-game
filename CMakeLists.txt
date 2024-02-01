# cmake tested versions
cmake_minimum_required(VERSION 3.22...3.27)

set(TARGET_SYSTEM_LINUX TRUE)
set(TARGET_SYSTEM_WIN32 FALSE)

if(TARGET_SYSTEM_WIN32)
    # target os: Windows
    set(CMAKE_SYSTEM_NAME Windows)
    # use x86 win64 mingw32 gcc & g++ compiler
    set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
    set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
    # path to mingw
    set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)
    # don't FIND_XXX in linux
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    # only FIND_XXX in win
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
endif()

# project name, description, & lang
project(untitled-game
    DESCRIPTION "It's an untitled game"
    LANGUAGES CXX)
# set version numbers
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

# declare exe
add_executable(untitled-game)
add_executable(testing)
#add_library(imgui-sfml)

target_sources(testing PRIVATE src/testing.cpp
    # imgui dep
    dep/imgui/imgui.cpp
    dep/imgui/imgui_draw.cpp
    dep/imgui/imgui_demo.cpp
    dep/imgui/imgui_tables.cpp
    dep/imgui/imgui_widgets.cpp
    dep/imgui-sfml/imgui-SFML.cpp
    )

# add src files to target
target_sources(untitled-game PRIVATE src/main.cpp
    src/utility.cpp
    src/debug.cpp
    src/app.cpp
    src/conf.cpp
    src/command.cpp
    src/command_queue.cpp
    src/creature.cpp
    src/entity.cpp
    src/game_config.h
    src/player.cpp
    src/p_task.cpp
    src/scene_node.cpp
    src/sprite_node.cpp
    src/text_node.cpp
    src/r_holders.cpp
    src/state.cpp
    src/s_stack.cpp
    src/s_title.cpp
    src/s_menu.cpp
    src/s_game.cpp
    src/s_loading.cpp
    src/s_pause.cpp
    src/s_settings.cpp
    src/world.cpp
    src/data_tables.cpp
    src/projectile.cpp
    src/pickup.cpp
    # imgui style config
    src/imguistyle.cpp
    # imgui dep
    dep/imgui/imgui.cpp
    dep/imgui/imgui_draw.cpp
    dep/imgui/imgui_demo.cpp
    dep/imgui/imgui_tables.cpp
    dep/imgui/imgui_widgets.cpp
    # imgui sfml backend dep
    dep/imgui-sfml/imgui-SFML.cpp
    )
#[[

    # imgui dep
    dep/imgui/imgui.cpp
    dep/imgui/imgui_widgets.cpp
    dep/imgui/imgui_draw.cpp
    dep/imgui/imgui_tables.cpp
    # imgui-sfml dep
    dep/imgui-sfml/imgui-SFML.cpp
    )

target_sources(imgui-sfml PUBLIC
    # imgui dep
    dep/imgui/imgui.cpp
    dep/imgui/imgui_widgets.cpp
    dep/imgui/imgui_draw.cpp
    dep/imgui/imgui_tables.cpp
    # imgui-sfml dep
    dep/imgui-sfml/imgui-SFML.cpp
    )

]]
#set(BUILD_SHARED_LIBS OFF) # build static lib

# link sfml static lib
#set(SFML_STATIC_LIBRARIES TRUE)
if(TARGET_SYSTEM_LINUX)
    target_include_directories(untitled-game PRIVATE
        "${CMAKE_SOURCE_DIR}/dep/linux/SFML-2.6.1/include/")
    target_link_directories(untitled-game PRIVATE
        "${CMAKE_SOURCE_DIR}/dep/linux/SFML-2.6.1/lib/")
    set(SFML_DIR "${CMAKE_SOURCE_DIR}/dep/linux/SFML-2.6.1/lib/cmake/SFML")
elseif(TARGET_SYSTEM_WIN32)
    target_include_directories(untitled-game PRIVATE
        "${CMAKE_SOURCE_DIR}/dep/SFML-2.6.1/include/")
    target_link_directories(untitled-game PRIVATE
        "${CMAKE_SOURCE_DIR}/dep/SFML-2.6.1/lib/")
    set(SFML_DIR "${CMAKE_SOURCE_DIR}/dep/SFML-2.6.1/lib/cmake/SFML")
endif()

find_package(SFML COMPONENTS network graphics window system REQUIRED)

# link dependencies (imgui & imgui-sfml)
target_include_directories(untitled-game PRIVATE
    "${CMAKE_SOURCE_DIR}/dep/imgui/")
target_include_directories(untitled-game PRIVATE
    "${CMAKE_SOURCE_DIR}/dep/imgui-sfml")
#target_link_directories(untitled-game PRIVATE
#    "${CMAKE_SOURCE_DIR}/dep/imgui-sfml/lib")

# link sfml static lib
#set(SFML_STATIC_LIBRARIES TRUE)
if(TARGET_SYSTEM_LINUX)
    target_include_directories(testing PRIVATE
        "${CMAKE_SOURCE_DIR}/dep/linux/SFML-2.6.1/include/")
    target_link_directories(testing PRIVATE
        "${CMAKE_SOURCE_DIR}/dep/linux/SFML-2.6.1/lib/")
    set(SFML_DIR "${CMAKE_SOURCE_DIR}/dep/linux/SFML-2.6.1/lib/cmake/SFML")
elseif(TARGET_SYSTEM_WIN32)
    target_include_directories(testing PRIVATE
        "${CMAKE_SOURCE_DIR}/dep/linux/SFML-2.6.1/include/")
    target_link_directories(testing PRIVATE
        "${CMAKE_SOURCE_DIR}/dep/linux/SFML-2.6.1/lib/")
    set(SFML_DIR "${CMAKE_SOURCE_DIR}/dep/linux/SFML-2.6.1/lib/cmake/SFML")
endif()
find_package(SFML COMPONENTS network graphics window system REQUIRED)

# link dependencies (imgui & imgui-sfml)
target_include_directories(testing PRIVATE
    "${CMAKE_SOURCE_DIR}/dep/imgui/")
target_include_directories(testing PRIVATE
    "${CMAKE_SOURCE_DIR}/dep/imgui-sfml")
#target_link_directories(testing PRIVATE
#    "${CMAKE_SOURCE_DIR}/dep/imgui-sfml/lib")
#[[target_include_directories(imgui-sfml PUBLIC
    "${CMAKE_SOURCE_DIR}/dep/SFML-2.6.1/include/")
target_link_directories(imgui-sfml PUBLIC
    "${CMAKE_SOURCE_DIR}/dep/SFML-2.6.1/lib/")
set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR "${CMAKE_SOURCE_DIR}/dep/SFML-2.6.1/lib/cmake/SFML")
find_package(SFML COMPONENTS network graphics window system REQUIRED)

target_link_libraries(imgui-sfml PUBLIC
    ws2_32
    sfml-graphics-s
    sfml-window-s
    sfml-system-s
    opengl32
    freetype
    winmm
    gdi32C
    )
]]
# link lib to target
#target_link_directories(testing PRIVATE
#    "/usr/lib/")
#target_include_directories(testing PRIVATE
#    "/usr/include/GL")
target_link_libraries(untitled-game PRIVATE
    #sfml-audio-s
    #openal32
    #flac
    #vorbisenc
    #vorbisfile
    #vorbis
    #ogg
    #sfml-network
    #ws2_32
    sfml-graphics
    sfml-window
    sfml-system
    OpenGL
    GL
    #opengl32
    #freetype
    #winmm
    #gdi32
    #libimgui-sfml-d.a
    #-lOpenGL32
    #ImGui-SFML::ImGui-SFML
    )

# target include/.h
target_include_directories(untitled-game PRIVATE ${CMAKE_SOURCE_DIR}/include)

# target features & options
target_compile_features(untitled-game PRIVATE cxx_std_20)
# m_contextdebug and release g++ flags
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall")
#set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall")

# copy res/lib/dep into build dir, for exec root access
add_custom_command(TARGET untitled-game PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res/ $<TARGET_FILE_DIR:untitled-game>)
add_custom_command(TARGET untitled-game PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/lib/ $<TARGET_FILE_DIR:untitled-game>)
# copy scripts into build dir
add_custom_command(TARGET untitled-game PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/scripts/ $<TARGET_FILE_DIR:untitled-game>)

# build doc with doxygen
# to only build doc for release mode...
#if (CMAKE_BUILD_TYPE MATCHES "^[Rr]elease")
#    # build doc
#endif()
option(BUILD_DOC "Build documentation" ON)
# check if doxygen is installed
find_package(Doxygen)
if (DOXYGEN_FOUND)
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message(STATUS "Doxygen build started")
    # NOTE: ALL builds doc together with app - all targets
    add_custom_target(doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)
else (DOXYGEN_FOUND)
    message(STATUS "Doxygen needs to be installed to generate doxygen documentation")
endif (DOXYGEN_FOUND)

# link lib to target
target_link_libraries(testing PRIVATE
    #sfml-audio-s
    #openal32
    #flac
    #vorbisenc
    #vorbisfile
    #vorbis
    #ogg
    #sfml-network
    sfml-graphics
    sfml-window
    sfml-system
    openal
    OpenGL
    GL
    #libgl
    #imgui
    #-lOpenGL32
    #ImGui-SFML::ImGui-SFML
    )

# target include/.h
target_include_directories(testing PRIVATE ${CMAKE_SOURCE_DIR}/include)

# target features & options
target_compile_features(testing PRIVATE cxx_std_20)
# debug and release g++ flags
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall")
#set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall")

# copy res/lib/dep into build dir, for exec root access
add_custom_command(TARGET testing PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res/ $<TARGET_FILE_DIR:testing>)
add_custom_command(TARGET testing PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/lib/ $<TARGET_FILE_DIR:testing>)
# copy scripts into build dir
add_custom_command(TARGET testing PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/scripts/ $<TARGET_FILE_DIR:testing>)
