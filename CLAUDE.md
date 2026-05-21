# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Architecture

This is a QMK userspace repo — it contains **only keymaps**, not keyboard definitions. The split is intentional:

- **Keyboard definition** (`keyboard.json`, `config.h`, `rules.mk`, `rp2040.c`, `mcuconf.h`, `features/`, etc.) lives in the qmk_firmware fork at `/home/max/PycharmProjects/qmk_firmware/keyboards/handwired/skeletyl/rp2040/`
- **Personal keymap** lives here at `keyboards/handwired/skeletyl/rp2040/keymaps/maximump/`

QMK's `is_keyboard()` validation only searches `qmk_home/keyboards/`, so custom keyboards must reside in the firmware fork, not the userspace.

QMK config (in `~/.config/qmk/qmk.ini`):
- `user.qmk_home` = `/home/max/PycharmProjects/qmk_firmware`
- `user.overlay_dir` = `/home/max/dev/qmk_userspace`
- `user.keyboard` = `handwired/skeletyl/rp2040`
- `user.keymap` = `maximump`

## Build Commands

```bash
# Build all targets defined in qmk.json
qmk userspace-compile

# Build a specific target
qmk compile -kb handwired/skeletyl/rp2040 -km maximump

# Via make (delegates to qmk_firmware's Makefile)
make handwired/skeletyl/rp2040:maximump

# Flash
qmk flash -kb handwired/skeletyl/rp2040 -km maximump
```

## Managing Build Targets

Build targets are declared in `qmk.json`. Keymap names must be **all lowercase** (enforced by the v1.1 userspace schema).

```bash
qmk userspace-add -kb handwired/skeletyl/rp2040 -km maximump
qmk userspace-remove -kb handwired/skeletyl/rp2040 -km maximump
qmk userspace-list
```

## CI Note

GitHub Actions (`.github/workflows/build_binaries.yaml`) builds against upstream `qmk/qmk_firmware@master`, not the local fork. Any changes in the fork that are required to compile the keyboard must also be present in upstream QMK for CI to pass.
