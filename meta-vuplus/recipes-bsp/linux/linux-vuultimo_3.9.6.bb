require linux-vuplus-3.9.6.inc

MACHINE_KERNEL_PR_append = "${PR_INC}.0"

SRC_URI += "\
	file://vu_ultimo_fixed_mtd.patch \
	"

COMPATIBLE_MACHINE = "vuultimo"
