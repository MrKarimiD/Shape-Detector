#-------------------------------------------------
#
# Project created by QtCreator 2014-08-04T12:33:35
#
#-------------------------------------------------

QT       += core gui xml network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ShapeDetection
TEMPLATE = app
unix:  LIBS += -lprotobuf

INCLUDEPATH += geom
INCLUDEPATH += Prototype_Messages

SOURCES += main.cpp\
        mainwindow.cpp \
    imageprocessing.cpp \
    filtersettings.cpp \
    networksender.cpp \
    geom/angle_deg.cpp \
    geom/circle_2d.cpp \
    geom/composite_region_2d.cpp \
    geom/convex_hull.cpp \
    geom/delaunay_triangulation.cpp \
    geom/line_2d.cpp \
    geom/matrix_2d.cpp \
    geom/polygon_2d.cpp \
    geom/ray_2d.cpp \
    geom/rect_2d.cpp \
    geom/sector_2d.cpp \
    geom/segment_2d.cpp \
    geom/triangle_2d.cpp \
    geom/vector_2d.cpp \
    Prototype_Messages/GameGround.pb.cc \
    System_Protobuf/SystemSettings.pb.cc \
    shape.cpp \
    camerasetting.cpp \
    getimage.cpp \
    imageprocfunction.cpp \
    filterprocces.cpp \
    shapedetector.cpp \
    showoutputsetting.cpp \
    matarray.cpp \
    colordetector.cpp

HEADERS  += mainwindow.h \
    imageprocessing.h \
    filtersettings.h \
    geom/geom.h \
    networksender.h \
    geom/angle_deg.h \
    geom/circle_2d.h \
    geom/composite_region_2d.h \
    geom/convex_hull.h \
    geom/delaunay_triangulation.h \
    geom/line_2d.h \
    geom/matrix_2d.h \
    geom/polygon_2d.h \
    geom/ray_2d.h \
    geom/rect_2d.h \
    geom/region_2d.h \
    geom/sector_2d.h \
    geom/segment_2d.h \
    geom/size_2d.h \
    geom/triangle_2d.h \
    geom/triangulation.h \
    geom/vector_2d.h \
    geom/geom.h \
    Prototype_Messages/GameGround.pb.h \
    Constants.h \
    System_Protobuf/SystemSettings.pb.h \
    shape.h \
    camerasetting.h \
    getimage.h \
    imageprocfunction.h \
    filterprocces.h \
    shapedetector.h \
    showoutputsetting.h \
    matarray.h \
    colordetector.h

FORMS    += mainwindow.ui

LIBS += -lopencv_core \
        -lopencv_highgui \
        -lopencv_imgproc \
        -lopencv_videostab \
        -lopencv_video \
        -lopencv_ts \
        -lopencv_stitching \
        -lopencv_superres \
        -lopencv_photo \
        -lopencv_objdetect \
        -lopencv_nonfree \
        -lopencv_ml \
        -lopencv_legacy \
        -lopencv_calib3d \
        -lopencv_flann \
        -lopencv_contrib \
        -lopencv_features2d
