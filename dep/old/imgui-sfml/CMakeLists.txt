#[[
# imgui-sfml build options:
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/dep/imgui/imgui-src/) # find imgui src dir
message("${IMGUI_DIR}")
option(IMGUI_SFML_FIND_SFML OFF) # won't be able to find_package(sfml) - error!
option(IMGUI_SFML_IMGUI_DEMO ON) # don't build demo

FetchContent_MakeAvailable(imgui-sfml)
message(STATUS "Fetch successful. imgui-sfml available")
]]
