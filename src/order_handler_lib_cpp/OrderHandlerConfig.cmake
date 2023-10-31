find_path(OrderHandler_INCLUDE_DIRS
  PATHS "${CMAKE_CURRENT_LIST_DIR}"
  REQUIRED
)

find_library(OrderHandler_LIBRARIES OrderHandler
  PATHS "${CMAKE_CURRENT_LIST_DIR}/build"
  REQUIRED
)

set(OrderHandler_FOUND TRUE)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OrderHandler DEFAULT_MSG OrderHandler_LIBRARIES OrderHandler_INCLUDE_DIRS)