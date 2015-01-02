HOMEPAGE = "https://www.gitorious.org/open-duckbox-project-sh4/tdt"
SECTION = "base"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://COPYING;md5=d32239bcb673463ab874e80d47fae504"
DEPENDS = "tdt-dvb-modules \
    jpeg \
    stslave-fw \
    "

SRCREV = "${AUTOREV}"
INC_PR = "r5"
PV = "0.1+git${SRCPV}"
PKGV = "0.1.+git${GITPKGV}"

SRC_URI = " \
    git://github.com/sklnet/tdt-tools.git;protocol=git \
    "

S = "${WORKDIR}/git"

inherit autotools 

EXTRA_OECONF = "--enable-silent-rules --enable-maintainer-mode --prefix="

LDFLAGS += "-lpthread -lrt"

do_configure() {
    cd ${S}
    ./autogen.sh
    oe_runconf
}

do_compile() {
    cd ${S}
    oe_runmake  'DESTDIR=${D}' 
}

do_install() {
    cd ${S}
    oe_runmake  'DESTDIR=${D}' install
    install -d ${D}${includedir}/mmeimage
    install -m 644 ${WORKDIR}/git/libmmeimage/*.h ${D}${includedir}/mmeimage
    install -d ${D}/${sysconfdir}/init.d
    install -d ${D}/${sysconfdir}/rc3.d
}

FILES_${PN}-dev += "${includedir}/mmeimage"

