# God Eater Burst HLE compatibility

Target: God Eater Burst PSP US, `ULUS10563`, `APP_VER 01.00`.

PortableKit source: branch `god-eater-burst`, commit `5030370987270d2bc9ac0c96936b3d0d40c12d31`.

## Current static import coverage

The original EBOOT import inventory contains 320 unique imports in 32 libraries.

| Library | Imports | Registered | Missing |
|---|---:|---:|---:|
| ThreadManForUser | 58 | 58 | 0 |
| sceJpeg | 12 | 12 | 0 |
| sceNetAdhocctl | 13 | 13 | 0 |
| UtilsForUser | 10 | 10 | 0 |
| Other libraries | 227 | 227 | 0 |
| **Total** | **320** | **320** | **0** |

The six UtilsForUser functions implemented by a multiline literal-name loop are included in the registered count. This is a static registration audit, not proof of semantic completeness or successful execution on hardware.

## Remaining import gaps

- No missing imports in the current static registration audit. JPEG decoder output, color conversion, TLS wait behavior, and callback reentrancy still require runtime tests.

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


## MJPEG implementation follow-up (PortableKit d20be3b)

- Added a host MJPEG decoder using FFmpeg's `AV_CODEC_ID_MJPEG`, enabled in the bundled FFmpeg decoder configuration.
- Registered the remaining 11 sceJpeg imports: MJPEG init/create/delete/finish, RGB decode and successive decode, YCbCr decode and successive decode, output-info query, MJPEG CSC and CSC with color option. The existing `sceJpegCsc` now shares the picture conversion path.
- Decoder output is normalized to planar YCbCr 4:2:0, including 4:2:2/4:4:4 source chroma subsampling and grayscale input. Guest output buffers are range-checked; MJPEG CSC crops luma and chroma planes to the requested dimensions.
- This closes the static import-registration gap only. Decoder format support, API edge cases, guest buffer layout, output color ordering, timing, and PSP partition-to-host memory mapping remain unverified on PSP game data and Android. Native build/CI must pass before treating this as complete.


## TLS partition validation correction (PortableKit 59b38ba)

TLS pool creation now accepts PSP partition IDs 1 through 6 instead of requiring one hard-coded partition ID. PortableKit currently maps the PSP high-memory attribute to its host high/low allocator; it does not yet emulate all six distinct PSP partition allocators. This compatibility limitation is explicit and requires game/runtime validation.


## Android build correction (PortableKit 57cbc94)

The first Sekhmet build using the callback/TLS changes exposed a missing closing brace after `register_msg_pipes()`, which caused the following ID-list registration function to be parsed as nested. This has been corrected. The updated PortableKit submodule is pinned below; a new Android build is required to verify this fix and compile the MJPEG implementation.

TLS pool validation additionally accepts the PSP priority attribute (`0x0100`) together with high-memory (`0x4000`), matching the known PSP attribute mask `0x4100`. Priority waiter ordering is still FIFO in the current host implementation.

MJPEG decoder initialization now cleans up partially allocated FFmpeg objects on failure, configures the codec before opening it, and rejects packed RGB pixel formats rather than misinterpreting their planes as YCbCr. The next Android CI run should compile this decoder path.

TLS pools with the PSP priority attribute now order waiting threads by their current kernel thread priority (stable FIFO for equal priorities); FIFO remains the default when the priority attribute is absent.

MJPEG CSC now enforces the MJPEG initialization state, supports the documented negative buffer-width encoding used by the MJPEG CSC path, and validates the known `sceJpegCsc` chroma subsampling values.
