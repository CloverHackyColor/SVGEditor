QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = SVGEditor
TEMPLATE = app

SOURCES += \
    main.cpp \
    svgeditor.cpp \
    nanosvg.cpp \
    nanosvgrast.cpp \
    b64cdecode.cpp \
    FloatLib.cpp \
	SvgDebugLog.cpp

HEADERS += \
    svgeditor.h \
    nanosvg.h \
    b64cdecode.h \
    FloatLib.h \
	EfiCommon.h \
	SvgDebugLog.h
    
FORMS += svgeditor.ui     

RESOURCES += \
    svgeditor.qrc