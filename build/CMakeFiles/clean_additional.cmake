# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "")
  file(REMOVE_RECURSE
  "CMakeFiles\\meucubo_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\meucubo_autogen.dir\\ParseCache.txt"
  "meucubo_autogen"
  )
endif()
