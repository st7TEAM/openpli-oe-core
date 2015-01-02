SUMMARY = "DVD access multimeda library"
SECTION = "libs/multimedia"
LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://COPYING;md5=64e753fa7d1ca31632bc383da3b57c27"
PV = "4.2.1"
PR = "r2"

inherit autotools pkgconfig git-project

SRC_URI = "git://git.videolan.org/libdvdread.git"
