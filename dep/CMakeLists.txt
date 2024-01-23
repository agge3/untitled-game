#[[include(FetchContent)

message(STATUS "Fetching dependencies...")

set(SFML_VERSION 2.6.1)

# fetch content to correct dep dir - sfml
set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/dep/sfml/")
FetchContent_Declare(
    sfml
    URL "https://github.com/SFML/SFML/archive/${SFML_VERSION}.zip"
    )
# dep/sfml exists! look at subdir CMakeLists.txt for further config
add_subdirectory(sfml)

set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/dep/imgui/")
message(STATUS "Fetching imgui...")
FetchContent_Declare(
    imgui
    GIT_REPOSITORY git@github.com:ocornut/imgui.git
    # fetching by commit hash is faster...
    GIT_TAG origin/docking
    )
# imgui doesn't have a cmake build!
add_subdirectory(imgui)
# still need to set imgui src dir for imgui-sfml though! ->
FetchContent_MakeAvailable(imgui)
#FetchContent_GetProperties(imgui SOURCE_DIR IMGUI_DIR)

set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/dep/imgui-sfml/")
FetchContent_Declare(
    imgui-sfml
    GIT_REPOSITORY git@github.com:SFML/imgui-sfml.git
    GIT_TAG origin/2.6.x
    )
add_subdirectory(imgui-sfml)
]]
