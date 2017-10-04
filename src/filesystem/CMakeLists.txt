set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(HDRS ${HDRS}
"${DIR}/avstream.hpp"
"${DIR}/bigarchive.hpp"
"${DIR}/bigstream.hpp"
"${DIR}/directory.hpp"
"${DIR}/entry.hpp"
"${DIR}/file.hpp"
"${DIR}/filestream.hpp"
"${DIR}/filesystem.hpp"
"${DIR}/memorystream.hpp"
"${DIR}/refpackstream.hpp"
"${DIR}/stream.hpp"
"${DIR}/util.hpp"
PARENT_SCOPE)

set(SRCS ${SRCS}
"${DIR}/avstream.cpp"
"${DIR}/bigarchive.cpp"
"${DIR}/bigstream.cpp"
"${DIR}/directory.cpp"
"${DIR}/file.cpp"
"${DIR}/filestream.cpp"
"${DIR}/filesystem.cpp"
"${DIR}/memorystream.cpp"
"${DIR}/refpackstream.cpp"
PARENT_SCOPE)