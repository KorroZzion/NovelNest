TARGET = ru.asdnp.NovelNest

CONFIG += \
    auroraapp

QT += sql

PKGCONFIG += \

SOURCES += \
    src/bookdatabasemanager.cpp \
    src/bookfinder.cpp \
    src/bookloader.cpp \
    src/booksmodel.cpp \
    src/main.cpp \

HEADERS += \
    src/bookdatabasemanager.h \
    src/bookfinder.h \
    src/bookloader.h \
    src/booksmodel.h

DISTFILES += \
    rpm/ru.asdnp.NovelNest.spec \

AURORAAPP_ICONS = 86x86 108x108 128x128 172x172

CONFIG += auroraapp_i18n

TRANSLATIONS += \
    translations/ru.asdnp.NovelNest.ts \
    translations/ru.asdnp.NovelNest-ru.ts \
