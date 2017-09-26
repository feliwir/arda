# - Try to find DirectX
# Once done this will define
#
# DirectX_INCLUDE_DIR

if (WIN32)
if(CMAKE_CL_64)
    set (DirectX_ARCHITECTURE x64)
else()
    set (DirectX_ARCHITECTURE x86)
endif()

find_package(WindowsSDK REQUIRED)

# With VS 2012 and Windows 8 SDK, the DirectX SDK is included as part of
# the Windows SDK.
find_path(DirectX_ROOT_DIR
    "Include/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/um/d3d11.h"
    "Include/um/d3d11.h"
    PATHS ${WINDOWSSDK_LATEST_DIR}
    DOC "DirectX root directory"
)

set(DirectX_BIN_SEARCH_PATH "${DirectX_ROOT_DIR}/bin")

set(DirectX_INCLUDE_SEARCH_PATHS
    "${DirectX_ROOT_DIR}/Include/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/um"
    "${DirectX_ROOT_DIR}/Include/um"
    "${DirectX_ROOT_DIR}/Include/shared"
    "${DirectX_ROOT_DIR}/Include"
)

# DirectX include directory
find_path(DirectX_INCLUDE_DIR d3d11.h
  PATHS
    ${DirectX_INCLUDE_SEARCH_PATHS}
  DOC "DirectX include path"
)

# DirectX bin directory
find_path(DirectX_BIN_DIR fxc.exe
  PATHS
      ${DirectX_BIN_SEARCH_PATH}
  PATH_SUFFIXES
      "${DirectX_ARCHITECTURE}/"
  DOC "DirectX bin path"
)

# DirectX libraries
# set(_DirectX_LIBRARIES d3d11 d3dcompiler dxguid dxgi)
# foreach(DirectX_LIB ${_DirectX_LIBRARIES})
#     find_library(DirectX_${DirectX_LIB}_LIBRARY NAMES ${DirectX_LIB} PATHS ${DirectX_LIBRARY_DIR}
#         NO_DEFAULT_PATH)
#     set(DirectX_LIBRARIES ${DirectX_LIBRARIES} ${DirectX_${DirectX_LIB}_LIBRARY})
#     mark_as_advanced(${DirectX_${DirectX_LIB}_LIBRARY})
# endforeach()
# mark_as_advanced(${DirectX_LIBRARIES})

endif(WIN32)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(DirectX DEFAULT_MSG DirectX_ROOT_DIR DirectX_INCLUDE_DIR DirectX_BIN_DIR)

