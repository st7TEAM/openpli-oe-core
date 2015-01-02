SUMMARY = "Spark71xx driver modules from TDT"
DESCRIPTION = "Spark71xx driver modules from TDT"
SRCDATE = "20140510"

require tdt-dvb-modules.inc

PR = "${INC_PR}.2"

do_install_append_spark7162() {
    find ${D} -name stmcore-display-sti7106.ko | xargs -r rm # we don't have a 7106 chip
}

