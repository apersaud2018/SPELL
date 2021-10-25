cmake_minimum_required(VERSION 3.0.0)
project(libspell VERSION 0.0.1)


add_library(libspell src/test.cpp)
# https://stackoverflow.com/questions/17511496/how-to-create-a-shared-library-with-cmake
set_target_properties(libspell PROPERTIES VERSION ${PROJECT_VERSION} PUBLIC_HEADER include/test.h)

target_include_directories(libspell PUBLIC include src)

include(GNUInstallDirs)

# configure_file(libspell.pc.in libspell.pc @ONLY)

install(TARGETS libspell LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
#install(FILES ${CMAKE_BINARY_DIR}/libspell.pc
#    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
