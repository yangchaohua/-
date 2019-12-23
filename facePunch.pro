#-------------------------------------------------
#
# Project created by QtCreator 2019-12-15T16:56:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = facePunch
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += /usr/local/include/opencv4

LIBS += -L/usr/local/lib  -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_calib3d -lopencv_ccalib -lopencv_core -lopencv_cvv -lopencv_datasets -lopencv_dnn -lopencv_dnn_objdetect -lopencv_dnn_superres -lopencv_dpm -lopencv_face -lopencv_features2d -lopencv_flann -lopencv_freetype -lopencv_fuzzy -lopencv_hfs -lopencv_highgui -lopencv_img_hash -lopencv_imgcodecs -lopencv_imgproc -lopencv_line_descriptor -lopencv_ml -lopencv_objdetect -lopencv_optflow -lopencv_phase_unwrapping -lopencv_photo -lopencv_plot -lopencv_quality -lopencv_reg -lopencv_rgbd -lopencv_saliency -lopencv_shape -lopencv_stereo -lopencv_stitching -lopencv_structured_light -lopencv_superres -lopencv_surface_matching -lopencv_text -lopencv_tracking -lopencv_video -lopencv_videoio -lopencv_videostab -lopencv_xfeatures2d -lopencv_ximgproc -lopencv_xobjdetect -lopencv_xphoto


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    opencv_process.cpp \
    cap.cpp \
    clientsocket.c

HEADERS += \
        mainwindow.h \
    main.h \
    opencv_process.h \
    clientsocket.h \
    cap.h

FORMS += \
        mainwindow.ui
