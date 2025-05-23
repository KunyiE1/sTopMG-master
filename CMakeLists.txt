#specify the version being used as well as the language
cmake_minimum_required(VERSION 3.1)
#Name your project here
project(TopPIC)
SET(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_VERBOSE_MAKEFILE ON)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin)

SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../lib)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  # Linux specific code
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-unknown-pragmas -std=c++14 -Wl,-rpath=$ORIGIN,--disable-new-dtags -O3 -DWITHOUT_MZ5 -fPIC")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux") 

IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  # Windows specific code
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-unknown-pragmas -std=c++14 -O3 -DWITHOUT_MZ5 -fPIC -fpermissive")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

# The options c++14 and mavx are set for the frugally-deep library
message(STATUS "system name " ${CMAKE_SYSTEM_NAME})

# for Qt
set(CMAKE_AUTOMOC ON)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt5Widgets)
find_package(Qt5Core)
find_package(Qt5Gui)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  # Linux specific code
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTOPPIC_LINUX")
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src
                      ${CMAKE_CURRENT_SOURCE_DIR}/ext
                      ${Qt5Widgets_INCLUDE_DIRS}
                      ${Qt5Core_INCLUDE_DIRS}
                      ${Qt5Gui_INCLUDE_DIRS})
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux") 

IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  # Windows specific code
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTOPPIC_WINDOWS -DWIN32")
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src
                      ${CMAKE_CURRENT_SOURCE_DIR}/ext
                      ${Qt5Widgets_INCLUDE_DIRS}
                      ${Qt5Core_INCLUDE_DIRS}
                      ${Qt5Gui_INCLUDE_DIRS})
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

message(STATUS "cmake cxx flags " ${CMAKE_CXX_FLAGS})

# pwiz
file(GLOB PWIZ_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/ext/pwiz/utility/minimxml/*.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/ext/pwiz/utility/misc/*.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/ext/pwiz/data/common/*.cpp
                    ${CMAKE_CURRENT_SOURCE_DIR}/ext/pwiz/data/msdata/*.cpp)
# nowide
file(GLOB NOWIDE_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/ext/boost/nowide/*.cpp)

# hts
file(GLOB HTS_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/ext/htslib/*.c)

# common
file(GLOB_RECURSE COMMON_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/common/*.cpp)
file(GLOB SEQ_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/seq/*.cpp)
file(GLOB PARA_SRCS  ${CMAKE_CURRENT_SOURCE_DIR}/src/para/*.cpp)
file(GLOB SPEC_SRCS  ${CMAKE_CURRENT_SOURCE_DIR}/src/ms/spec/*.cpp
                     ${CMAKE_CURRENT_SOURCE_DIR}/src/ms/factory/*.cpp
                     ${CMAKE_CURRENT_SOURCE_DIR}/src/ms/env/*.cpp
                     ${CMAKE_CURRENT_SOURCE_DIR}/src/ms/feature/*.cpp)
file(GLOB PRSM_SRCS  ${CMAKE_CURRENT_SOURCE_DIR}/src/prsm/*.cpp)

# classes for topfd 
file(GLOB_RECURSE TOPFD_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/topfd/*.cpp)

# topfd console 
file(GLOB TOPFD_CONSOLE_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/console/topfd_argument.cpp
                             ${CMAKE_CURRENT_SOURCE_DIR}/src/console/topfd.cpp)

# topfd gui
file(GLOB TOPFD_GUI_SRCS    ${CMAKE_CURRENT_SOURCE_DIR}/src/topfd/common/topfd_para.cpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/topfd/*.cpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/topfd/*.qrc)

# topfd_single console main
file(GLOB TOPFD_SINGLE_PROC_SRCS   ${CMAKE_CURRENT_SOURCE_DIR}/src/console/topfd_single_process.cpp)
file(GLOB TOPFD_SINGLE_CONSOLE_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/console/topfd_single_argument.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/src/console/topfd_single.cpp)
 
# classes for toppic
file(GLOB_RECURSE FILTER_SRCS  ${CMAKE_CURRENT_SOURCE_DIR}/src/filter/*.cpp)

file(GLOB SEARCH_SRCS  ${CMAKE_CURRENT_SOURCE_DIR}/src/search/diag/*.cpp 
                       ${CMAKE_CURRENT_SOURCE_DIR}/src/search/zeroptmsearch/*.cpp
                       ${CMAKE_CURRENT_SOURCE_DIR}/src/search/oneptmsearch/*.cpp
                       ${CMAKE_CURRENT_SOURCE_DIR}/src/search/varptmsearch/*.cpp
                       ${CMAKE_CURRENT_SOURCE_DIR}/src/search/ptmsearch/*.cpp
                       ${CMAKE_CURRENT_SOURCE_DIR}/src/search/duplicatematch/*.cpp)

file(GLOB TDGF_SRCS   ${CMAKE_CURRENT_SOURCE_DIR}/src/stat/count/*.cpp
                      ${CMAKE_CURRENT_SOURCE_DIR}/src/stat/tdgf/*.cpp)
file(GLOB LOCAL_SRCS   ${CMAKE_CURRENT_SOURCE_DIR}/src/stat/local/*.cpp)
file(GLOB VISUAL_SRCS   ${CMAKE_CURRENT_SOURCE_DIR}/src/visual/*.cpp)

# toppic console main
file(GLOB TOPPIC_PROC_SRCS  ${CMAKE_CURRENT_SOURCE_DIR}/src/console/toppic_process.cpp
		                  	    ${CMAKE_CURRENT_SOURCE_DIR}/src/console/toppic_argument.cpp)

file(GLOB TOPPIC_CONSOLE_SRCS	${CMAKE_CURRENT_SOURCE_DIR}/src/console/toppic.cpp)

# toppic gui main
file(GLOB TOPPIC_GUI_SRCS   ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/toppic/*.cpp
	                  		    ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/toppic/*.qrc
                            ${CMAKE_CURRENT_SOURCE_DIR}/src/console/toppic_argument.cpp)

# classes for topmg
file(GLOB GRAPH_ALIGN_SRCS  ${CMAKE_CURRENT_SOURCE_DIR}/src/search/diag/*.cpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/src/search/graph/*.cpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/src/search/graphalign/*.cpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/src/search/duplicatematch/*.cpp)

file(GLOB MCMC_SRCS  ${CMAKE_CURRENT_SOURCE_DIR}/src/stat/count/*.cpp
                     ${CMAKE_CURRENT_SOURCE_DIR}/src/stat/mcmc/*.cpp)

#file(GLOB TDGF_SRCS  ${CMAKE_CURRENT_SOURCE_DIR}/src/stat/tdgf/*.cpp)

# topmg console main
file(GLOB TOPMG_PROC_SRCS   ${CMAKE_CURRENT_SOURCE_DIR}/src/console/topmg_process.cpp
	                  		    ${CMAKE_CURRENT_SOURCE_DIR}/src/console/topmg_argument.cpp
                      			${CMAKE_CURRENT_SOURCE_DIR}/src/console/topindex_file_name.cpp)

file(GLOB TOPMG_CONSOLE_SRCS   ${CMAKE_CURRENT_SOURCE_DIR}/src/console/topmg.cpp)

# topmg gui main
file(GLOB TOPMG_GUI_SRCS   ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/topmg/*.cpp
	                  		   ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/topmg/*.qrc
                           ${CMAKE_CURRENT_SOURCE_DIR}/src/console/topmg_argument.cpp)

# topdiff classes
file(GLOB DIFF_SRCS   ${CMAKE_CURRENT_SOURCE_DIR}/src/merge/*.cpp)

file(GLOB DIFF_SEARCH_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/search/duplicatematch/*.cpp)

# topdiff console main
file(GLOB TOPDIFF_PROC_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/console/topdiff_process.cpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/src/console/topdiff_argument.cpp)
file(GLOB TOPDIFF_CONSOLE_SRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/console/topdiff.cpp)

# topdiff gui main 
file(GLOB TOPDIFF_GUI_SRCS   ${CMAKE_CURRENT_SOURCE_DIR}/src/console/topdiff_argument.cpp
                             ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/topdiff/*.cpp
		                         ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/topdiff/*.qrc)

# topindex classes
file(GLOB FILTER_INDEX_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/filter/massmatch/*.cpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/src/filter/mng/*.cpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/src/filter/index/*.cpp)

# topindex console main
file(GLOB TOPINDEX_PROC_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/console/topindex_process.cpp
                             ${CMAKE_CURRENT_SOURCE_DIR}/src/console/topindex_argument.cpp)					 

file(GLOB TOPINDEX_CONSOLE_SRCS
        		     ${CMAKE_CURRENT_SOURCE_DIR}/src/console/topindex.cpp)

# topindex gui main 
file(GLOB TOPINDEX_GUI_SRCS   ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/topindex/*.cpp
			                        ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/topindex/*.qrc
                              ${CMAKE_CURRENT_SOURCE_DIR}/src/console/topindex_argument.cpp)


# unit test using catch
file(GLOB UNIT_TEST_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/test/*.cpp)

#gui to run exe in command line
file(GLOB GUI_UTIL_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/util/*.cpp
                        ${CMAKE_CURRENT_SOURCE_DIR}/src/common/util/*.cpp
                        ${CMAKE_CURRENT_SOURCE_DIR}/src/common/base/*.cpp
                        ${CMAKE_CURRENT_SOURCE_DIR}/src/common/xml/*.cpp)

# topmerge console main
#file(GLOB TOPMERGE_PROC_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/console/topmerge_process.cpp
#                             ${CMAKE_CURRENT_SOURCE_DIR}/src/console/topmerge_argument.cpp)		
                             
#file(GLOB TOPMERGE_SEARCH_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/search/duplicatematch/*.cpp)			 

#file(GLOB TOPMERGE_CONSOLE_SRCS
#                     		     ${CMAKE_CURRENT_SOURCE_DIR}/src/console/topmerge.cpp)

# topindex gui main 
#file(GLOB TOPMERGE_GUI_SRCS   ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/topmerge/*.cpp
#			                        ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/topmerge/*.qrc)

# topconvert
#file(GLOB TOPCONVERT_SRCS     ${CMAKE_CURRENT_SOURCE_DIR}/src/console/topconvert_process.cpp
#		                  	      ${CMAKE_CURRENT_SOURCE_DIR}/src/console/toppic_argument.cpp
#                              ${CMAKE_CURRENT_SOURCE_DIR}/src/console/topconvert.cpp)
                              
#file(GLOB TOPCONVERT_SEARCH_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/search/duplicatematch/*.cpp)

add_executable(topfd 
  ${TOPFD_CONSOLE_SRCS} ${TOPFD_SRCS} ${COMMON_SRCS} ${PARA_SRCS} 
  ${SPEC_SRCS} ${ENV_SRCS} ${FEATURE_SRCS} ${PWIZ_SRCS} ${NOWIDE_SRCS} src/prsm/evalue_prsm.cpp src/prsm/evalue_prsm.hpp)

add_executable(topindex 
  ${TOPINDEX_CONSOLE_SRCS} ${TOPINDEX_PROC_SRCS} ${HTS_SRCS} ${COMMON_SRCS}
  ${SEQ_SRCS} ${PARA_SRCS} ${FILTER_INDEX_SRCS} src/prsm/evalue_prsm.cpp src/prsm/evalue_prsm.hpp)

add_executable(toppic 
	${TOPPIC_CONSOLE_SRCS} ${TOPPIC_PROC_SRCS} ${HTS_SRCS} ${COMMON_SRCS} 
  ${SEQ_SRCS} ${PARA_SRCS} ${SPEC_SRCS}  
  ${PRSM_SRCS} ${FILTER_SRCS} ${SEARCH_SRCS}  
  ${TDGF_SRCS} ${LOCAL_SRCS} ${VISUAL_SRCS} src/filter/LCS/lcs_filter.hpp src/scaffold/scaffold.cpp src/scaffold/scaffold.hpp src/scaffold/scaffold_processor.cpp src/scaffold/scaffold_processor.hpp src/scaffold/scaffold_mng.cpp src/scaffold/scaffold_mng.hpp src/prsm/evalue_prsm.cpp src/prsm/evalue_prsm.hpp)

add_executable(topmg 
	${TOPMG_CONSOLE_SRCS} ${TOPMG_PROC_SRCS} ${HTS_SRCS} ${COMMON_SRCS}
  ${SEQ_SRCS} ${PARA_SRCS} ${SPEC_SRCS} ${PRSM_SRCS} ${FILTER_SRCS}
	${GRAPH_ALIGN_SRCS} ${MCMC_SRCS} ${VISUAL_SRCS} src/filter/LCS/lcs_filter.hpp src/search/LCS/lcs_alignment_processor.cpp src/scaffold/scaffold.cpp src/scaffold/scaffold.hpp src/scaffold/scaffold_processor.cpp src/scaffold/scaffold_processor.hpp src/scaffold/scaffold_mng.cpp src/scaffold/scaffold_mng.hpp src/prsm/evalue_prsm.cpp src/prsm/evalue_prsm.hpp
        src/stat/tdgf/tdgf_mng.cpp
        src/stat/tdgf/tdgf_mng.hpp

        src/stat/tdgf/evalue_processor.cpp
        src/stat/tdgf/evalue_processor.hpp

        src/stat/tdgf/comp_pvalue_array.cpp
        src/stat/tdgf/comp_pvalue_array.hpp

        src/stat/tdgf/comp_pvalue_lookup_table.cpp
        src/stat/tdgf/comp_pvalue_lookup_table.hpp

        src/stat/tdgf/comp_prob_value_array.cpp
        src/stat/tdgf/comp_prob_value_array.hpp

        src/stat/tdgf/comp_prob_value.hpp
        src/stat/tdgf/comp_prob_value.cpp

        src/stat/tdgf/prob_peak.cpp
        src/stat/tdgf/prob_peak.hpp)

add_executable(topdiff 
  ${TOPDIFF_CONSOLE_SRCS} ${TOPDIFF_PROC_SRCS} ${HTS_SRCS} ${COMMON_SRCS} ${DIFF_SEARCH_SRCS}
  ${SEQ_SRCS} ${PARA_SRCS} ${SPEC_SRCS} ${PRSM_SRCS} ${DIFF_SRCS} src/prsm/evalue_prsm.cpp src/prsm/evalue_prsm.hpp)

add_executable(topfd_gui ${TOPFD_GUI_SRCS} ${GUI_UTIL_SRCS} src/prsm/evalue_prsm.cpp src/prsm/evalue_prsm.hpp)

add_executable(topindex_gui ${TOPINDEX_GUI_SRCS} ${GUI_UTIL_SRCS} src/prsm/evalue_prsm.cpp src/prsm/evalue_prsm.hpp)

add_executable(toppic_gui ${TOPPIC_GUI_SRCS} ${GUI_UTIL_SRCS} src/prsm/evalue_prsm.cpp src/prsm/evalue_prsm.hpp)

add_executable(topmg_gui ${TOPMG_GUI_SRCS} ${GUI_UTIL_SRCS} src/prsm/evalue_prsm.cpp src/prsm/evalue_prsm.hpp)

add_executable(topdiff_gui ${TOPDIFF_GUI_SRCS} ${GUI_UTIL_SRCS} src/prsm/evalue_prsm.cpp src/prsm/evalue_prsm.hpp)

#add_executable(topfd_single 
#  ${TOPFD_SINGLE_CONSOLE_SRCS} ${TOPFD_SINGLE_PROC_SRCS} ${TOPFD_SRCS}
#  ${SPEC_SRCS} ${ENV_SRCS} ${PWIZ_SRCS} ${COMMON_SRCS}) 

#add_executable(topmerge ${TOPMERGE_CONSOLE_SRCS} ${TOPMERGE_PROC_SRCS} ${HTS_SRCS} ${COMMON_SRCS} 
#  ${TOPMERGE_SEARCH_SRCS} ${SEQ_SRCS} ${PARA_SRCS} ${SPEC_SRCS} ${LOCAL_SRCS} ${PRSM_SRCS} ${VISUAL_SRCS}) 
  
#add_executable(topmerge_gui ${TOPMERGE_GUI_SRCS} ${TOPMERGE_GUI_QRC} 
#  ${TOPMERGE_PROC_SRCS} ${HTS_SRCS} ${COMMON_SRCS} ${TOPMERGE_SEARCH_SRCS}
#  ${SEQ_SRCS} ${PARA_SRCS} ${SPEC_SRCS} ${LOCAL_SRCS} ${PRSM_SRCS} ${VISUAL_SRCS} ${GUI_UTIL_SRCS}) 

#add_executable(topconvert ${TOPCONVERT_SRCS} ${HTS_SRCS} ${COMMON_SRCS} 
#  ${SEQ_SRCS} ${PARA_SRCS} ${SPEC_SRCS} ${FEATURE_SRCS} ${PRSM_SRCS} ${TOPCONVERT_SEARCH_SRCS})

#add_executable(toppic_test 
#  ${TEST_SRCS} ${HTS_SRCS} ${COMMON_SRCS} 
#  ${SEQ_SRCS} ${SPEC_SRCS} ${ENV_SRCS} ${FEATURE_SRCS} ${PRSM_SRCS} ${FILTER_SRCS}  
#  ${ZERO_PTM_SRCS} ${ONE_PTM_SEARCH_SRCS} ${PTM_SRCS} 
#  ${TDGF_SRCS} ${LOCAL_SRCS} ${VISUAL_SRCS})


#add_executable(toppic_match_peak ${TOPPIC_MATCH_PEAK_SRCS} ${TOPPIC_ARGU_SRCS} ${HTS_SRCS} ${BASE_SRCS} ${SPEC_SRCS} ${PRSM_SRCS} ${PRSM_VIEW_SRCS})

#add_executable(topfd_train ${TOPFD_TRAIN_SRCS} ${TOPFD_ARGU_SRCS} ${HTS_SRCS} ${BASE_SRCS} ${SPEC_SRCS} ${FEATURE_SRCS} ${PWIZ_SRC})

IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  target_link_libraries(topfd xerces-c
    boost_program_options-mt boost_filesystem-mt
    boost_system-mt boost_thread-mt
    boost_iostreams-mt boost_chrono-mt pthread z psapi Ws2_32)

  target_link_libraries(topindex xerces-c boost_program_options-mt
    boost_serialization-mt boost_filesystem-mt boost_system-mt boost_thread-mt pthread z Ws2_32)

  target_link_libraries(toppic xerces-c
    boost_filesystem-mt boost_system-mt 
    boost_serialization-mt
    boost_program_options-mt boost_thread-mt pthread z Ws2_32)

  target_link_libraries(topmg xerces-c
    boost_filesystem-mt boost_system-mt boost_serialization-mt
    boost_program_options-mt boost_thread-mt pthread z Ws2_32)

  target_link_libraries(topdiff xerces-c boost_program_options-mt
    boost_filesystem-mt boost_system-mt boost_thread-mt pthread z Ws2_32)

  # for Qt
  SET(QTOPTION -Wl,-subsystem,windows)
  target_link_libraries(topfd_gui ${QTOPTION} Qt5Widgets Qt5Core Qt5Gui
    xerces-c boost_filesystem-mt boost_system-mt boost_thread-mt 
    boost_iostreams-mt boost_chrono-mt pthread z psapi Ws2_32)

  target_link_libraries(topindex_gui ${QTOPTION} Qt5Widgets Qt5Core Qt5Gui xerces-c
    boost_filesystem-mt boost_system-mt boost_serialization-mt 
    boost_program_options-mt boost_thread-mt pthread z Ws2_32)

  target_link_libraries(toppic_gui ${QTOPTION} Qt5Widgets Qt5Core Qt5Gui xerces-c
    boost_filesystem-mt boost_system-mt 
    boost_serialization-mt
    boost_program_options-mt boost_thread-mt pthread z Ws2_32)

  target_link_libraries(topmg_gui ${QTOPTION} Qt5Widgets Qt5Core Qt5Gui xerces-c
    boost_filesystem-mt boost_system-mt boost_serialization-mt
    boost_program_options-mt boost_thread-mt pthread z Ws2_32)

  target_link_libraries(topdiff_gui ${QTOPTION} Qt5Widgets Qt5Core Qt5Gui xerces-c
    boost_filesystem-mt boost_system-mt 
    boost_program_options-mt boost_thread-mt pthread z Ws2_32)

  #   target_link_libraries(topfd_single xerces-c
  #  boost_program_options-mt boost_filesystem-mt
  #  boost_system-mt boost_thread-mt
  #  boost_iostreams-mt boost_chrono-mt pthread z Ws2_32)

  #target_link_libraries(topmerge xerces-c boost_program_options-mt
  #  boost_serialization-mt boost_filesystem-mt boost_system-mt boost_thread-mt pthread z Ws2_32)
	
  #target_link_libraries(topconvert xerces-c boost_program_options-mt
  #  boost_filesystem-mt boost_system-mt boost_thread-mt pthread z Ws2_32)	

  #target_link_libraries(topmerge_gui ${QTOPTION} Qt5Widgets Qt5Core Qt5Gui xerces-c
  #  boost_filesystem-mt boost_system-mt boost_serialization-mt 
  #  boost_program_options-mt boost_thread-mt pthread z Ws2_32)

ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows") 

# xerces-c boost_system, boost_thread boost_iostreams boost_chrono pthread z are required by
# ProteoWizard
IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  target_link_libraries(topfd xerces-c boost_program_options
    boost_filesystem boost_system boost_thread boost_iostreams boost_chrono
    pthread z)

  target_link_libraries(topindex xerces-c boost_program_options
    boost_filesystem boost_system boost_serialization boost_thread pthread z)

  target_link_libraries(toppic xerces-c boost_program_options
    boost_filesystem boost_system boost_serialization boost_thread pthread z)

  target_link_libraries(topmg xerces-c boost_program_options
    boost_filesystem boost_system boost_serialization boost_thread pthread z)

  target_link_libraries(topdiff xerces-c boost_program_options
    boost_filesystem boost_system boost_thread pthread z)

  target_link_libraries(topfd_gui Qt5Widgets Qt5Core Qt5Gui
    xerces-c boost_filesystem boost_system boost_thread boost_iostreams boost_chrono pthread z)

  target_link_libraries(topindex_gui Qt5Widgets Qt5Core Qt5Gui
    xerces-c boost_program_options boost_filesystem boost_serialization 
    boost_system boost_thread boost_iostreams pthread z)

  target_link_libraries(toppic_gui Qt5Widgets Qt5Core Qt5Gui 
    xerces-c boost_program_options boost_filesystem boost_system boost_serialization boost_thread pthread z)

  target_link_libraries(topmg_gui Qt5Widgets Qt5Core Qt5Gui xerces-c boost_program_options
    boost_filesystem boost_system boost_serialization boost_thread pthread z)

  target_link_libraries(topdiff_gui Qt5Widgets Qt5Core Qt5Gui
    xerces-c boost_program_options boost_filesystem boost_system boost_thread boost_iostreams pthread z)

  #target_link_libraries(topfd_single xerces-c boost_program_options
  #  boost_filesystem boost_system boost_thread boost_iostreams boost_chrono
  #  pthread z)

  #target_link_libraries(topmerge xerces-c boost_program_options
  #  boost_filesystem boost_system boost_serialization boost_thread pthread z)

  #target_link_libraries(topmerge_gui Qt5Widgets Qt5Core Qt5Gui
  #  xerces-c boost_program_options boost_filesystem boost_serialization boost_system boost_thread boost_iostreams pthread z)  

  #target_link_libraries(topconvert xerces-c boost_program_options
  #  boost_filesystem boost_system boost_thread pthread z)


  #    target_link_libraries(toppic_test xerces-c boost_program_options
  #  boost_filesystem boost_system boost_thread pthread z)

  #    target_link_libraries(toppic_match_peak xerces-c boost_program_options
  #        boost_filesystem boost_system boost_thread pthread z)

  #    target_link_libraries(topfd_train xerces-c boost_program_options
  #        boost_filesystem boost_system boost_thread boost_iostreams boost_chrono pthread z)

  #file(MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/usr/bin)
install (TARGETS topfd topindex toppic topmg topdiff topfd_gui topindex_gui
  toppic_gui topmg_gui topdiff_gui DESTINATION bin/)
install (DIRECTORY resources/ DESTINATION etc/toppic)

ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux") 

