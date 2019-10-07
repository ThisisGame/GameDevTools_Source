QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    lua-5.3.5/src/lapi.c \
    lua-5.3.5/src/lauxlib.c \
    lua-5.3.5/src/lbaselib.c \
    lua-5.3.5/src/lbitlib.c \
    lua-5.3.5/src/lcode.c \
    lua-5.3.5/src/lcorolib.c \
    lua-5.3.5/src/lctype.c \
    lua-5.3.5/src/ldblib.c \
    lua-5.3.5/src/ldebug.c \
    lua-5.3.5/src/ldo.c \
    lua-5.3.5/src/ldump.c \
    lua-5.3.5/src/lfunc.c \
    lua-5.3.5/src/lgc.c \
    lua-5.3.5/src/linit.c \
    lua-5.3.5/src/liolib.c \
    lua-5.3.5/src/llex.c \
    lua-5.3.5/src/lmathlib.c \
    lua-5.3.5/src/lmem.c \
    lua-5.3.5/src/loadlib.c \
    lua-5.3.5/src/lobject.c \
    lua-5.3.5/src/lopcodes.c \
    lua-5.3.5/src/loslib.c \
    lua-5.3.5/src/lparser.c \
    lua-5.3.5/src/lstate.c \
    lua-5.3.5/src/lstring.c \
    lua-5.3.5/src/lstrlib.c \
    lua-5.3.5/src/ltable.c \
    lua-5.3.5/src/ltablib.c \
    lua-5.3.5/src/ltm.c \
    lua-5.3.5/src/lua.c \
    lua-5.3.5/src/lundump.c \
    lua-5.3.5/src/lutf8lib.c \
    lua-5.3.5/src/lvm.c \
    lua-5.3.5/src/lzio.c \
    main.cpp \
    operationdialog.cpp

HEADERS += \
    lua-5.3.5/src/lapi.h \
    lua-5.3.5/src/lauxlib.h \
    lua-5.3.5/src/lcode.h \
    lua-5.3.5/src/lctype.h \
    lua-5.3.5/src/ldebug.h \
    lua-5.3.5/src/ldo.h \
    lua-5.3.5/src/lfunc.h \
    lua-5.3.5/src/lgc.h \
    lua-5.3.5/src/llex.h \
    lua-5.3.5/src/llimits.h \
    lua-5.3.5/src/lmem.h \
    lua-5.3.5/src/lobject.h \
    lua-5.3.5/src/lopcodes.h \
    lua-5.3.5/src/lparser.h \
    lua-5.3.5/src/lprefix.h \
    lua-5.3.5/src/lstate.h \
    lua-5.3.5/src/lstring.h \
    lua-5.3.5/src/ltable.h \
    lua-5.3.5/src/ltm.h \
    lua-5.3.5/src/lua.h \
    lua-5.3.5/src/lua.hpp \
    lua-5.3.5/src/luaconf.h \
    lua-5.3.5/src/lualib.h \
    lua-5.3.5/src/lundump.h \
    lua-5.3.5/src/lvm.h \
    lua-5.3.5/src/lzio.h \
    operationdialog.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
