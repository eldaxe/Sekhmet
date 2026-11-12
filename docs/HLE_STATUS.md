# God Eater Burst HLE compatibility

Target: God Eater Burst PSP US, `ULUS10563`, `APP_VER 01.00`.

PortableKit source: branch `god-eater-burst`, commit `72d83d0b5e6a3ebf65ea7843663361d75ef8018b`.

## Current static import coverage

The original EBOOT import inventory contains 320 unique imports in 32 libraries.

| Library | Imports | Registered | Missing |
|---|---:|---:|---:|
| ThreadManForUser | 58 | 56 | 2 |
| sceJpeg | 12 | 1 | 11 |
| sceNetAdhocctl | 13 | 13 | 0 |
| UtilsForUser | 10 | 10 | 0 |
| Other libraries | 227 | 227 | 0 |
| **Total** | **320** | **307** | **13** |

The six UtilsForUser functions implemented by a multiline literal-name loop are included in the registered count. This is a static registration audit, not proof of semantic completeness or successful execution on hardware.

## Remaining import gaps

- `sceJpeg`: 11 MJPEG lifecycle/decode functions. Only `sceJpegCsc` is registered.
- `ThreadManForUser`:
  - `sceKernelReferThreadEventHandlerStatus` — no thread-event-handler object registry exists.
  - `sceKernelReferTlsplStatus` — TLS pool objects are not modelled.

`sceKernelGetThreadmanIdList` is now registered and enumerates the object registries currently modelled by PortableKit. Thread-event-handler type 9 returns an empty list until that object type is implemented.

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
