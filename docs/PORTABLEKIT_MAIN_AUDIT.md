# PortableKit main audit for God Eater Burst

Audited source: PortableKit branch `god-eater-burst`, initially forked from PortableKit `main` at commit `33800c4060c53f3233e08f8ab2cc4590a0872063`.

## Findings verified in source

### Profile API

- `host/profile.hpp` defines `portablekit::GameProfile`; the Sekhmet profile fields currently used are present in this API.
- `GameProfile` includes identity, executable hashes, decryption tag/key, load base, guest RAM, boot path, save folders, ad-hoc product code, frame cadence, language and confirm-button fields.
- `DiscFileAlias` and `memory_stick_patch_folder` are also present, but must only be filled after checking the actual ULUS10563 filesystem and paths.

### HLE registration

`host/system.cpp` installs the built-in HLE groups in this order:

1. ThreadMan
2. SysMem
3. IoFileMgr
4. System
5. Media
6. ATRAC
7. MPEG
8. PSMF player / PSMF
9. Offline networking
10. Font
11. Utility
12. Ad-hoc networking
13. Game profile extra HLE hook
14. Linked HLE extension modules

It scans imports from the actual loaded ELF and checks each exact `(library, NID)` pair with `hle.bound()`. Anything not bound is either logged as an unimplemented import or bound to a logging stub. This is the authoritative static HLE coverage check.

### NID name table is not an implementation table

At this branch snapshot, `configs/nids.csv` contains 567 unique `(library, NID)` names. It is a symbol-name registry, not proof that 567 functions have working handlers. Implementation is determined by HLE registration and the exact `hle.bound(library, nid)` result.

### Renderer and audio source presence

- GE command processing is in `host/gpu/ge_state.cpp`; presence of the renderer does not establish compatibility with every command sequence used by this game.
- SAS mixing is in `host/audio/sas_core.cpp`.
- ATRAC decoding is in `host/audio/atrac_decoder.cpp`; codec availability depends on the build's FFmpeg configuration.
- These systems must be tested from the game's runtime trace. No game-specific rendering or audio compatibility is claimed by this source audit.

## Required runtime audit

After a debug APK or host build is available, install the player's own extracted ULUS10563 game data and run an interpreter-only boot with bounded time:

```sh
GEBURST_NO_RENDER=1 GEBURST_NO_AUDIO=1 \
GEBURST_LIST_STUBS=1 timeout 90 SekhmetNative
```

Retain the exact output:

- `HLE imports: N total, M implemented, S logging stubs`
- `Unimplemented imports: ...` and each library/NID under it
- Every first-call `[hle-stub]` line, including arguments and return address
- The final runtime stop reason

Then implement the first required missing function with correct semantics and repeat. Use `GEBURST_STRICT_HLE=1` after the initial discovery pass to prevent logging stubs from returning misleading success values.

## Data limitation

Earlier project notes report 320 imports across 32 libraries, but the exact ULUS10563 import CSV is not presently in this repository or available in the retrieved Library results. A similarly named `ge2_report_imports.csv` belongs to the separate God Eater 2 / NPJH50832 project and must not be used for this audit.
