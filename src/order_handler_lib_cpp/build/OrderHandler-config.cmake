# Compute installation prefix relative to this file.
get_filename_component(_dir "${CMAKE_CURRENT_LIST_FILE}" PATH)
get_filename_component(_prefix "${_dir}/../.." ABSOLUTE)

# Import the targets.
include("${_prefix}/lib/OrderHandler-1.0/OrderHandler-targets.cmake")

# Report other information.
set(OrderHandler_INCLUDE_DIRS "${_prefix}/include/OrderHandler-1.0")
