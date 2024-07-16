cmake_minimum_required(VERSION 3.5)

project(QSyncUi VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(WINDOWS_BUILD ${MSVC})
set(SERVICE_COMPONENTS s3)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core Network Sql Core5Compat)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core Network Sql Core5Compat)
# Find the AWS SDK for C++ package.

set(PROJECT_SOURCES
        main.cpp



)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(QSyncUi
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}

        user.h user.cpp
        synctask.h synctask.cpp
        mainwindow.h mainwindow.cpp
        homeview.h homeview.cpp
        apirequest.h apirequest.cpp
        apiresponse.h apiresponse.cpp
        synccore.h synccore.cpp
        loginwin.h loginwin.cpp
        resetwin.h resetwin.cpp
        signinwin.h signinwin.cpp
        databasemanager.h databasemanager.cpp
        syncing_view.h syncing_view.cpp

        SyncThread.h SyncThread.cpp
        filemange_view.h filemange_view.cpp
        historysync_view.h historysync_view.cpp
        avatarImage.qrc
        logo.rc
        filequeue.h filequeue.cpp

        linknewfolder_window.h linknewfolder_window.cpp
        tasktoken.h tasktoken.cpp
        filecard.h filecard.cpp
        signhelper.h signhelper.cpp
        historycard.h historycard.cpp
        historyview.h historyview.cpp
        historyviewcard.h historyviewcard.cpp
        cosclient.h cosclient.cpp
        setexcludeditems_view.h setexcludeditems_view.cpp
        xmlprocesser.h xmlprocesser.cpp
        bucket.h bucket.cpp
        messagechannel.h messagechannel.cpp
        dircardproxy.h dircardproxy.cpp

        modifyinfor_win.h modifyinfor_win.cpp
        crc64util.h crc64util.cpp
        filecardproxy.h filecardproxy.cpp
        usermanager.h usermanager.cpp
        historycardproxy.h historycardproxy.cpp
        historyviewcardproxy.h historyviewcardproxy.cpp


        taskmanager.h taskmanager.cpp
        globalvalue.h globalvalue.cpp
        cloudlistener.h cloudlistener.cpp


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
    thirdParty/wtr
)
find_package(Qt6 REQUIRED COMPONENTS Xml)
target_link_libraries(QSyncUi PRIVATE Qt6::Xml Qt6::Core5Compat)
target_link_libraries(QSyncUi PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Xml Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Sql elawidgettools)
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
