FILESEXTRAPATHS_append := "${THISDIR}/files"

SRC_URI_append_sh4 = " \
    file://libav-fix-sh4-compile-gcc48.patch;patch=1 \
    "

