DESCRIPTION = "Linux kernel from stlinux"
LICENSE = "GPLv2"
SECTION = "kernel"
PACKAGE_ARCH = "${MACHINE_ARCH}"
LIC_FILES_CHKSUM = "file://COPYING;md5=d7810fab7487fb0aad327b76f1be7cd7"
KV = "2.6.32"

DEPENDS_spark7162 += " \
           stlinux24-sh4-stx7105-fdma-firmware \
"
DEPENDS_spark += " \
           stlinux24-sh4-stx7111-fdma-firmware \
"

inherit kernel machine_kernel_pr

SRCDATE = "20140403"
MACHINE_KERNEL_PR_append = ".6"

STM_PATCH_STR = "0214"
LINUX_VERSION = "2.6.32.61"
SRCREV = "5cf7f6f209d832a4cf645125598f86213f556fb3"

SRC_URI = "git://github.com/project-magpie/linux-sh4-2.6.32.y.git;protocol=https;branch=spark71xx-0214 \
    file://console.map.c-workaround-for-gcc-4.8.2-build.patch \
    file://st-coprocessor.h \
    file://defconfig \
"


S = "${WORKDIR}/git"
PARALLEL_MAKEINST = ""

export OS = "Linux"
KERNEL_OBJECT_SUFFIX = "ko"
#KERNEL_OUTPUT = "uImage"
KERNEL_IMAGETYPE = "uImage"
KERNEL_IMAGEDEST = "/tmp"

FILES_kernel-image = "${KERNEL_IMAGEDEST}/${KERNEL_IMAGETYPE}"

KEEPUIMAGE = "true"

do_configure () {
    rm -f ${S}/.config || true
    cp ${WORKDIR}/defconfig ${S}/.config
    sed -i "s#^\(CONFIG_EXTRA_FIRMWARE_DIR=\).*#\1\"${STAGING_DIR_HOST}/lib/firmware\"#" .config;
        yes '' | oe_runmake oldconfig
    if test -e scripts/Makefile.fwinst ; then
        sed -i -e "s:-m0644:-m 0644:g" scripts/Makefile.fwinst
    fi
}

do_install_append() {
    kerneldir=${D}${KERNEL_SRC_PATH}
    if [ -f include/linux/bounds.h ]; then
        mkdir -p $kerneldir/include/linux
        cp include/linux/bounds.h $kerneldir/include/linux/bounds.h
    fi
    if [ -f include/asm-sh/machtypes.h ]; then
        mkdir -p $kerneldir/include/asm-sh
        cp include/asm-sh/machtypes.h $kerneldir/include/asm-sh
    fi
    install -d ${D}${includedir}/linux	
    install -m 644 ${WORKDIR}/st-coprocessor.h ${D}${includedir}/linux
    oe_runmake headers_install INSTALL_HDR_PATH=${D}${exec_prefix}/src/linux-${KERNEL_VERSION} ARCH=$ARCH
    cp ${D}${KERNEL_IMAGEDEST}/${KERNEL_IMAGETYPE}-${KERNEL_VERSION} ${D}${KERNEL_IMAGEDEST}/${KERNEL_IMAGETYPE}
}

# hack to override kernel.bbclass...
# uimages are already built in kernel compile
do_uboot_mkimage() {
	:
}

FILES_kernel-dev += "${includedir}/linux"

# bitbake.conf only prepends PARALLEL make in tasks called do_compile, which isn't the case for compile_modules
# So explicitly enable it for that in here
EXTRA_OEMAKE = "${PARALLEL_MAKE} "

PACKAGES =+ "kernel-headers"
FILES_kernel-headers = "${exec_prefix}/src/linux*"

pkg_postinst_kernel-image_spark7162 () {
    echo "Just for test, not doing anything yet..."
    if [ "x$D" == "x" ]; then
        if [ -f /${KERNEL_IMAGEDEST}/${KERNEL_IMAGETYPE} ] ; then
            echo "flash_erase /dev/mtd5 0 0"
            echo "nandwrite -p /dev/mtd5 /${KERNEL_IMAGEDEST}/${KERNEL_IMAGETYPE}"
        fi
    fi
    echo "rm -f /${KERNEL_IMAGEDEST}/${KERNEL_IMAGETYPE}"
    true
}



