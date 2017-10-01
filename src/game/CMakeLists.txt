set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(states)

set(HDRS ${HDRS}
"${DIR}/game.hpp"
PARENT_SCOPE)

set(SRCS ${SRCS}
"${DIR}/game.cpp"
PARENT_SCOPE)