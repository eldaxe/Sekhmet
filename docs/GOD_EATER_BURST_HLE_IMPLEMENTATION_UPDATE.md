# God Eater Burst HLE implementation update

Target: God Eater Burst PSP US, `ULUS10563`.
PortableKit branch: `god-eater-burst`.
PortableKit commit: `b2c0a039edd01de25c804eb7b1b27ee79eefba13`.

## Current static import coverage

The original EBOOT inventory contains 320 unique imports in 32 libraries. The latest static source scan, including literal-name registration loops and local `configs/nids.csv` mappings, finds 307 matching registrations and 13 unregistered imports. The six `UtilsForUser` imports registered through a multiline literal-name loop are counted as implemented.

| Status | Count |
|---|---:|
| Matching HLE registrations | 307 |
| Imports without a matching registration | 13 |
| Total | 320 |

This is a static source-registration result, not a runtime or semantic completeness claim. Five function-name mappings are supplied by the local NID configuration but are not exact name matches in the external PPSSPP NID database; the local import config is retained for those names.

## Implemented in this pass

### UMD (`sceUmdUser`)
- `sceUmdGetDiscInfo`: validates the 8-byte info structure and reports a game disc when an ISO is mounted.
- `sceUmdReplaceProhibit` and `sceUmdReplacePermit`: no-op success because the runtime mounts one immutable image.

### Power (`scePower`)
- `scePowerGetBatteryLifePercent`: reports 100%.
- `scePowerGetBatteryLifeTime`: reports 0 (AC-powered convention).
- `scePowerRequestSuspend`: no-op success; must not suspend the Android host.
- Other imported Power calls already had matching registrations.

### RTC (`sceRtc`)
- `sceRtcConvertUtcToLocalTime`: converts a PSP UTC tick using the host timezone.
- `sceRtcFormatRFC3339LocalTime`: writes a local RFC3339 timestamp with UTC offset.
- `sceRtcSetAlarmTick`: no-op success, consistent with the existing unmodelled RTC alarm behavior.
- Fixed a local/UTC `tm` copy-order bug in the timezone offset helper.

### ThreadManForUser
Added fixed-size, size-bounded status output for:
- `sceKernelReferSemaStatus`
- `sceKernelReferMutexStatus`
- `sceKernelReferLwMutexStatusByID`
- `sceKernelReferMbxStatus`
- `sceKernelReferCallbackStatus`
- `sceKernelReferVTimerStatus`

The runtime now retains initial semaphore/mutex counts so those status structures can report them. The lightweight-mutex status handler writes the distinct 64-byte PSP `SceKernelLwMutexInfo` layout, including UID and work-area pointer. Mailbox packet links are still host-side; `packetListHead` is written as zero and is not a complete PSP guest linked-list emulation.

### SysMemUserForUser
- Added the `sceKernelSetCompiledSdkVersion600_602` alias using the existing SDK-version setter behavior.

### UtilsForUser
The imported cache/GPO functions and `sceKernelGetGPI` were already registered through a literal-name loop / explicit registration. The earlier per-NID audit incorrectly classified the loop registrations as missing; this pass corrects that audit result. Cache operations are host no-ops because the host has no PSP data cache.

## Remaining unregistered imports (16)

| Library | Missing imports |
|---|---:|
| `sceJpeg` | 11 |
| `sceNetAdhocctl` | 2 |
| `ThreadManForUser` | 3 |
| **Total** | **16** |

The remaining ThreadMan imports are:
- `sceKernelReferThreadEventHandlerStatus`
- `sceKernelReferTlsplStatus`
- `sceKernelGetThreadmanIdList`

The two remaining `sceNetAdhocctl` imports are:
- `sceNetAdhocctlGetState`
- `sceNetAdhocctlGetParameter`

The 11 `sceJpeg` imports are the MJPEG lifecycle/decode functions. Only `sceJpegCsc` is registered. Implement MJPEG only after a runtime trace confirms the game calls these APIs.

## Sekhmet integration

Sekhmet's build no longer compiles `hle/geburst_crypto`; PortableKit owns the four `sceMd5` handlers. The old extension source remains in the repository but is not part of the build. The Sekhmet submodule pointer is updated to the PortableKit commit listed above.

## Validation status

- Source changes and NID registration updates were committed to GitHub.
- No local Android/Termux build was performed.
- No device boot or runtime test was performed.
- Status structure layouts were implemented from documented PSP layouts and cross-checked against PPSSPP; the new handlers still need runtime tests with God Eater Burst.


## Follow-up implementation (PortableKit 72d83d0)

### ThreadManForUser
- Added `sceKernelGetThreadmanIdList` (NID `0x94416130`).
- Supports ID enumeration for threads, semaphores, event flags, mailboxes, VPLs, FPLs, message pipes, callbacks, alarms, and VTimers.
- Supports the sleep, delay, suspended, and dormant thread categories.
- Checks buffer capacity, output pointers, and invalid type values.
- Thread-event-handler enumeration (type 9) currently returns an empty list because the runtime has no thread-event-handler registry.
- `sceKernelReferThreadEventHandlerStatus` and `sceKernelReferTlsplStatus` remain unregistered; thread-event handler and TLS pool objects are not modelled yet.

### sceSasCore
- Removed duplicate registrations of `__sceSasGetAllEnvelopeHeights` and `__sceSasGetPauseFlag`.
- Kept the implementation that returns the live per-voice envelope values and pause bitmask.
- Added complete guest output-range validation before writing all 32 envelope values.

## Updated remaining unregistered imports

| Library | Missing |
|---|---:|
| `sceJpeg` | 11 |
| `ThreadManForUser` | 1 |
| **Total** | **12** |

ThreadMan gap: `sceKernelReferTlsplStatus`.

## Validation
- GitHub source and NID mapping changes committed.
- Static source inspection confirms one registration each for the two SAS query handlers.
- No native build or device test was run; runtime behavior remains unverified.


## Thread event handler follow-up (PortableKit 7443b43, lifecycle dispatch 4b903bc)

- Added a runtime registry for thread event handler metadata.
- Added `sceKernelRegisterThreadEventHandler` (NID `0x0C106E53`), `sceKernelReleaseThreadEventHandler` (NID `0x72F3C145`), and `sceKernelReferThreadEventHandlerStatus` (NID `0x369EEB6B`).
- Status output follows the PSP SDK structure: size, name[32], thread ID, mask, handler address, common argument; output writes are bounded by both the caller-supplied size and guest-memory range.
- Registration accepts the PSP thread selectors CURRENT, ALL_USER, ALL_KERNEL and ALL; CURRENT remains restricted to EXIT events. Zero event mask is accepted.
- `sceKernelGetThreadmanIdList` type 9 now enumerates registered thread-event handler UIDs.
- Important limitation: guest callbacks are now dispatched for CREATE, START, EXIT and DELETE. Handlers run in registration order and receive `(eventMask, threadId, commonArg)`. CURRENT is bound to the registering thread and remains EXIT-only. ALL_USER and ALL cover modeled user threads; ALL_KERNEL is accepted but has no kernel-thread objects to match. This remains source-reviewed, not runtime-verified; callback reentrancy and in-flight handler release still need tests.
- `sceKernelReferTlsplStatus` remains unimplemented; a correct TLS pool requires allocation ownership, per-thread cleanup, wait queues and object status, not a fabricated status response.


## TLS pool and callback reentrancy follow-up (PortableKit d8136f4)

PortableKit now models TLS pool lifecycle and `sceKernelReferTlsplStatus`, including per-thread allocation ownership, zeroing on allocate/free, thread-exit cleanup, FIFO wait queue behavior, the 60-byte status structure, and ThreadMan ID-list type 12. `sceKernelGetTlsAddr` is registered in `Kernel_Library`. Error codes are aligned with PPSSPP's PSP kernel definitions.

Thread-event callback dispatch now snapshots UIDs and checks that each handler still exists before invoking it. This prevents a callback from causing a later released handler to run. The guest-call stack remains per thread so a callback can block and resume or make a nested guest call. This is not yet runtime-tested.

The 11 MJPEG imports remain outstanding. Do not treat their registration or decoding as implemented until the FFmpeg/JPEG decode and YCbCr paths are wired and tested.

Validation: source review only. No native NDK build or device test has been completed.


## MJPEG HLE update (PortableKit d20be3b)

PortableKit now has an FFmpeg MJPEG decoder and registers the remaining 11 sceJpeg MJPEG APIs, alongside the existing CSC entry point. The decoder converts supported planar JPEG output to the runtime's YCbCr 4:2:0 picture layout, and the HLE implements RGB decode, YCbCr decode, output-info query, CSC, and MJPEG init/create/delete/finish state transitions. CSC now copies/crops the source planes rather than changing dimensions over the original un-cropped buffers.

This update closes the static import registration audit (320/320 names registered), but is not yet runtime-verified. Native build, tests using actual JPEG samples, God Eater Burst movie playback, and Android device validation are still required. Do not interpret the static count as proof of exact PSP behavior.
