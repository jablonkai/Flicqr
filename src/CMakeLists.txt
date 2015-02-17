project(Flicqr)

set(Flicqr_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/src/)

set(liboauthcpp_LIBRARY /usr/local/lib/liboauthcpp.a)

set(Flicqr_SRCS
    flickrapi.cpp
    main.cpp
    mainwindow.cpp
    photo.cpp
    photoset.cpp
    photosetsmodel.cpp
    settings.cpp)
set(Flicqr_MOC_HDRS
    flickrapi.h
    mainwindow.h
    photo.h
    photoset.h
    photosetsmodel.h
    settings.h)
set(Flicqr_UIS
    mainwindow.ui)
#set(Flicqr_RCCS
#    Flicqr.qrc)

qt5_wrap_cpp(Flicqr_MOC_SRCS ${Flicqr_MOC_HDRS})
qt5_wrap_ui(Flicqr_UIS_H ${Flicqr_UIS})
#QT5_ADD_RESOURCES(Flicqr_RCC_SRCS ${Flicqr_RCCS})

add_executable(flicqr
    ${Flicqr_SRCS}
    ${Flicqr_MOC_SRCS}
    ${Flicqr_UIS_H})
#    ${Flicqr_RCC_SRCS})
target_link_libraries(flicqr
    ${Qt5Core_LIBRARIES}
    ${Qt5Gui_LIBRARIES}
    ${Qt5Network_LIBRARIES}
    ${Qt5Widgets_LIBRARIES}
    ${liboauthcpp_LIBRARY})
include_directories(${CMAKE_CURRENT_BINARY_DIR}
#    ${Qt5Core_INCLUDE_DIRS}
#    ${Qt5Gui_INCLUDE_DIRS}
#    ${Qt5Widgets_INCLUDE_DIRS}
    ${Flicqr_INCLUDE_DIR})
