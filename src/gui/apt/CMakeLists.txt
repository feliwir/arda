set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(HDRS ${HDRS}
"${DIR}/apt.hpp"
"${DIR}/constant.hpp"
PARENT_SCOPE)

set(SRCS ${SRCS}
"${DIR}/apt.cpp"
"${DIR}/constant.cpp"
PARENT_SCOPE)