cmake_minimum_required(VERSION 3.17)
project(spell VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

# load either 6 or 5
find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools REQUIRED)

set(PROJECT_SOURCES
        src/main.cpp
)

# include sub cmake files here
include(src/CMakeLists.txt)

# Gen Translations
include(CMakeGenTranslations.txt)

qt_add_executable(spell
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
		${TS_FILES}
		${CMAKE_CURRENT_BINARY_DIR}/lang/langs.cpp
    )
	
target_include_directories(spell PUBLIC include src)

message(NOTICE TS_FILES=${TS_FILES})


add_subdirectory(libs/libspell)
# add_executable(spell src/main.cpp)

target_link_libraries(spell PRIVATE Qt${QT_VERSION_MAJOR}::Widgets libspell)

set_target_properties(spell PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER fit.spell.edu
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)


qt_finalize_executable(spell)

