set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(HDRS ${HDRS}
"${DIR}/cutscene.hpp"
"${DIR}/game.hpp"
"${DIR}/state.hpp"
PARENT_SCOPE)

set(SRCS ${SRCS}
"${DIR}/cutscene.cpp"
"${DIR}/game.cpp"
"${DIR}/state.cpp"
PARENT_SCOPE)