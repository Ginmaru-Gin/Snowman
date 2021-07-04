cmake_minimum_required(VERSION 3.19)
project(Snowman)
set(HEADERS abilities.h drawing.h game.h mechanics.h objects.h records.h resource.h skin.h sound.h variables.h)

set(SOURCES abilities.c drawing.c game.c mechanics.c objects.c records.c skin.c sound.c)

set(RESOURCES Lab6.rc)

add_executable(Snowman WIN32 main.c ${HEADERS} ${SOURCES} ${RESOURCES})
target_link_libraries(Snowman msimg32.lib winmm.lib)