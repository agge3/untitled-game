#[[
message(STATUS "Fetching sfml...")

# sfml build options:
set(SFML_BUILD_NETWORK FALSE) # don't want to build network
set(BUILD_SHARED_LIBS OFF) # build static lib
set(SFML_STATIC_LIBRARIES TRUE)

# after fetching, make sfml available
FetchContent_MakeAvailable(sfml)
message(STATUS "Fetch successful. sfml available")
]]
