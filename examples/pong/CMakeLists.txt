project(pong)

file(GLOB_RECURSE SOURCES "src/*.cc")

add_executable(pong ${SOURCES})

target_link_libraries(pong PRIVATE kingom)