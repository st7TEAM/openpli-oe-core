require linux-vuplus-3.9.6.inc

MACHINE_KERNEL_PR_append = "${PR_INC}.0"

SRC_URI += "\
	file://linux-sata_bcm7335.patch \
	"

COMPATIBLE_MACHINE = "vuduo"
