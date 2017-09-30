set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(HDRS ${HDRS}
"${DIR}/cutscene.hpp"
"${DIR}/state.hpp"
"${DIR}/titlescreen.hpp"
PARENT_SCOPE)

set(SRCS ${SRCS}
"${DIR}/cutscene.cpp"
"${DIR}/state.cpp"
"${DIR}/titlescreen.cpp"
PARENT_SCOPE)