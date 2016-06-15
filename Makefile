BOOTMAGIC_ENABLE=no
COMMAND_ENABLE=no
SLEEP_LED_ENABLE=no
UNICODE_ENABLE=no

KEYMAP_VERSION = $(shell \
 if [ -d "${KEYMAP_PATH}/.git" ]; then \
  cd "${KEYMAP_PATH}" && git describe --abbrev=6 --dirty --always --tags --match 'v*' 2>/dev/null; \
 else echo QMK; fi)

OPT_DEFS += -DKEYMAP_VERSION=\"$(KEYMAP_VERSION)\"

ifndef QUANTUM_DIR
	include ../../../../Makefile
endif
