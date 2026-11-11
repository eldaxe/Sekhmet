# God Eater Burst HLE compatibility

Target: US PSP release `ULUS10563`, `APP_VER 01.00`.

The exact 320-import static source-registration comparison is now committed in [GOD_EATER_BURST_HLE_IMPORT_AUDIT.md](GOD_EATER_BURST_HLE_IMPORT_AUDIT.md). On PortableKit `14f5fa2`, 277 imports have matching HLE registrations, 39 resolved imports have no matching registration, and 4 NIDs remain unresolved. These are source matches, not runtime-verified behavior.

## Import inventory from EBOOT

| Library | Imports | Registered | Missing | Unknown |
|---|---:|---:|---:|---:|
| ThreadManForUser | 58 | 49 | 9 | 0 |
| sceSasCore | 30 | 30 | 0 | 0 |
| sceMpeg | 24 | 24 | 0 | 0 |
| sceUtility | 15 | 15 | 0 | 0 |
| IoFileMgrForUser | 14 | 14 | 0 | 0 |
| sceRtc | 13 | 10 | 3 | 0 |
| sceNetAdhocctl | 13 | 11 | 2 | 0 |
| SysMemUserForUser | 12 | 8 | 1 | 3 |
| sceJpeg | 12 | 1 | 11 | 0 |
| sceGe_user | 11 | 11 | 0 | 0 |
| sceAudio | 11 | 11 | 0 | 0 |
| sceLibFont | 11 | 11 | 0 | 0 |
| sceAtrac3plus | 6 | 6 | 0 | 0 |
| sceNetAdhocDiscover | 6 | 6 | 0 | 0 |
| sceNetAdhoc | 6 | 6 | 0 | 0 |
| sceUmdUser | 9 | 6 | 3 | 0 |
| scePower | 9 | 4 | 4 | 1 |
| Other 15 libraries | 46 | 46 | 0 | 0 |

See the linked audit for the complete 320-row inventory, exact NIDs, handler locations, and the list of 39 missing registrations plus 4 unresolved NIDs.

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


## Follow-up implementation status (2026-11-12)

PortableKit's `god-eater-burst` branch now has an initial PPSSPP-referenced
implementation of `sceKernelSetAlarm`, `sceKernelSetSysClockAlarm`,
`sceKernelCancelAlarm`, and `sceKernelReferAlarmStatus`. The alarm handlers
run through the guest interrupt-call path, with virtual microsecond deadlines
and repeat/stop behavior from the handler return value. This code has been
committed but is not yet build- or device-validated.

Ignored GE opcode diagnostics now retain up to four data values per opcode.
Allegrex BREAK reports its encoded exception code but still stops: the PSP
exception-vector/CP0 path has not been implemented. These are diagnostic and
initial HLE changes, not a claim that the reported game assertion is fixed.


## Current source audit update (2026-11-12)

- PortableKit `god-eater-burst` commit `14f5fa2736423767c5eb845492101ad269f1ef47` now has matching registrations for all 30 imported `sceSasCore` functions and all 4 imported `sceMd5` functions.
- The 3 previously missing SAS ATRAC3 entry points now have registrations. Reverb calls remain success-returning no-op shims.
- `sceJpeg` still has only `sceJpegCsc` registered; 11 MJPEG imports are missing.
- Sekhmet's build no longer compiles `hle/geburst_crypto`, avoiding a second MD5 implementation alongside PortableKit's built-in handlers. The old extension source remains in the repository but is not part of the build.
- This is a static audit only. Build, device boot, and per-function behavior still require validation.
