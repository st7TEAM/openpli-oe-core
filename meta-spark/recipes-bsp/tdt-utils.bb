require tdt-utils.inc

PR = "${INC_PR}.1"

RPROVIDES_${PN} = " \
        fp_control \
        libmmeimage \
        stfbcontrol \
        ustslave \
	"
