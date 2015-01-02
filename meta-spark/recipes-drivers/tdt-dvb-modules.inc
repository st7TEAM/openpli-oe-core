HOMEPAGE = "http://gitorious.org/open-duckbox-project-sh4"
SECTION = "kernel/modules"
RDEPENDS_${PN} = "tdt-utils"

LICENSE = " GPLv2"
LIC_FILES_CHKSUM = "file://${WORKDIR}/COPYING;md5=751419260aa954499f7abaabaa882bbe"

inherit module gitpkgv

PACKAGES = "${PN} ${PN}-dev"

SRCREV = "${AUTOREV}"
INC_PR = "r3"
PV = "0.3+git${SRCPV}"
PKGV = "0.3.+git${GITPKGV}"

BINARY_PTI_NP_PATH ?= "/data/pti_np"

SRC_URI = " \
    git://github.com/sklnet/max-tdt-driver.git;protocol=git \
    file://aotom.sh \
    file://COPYING \
" 

FILES_${PN} = "${sysconfdir}/init.d ${sysconfdir}/rcS.d"
FILES = ""

S = "${WORKDIR}/git"

do_configure_prepend () {
    if [ -L include/stmfb ]; then
        rm include/stmfb
    fi
    if [ -L include/player2 ]; then
        rm include/player2
    fi
    if [ -L include/multicom ]; then
        rm include/multicom
    fi
    if [ -L player2 ]; then
        rm player2
    fi
    if [ -L stgfb/stmfb ]; then
        rm stgfb/stmfb
    fi
    if [ -L multicom ]; then
        rm multicom 
    fi
    ln -s ${S}/include/stmfb-3.1_stm24_0104 ${S}/include/stmfb
    ln -s ${S}/include/player2_191 ${S}/include/player2
    ln -s ${S}/multicom-3.2.4/include ${S}/include/multicom
    ln -s ${S}/player2_191 ${S}/player2
    ln -s ${S}/multicom-3.2.4 ${S}/multicom
    ln -s ${S}/stgfb/stmfb-3.1_stm24_0104 ${S}/stgfb/stmfb
    rm -f .config 
    printf "export CONFIG_PLAYER_191=y\nexport CONFIG_MULTICOM324=y\n" > .config
    # disable wireless build
    sed -i 's/^\(obj-y.*+= wireless\)/# \1/' Makefile
    # disable led and button - it's not for spark
    sed -i 's@^\(obj-y.*+= \(led\|button\)/\)@# \1@' Makefile
}

do_compile() {
    unset CFLAGS CPPFLAGS CXXFLAGS LDFLAGS
    oe_runmake KERNEL_PATH=${STAGING_KERNEL_DIR}   \
        KERNEL_SRC=${STAGING_KERNEL_DIR}    \
        KERNEL_VERSION=${KERNEL_VERSION}    \
        -C ${STAGING_KERNEL_DIR}   \
        ${@d.getVar('MACHINE',1).upper()}=1 \
        M=${S} V=1 \
        ARCH=sh \
        PLAYER191=player191 \
        DRIVER_TOPDIR="${S}" \
        KERNEL_LOCATION="${STAGING_KERNEL_DIR}" \
        CONFIG_KERNEL_BUILD="${STAGING_KERNEL_DIR}" \
        CONFIG_KERNEL_PATH="${STAGING_KERNEL_DIR}" \
        CONFIG_MODULES_PATH="${D}" \
        CONFIG_PLAYER_191=y \
        CCFLAGSY="-I${STAGING_DIR_HOST}/usr/include" \
        modules 
}

do_install() {
    unset CFLAGS CPPFLAGS CXXFLAGS LDFLAGS
    oe_runmake KERNEL_PATH=${STAGING_KERNEL_DIR}   \
        KERNEL_SRC=${STAGING_KERNEL_DIR}    \
        KERNEL_VERSION=${KERNEL_VERSION}    \
        -C ${STAGING_KERNEL_DIR}   \
        ${@d.getVar('MACHINE',1).upper()}=1 \
        M=${S} V=1 \
        ARCH=sh \
        PLAYER191=player191 \
        DRIVER_TOPDIR="${S}" \
        KERNEL_LOCATION="${STAGING_KERNEL_DIR}" \
        CONFIG_KERNEL_BUILD="${STAGING_KERNEL_DIR}" \
        CONFIG_KERNEL_PATH="${STAGING_KERNEL_DIR}" \
        CONFIG_MODULES_PATH="${D}" \
        CONFIG_PLAYER_191=y \
        CCFLAGSY="-I${STAGING_DIR_HOST}/usr/include" \
        INSTALL_MOD_PATH=${D} modules_install

    # install header files
    install -d ${D}/${includedir}/linux/dvb
    install -m 644 bpamem/bpamem.h ${D}/${includedir}
    install -m 644 frontcontroller/aotom/aotom_main.h ${D}/${includedir}
    install -m 644 player2/linux/include/linux/dvb/stm_ioctls.h ${D}/${includedir}/linux/dvb
    install -m 644 stgfb/stmfb/linux/drivers/video/stmfb.h ${D}/${includedir}/linux
    install -m 644 multicom/include/mme.h ${D}/${includedir}
    install -m 644 include/player2/JPEG_VideoTransformerTypes.h ${D}/${includedir}

    #install modutils config
    install -d ${D}/${sysconfdir}/modules-load.d
    install -d ${D}/${sysconfdir}/modprobe.d
    install -d ${D}/${sysconfdir}/init.d
    install -d ${D}/${sysconfdir}/rcS.d
    install -m 0755 ${WORKDIR}/aotom.sh ${D}${sysconfdir}/init.d
    ln -sf ../init.d/aotom.sh ${D}${sysconfdir}/rcS.d/S04aotom

    # if a custom pti.ko is present, overwrite the tdt one
    if [ -e ${BINARY_PTI_NP_PATH}/pti_${STM_PATCH_STR}.ko ]; then
        echo "Found custom pti.." 
        install -m 644 ${BINARY_PTI_NP_PATH}/pti_${STM_PATCH_STR}.ko ${D}/lib/modules/${KERNEL_VERSION}/extra/pti/pti.ko
    fi
}

FILES_${PN}-dev += "${includedir}"

