# God Eater Burst HLE compatibility

Target: God Eater Burst PSP US, `ULUS10563`, `APP_VER 01.00`.

PortableKit source: branch `god-eater-burst`, commit `b2c0a039edd01de25c804eb7b1b27ee79eefba13`.

## Current static import coverage

The original EBOOT import inventory contains 320 unique imports in 32 libraries.

| Library | Imports | Registered | Missing |
|---|---:|---:|---:|
| ThreadManForUser | 58 | 57 | 1 |
| sceJpeg | 12 | 1 | 11 |
| sceNetAdhocctl | 13 | 13 | 0 |
| UtilsForUser | 10 | 10 | 0 |
| Other libraries | 227 | 227 | 0 |
| **Total** | **320** | **308** | **12** |

The six UtilsForUser functions implemented by a multiline literal-name loop are included in the registered count. This is a static registration audit, not proof of semantic completeness or successful execution on hardware.

## Remaining import gaps

- `sceJpeg`: 11 MJPEG lifecycle/decode functions. Only `sceJpegCsc` is registered.
- `ThreadManForUser`:
  - Thread-event handler callback delivery is not yet wired into thread lifecycle; registration, release, status and ID enumeration metadata are implemented.
  - `sceKernelReferTlsplStatus` — TLS pool objects are not modelled.

`sceKernelGetThreadmanIdList` is now registered and enumerates the object registries currently modelled by PortableKit. Thread-event-handler type 9 enumerates the registered handler objects.

## Recent implementation

- Added `sceNetAdhocctlGetState` and `sceNetAdhocctlGetParameter`.
- Added `sceKernelGetThreadmanIdList`.
- Removed duplicate `__sceSasGetAllEnvelopeHeights` and `__sceSasGetPauseFlag` registrations. The remaining SAS query handlers return live core state, and envelope output is range-checked.
- Existing ThreadMan alarms, synchronization objects, callback status and lightweight mutex status remain implemented.

## Known partial implementations

- SAS reverb operations are accepted but reverb is not modelled.
- MPEG/PSMF and font implementations require game-level validation.
- MJPEG APIs are not implemented; add them when runtime traces show they are needed.
- PRX metadata-only modules do not automatically execute their `module_start` routines.

## Validation status

- GitHub commits and submodule pointer are maintained in the Sekhmet repository.
- No native Android/NDK build or on-device test has been completed for the latest changes.
- Treat all new HLE handlers as source-reviewed, not runtime-verified.

See [GOD_EATER_BURST_HLE_IMPLEMENTATION_UPDATE.md](GOD_EATER_BURST_HLE_IMPLEMENTATION_UPDATE.md) for implementation details.


## Latest thread-event handler update

PortableKit now models the thread-event handler object registry and the register, release and status-query APIs. The status structure matches the PSP SDK layout. PortableKit now dispatches guest callbacks for CREATE, START, EXIT and DELETE. CURRENT is bound to the registering thread; ALL_USER and ALL are supported for modeled user threads. ALL_KERNEL is accepted but cannot match because this runtime does not model kernel threads.

The remaining documented import gaps are 11 `sceJpeg` MJPEG APIs and `sceKernelReferTlsplStatus`. Lifecycle dispatch is source-reviewed only; no native build or Android device test has been run.
