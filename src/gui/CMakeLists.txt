set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(apt)

set(HDRS ${HDRS}
"${DIR}/gui.hpp"
PARENT_SCOPE)

set(SRCS ${SRCS}
"${DIR}/gui.cpp"
PARENT_SCOPE)