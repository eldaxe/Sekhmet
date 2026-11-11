# God Eater Burst HLE implementation update

Target: God Eater Burst PSP US, `ULUS10563`.
PortableKit branch: `god-eater-burst`.
PortableKit commit: `57b7fb13eb5712518651100d0f679cd2da56c48d`.

## Current static import coverage

The original EBOOT inventory contains 320 unique imports in 32 libraries. The latest source scan, including literal-name registration loops and local `configs/nids.csv` mappings, finds:

| Status | Count |
|---|---:|
| Matching HLE registrations | 304 |
| Imports without a matching registration | 16 |
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

The runtime now retains initial semaphore/mutex counts so those status structures can report them. Mailbox packet links are still host-side; `packetListHead` is written as zero and is not a complete PSP guest linked-list emulation.

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
