# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\topdiff_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\topdiff_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\topdiff_gui_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\topdiff_gui_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\topfd_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\topfd_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\topfd_gui_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\topfd_gui_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\topindex_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\topindex_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\topindex_gui_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\topindex_gui_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\topmg_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\topmg_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\topmg_gui_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\topmg_gui_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\toppic_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\toppic_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\toppic_gui_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\toppic_gui_autogen.dir\\ParseCache.txt"
  "topdiff_autogen"
  "topdiff_gui_autogen"
  "topfd_autogen"
  "topfd_gui_autogen"
  "topindex_autogen"
  "topindex_gui_autogen"
  "topmg_autogen"
  "topmg_gui_autogen"
  "toppic_autogen"
  "toppic_gui_autogen"
  )
endif()
