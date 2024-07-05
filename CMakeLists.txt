cmake_minimum_required(VERSION 3.5)

project(QSyncUi VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(WINDOWS_BUILD ${MSVC})
set(SERVICE_COMPONENTS s3)

if (WINDOWS_BUILD) # Set the location where CMake can find the installed libraries for the AWS SDK.
    message(STATUS "CMAKE_SYSTEM_PREFIX_PATH: ${CMAKE_SYSTEM_PREFIX_PATH}")
    message(STATUS "SYSTEM_MODULE_PATH: ${SYSTEM_MODULE_PATH}")
    string(REPLACE ";" "/aws;" SYSTEM_MODULE_PATH "${CMAKE_SYSTEM_PREFIX_PATH}/aws")
    list(APPEND CMAKE_PREFIX_PATH ${SYSTEM_MODULE_PATH})
    message(STATUS "SYSTEM_MODULE_PATH: ${SYSTEM_MODULE_PATH}")
endif ()
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core Network Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core Network Sql)
# Find the AWS SDK for C++ package.
find_package(AWSSDK REQUIRED COMPONENTS ${SERVICE_COMPONENTS})

if (WINDOWS_BUILD AND AWSSDK_INSTALL_AS_SHARED_LIBS)
    # Copy relevant AWS SDK for C++ libraries into the current binary directory for running and debugging.

    # set(BIN_SUB_DIR "/Debug") # if you are building from the command line you may need to uncomment this
    # and set the proper subdirectory to the executables' location.

    AWSSDK_CPY_DYN_LIBS(SERVICE_COMPONENTS "" ${CMAKE_CURRENT_BINARY_DIR}${BIN_SUB_DIR})
endif ()
set(PROJECT_SOURCES
        main.cpp
        loginwindow.cpp
        loginwindow.h
        loginwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(QSyncUi
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        mycore.h mycore.cpp
        user.h user.cpp
        synctask.h synctask.cpp
        awsutils.h awsutils.cpp
        reset.h reset.cpp reset.ui
        signin.h signin.cpp signin.ui
        mainwindow.h mainwindow.cpp
        homeview.h homeview.cpp
        apirequest.h apirequest.cpp
        apiresponse.h apiresponse.cpp


    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET QSyncUi APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(QSyncUi SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        qt_add_resources(PROJECT_SOURCES
        src/ElaWidgetTools.qrc
    )
        add_executable(QSyncUi
            ${PROJECT_SOURCES}
            src/ElaWidgetTools.qrc
        )
    endif()
endif()
add_subdirectory(thirdParty/ElaWidgetTools)
include_directories(
    thirdParty/ElaWidgetTools/include
)

target_link_libraries(QSyncUi PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Sql ${AWSSDK_LINK_LIBRARIES} elawidgettools)
# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.QSyncUi)
endif()
set_target_properties(QSyncUi PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)

install(TARGETS QSyncUi
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QSyncUi)
endif()
