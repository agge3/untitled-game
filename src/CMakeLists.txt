#[[
# declare exe
add_executable(untitled_game)

# add src files to target
target_sources(untitled_game PUBLIC main.cpp
    utility.cpp
    debug.cpp
    app.cpp
    conf.cpp
    command.cpp
    command_queue.cpp
    creature.cpp
    entity.cpp
    game_config.h
    player.cpp
    p_task.cpp
    scene_node.cpp
    sprite_node.cpp
    r_holders.cpp
    state.cpp
    s_stack.cpp
    s_title.cpp
    s_menu.cpp
    s_game.cpp
    s_loading.cpp
    s_pause.cpp
    s_settings.cpp
    world.cpp
    )

# link lib to target
target_link_libraries(untitled_game PRIVATE
    #sfml-audio-s
    #openal32
    #flac
    #vorbisenc
    #vorbisfile
    #vorbis
    #ogg
    #sfml-network
    ws2_32
    sfml-graphics
    sfml-window
    sfml-system
    opengl32
    freetype
    winmm
    gdi32
    #ImGui-SFML::ImGui-SFML
    )
]]
