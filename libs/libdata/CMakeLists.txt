cmake_minimum_required(VERSION 3.0.0)
project(libdata VERSION 0.0.1)


add_library(libdata src/IntermediateDataStructure.cpp src/AudioData.cpp)

# https://stackoverflow.com/questions/17511496/how-to-create-a-shared-library-with-cmake
set_target_properties(libdata PROPERTIES VERSION ${PROJECT_VERSION} PUBLIC_HEADER include/IntermediateDataStructure.h)

target_include_directories(libdata PUBLIC include src)

include(GNUInstallDirs)

# configure_file(libdata.pc.in libdata.pc @ONLY)

install(TARGETS libspell LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
#install(FILES ${CMAKE_BINARY_DIR}/libdata.pc
#    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
