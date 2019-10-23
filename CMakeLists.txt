cmake_minimum_required (VERSION 2.6)
PROJECT (JEU_MILLIONS_1)
# The version number.
set (Tutorial_VERSION_MAJOR 0)
set (Tutorial_VERSION_MINOR 1)

#add_definitions(-std=c++11)
ADD_DEFINITIONS(${GTK3_CFLAGS_OTHER})

aux_source_directory(src SRC_LIST)
aux_source_directory(src/game SRC_LIST)
aux_source_directory(src/graphics SRC_LIST)
aux_source_directory(src/sound SRC_LIST)
aux_source_directory(src/system SRC_LIST)
aux_source_directory(src/visual_novel SRC_LIST)

add_executable(JEU_MILLIONS_1 main.c accueil.c accueil.h administration.c administration.h jeu.c jeu.h joueur_question.c joueur_question.h login_creation.c login_creation.h util.c util.h)

INCLUDE(FindPkgConfig)
FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(FREETYPE2 REQUIRED freetype2)
pkg_check_modules(HARFBUZZ REQUIRED harfbuzz)


include_directories(
        ${FREETYPE2_INCLUDE_DIRS}
        ${HARFBUZZ_INCLUDE_DIRS}
        ${GTK3_INCLUDE_DIRS})

link_directories (
        ${FREETYPE2_LIBRARY_DIRS}
        ${HARFBUZZ_LIBRARY_DIRS}
        ${GTK3_LIBRARY_DIRS})

target_link_libraries (JEU_MILLIONS_1
        ${FREETYPE2_LIBRARIES}
        ${HARFBUZZ_LIBRARIES}
        ${GTK3_LIBRARIES})