cmake_minimum_required(VERSION 3.7)

project(radius_src VERSION 1.0.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Qt5 COMPONENTS Widgets OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
#find_package(FFTW REQUIRED)
pkg_check_modules(FFTW IMPORTED_TARGET REQUIRED fftw3)
include_directories(PkgConfig::FFTW)
link_libraries     (PkgConfig::FFTW)
if (Qt5Gui_FOUND)
  get_target_property(QT_INCLUDE_DIR Qt5::Gui INTERFACE_INCLUDE_DIRECTORIES)
endif ()

find_path ( QWT_INCLUDE_DIR
  NAMES qwt_plot.h
  HINTS ${QT_INCLUDE_DIR}
  PATH_SUFFIXES qwt qwt-qt5 qwt6
)

set ( QWT_INCLUDE_DIRS ${QWT_INCLUDE_DIR} )

# version
set ( _VERSION_FILE ${QWT_INCLUDE_DIR}/qwt_global.h )
if ( EXISTS ${_VERSION_FILE} )
  file ( STRINGS ${_VERSION_FILE} _VERSION_LINE REGEX "define[ ]+QWT_VERSION_STR" )
  if ( _VERSION_LINE )
    string ( REGEX REPLACE ".*define[ ]+QWT_VERSION_STR[ ]+\"([^\"]*)\".*" "\\1" QWT_VERSION_STRING "${_VERSION_LINE}" )
  endif ()
endif ()
unset ( _VERSION_FILE )

find_library ( QWT_LIBRARY
  NAMES qwt qwt-qt5
  HINTS ${QT_LIBRARY_DIR}
)

set ( QWT_LIBRARIES ${QWT_LIBRARY} )

include ( FindPackageHandleStandardArgs )
find_package_handle_standard_args( Qwt REQUIRED_VARS QWT_LIBRARY QWT_INCLUDE_DIR VERSION_VAR QWT_VERSION_STRING )

if (Qwt_FOUND AND NOT TARGET Qwt::Qwt)
  add_library(Qwt::Qwt UNKNOWN IMPORTED)
  set_target_properties(Qwt::Qwt PROPERTIES
                        INTERFACE_INCLUDE_DIRECTORIES "${QWT_INCLUDE_DIRS}"
                        IMPORTED_LOCATION "${QWT_LIBRARIES}")
endif ()

find_package( OpenCV REQUIRED )

include_directories(${PROJECT_SOURCE_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${Qt5Widgets_INCLUDE_DIRS}
                    ${Qt5Core_INCLUDE_DIRS}
                    ${Qt5Gui_INCLUDE_DIRS}
		            ${Qt5OpenGL_INCLUDE_DIRS}
		            ${QWT_INCLUDE_DIRS}
                    ${OpenCV_INCLUDE_DIRS}
		            #${FFTW_INCLUDE_DIRS}
                    ${PROJECT_SOURCE_DIR}/gologram 
                    ${PROJECT_SOURCE_DIR}/gui
                    ${PROJECT_SOURCE_DIR}/gui/ui
                    ${PROJECT_SOURCE_DIR}/misc )


set(HEADERS
    fft_c.h
    matrix.h
    calcim.h
    radarConvIm.h
    ConvDistPhys.h
    ConvDistThread.h
    ConvDistColumnThread.h
    ConvAzimuthPhys.h
    ConvAzimuthThread.h
    radapplication.h
    RadSettings.h
    #assistant.h
    #hoverpoints.h
    ImageGolographicThread.h
    ImageGeneratorObject.h
    rhistogram.h
    RadarImageEq.h
    qimage_to_cvmat.h
    cvmat_to_qimage.h
    histogramCalc.h
    src_config.h
    gologram/cuboid.h
    gologram/gologramma.h
    gologram/imagecreatorform.h
    gologram/model3Dconst.h
    gologram/gobjloader.h
    gui/radDataWidget.h
    gui/radTableWidget.h
    gui/rggImageWidget.h
    gui/radMdiArea.h
    gui/ui/ffttimewidget.h
    gui/ui/convdistancewidget.h
    gui/ui/convazimuthwidget.h
    gui/ui/savesettingsform.h
    gui/ui/editsettingsform.h
    #gui/OpenGl/window.h
    #gui/OpenGl/glwidget.h
    #gui/OpenGl/qtlogo.h
    #gui/OpenGl/object.h
    #gui/OpenGl/objloader.h
    gui/golographicModel.h
    gui/golographicWidget.h
    gui/golographicDelegate.h
    #gui/arthurwidgets.h
    #gui/arthurstyle.h
    gui/rhistwidget.h
    #gui/xform.h
    misc/constants1.h
)

set(FORMS
    gologram/image_creator_form.ui
    gui/ui/fft_time_widget.ui
    gui/ui/conv_distance_widget.ui
    gui/ui/conv_azimuth_widget.ui
    gui/ui/save_settings_form.ui
    gui/ui/edit_settings_form.ui
)

set(SOURCES
    fft_c.cpp
    matrix.cpp
    calcim.cpp
    radarConvIm.cpp
    ConvDistPhys.cpp
    ConvDistThread.cpp
    ConvDistColumnThread.cpp
    ConvAzimuthPhys.cpp
    ConvAzimuthThread.cpp
    radapplication.cpp
    RadSettings.cpp
    #assistant.cpp
    #hoverpoints.cpp
    ImageGolographicThread.cpp
    ImageGeneratorObject.cpp
    rhistogram.cpp
    RadarImageEq.cpp
    qimage_to_cvmat.cpp
    cvmat_to_qimage.cpp
    histogramCalc.cpp
    gologram/cuboid.cpp
    gologram/gologramma.cpp
    gologram/imagecreatorform.cpp
    gologram/model3Dconst.cpp
    gologram/gobjloader.cpp
    gui/radDataWidget.cpp
    gui/radTableWidget.cpp
    gui/rggImageWidget.cpp
    gui/radMdiArea.cpp
    gui/ui/ffttimewidget.cpp
    gui/ui/convdistancewidget.cpp
    gui/ui/convazimuthwidget.cpp
    gui/ui/savesettingsform.cpp
    gui/ui/editsettingsform.cpp
    #gui/OpenGl/window.cpp
    #gui/OpenGl/glwidget.cpp
    #gui/OpenGl/qtlogo.cpp
    #gui/OpenGl/object.cpp
    #gui/OpenGl/objloader.cpp
    gui/golographicModel.cpp
    gui/golographicWidget.cpp
    gui/golographicDelegate.cpp
    #gui/arthurwidgets.cpp
    #gui/arthurstyle.cpp
    gui/rhistwidget.cpp
    #gui/xform.cpp
)

set(RESOURCES
)

qt5_wrap_ui(UIS_HDRS ${FORMS} )
add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS} ${UIS_HDRS}
	${RESOURCES} )

target_link_libraries(${PROJECT_NAME} Qt5::Widgets fftw3 fftw3_threads ${QWT_LIBRARIES} ${OpenCV_LIBS})

set(${PROJECT_NAME}_RUNTIME_LIBRARY_DIRS ${CMAKE_INSTALL_LIBDIR} )

target_include_directories(${PROJECT_NAME} PUBLIC
	$<INSTALL_INTERFACE:include/${PROJECT_NAME}> ${FFTW_INCLUDE_DIRS})

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION lib
)

install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

