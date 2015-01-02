FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

#SRC_URI_append_sh4 = " \
#    file://gcc-4.7.0-sh-use-gnu-hash-style.patch \
#    file://gcc-4.7.3-stack-protect-strong.patch \
#    file://gcc-4.8.2-dwarf4.patch \
#    file://gcc-4.8.2-stm-140225.patch \
#"

SRC_URI_append_sh4 = " \
    file://gcc-4.8.2-stm-140225.patch \
"
