# God Eater Burst HLE compatibility

Target: US PSP release `ULUS10563`, `APP_VER 01.00`.

The previous EBOOT audit summary reports 320 function imports across 32 library records. The original NID-by-NID CSV is not currently available in the Sekhmet repository, so the exact 320-entry comparison remains pending. This document is a tracking ledger; an import is not considered supported merely because a library name exists in PortableKit.

## Import inventory from EBOOT

| Library | Import count | Audit status |
|---|---:|---|
| ThreadManForUser | 58 | Pending per-NID audit |
| sceSasCore | 30 | Pending per-NID audit |
| sceMpeg | 24 | Pending per-NID audit |
| sceUtility | 15 | Pending per-NID audit |
| IoFileMgrForUser | 14 | Pending per-NID audit |
| sceRtc | 13 | Pending per-NID audit |
| sceNetAdhocctl | 13 | Pending per-NID audit |
| SysMemUserForUser | 12 | Pending per-NID audit |
| sceJpeg | 12 | Pending per-NID audit |
| sceGe_user | 11 | Pending per-NID audit |
| sceAudio | 11 | Pending per-NID audit |
| sceLibFont | 11 | Pending per-NID audit |
| ModuleMgrForUser | 10 | Pending per-NID audit |
| UtilsForUser | 10 | Pending per-NID audit |
| scePower | 9 | Pending per-NID audit |
| sceUmdUser | 9 | Pending per-NID audit |
| Kernel_Library | 8 | Pending per-NID audit |
| Other libraries | 50 | Exact per-library breakdown to be imported from the EBOOT audit |

## Status definitions

- **Implemented:** matching NID exists and behavior is reviewed.
- **Partial:** handler exists but semantics, error paths, timing, or memory effects are incomplete.
- **Missing:** no handler is registered for the NID.
- **Needs testing:** handler exists but has not been exercised by this title.

## Rules

1. Audit exact NIDs, not just function names or library totals.
2. Record the PortableKit handler and source file for every implemented NID.
3. Mark functions reached during boot separately from functions only imported.
4. Do not add success-returning stubs for functions that must perform real work.
5. Add game-specific behavior through the profile HLE hook only when the behavior is genuinely title-specific; otherwise fix PortableKit.

## Known executable facts

- Encrypted EBOOT SHA-256: `aa78e5d222340a84532600eb30d5694f91f7750493529af584e4813c4392a6e9`
- Decrypted ELF SHA-256: `1c6d7966fdc93e4b78a1b4807bbb3c7eb9c3030986790a521224d44b59ab34c4`
- PSP load base selected for initial profile: `0x08804000`
- Import inventory: 320 function imports in 32 library records.

The complete NID-by-NID list must be regenerated from the decrypted ULUS10563 EBOOT and committed as a plain-text audit artifact (no executable bytes). Counts above are a starting inventory, not a claim that the functions are implemented.

## Current source audit (2026-11-12)

The first shared-framework audit found that PortableKit already registers the four standard
`sceMd5` imports in `host/hle/hle_system.cpp`:

| NID | Function | Owner |
|---|---|---|
| `0x19884A15` | `sceMd5BlockInit` | PortableKit |
| `0xA30206C2` | `sceMd5BlockUpdate` | PortableKit |
| `0x4876AFFF` | `sceMd5BlockResult` | PortableKit |
| `0x98E31A9E` | `sceMd5Digest` | PortableKit |

Sekhmet previously compiled a second implementation of these same functions from
`hle/geburst_crypto`. That duplicate extension has been removed from the Sekhmet
build: the framework implementation is now the single owner. This is not a claim
that the four calls have been exercised by ULUS10563 at runtime.

PortableKit's existing ThreadMan implementation also covers many kernel facilities
(thread lifecycle, waits, semaphores, event flags, mutexes, callbacks, timers,
mailboxes, VPL/FPL and message pipes). Presence in source is not enough to mark
the corresponding ULUS10563 NIDs verified; the exact import table and runtime
call trace are still required.

## Next audit gate

Before adding more HLE, produce the decrypted ULUS10563 import inventory as
`library,NID,name` rows and compare each row with PortableKit's actual HLE
registration. For each missing or partial import, record the first call site,
argument registers, guest memory inputs/outputs, expected PSP error behavior,
and whether the function is reached during boot. Use PPSSPP as a behavioral
reference, not as a source to copy blindly.

Do not use the God Eater 2 / NPJH50832 import inventory for this target.


## Runtime trace: first Android boot (log.txt, 2026-11-12)

Observed facts from the submitted run:

- Vulkan initializes on Adreno 650 at a 960x544 guest target; audio opens at 44.1 kHz stereo.
- The executable hash matches the documented decrypted ULUS10563 ELF.
- UMD reaches MEDIA_IN, READY and READABLE; the guest opens
  `disc0:/PSP_GAME/USRDIR/package.rdp`.
- Stock HLE metadata modules `libfont.prx`, `libdeflt.prx`, `libccc.prx`,
  and `libmd5.prx` are registered, but their PRX code is not mapped.
- The HLE extension reports four unused MD5 functions because PortableKit
  already implements them. This is expected duplicate suppression, not a crash.
- The first logged HLE stub is `UtilsForUser::sceKernelGetGPI`.
- The next blocking unknown import is
  `ThreadManForUser::0x6652B8CA`, with
  `a0=0x00000BB8`, `a1=0x08830998`, `a2=0x08A89A5C`,
  `a3=0x08A89B44`, `ra=0x08831F84`.
- PSP NID status identifies `0x6652B8CA` as `sceKernelSetAlarm`.
  The exact call semantics and argument interpretation still need validation
  against PSP documentation / a known implementation before adding a handler.
- Immediately after that stub, the game reports
  `assert : src/fw/frame.c 826`.
- Later the interpreter stops on word `0x0000000D` at
  `0x088102CC`, decoded as a MIPS/Allegrex `break` instruction. Since the
  guest has just reported an assertion, this may be the game's deliberate
  assertion trap. Do not silently skip it; capture the exception code and
  guest call stack / PC context first.
- The run records 61 distinct ignored GE command opcodes. The displayed
  `first=0x0` for every opcode is insufficient to infer their intended
  parameters; inspect the GE command capture path and raw command words.

The font warning (no TrueType font) explains blank game text but is not the
reported runtime stop. The missing texture-pack folder is informational.

### Immediate next diagnostics

1. Resolve `sceKernelSetAlarm` with the exact PSP ABI and timer/callback
   behavior; add a real implementation, not a success stub.
2. Capture the guest backtrace and the code around `0x08831F84`,
   `0x088102CC`, and the assertion site.
3. Decode and retain raw GE command words, not only opcode counts.
4. Fix Allegrex `break` by implementing the correct guest exception path;
   do not convert it to a NOP.
5. Provide a valid TTF/OTF font to distinguish UI text rendering from the
   HLE/runtime failure.
