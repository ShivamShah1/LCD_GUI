SUMMARY = "Nitrogen Dashboard"

DESCRIPTION = "Nitrogen Dashboard"
LICENSE = "Proprietary"
LIC_FILES_CHKSUM = "file://LICENSE;md5=d41d8cd98f00b204e9800998ecf8427e"
PR = "r8"
SECTION = "base"
DEPENDS = "qtbase qtdeclarative qtgraphicaleffects "

SRC_URI = " \
 file://LICENSE \
 file://homescreen.cpp \
 file://homescreen.h \
 file://homescreen.ui \
 file://infoscreen.cpp \
 file://infoscreen.h \
 file://infoscreen.ui \
 file://main.cpp \
 file://logo.png \
 file://resources.qrc \
 file://nitro.pro \
 file://packetsscreen.cpp \
 file://packetsscreen.h \
 file://packetsscreen.ui \
 file://qrc_resources.cpp \
"

S = "${WORKDIR}"

do_install_append() {
}

require recipes-qt/qt5/qt5.inc

FILES_${PN} += " \
        ${sysconfdir}/init.d \
        ${sysconfdir}/profile.d \
        ${sysconfdir} \
        /opt/ \
"
