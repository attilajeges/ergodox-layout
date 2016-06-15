BOOTMAGIC_ENABLE=no
COMMAND_ENABLE=no
SLEEP_LED_ENABLE=no
UNICODE_ENABLE=no

OPT_DEFS += -DKEYMAP_VERSION=\"$(shell cd "${KEYMAP_PATH}" && git describe --abbrev=6 --dirty --always --tags --match 'v*' 2>/dev/null || date +"%Y-%m-%d")\"

ifndef QUANTUM_DIR
	include ../../../../Makefile
endif
