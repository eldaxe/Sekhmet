# God Eater Burst HLE compatibility

Target: God Eater Burst PSP US, `ULUS10563`, `APP_VER 01.00`.

PortableKit source: branch `god-eater-burst`, commit `d8136f437bd71c47572011c5c54d8355696011df`.

## Current static import coverage

The original EBOOT import inventory contains 320 unique imports in 32 libraries.

| Library | Imports | Registered | Missing |
|---|---:|---:|---:|
| ThreadManForUser | 58 | 58 | 0 |
| sceJpeg | 12 | 1 | 11 |
| sceNetAdhocctl | 13 | 13 | 0 |
| UtilsForUser | 10 | 10 | 0 |
| Other libraries | 227 | 227 | 0 |
| **Total** | **320** | **309** | **11** |

The six UtilsForUser functions implemented by a multiline literal-name loop are included in the registered count. This is a static registration audit, not proof of semantic completeness or successful execution on hardware.

## Remaining import gaps

- `sceJpeg`: 11 MJPEG lifecycle/decode functions. Only `sceJpegCsc` is registered.
- `ThreadManForUser`: no remaining missing imports in the current static registration audit. TLS pool semantics and callback reentrancy still require runtime tests.

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

The remaining documented import gaps are the 11 `sceJpeg` MJPEG APIs. TLS pool creation, deletion, per-thread allocation/free, status reporting and ID-list type 12 are now registered. Thread-event dispatch now skips handlers released by an earlier handler in the same event. Both areas are source-reviewed only; no native build or Android device test has been run.


## TLS pool and callback lifecycle follow-up (PortableKit d8136f4)

- Added TLS pool object tracking and handlers for `sceKernelCreateTlspl`, `sceKernelDeleteTlspl`, `sceKernelFreeTlspl`, and `sceKernelReferTlsplStatus`.
- Added `Kernel_Library::sceKernelGetTlsAddr`, per-thread slot ownership, zero-on-allocation/free, thread-exit cleanup, FIFO wait handling when the pool is full, and ThreadMan ID-list type 12.
- TLS status uses the 60-byte `SceKernelTlsplInfo` layout and reports name, attributes, index, block size, total/free blocks, and waiting threads.
- Corrected TLS error values to the PSP SDK values: UNKNOWN_TLSPL_ID `0x800201D0`, TOO_MANY_TLSPL `0x800201D1`, TLSPL_IN_USE `0x800201D2`.
- Callback dispatch snapshots handler UIDs and re-checks registration before invocation, so a handler released during a preceding callback is skipped. An executing handler may return normally.
- Still unverified: blocking/nested callback execution, TLS wait contention/timeouts, and Android device behavior. No native build has been completed for this update.
