set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(HDRS ${HDRS}
"${DIR}/application.hpp"
"${DIR}/config.hpp"
"${DIR}/debugger.hpp"
"${DIR}/exception.hpp"
"${DIR}/global.hpp"
PARENT_SCOPE)

set(SRCS ${SRCS}
"${DIR}/application.cpp"
"${DIR}/config.cpp"
"${DIR}/debugger.cpp"
"${DIR}/global.cpp"
PARENT_SCOPE)