set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(HDRS ${HDRS}
"${DIR}/all.hpp"
"${DIR}/gamedata.hpp"
"${DIR}/weapon.hpp" 
PARENT_SCOPE)

set(SRCS ${SRCS}
"${DIR}/gamedata.cpp"
"${DIR}/weapon.cpp" 
PARENT_SCOPE)