QT       += core gui
LIBS -= -ld2d1

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG -= c++11
#QMAKE_CXXFLAGS += /std:c++latest
CONFIG += static

QMAKE_CXXFLAGS_RELEASE += -O2

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += _SIMULATOR_
DEFINES += WC12H
DEFINES += USING_SCREWHOLES
DEFINES += USING_WS2812
DEFINES += ENABLE_DCF77
DEFINES += ENABLE_LANGUAGE
DEFINES += ENABLE_SECONDS
DEFINES += ENABLE_NIGHTMODE
DEFINES += ENABLE_LDR
DEFINES += ENABLE_ANIMATIONS
DEFINES += ENABLE_TEMP_SYMBOL
DEFINES -= ENABLE_NIGHT_POWEROFF

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ../../firmware/v1.1/wordclock/inc
INCLUDEPATH += inc

SOURCES += \
    main.cpp \
    src/mainwindow.cpp \
    src/mainwindow_slots.cpp \
    src/wordclock/src/driver/hardware.cpp \
    ../../firmware/v1.1/wordclock/src/libs/onewire.cpp \
    src/wordclock/src/driver/rtclock.cpp \
    src/wordclock/src/driver/spi.cpp \
    src/wordclock/src/driver/stm32.cpp \
    src/wordclock/src/driver/usart.cpp \
    ../../firmware/v1.1/wordclock/src/libs/dcf77.cpp \
    src/wordclock/src/libs/ds18x20.cpp \
    src/wordclock/src/libs/m95m01.cpp \
    ../../firmware/v1.1/wordclock/src/util/key.cpp \
    ../../firmware/v1.1/wordclock/src/util/modbus.cpp \
    ../../firmware/v1.1/wordclock/src/util/modbus_ascii.cpp \
    ../../firmware/v1.1/wordclock/src/util/modbus_ascii_master.cpp \
    ../../firmware/v1.1/wordclock/src/util/modbus_rtu.cpp \
    ../../firmware/v1.1/wordclock/src/util/modbus_rtu_master.cpp \
    ../../firmware/v1.1/wordclock/src/util/timer.cpp \
    ../../firmware/v1.1/wordclock/src/wordclock/animation.cpp \
    ../../firmware/v1.1/wordclock/src/wordclock/colors.cpp \
    ../../firmware/v1.1/wordclock/src/wordclock/container.cpp \
    ../../firmware/v1.1/wordclock/src/wordclock/sk6812.cpp \
    ../../firmware/v1.1/wordclock/src/wordclock/ws2812b.cpp \
    ../../firmware/v1.1/wordclock/src/wordclock/settings.cpp \
    ../../firmware/v1.1/wordclock/src/wordclock/wordclock.cpp \
    ../../firmware/v1.1/wordclock/src/wordclock/wordclock_clock.cpp \
    ../../firmware/v1.1/wordclock/src/wordclock/wordclock_const.cpp \
    ../../firmware/v1.1/wordclock/src/wordclock/wordclock_display.cpp \
    ../../firmware/v1.1/wordclock/src/wordclock/wordclock_menu.cpp

HEADERS += \
    inc/stm32wb55xx.h \
    inc/mainwindow.h \
    ../../firmware/v1.1/wordclock/inc/driver/clocks.hpp \
    ../../firmware/v1.1/wordclock/inc/libs/onewire.hpp \
    ../../firmware/v1.1/wordclock/inc/driver/rtclock.hpp \
    ../../firmware/v1.1/wordclock/inc/driver/spi.hpp \
    ../../firmware/v1.1/wordclock/inc/driver/stm32.hpp \
    ../../firmware/v1.1/wordclock/inc/driver/usart.hpp \
    ../../firmware/v1.1/wordclock/inc/libs/dcf77.hpp \
    ../../firmware/v1.1/wordclock/inc/libs/ds18x20.hpp \
    ../../firmware/v1.1/wordclock/inc/libs/m95m01.hpp \
    ../../firmware/v1.1/wordclock/inc/util/clock.hpp \
    ../../firmware/v1.1/wordclock/inc/util/key.hpp \
    ../../firmware/v1.1/wordclock/inc/util/modbus.hpp \
    ../../firmware/v1.1/wordclock/inc/util/timer.hpp \
    ../../firmware/v1.1/wordclock/inc/wordclock/animation.hpp \
    ../../firmware/v1.1/wordclock/inc/wordclock/colors.hpp \
    ../../firmware/v1.1/wordclock/inc/wordclock/container.hpp \
    ../../firmware/v1.1/wordclock/inc/wordclock/sk6812.hpp \
    ../../firmware/v1.1/wordclock/inc/wordclock/ws2812b.hpp \
    ../../firmware/v1.1/wordclock/inc/wordclock/settings.hpp \
    ../../firmware/v1.1/wordclock/inc/wordclock/wordclock.hpp \
    ../../firmware/v1.1/wordclock/inc/wordclock/wordclock_typedef.hpp \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
