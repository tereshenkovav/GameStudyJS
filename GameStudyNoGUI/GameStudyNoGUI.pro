QT       += core script

CONFIG += c++11

DESTDIR = ../bin

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += KEYCODE_FROM_SFML

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

COREDIR = ../GameStudyJS-core
INCLUDEPATH += $$COREDIR
DEPENDPATH += $$COREDIR

SOURCES += \
    $$COREDIR/animation.cpp \
    $$COREDIR/game.cpp \
    $$COREDIR/line.cpp \
    $$COREDIR/pcxtexloader.cpp \
    $$COREDIR/qgamesystem.cpp \
    $$COREDIR/rect.cpp \
    $$COREDIR/sound.cpp \
    $$COREDIR/sprite.cpp \
    $$COREDIR/text.cpp \
    extproc.cpp \
    main.cpp

HEADERS += \
    $$COREDIR/animation.h \
    $$COREDIR/game.h \
    $$COREDIR/line.h \
    $$COREDIR/pcxtexloader.h \
    $$COREDIR/qgamesystem.h \
    $$COREDIR/rect.h \
    $$COREDIR/sound.h \
    $$COREDIR/sprite.h \
    $$COREDIR/text.h \
    extproc.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32 {
  LIBS+=-L"../SFML-2.5.1-mingw/lib"

  CONFIG(release, debug|release): LIBS += -lsfml-audio -lsfml-graphics -lsfml-main -lsfml-network -lsfml-window -lsfml-system

  CONFIG(debug, debug|release): LIBS += -lsfml-audio-d -lsfml-graphics-d -lsfml-main-d -lsfml-network-d -lsfml-window-d -lsfml-system-d

  INCLUDEPATH+=../SFML-2.5.1-mingw/include

  DEPENDPATH+=../SFML-2.5.1-mingw/include
}

unix {
  CONFIG(release, debug|release): LIBS += -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-window -lsfml-system
  CONFIG(debug, debug|release): LIBS += -lsfml-audio-d -lsfml-graphics-d -lsfml-network-d -lsfml-window-d -lsfml-system-d
}

