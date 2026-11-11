# God Eater Burst (ULUS10563) — PortableKit HLE import audit

## Scope and method

- Executable: decrypted original `EBOOT.BIN`, ULUS10563 / APP_VER 01.00.
- Import source: `ge_burst_imports.csv`, reconstructed from `.lib.stub` and NID tables; 320 imports, 32 libraries.
- Framework: `eldaxe/PortableKit`, branch `god-eater-burst`, commit `14f5fa2736423767c5eb845492101ad269f1ef47`.
- Checked all `host/hle/*.cpp` files in that commit for explicit `hle.add()` / `hle.try_add()` registrations and literal-name lists registered through a loop.
- NID names resolved against `pspdev/psp-ghidra-scripts/ppsspp_niddb.xml`; source registration matched by exact `(library, function name)`.
- This is a static source-registration audit, not a runtime test. `REGISTERED` does not mean behavior is complete or verified.

## Result

| Metric | Count |
|---|---:|
| EBOOT imports | 320 |
| Unique (library, NID) pairs | [object Set] |
| Names resolved | 316 |
| Names unresolved | 4 |
| Matching registrations | 277 |
| Resolved names without registration | 39 |

Interpretation: 277 imports have a matching registration, 39 resolved imports have no matching registration, and 4 imports remain unresolved by the NID database. The 277 registrations include `try_add` entries and shims; they are not a claim of full PSP behavior.

## Per-library summary

| Library | Imports | Registered | Missing | Unknown |
|---|---:|---:|---:|---:|
| `sceCtrl` | 3 | 3 | 0 | 0 |
| `sceUmdUser` | 9 | 6 | 3 | 0 |
| `scePower` | 9 | 4 | 4 | 1 |
| `sceImpose` | 2 | 2 | 0 | 0 |
| `sceDisplay` | 3 | 3 | 0 | 0 |
| `sceGe_user` | 11 | 11 | 0 | 0 |
| `sceAudio` | 11 | 11 | 0 | 0 |
| `sceSasCore` | 30 | 30 | 0 | 0 |
| `sceAtrac3plus` | 6 | 6 | 0 | 0 |
| `sceJpeg` | 12 | 1 | 11 | 0 |
| `sceMd5` | 4 | 4 | 0 | 0 |
| `sceLibFont` | 11 | 11 | 0 | 0 |
| `sceUtility` | 15 | 15 | 0 | 0 |
| `sceMpeg` | 24 | 24 | 0 | 0 |
| `sceRtc` | 13 | 10 | 3 | 0 |
| `sceWlanDrv` | 2 | 2 | 0 | 0 |
| `sceOpenPSID` | 1 | 1 | 0 | 0 |
| `sceNetAdhocDiscover` | 6 | 6 | 0 | 0 |
| `sceNetAdhocctl` | 13 | 11 | 2 | 0 |
| `sceNetAdhoc` | 6 | 6 | 0 | 0 |
| `sceNet` | 2 | 2 | 0 | 0 |
| `scePspNpDrm_user` | 4 | 4 | 0 | 0 |
| `InterruptManager` | 3 | 3 | 0 | 0 |
| `IoFileMgrForUser` | 14 | 14 | 0 | 0 |
| `Kernel_Library` | 8 | 8 | 0 | 0 |
| `LoadExecForUser` | 2 | 2 | 0 | 0 |
| `ModuleMgrForUser` | 10 | 10 | 0 | 0 |
| `StdioForUser` | 3 | 3 | 0 | 0 |
| `SysMemUserForUser` | 12 | 8 | 1 | 3 |
| `ThreadManForUser` | 58 | 49 | 9 | 0 |
| `UtilsForUser` | 10 | 4 | 6 | 0 |
| `sceSuspendForUser` | 3 | 3 | 0 | 0 |

## Missing registrations

| Index | Library | NID | Function | Stub address |
|---:|---|---|---|---|
| 3 | `sceUmdUser` | `0x340B7686` | `sceUmdGetDiscInfo` | `0x0020B8D0` |
| 7 | `sceUmdUser` | `0x87533940` | `sceUmdReplaceProhibit` | `0x0020B8F0` |
| 11 | `sceUmdUser` | `0xCBE9F02A` | `sceUmdReplacePermit` | `0x0020B910` |
| 13 | `scePower` | `0x2085D15D` | `scePowerGetBatteryLifePercent` | `0x0020B700` |
| 16 | `scePower` | `0x8EFB3FA2` | `scePowerGetBatteryLifeTime` | `0x0020B718` |
| 17 | `scePower` | `0xA85880D0` | `scePower_a85880d0_IsPSPNonFat` | `0x0020B720` |
| 18 | `scePower` | `0xAC32C9CC` | `scePowerRequestSuspend` | `0x0020B728` |
| 84 | `sceJpeg` | `0x04B5AE02` | `sceJpegMJpegCsc` | `0x0020B480` |
| 85 | `sceJpeg` | `0x04B93CEF` | `sceJpegDecodeMJpeg` | `0x0020B488` |
| 86 | `sceJpeg` | `0x227662D7` | `sceJpegDecodeMJpegYCbCrSuccessively` | `0x0020B490` |
| 87 | `sceJpeg` | `0x48B602B7` | `sceJpegDeleteMJpeg` | `0x0020B498` |
| 88 | `sceJpeg` | `0x64B6F978` | `sceJpegDecodeMJpegSuccessively` | `0x0020B4A0` |
| 90 | `sceJpeg` | `0x7D2F3D7F` | `sceJpegFinishMJpeg` | `0x0020B4B0` |
| 91 | `sceJpeg` | `0x8F2BB012` | `sceJpegGetOutputInfo` | `0x0020B4B8` |
| 92 | `sceJpeg` | `0x91EED83C` | `sceJpegDecodeMJpegYCbCr` | `0x0020B4C0` |
| 93 | `sceJpeg` | `0x9D47469C` | `sceJpegCreateMJpeg` | `0x0020B4C8` |
| 94 | `sceJpeg` | `0xA06A75C4` | `sceJpegMJpegCscWithColorOption` | `0x0020B4D0` |
| 95 | `sceJpeg` | `0xAC9E70E6` | `sceJpegInitMJpeg` | `0x0020B4D8` |
| 152 | `sceRtc` | `0x27F98543` | `sceRtcFormatRFC3339LocalTime` | `0x0020B770` |
| 153 | `sceRtc` | `0x34885E0D` | `sceRtcConvertUtcToLocalTime` | `0x0020B778` |
| 156 | `sceRtc` | `0x7D1FBED3` | `sceRtcSetAlarmTick` | `0x0020B790` |
| 178 | `sceNetAdhocctl` | `0x75ECD386` | `sceNetAdhocctlGetState` | `0x0020B6B8` |
| 181 | `sceNetAdhocctl` | `0xDED9D28E` | `sceNetAdhocctlGetParameter` | `0x0020B6D0` |
| 243 | `SysMemUserForUser` | `0x35669D4C` | `sceKernelSetCompiledSdkVersion600_602` | `0x0020B110` |
| 268 | `ThreadManForUser` | `0x369EEB6B` | `sceKernelReferThreadEventHandlerStatus` | `0x0020B1D8` |
| 275 | `ThreadManForUser` | `0x4C145944` | `sceKernelReferLwMutexStatusByID` | `0x0020B210` |
| 278 | `ThreadManForUser` | `0x5F32BEAA` | `sceKernelReferVTimerStatus` | `0x0020B228` |
| 285 | `ThreadManForUser` | `0x721067F3` | `sceKernelReferTlsplStatus` | `0x0020B260` |
| 286 | `ThreadManForUser` | `0x730ED8BC` | `sceKernelReferCallbackStatus` | `0x0020B268` |
| 295 | `ThreadManForUser` | `0x94416130` | `sceKernelGetThreadmanIdList` | `0x0020B2B0` |
| 300 | `ThreadManForUser` | `0xA8E8C846` | `sceKernelReferMbxStatus` | `0x0020B2D8` |
| 301 | `ThreadManForUser` | `0xA9C2CB9A` | `sceKernelReferMutexStatus` | `0x0020B2E0` |
| 306 | `ThreadManForUser` | `0xBC6FEBC5` | `sceKernelReferSemaStatus` | `0x0020B308` |
| 307 | `UtilsForUser` | `0x6AD345D7` | `sceKernelSetGPO` | `0x0020B310` |
| 308 | `UtilsForUser` | `0x79D1C3FA` | `sceKernelDcacheWritebackAll` | `0x0020B318` |
| 309 | `UtilsForUser` | `0xB435DEC5` | `sceKernelDcacheWritebackInvalidateAll` | `0x0020B320` |
| 310 | `UtilsForUser` | `0xBFA98062` | `sceKernelDcacheInvalidateRange` | `0x0020B328` |
| 311 | `UtilsForUser` | `0x34B9FA9E` | `sceKernelDcacheWritebackInvalidateRange` | `0x0020B330` |
| 313 | `UtilsForUser` | `0x3EE30821` | `sceKernelDcacheWritebackRange` | `0x0020B340` |

## Unresolved NIDs

These four imports are not named by the NID database used in this pass. No function names are guessed.

| Index | Library | NID | Stub address |
|---:|---|---|---|
| 15 | `scePower` | `0x469989AD` | `0x0020B710` |
| 238 | `SysMemUserForUser` | `0xDB83A952` | `0x0020B0E8` |
| 241 | `SysMemUserForUser` | `0xFE707FDF` | `0x0020B100` |
| 244 | `SysMemUserForUser` | `0x50F61D8A` | `0x0020B118` |

## Full 320-import inventory and source registration

`REGISTERED` means a matching registration was found. The source location is the first matching registration discovered by the audit. `try_add` means conditional registration. `MISSING` and `UNKNOWN NID` are explained above.

| Index | Library | NID | Function | Status | Registration source |
|---:|---|---|---|---|---|
| 0 | `sceCtrl` | `0x1F4011E6` | `sceCtrlSetSamplingMode` | **REGISTERED** | `host/hle/hle_media.cpp:578` |
| 1 | `sceCtrl` | `0x1F803938` | `sceCtrlReadBufferPositive` | **REGISTERED** | `host/hle/hle_media.cpp:594` |
| 2 | `sceCtrl` | `0x6A2774F3` | `sceCtrlSetSamplingCycle` | **REGISTERED** | `host/hle/hle_media.cpp:573` |
| 3 | `sceUmdUser` | `0x340B7686` | `sceUmdGetDiscInfo` | **MISSING** | — |
| 4 | `sceUmdUser` | `0x46EBB729` | `sceUmdCheckMedium` | **REGISTERED** | `host/hle/hle_io.cpp:1089` |
| 5 | `sceUmdUser` | `0x4A9E5E29` | `sceUmdWaitDriveStatCB` | **REGISTERED** | `host/hle/hle_io.cpp:1095` |
| 6 | `sceUmdUser` | `0x6B4A146C` | `sceUmdGetDriveStat` | **REGISTERED** | `host/hle/hle_io.cpp:1059` |
| 7 | `sceUmdUser` | `0x87533940` | `sceUmdReplaceProhibit` | **MISSING** | — |
| 8 | `sceUmdUser` | `0xAEE7404D` | `sceUmdRegisterUMDCallBack` | **REGISTERED** | `host/hle/hle_io.cpp:1067` |
| 9 | `sceUmdUser` | `0xBD2BDE07` | `sceUmdUnRegisterUMDCallBack` | **REGISTERED** | `host/hle/hle_io.cpp:1077` |
| 10 | `sceUmdUser` | `0xC6183D47` | `sceUmdActivate` | **REGISTERED** | `host/hle/hle_io.cpp:1056` |
| 11 | `sceUmdUser` | `0xCBE9F02A` | `sceUmdReplacePermit` | **MISSING** | — |
| 12 | `scePower` | `0x04B7766E` | `scePowerRegisterCallback` | **REGISTERED** | `host/hle/hle_system.cpp:454` |
| 13 | `scePower` | `0x2085D15D` | `scePowerGetBatteryLifePercent` | **MISSING** | — |
| 14 | `scePower` | `0x34F9C463` | `scePowerGetPllClockFrequencyInt` | **REGISTERED** | `host/hle/hle_system.cpp:490` |
| 15 | `scePower` | `0x469989AD` | — | **UNKNOWN NID** | — |
| 16 | `scePower` | `0x8EFB3FA2` | `scePowerGetBatteryLifeTime` | **MISSING** | — |
| 17 | `scePower` | `0xA85880D0` | `scePower_a85880d0_IsPSPNonFat` | **MISSING** | — |
| 18 | `scePower` | `0xAC32C9CC` | `scePowerRequestSuspend` | **MISSING** | — |
| 19 | `scePower` | `0xDFA8BAF8` | `scePowerUnregisterCallback` | **REGISTERED** | `host/hle/hle_system.cpp:465` |
| 20 | `scePower` | `0xFDB5BFE9` | `scePowerGetCpuClockFrequencyInt` | **REGISTERED** | `host/hle/hle_system.cpp:484` |
| 21 | `sceImpose` | `0x36AA6E91` | `sceImposeSetLanguageMode` | **REGISTERED** | `host/hle/hle_system.cpp:592` |
| 22 | `sceImpose` | `0x72189C48` | `sceImposeSetUMDPopup` | **REGISTERED** | `host/hle/hle_system.cpp:610` |
| 23 | `sceDisplay` | `0x0E20F177` | `sceDisplaySetMode` | **REGISTERED** | `host/hle/hle_media.cpp:552` |
| 24 | `sceDisplay` | `0x289D82FE` | `sceDisplaySetFrameBuf` | **REGISTERED** | `host/hle/hle_media.cpp:559` |
| 25 | `sceDisplay` | `0x46F186C3` | `sceDisplayWaitVblankStartCB` | **REGISTERED** | `host/hle/hle_media.cpp:654` |
| 26 | `sceGe_user` | `0x03444EB4` | `sceGeListSync` | **REGISTERED** | `host/hle/hle_media.cpp:750` |
| 27 | `sceGe_user` | `0x05DB22CE` | `sceGeUnsetCallback` | **REGISTERED** | `host/hle/hle_media.cpp:723` |
| 28 | `sceGe_user` | `0x1C0D95A6` | `sceGeListEnQueueHead` | **REGISTERED** | `host/hle/hle_media.cpp:739` |
| 29 | `sceGe_user` | `0x1F6752AD` | `sceGeEdramGetSize` | **REGISTERED** | `host/hle/hle_media.cpp:713` |
| 30 | `sceGe_user` | `0x4C06E472` | `sceGeContinue` | **REGISTERED** | `host/hle/hle_media.cpp:761` |
| 31 | `sceGe_user` | `0xA4FC06A4` | `sceGeSetCallback` | **REGISTERED** | `host/hle/hle_media.cpp:715` |
| 32 | `sceGe_user` | `0xAB49E76A` | `sceGeListEnQueue` | **REGISTERED** | `host/hle/hle_media.cpp:738` |
| 33 | `sceGe_user` | `0xB287BD61` | `sceGeDrawSync` | **REGISTERED** | `host/hle/hle_media.cpp:756` |
| 34 | `sceGe_user` | `0xB448EC0D` | `sceGeBreak` | **REGISTERED** | `host/hle/hle_media.cpp:760` |
| 35 | `sceGe_user` | `0xE0D68148` | `sceGeListUpdateStallAddr` | **REGISTERED** | `host/hle/hle_media.cpp:740` |
| 36 | `sceGe_user` | `0xE47E40E4` | `sceGeEdramGetAddr` | **REGISTERED** | `host/hle/hle_media.cpp:712` |
| 37 | `sceAudio` | `0x01562BA3` | `sceAudioOutput2Reserve` | **REGISTERED** | `host/hle/hle_media.cpp:930` |
| 38 | `sceAudio` | `0x2D53F36E` | `sceAudioOutput2OutputBlocking` | **REGISTERED** | `host/hle/hle_media.cpp:968` |
| 39 | `sceAudio` | `0x136CAF51` | `sceAudioOutputBlocking` | **REGISTERED** | `host/hle/hle_media.cpp:914` |
| 40 | `sceAudio` | `0x13F592BC` | `sceAudioOutputPannedBlocking` | **REGISTERED** | `host/hle/hle_media.cpp:910` |
| 41 | `sceAudio` | `0x5EC81C55` | `sceAudioChReserve` | **REGISTERED** | `host/hle/hle_media.cpp:866` |
| 42 | `sceAudio` | `0x6FC46853` | `sceAudioChRelease` | **REGISTERED** | `host/hle/hle_media.cpp:885` |
| 43 | `sceAudio` | `0x95FD0C2D` | `sceAudioChangeChannelConfig` | **REGISTERED** | `host/hle/hle_media.cpp:893` |
| 44 | `sceAudio` | `0xB011922F` | `sceAudioGetChannelRestLength` | **REGISTERED** | `host/hle/hle_media.cpp:899` |
| 45 | `sceAudio` | `0xB7E1D8E7` | `sceAudioChangeChannelVolume` | **REGISTERED** | `host/hle/hle_media.cpp:897` |
| 46 | `sceAudio` | `0xCB2E439E` | `sceAudioSetChannelDataLen` | **REGISTERED** | `host/hle/hle_media.cpp:889` |
| 47 | `sceAudio` | `0xE2D56B2D` | `sceAudioOutputPanned` | **REGISTERED** (`try_add`) | `host/hle/hle_media.cpp:911` |
| 48 | `sceSasCore` | `0x019B25EB` | `__sceSasSetADSR` | **REGISTERED** (`try_add`) | `host/hle/hle_media.cpp:1109` |
| 49 | `sceSasCore` | `0x07F58C24` | `__sceSasGetAllEnvelopeHeights` | **REGISTERED** (`try_add`) | `host/hle/hle_media.cpp:1122` |
| 50 | `sceSasCore` | `0x267A6DD2` | `__sceSasRevParam` | **REGISTERED** | `host/hle/hle_media.cpp:1158` |
| 51 | `sceSasCore` | `0x2C8E6AB3` | `__sceSasGetPauseFlag` | **REGISTERED** (`try_add`) | `host/hle/hle_media.cpp:1137` |
| 52 | `sceSasCore` | `0x33D4AB37` | `__sceSasRevType` | **REGISTERED** | `host/hle/hle_media.cpp:1158` |
| 53 | `sceSasCore` | `0x42778A9F` | `__sceSasInit` | **REGISTERED** | `host/hle/hle_media.cpp:982` |
| 54 | `sceSasCore` | `0x440CA7D8` | `__sceSasSetVolume` | **REGISTERED** | `host/hle/hle_media.cpp:1084` |
| 55 | `sceSasCore` | `0x4AA9EAD6` | `__sceSasSetVoiceATRAC3` | **REGISTERED** | `host/hle/hle_media.cpp:988` |
| 56 | `sceSasCore` | `0x50A14DFC` | `__sceSasCoreWithMix` | **REGISTERED** (`try_add`) | `host/hle/hle_media.cpp:1174` |
| 57 | `sceSasCore` | `0x5F9529F6` | `__sceSasSetSL` | **REGISTERED** (`try_add`) | `host/hle/hle_media.cpp:1118` |
| 58 | `sceSasCore` | `0x68A46B95` | `__sceSasGetEndFlag` | **REGISTERED** | `host/hle/hle_media.cpp:1163` |
| 59 | `sceSasCore` | `0x7497EA85` | `__sceSasConcatenateATRAC3` | **REGISTERED** | `host/hle/hle_media.cpp:1028` |
| 60 | `sceSasCore` | `0x74AE582A` | `__sceSasGetEnvelopeHeight` | **REGISTERED** (`try_add`) | `host/hle/hle_media.cpp:1140` |
| 61 | `sceSasCore` | `0x76F01ACA` | `__sceSasSetKeyOn` | **REGISTERED** | `host/hle/hle_media.cpp:1093` |
| 62 | `sceSasCore` | `0x787D04D5` | `__sceSasSetPause` | **REGISTERED** (`try_add`) | `host/hle/hle_media.cpp:1101` |
| 63 | `sceSasCore` | `0x99944089` | `__sceSasSetVoice` | **REGISTERED** | `host/hle/hle_media.cpp:1060` |
| 64 | `sceSasCore` | `0x9EC3676A` | `__sceSasSetADSRmode` | **REGISTERED** (`try_add`) | `host/hle/hle_media.cpp:1114` |
| 65 | `sceSasCore` | `0xA0CF2FA4` | `__sceSasSetKeyOff` | **REGISTERED** | `host/hle/hle_media.cpp:1097` |
| 66 | `sceSasCore` | `0xA3589D81` | `__sceSasCore` | **REGISTERED** | `host/hle/hle_media.cpp:1170` |
| 67 | `sceSasCore` | `0xAD84D37F` | `__sceSasSetPitch` | **REGISTERED** | `host/hle/hle_media.cpp:1080` |
| 68 | `sceSasCore` | `0xB7660A23` | `__sceSasSetNoise` | **REGISTERED** (`try_add`) | `host/hle/hle_media.cpp:1074` |
| 69 | `sceSasCore` | `0xBD11B7C2` | `__sceSasGetGrain` | **REGISTERED** (`try_add`) | `host/hle/hle_media.cpp:1128` |
| 70 | `sceSasCore` | `0xCBCD4F79` | `__sceSasSetSimpleADSR` | **REGISTERED** | `host/hle/hle_media.cpp:1089` |
| 71 | `sceSasCore` | `0xD1E0A01E` | `__sceSasSetGrain` | **REGISTERED** (`try_add`) | `host/hle/hle_media.cpp:1131` |
| 72 | `sceSasCore` | `0xD5A229C9` | `__sceSasRevEVOL` | **REGISTERED** | `host/hle/hle_media.cpp:1158` |
| 73 | `sceSasCore` | `0xE175EF66` | `__sceSasGetOutputmode` | **REGISTERED** | `host/hle/hle_media.cpp:1160` |
| 74 | `sceSasCore` | `0xE1CD9561` | `__sceSasSetVoicePCM` | **REGISTERED** (`try_add`) | `host/hle/hle_media.cpp:1067` |
| 75 | `sceSasCore` | `0xE855BF76` | `__sceSasSetOutputmode` | **REGISTERED** (`try_add`) | `host/hle/hle_media.cpp:1134` |
| 76 | `sceSasCore` | `0xF6107F00` | `__sceSasUnsetATRAC3` | **REGISTERED** | `host/hle/hle_media.cpp:1038` |
| 77 | `sceSasCore` | `0xF983B186` | `__sceSasRevVON` | **REGISTERED** | `host/hle/hle_media.cpp:1158` |
| 78 | `sceAtrac3plus` | `0x132F1ECA` | `sceAtracReinit` | **REGISTERED** | `host/hle/hle_atrac.cpp:677` |
| 79 | `sceAtrac3plus` | `0x231FC6B7` | `_sceAtracGetContextAddress` | **REGISTERED** (`try_add`) | `host/hle/hle_atrac.cpp:575` |
| 80 | `sceAtrac3plus` | `0x61EB33F5` | `sceAtracReleaseAtracID` | **REGISTERED** | `host/hle/hle_atrac.cpp:681` |
| 81 | `sceAtrac3plus` | `0x6A8C3CD5` | `sceAtracDecodeData` | **REGISTERED** | `host/hle/hle_atrac.cpp:693` |
| 82 | `sceAtrac3plus` | `0x7A20E7AF` | `sceAtracSetDataAndGetID` | **REGISTERED** | `host/hle/hle_atrac.cpp:550` |
| 83 | `sceAtrac3plus` | `0x868120B5` | `sceAtracSetLoopNum` | **REGISTERED** | `host/hle/hle_atrac.cpp:803` |
| 84 | `sceJpeg` | `0x04B5AE02` | `sceJpegMJpegCsc` | **MISSING** | — |
| 85 | `sceJpeg` | `0x04B93CEF` | `sceJpegDecodeMJpeg` | **MISSING** | — |
| 86 | `sceJpeg` | `0x227662D7` | `sceJpegDecodeMJpegYCbCrSuccessively` | **MISSING** | — |
| 87 | `sceJpeg` | `0x48B602B7` | `sceJpegDeleteMJpeg` | **MISSING** | — |
| 88 | `sceJpeg` | `0x64B6F978` | `sceJpegDecodeMJpegSuccessively` | **MISSING** | — |
| 89 | `sceJpeg` | `0x67F0ED84` | `sceJpegCsc` | **REGISTERED** | `host/hle/hle_mpeg.cpp:701` |
| 90 | `sceJpeg` | `0x7D2F3D7F` | `sceJpegFinishMJpeg` | **MISSING** | — |
| 91 | `sceJpeg` | `0x8F2BB012` | `sceJpegGetOutputInfo` | **MISSING** | — |
| 92 | `sceJpeg` | `0x91EED83C` | `sceJpegDecodeMJpegYCbCr` | **MISSING** | — |
| 93 | `sceJpeg` | `0x9D47469C` | `sceJpegCreateMJpeg` | **MISSING** | — |
| 94 | `sceJpeg` | `0xA06A75C4` | `sceJpegMJpegCscWithColorOption` | **MISSING** | — |
| 95 | `sceJpeg` | `0xAC9E70E6` | `sceJpegInitMJpeg` | **MISSING** | — |
| 96 | `sceMd5` | `0x19884A15` | `sceMd5BlockInit` | **REGISTERED** | `host/hle/hle_system.cpp:195` |
| 97 | `sceMd5` | `0x4876AFFF` | `sceMd5BlockResult` | **REGISTERED** | `host/hle/hle_system.cpp:216` |
| 98 | `sceMd5` | `0x98E31A9E` | `sceMd5Digest` | **REGISTERED** | `host/hle/hle_system.cpp:226` |
| 99 | `sceMd5` | `0xA30206C2` | `sceMd5BlockUpdate` | **REGISTERED** | `host/hle/hle_system.cpp:204` |
| 100 | `sceLibFont` | `0x02D7F94B` | `sceFontFlush` | **REGISTERED** | `host/hle/hle_font.cpp:340` |
| 101 | `sceLibFont` | `0x099EF33C` | `sceFontFindOptimumFont` | **REGISTERED** | `host/hle/hle_font.cpp:288` |
| 102 | `sceLibFont` | `0x0DA7535E` | `sceFontGetFontInfo` | **REGISTERED** | `host/hle/hle_font.cpp:351` |
| 103 | `sceLibFont` | `0x3AEA8CB6` | `sceFontClose` | **REGISTERED** | `host/hle/hle_font.cpp:346` |
| 104 | `sceLibFont` | `0x574B6FBC` | `sceFontDoneLib` | **REGISTERED** | `host/hle/hle_font.cpp:279` |
| 105 | `sceLibFont` | `0x67F17ED7` | `sceFontNewLib` | **REGISTERED** | `host/hle/hle_font.cpp:268` |
| 106 | `sceLibFont` | `0x980F4895` | `sceFontGetCharGlyphImage` | **REGISTERED** | `host/hle/hle_font.cpp:378` |
| 107 | `sceLibFont` | `0xA834319D` | `sceFontOpen` | **REGISTERED** | `host/hle/hle_font.cpp:306` |
| 108 | `sceLibFont` | `0xBB8E7FE6` | `sceFontOpenUserMemory` | **REGISTERED** | `host/hle/hle_font.cpp:315` |
| 109 | `sceLibFont` | `0xDCC80C2F` | `sceFontGetCharInfo` | **REGISTERED** | `host/hle/hle_font.cpp:359` |
| 110 | `sceLibFont` | `0xEE232411` | `sceFontSetAltCharacterCode` | **REGISTERED** | `host/hle/hle_font.cpp:328` |
| 111 | `sceUtility` | `0x2A2B3DE0` | `sceUtilityLoadModule` | **REGISTERED** | `host/hle/hle_system.cpp:654` |
| 112 | `sceUtility` | `0x2AD8E239` | `sceUtilityMsgDialogInitStart` | **REGISTERED** | `host/hle/hle_utility.cpp:398` |
| 113 | `sceUtility` | `0x3DFAEBA9` | `sceUtilityOskShutdownStart` | **REGISTERED** | `host/hle/hle_utility.cpp:299` |
| 114 | `sceUtility` | `0x4B85C861` | `sceUtilityOskUpdate` | **REGISTERED** | `host/hle/hle_utility.cpp:287` |
| 115 | `sceUtility` | `0x50C4CD57` | `sceUtilitySavedataInitStart` | **REGISTERED** | `host/hle/hle_savedata.cpp:502` |
| 116 | `sceUtility` | `0x67AF3428` | `sceUtilityMsgDialogShutdownStart` | **REGISTERED** | `host/hle/hle_utility.cpp:427` |
| 117 | `sceUtility` | `0x8874DBE0` | `sceUtilitySavedataGetStatus` | **REGISTERED** | `host/hle/hle_savedata.cpp:523` |
| 118 | `sceUtility` | `0x95FC253B` | `sceUtilityMsgDialogUpdate` | **REGISTERED** | `host/hle/hle_utility.cpp:418` |
| 119 | `sceUtility` | `0x9790B33C` | `sceUtilitySavedataShutdownStart` | **REGISTERED** | `host/hle/hle_savedata.cpp:529` |
| 120 | `sceUtility` | `0x9A1C91D7` | `sceUtilityMsgDialogGetStatus` | **REGISTERED** | `host/hle/hle_utility.cpp:423` |
| 121 | `sceUtility` | `0xA5DA2406` | `sceUtilityGetSystemParamInt` | **REGISTERED** | `host/hle/hle_system.cpp:627` |
| 122 | `sceUtility` | `0xD4B95FFB` | `sceUtilitySavedataUpdate` | **REGISTERED** | `host/hle/hle_savedata.cpp:518` |
| 123 | `sceUtility` | `0xE49BFE92` | `sceUtilityUnloadModule` | **REGISTERED** | `host/hle/hle_system.cpp:657` |
| 124 | `sceUtility` | `0xF3F76017` | `sceUtilityOskGetStatus` | **REGISTERED** | `host/hle/hle_utility.cpp:293` |
| 125 | `sceUtility` | `0xF6269B82` | `sceUtilityOskInitStart` | **REGISTERED** | `host/hle/hle_utility.cpp:268` |
| 126 | `sceMpeg` | `0x0E3C2E9D` | `sceMpegAvcDecode` | **REGISTERED** | `host/hle/hle_mpeg.cpp:515` |
| 127 | `sceMpeg` | `0x13407F13` | `sceMpegRingbufferDestruct` | **REGISTERED** | `host/hle/hle_mpeg.cpp:267` |
| 128 | `sceMpeg` | `0x167AFD9E` | `sceMpegInitAu` | **REGISTERED** | `host/hle/hle_mpeg.cpp:368` |
| 129 | `sceMpeg` | `0x21FF80E4` | `sceMpegQueryStreamOffset` | **REGISTERED** | `host/hle/hle_mpeg.cpp:320` |
| 130 | `sceMpeg` | `0x37295ED8` | `sceMpegRingbufferConstruct` | **REGISTERED** | `host/hle/hle_mpeg.cpp:249` |
| 131 | `sceMpeg` | `0x42560F23` | `sceMpegRegistStream` | **REGISTERED** | `host/hle/hle_mpeg.cpp:346` |
| 132 | `sceMpeg` | `0x4571CC64` | `sceMpegAvcDecodeFlush` | **REGISTERED** | `host/hle/hle_mpeg.cpp:592` |
| 133 | `sceMpeg` | `0x591A4AA2` | `sceMpegUnRegistStream` | **REGISTERED** | `host/hle/hle_mpeg.cpp:357` |
| 134 | `sceMpeg` | `0x606A4649` | `sceMpegDelete` | **REGISTERED** | `host/hle/hle_mpeg.cpp:315` |
| 135 | `sceMpeg` | `0x682A619B` | `sceMpegInit` | **REGISTERED** | `host/hle/hle_mpeg.cpp:288` |
| 136 | `sceMpeg` | `0x740FCCD1` | `sceMpegAvcDecodeStop` | **REGISTERED** | `host/hle/hle_mpeg.cpp:545` |
| 137 | `sceMpeg` | `0x769BEBB6` | `sceMpegRingbufferQueryPackNum` | **REGISTERED** | `host/hle/hle_mpeg.cpp:244` |
| 138 | `sceMpeg` | `0x800C44DF` | `sceMpegAtracDecode` | **REGISTERED** | `host/hle/hle_mpeg.cpp:670` |
| 139 | `sceMpeg` | `0x874624D6` | `sceMpegFinish` | **REGISTERED** | `host/hle/hle_mpeg.cpp:288` |
| 140 | `sceMpeg` | `0xA780CF7E` | `sceMpegMallocAvcEsBuf` | **REGISTERED** | `host/hle/hle_mpeg.cpp:361` |
| 141 | `sceMpeg` | `0xB240A59E` | `sceMpegRingbufferPut` | **REGISTERED** | `host/hle/hle_mpeg.cpp:279` |
| 142 | `sceMpeg` | `0xB5F6DC87` | `sceMpegRingbufferAvailableSize` | **REGISTERED** | `host/hle/hle_mpeg.cpp:271` |
| 143 | `sceMpeg` | `0xC132E22F` | `sceMpegQueryMemSize` | **REGISTERED** | `host/hle/hle_mpeg.cpp:290` |
| 144 | `sceMpeg` | `0xCEB870B1` | `sceMpegFreeAvcEsBuf` | **REGISTERED** | `host/hle/hle_mpeg.cpp:364` |
| 145 | `sceMpeg` | `0xD7A29F46` | `sceMpegRingbufferQueryMemSize` | **REGISTERED** | `host/hle/hle_mpeg.cpp:237` |
| 146 | `sceMpeg` | `0xD8C5F121` | `sceMpegCreate` | **REGISTERED** | `host/hle/hle_mpeg.cpp:294` |
| 147 | `sceMpeg` | `0xE1CE83A7` | `sceMpegGetAtracAu` | **REGISTERED** | `host/hle/hle_mpeg.cpp:414` |
| 148 | `sceMpeg` | `0xF8DCB679` | `sceMpegQueryAtracEsSize` | **REGISTERED** | `host/hle/hle_mpeg.cpp:378` |
| 149 | `sceMpeg` | `0xFE246728` | `sceMpegGetAvcAu` | **REGISTERED** | `host/hle/hle_mpeg.cpp:395` |
| 150 | `sceRtc` | `0x011F03C1` | `sceRtcGetAccumulativeTime` | **REGISTERED** | `host/hle/hle_system.cpp:605` |
| 151 | `sceRtc` | `0x26D7A24A` | `sceRtcTickAddHours` | **REGISTERED** | `host/hle/hle_system.cpp:794` |
| 152 | `sceRtc` | `0x27F98543` | `sceRtcFormatRFC3339LocalTime` | **MISSING** | — |
| 153 | `sceRtc` | `0x34885E0D` | `sceRtcConvertUtcToLocalTime` | **MISSING** | — |
| 154 | `sceRtc` | `0x3F7AD767` | `sceRtcGetCurrentTick` | **REGISTERED** | `host/hle/hle_system.cpp:522` |
| 155 | `sceRtc` | `0x6FF40ACC` | `sceRtcGetTick` | **REGISTERED** | `host/hle/hle_system.cpp:710` |
| 156 | `sceRtc` | `0x7D1FBED3` | `sceRtcSetAlarmTick` | **MISSING** | — |
| 157 | `sceRtc` | `0x7ED29E40` | `sceRtcSetTick` | **REGISTERED** | `host/hle/hle_system.cpp:721` |
| 158 | `sceRtc` | `0xE1C93E47` | `sceRtcGetTime64_t` | **REGISTERED** | `host/hle/hle_system.cpp:763` |
| 159 | `sceRtc` | `0xE51B4B7A` | `sceRtcTickAddDays` | **REGISTERED** | `host/hle/hle_system.cpp:795` |
| 160 | `sceRtc` | `0xE6605BCA` | `sceRtcTickAddMinutes` | **REGISTERED** | `host/hle/hle_system.cpp:793` |
| 161 | `sceRtc` | `0xE7C27D1B` | `sceRtcGetCurrentClockLocalTime` | **REGISTERED** | `host/hle/hle_system.cpp:500` |
| 162 | `sceRtc` | `0xF2A4AFE5` | `sceRtcTickAddSeconds` | **REGISTERED** | `host/hle/hle_system.cpp:792` |
| 163 | `sceWlanDrv` | `0x0C622081` | `sceWlanGetEtherAddr` | **REGISTERED** | `host/hle/hle_adhoc.cpp:330` |
| 164 | `sceWlanDrv` | `0xD7763699` | `sceWlanGetSwitchState` | **REGISTERED** | `host/hle/hle_adhoc.cpp:335` |
| 165 | `sceOpenPSID` | `0xC69BEBCE` | `sceOpenPSIDGetOpenPSID` | **REGISTERED** | `host/hle/hle_system.cpp:611` |
| 166 | `sceNetAdhocDiscover` | `0x52DE1B97` | `sceNetAdhocDiscoverUpdate` | **REGISTERED** | `host/hle/hle_adhoc.cpp:874` |
| 167 | `sceNetAdhocDiscover` | `0x941B3877` | `sceNetAdhocDiscoverInitStart` | **REGISTERED** | `host/hle/hle_adhoc.cpp:870` |
| 168 | `sceNetAdhocDiscover` | `0x944DDBC6` | `sceNetAdhocDiscoverGetStatus` | **REGISTERED** | `host/hle/hle_adhoc.cpp:876` |
| 169 | `sceNetAdhocDiscover` | `0xA2246614` | `sceNetAdhocDiscoverTerm` | **REGISTERED** | `host/hle/hle_adhoc.cpp:882` |
| 170 | `sceNetAdhocDiscover` | `0xA423A21B` | `sceNetAdhocDiscoverRequestSuspend` | **REGISTERED** (`try_add`) | `host/hle/hle_adhoc.cpp:314` |
| 171 | `sceNetAdhocDiscover` | `0xF7D13214` | `sceNetAdhocDiscoverStop` | **REGISTERED** | `host/hle/hle_adhoc.cpp:878` |
| 172 | `sceNetAdhocctl` | `0x08FFF7A0` | `sceNetAdhocctlScan` | **REGISTERED** | `host/hle/hle_adhoc.cpp:375` |
| 173 | `sceNetAdhocctl` | `0x0AD043ED` | `sceNetAdhocctlConnect` | **REGISTERED** | `host/hle/hle_adhoc.cpp:439` |
| 174 | `sceNetAdhocctl` | `0x20B317A0` | `sceNetAdhocctlAddHandler` | **REGISTERED** | `host/hle/hle_adhoc.cpp:363` |
| 175 | `sceNetAdhocctl` | `0x34401D65` | `sceNetAdhocctlDisconnect` | **REGISTERED** | `host/hle/hle_adhoc.cpp:406` |
| 176 | `sceNetAdhocctl` | `0x5E7F79C9` | `sceNetAdhocctlJoin` | **REGISTERED** | `host/hle/hle_adhoc.cpp:443` |
| 177 | `sceNetAdhocctl` | `0x6402490B` | `sceNetAdhocctlDelHandler` | **REGISTERED** | `host/hle/hle_adhoc.cpp:371` |
| 178 | `sceNetAdhocctl` | `0x75ECD386` | `sceNetAdhocctlGetState` | **MISSING** | — |
| 179 | `sceNetAdhocctl` | `0x81AEE1BE` | `sceNetAdhocctlGetScanInfo` | **REGISTERED** | `host/hle/hle_adhoc.cpp:380` |
| 180 | `sceNetAdhocctl` | `0x9D689E13` | `sceNetAdhocctlTerm` | **REGISTERED** | `host/hle/hle_adhoc.cpp:356` |
| 181 | `sceNetAdhocctl` | `0xDED9D28E` | `sceNetAdhocctlGetParameter` | **MISSING** | — |
| 182 | `sceNetAdhocctl` | `0xE162CB14` | `sceNetAdhocctlGetPeerList` | **REGISTERED** | `host/hle/hle_adhoc.cpp:464` |
| 183 | `sceNetAdhocctl` | `0xE26F226E` | `sceNetAdhocctlInit` | **REGISTERED** | `host/hle/hle_adhoc.cpp:343` |
| 184 | `sceNetAdhocctl` | `0xEC0635C1` | `sceNetAdhocctlCreate` | **REGISTERED** | `host/hle/hle_adhoc.cpp:435` |
| 185 | `sceNetAdhoc` | `0x6F92741B` | `sceNetAdhocPdpCreate` | **REGISTERED** | `host/hle/hle_adhoc.cpp:547` |
| 186 | `sceNetAdhoc` | `0x7F27BB5E` | `sceNetAdhocPdpDelete` | **REGISTERED** | `host/hle/hle_adhoc.cpp:563` |
| 187 | `sceNetAdhoc` | `0xA62C6F57` | `sceNetAdhocTerm` | **REGISTERED** | `host/hle/hle_adhoc.cpp:542` |
| 188 | `sceNetAdhoc` | `0xABED3790` | `sceNetAdhocPdpSend` | **REGISTERED** | `host/hle/hle_adhoc.cpp:571` |
| 189 | `sceNetAdhoc` | `0xDFE53E03` | `sceNetAdhocPdpRecv` | **REGISTERED** | `host/hle/hle_adhoc.cpp:590` |
| 190 | `sceNetAdhoc` | `0xE1D621D7` | `sceNetAdhocInit` | **REGISTERED** | `host/hle/hle_adhoc.cpp:536` |
| 191 | `sceNet` | `0x281928A9` | `sceNetTerm` | **REGISTERED** | `host/hle/hle_adhoc.cpp:305` |
| 192 | `sceNet` | `0x39AF39A6` | `sceNetInit` | **REGISTERED** | `host/hle/hle_adhoc.cpp:304` |
| 193 | `scePspNpDrm_user` | `0x08D98894` | `sceNpDrmEdataSetupKey` | **REGISTERED** | `host/hle/hle_system.cpp:564` |
| 194 | `scePspNpDrm_user` | `0x219EF5CC` | `sceNpDrmEdataGetDataSize` | **REGISTERED** | `host/hle/hle_system.cpp:584` |
| 195 | `scePspNpDrm_user` | `0x275987D1` | `sceNpDrmRenameCheck` | **REGISTERED** | `host/hle/hle_system.cpp:544` |
| 196 | `scePspNpDrm_user` | `0xA1336091` | `sceNpDrmSetLicenseeKey` | **REGISTERED** | `host/hle/hle_system.cpp:533` |
| 197 | `InterruptManager` | `0xCA04A2B9` | `sceKernelRegisterSubIntrHandler` | **REGISTERED** | `host/hle/hle_system.cpp:436` |
| 198 | `InterruptManager` | `0xD61E6961` | `sceKernelReleaseSubIntrHandler` | **REGISTERED** | `host/hle/hle_system.cpp:442` |
| 199 | `InterruptManager` | `0xFB8E22EC` | `sceKernelEnableSubIntr` | **REGISTERED** | `host/hle/hle_system.cpp:445` |
| 200 | `IoFileMgrForUser` | `0x06A70004` | `sceIoMkdir` | **REGISTERED** | `host/hle/hle_io.cpp:923` |
| 201 | `IoFileMgrForUser` | `0x42EC03AC` | `sceIoWrite` | **REGISTERED** | `host/hle/hle_io.cpp:768` |
| 202 | `IoFileMgrForUser` | `0x54F5FB11` | `sceIoDevctl` | **REGISTERED** | `host/hle/hle_io.cpp:945` |
| 203 | `IoFileMgrForUser` | `0x63632449` | `sceIoIoctl` | **REGISTERED** | `host/hle/hle_io.cpp:941` |
| 204 | `IoFileMgrForUser` | `0x6A638D83` | `sceIoRead` | **REGISTERED** | `host/hle/hle_io.cpp:765` |
| 205 | `IoFileMgrForUser` | `0x810C4BC3` | `sceIoClose` | **REGISTERED** | `host/hle/hle_io.cpp:762` |
| 206 | `IoFileMgrForUser` | `0xACE946E8` | `sceIoGetstat` | **REGISTERED** | `host/hle/hle_io.cpp:803` |
| 207 | `IoFileMgrForUser` | `0xB29DDF9C` | `sceIoDopen` | **REGISTERED** | `host/hle/hle_io.cpp:825` |
| 208 | `IoFileMgrForUser` | `0xE3EB004C` | `sceIoDread` | **REGISTERED** | `host/hle/hle_io.cpp:873` |
| 209 | `IoFileMgrForUser` | `0xEB092469` | `sceIoDclose` | **REGISTERED** | `host/hle/hle_io.cpp:895` |
| 210 | `IoFileMgrForUser` | `0xF27A9C51` | `sceIoRemove` | **REGISTERED** | `host/hle/hle_io.cpp:925` |
| 211 | `IoFileMgrForUser` | `0x109F50BC` | `sceIoOpen` | **REGISTERED** | `host/hle/hle_io.cpp:754` |
| 212 | `IoFileMgrForUser` | `0x27EB27B8` | `sceIoLseek` | **REGISTERED** | `host/hle/hle_io.cpp:771` |
| 213 | `IoFileMgrForUser` | `0x779103A0` | `sceIoRename` | **REGISTERED** | `host/hle/hle_io.cpp:926` |
| 214 | `Kernel_Library` | `0x092968F4` | `sceKernelCpuSuspendIntr` | **REGISTERED** | `host/hle/hle_threadman.cpp:1099` |
| 215 | `Kernel_Library` | `0xA089ECA4` | `sceKernelMemset` | **REGISTERED** | `host/hle/hle_threadman.cpp:1117` |
| 216 | `Kernel_Library` | `0xBEA46419` | `sceKernelLockLwMutex` | **REGISTERED** | `host/hle/hle_threadman.cpp:980` |
| 217 | `Kernel_Library` | `0x15B6446B` | `sceKernelUnlockLwMutex` | **REGISTERED** | `host/hle/hle_threadman.cpp:1014` |
| 218 | `Kernel_Library` | `0x1839852A` | `sceKernelMemcpy` | **REGISTERED** | `host/hle/hle_threadman.cpp:1091` |
| 219 | `Kernel_Library` | `0x293B45B8` | `sceKernelGetThreadId` | **REGISTERED** | `host/hle/hle_threadman.cpp:55` |
| 220 | `Kernel_Library` | `0x37431849` | `sceKernelTryLockLwMutex_600` | **REGISTERED** | `host/hle/hle_threadman.cpp:985` |
| 221 | `Kernel_Library` | `0x5F10D406` | `sceKernelCpuResumeIntr` | **REGISTERED** | `host/hle/hle_threadman.cpp:1108` |
| 222 | `LoadExecForUser` | `0x05572A5F` | `sceKernelExitGame` | **REGISTERED** | `host/hle/hle_system.cpp:292` |
| 223 | `LoadExecForUser` | `0x4AC57943` | `sceKernelRegisterExitCallback` | **REGISTERED** | `host/hle/hle_system.cpp:291` |
| 224 | `ModuleMgrForUser` | `0x2E0911AA` | `sceKernelUnloadModule` | **REGISTERED** | `host/hle/hle_system.cpp:422` |
| 225 | `ModuleMgrForUser` | `0xD1FF982A` | `sceKernelStopModule` | **REGISTERED** | `host/hle/hle_system.cpp:413` |
| 226 | `ModuleMgrForUser` | `0xD8B73127` | `sceKernelGetModuleIdByAddress` | **REGISTERED** | `host/hle/hle_system.cpp:304` |
| 227 | `ModuleMgrForUser` | `0xF0A26395` | `sceKernelGetModuleId` | **REGISTERED** | `host/hle/hle_system.cpp:301` |
| 228 | `ModuleMgrForUser` | `0xF2D8D1B4` | `sceKernelLoadModuleNpDrm` | **REGISTERED** | `host/hle/hle_system.cpp:341` |
| 229 | `ModuleMgrForUser` | `0x50F0C1EC` | `sceKernelStartModule` | **REGISTERED** | `host/hle/hle_system.cpp:401` |
| 230 | `ModuleMgrForUser` | `0x644395E2` | `sceKernelGetModuleIdList` | **REGISTERED** | `host/hle/hle_system.cpp:352` |
| 231 | `ModuleMgrForUser` | `0x748CBED9` | `sceKernelQueryModuleInfo` | **REGISTERED** | `host/hle/hle_system.cpp:371` |
| 232 | `ModuleMgrForUser` | `0x977DE386` | `sceKernelLoadModule` | **REGISTERED** | `host/hle/hle_system.cpp:340` |
| 233 | `ModuleMgrForUser` | `0x8F2DF740` | `sceKernelStopUnloadSelfModuleWithStatus` | **REGISTERED** | `host/hle/hle_system.cpp:430` |
| 234 | `StdioForUser` | `0x172D316E` | `sceKernelStdin` | **REGISTERED** | `host/hle/hle_system.cpp:296` |
| 235 | `StdioForUser` | `0xA6BAB2E9` | `sceKernelStdout` | **REGISTERED** | `host/hle/hle_system.cpp:297` |
| 236 | `StdioForUser` | `0xF78BA90A` | `sceKernelStderr` | **REGISTERED** | `host/hle/hle_system.cpp:298` |
| 237 | `SysMemUserForUser` | `0x13A5ABEF` | `sceKernelPrintf` | **REGISTERED** | `host/hle/hle_sysmem.cpp:95` |
| 238 | `SysMemUserForUser` | `0xDB83A952` | — | **UNKNOWN NID** | — |
| 239 | `SysMemUserForUser` | `0xF77D77CB` | `sceKernelSetCompilerVersion` | **REGISTERED** | `host/hle/hle_sysmem.cpp:89` |
| 240 | `SysMemUserForUser` | `0xF919F628` | `sceKernelTotalFreeMemSize` | **REGISTERED** | `host/hle/hle_sysmem.cpp:85` |
| 241 | `SysMemUserForUser` | `0xFE707FDF` | — | **UNKNOWN NID** | — |
| 242 | `SysMemUserForUser` | `0x1B4217BC` | `sceKernelSetCompiledSdkVersion603_605` | **REGISTERED** | `host/hle/hle_sysmem.cpp:90` |
| 243 | `SysMemUserForUser` | `0x35669D4C` | `sceKernelSetCompiledSdkVersion600_602` | **MISSING** | — |
| 244 | `SysMemUserForUser` | `0x50F61D8A` | — | **UNKNOWN NID** | — |
| 245 | `SysMemUserForUser` | `0xA291F107` | `sceKernelMaxFreeMemSize` | **REGISTERED** | `host/hle/hle_sysmem.cpp:82` |
| 246 | `SysMemUserForUser` | `0x237DBD4F` | `sceKernelAllocPartitionMemory` | **REGISTERED** | `host/hle/hle_sysmem.cpp:50` |
| 247 | `SysMemUserForUser` | `0x9D9A5BA1` | `sceKernelGetBlockHeadAddr` | **REGISTERED** | `host/hle/hle_sysmem.cpp:57` |
| 248 | `SysMemUserForUser` | `0xB6D61D02` | `sceKernelFreePartitionMemory` | **REGISTERED** | `host/hle/hle_sysmem.cpp:54` |
| 249 | `ThreadManForUser` | `0xCEADEB47` | `sceKernelDelayThread` | **REGISTERED** | `host/hle/hle_threadman.cpp:168` |
| 250 | `ThreadManForUser` | `0xD59EAD2F` | `sceKernelWakeupThread` | **REGISTERED** | `host/hle/hle_threadman.cpp:153` |
| 251 | `ThreadManForUser` | `0xD6DA4BA1` | `sceKernelCreateSema` | **REGISTERED** | `host/hle/hle_threadman.cpp:325` |
| 252 | `ThreadManForUser` | `0xD8199E4C` | `sceKernelReferFplStatus` | **REGISTERED** | `host/hle/hle_threadman.cpp:1521` |
| 253 | `ThreadManForUser` | `0xDB738F35` | `sceKernelGetSystemTime` | **REGISTERED** | `host/hle/hle_threadman.cpp:291` |
| 254 | `ThreadManForUser` | `0x1FB15A32` | `sceKernelSetEventFlag` | **REGISTERED** | `host/hle/hle_threadman.cpp:453` |
| 255 | `ThreadManForUser` | `0xE81CAF8F` | `sceKernelCreateCallback` | **REGISTERED** | `host/hle/hle_threadman.cpp:656` |
| 256 | `ThreadManForUser` | `0xE9B3061E` | `sceKernelSendMbx` | **REGISTERED** | `host/hle/hle_threadman.cpp:833` |
| 257 | `ThreadManForUser` | `0xEA748E31` | `sceKernelChangeCurrentThreadAttr` | **REGISTERED** | `host/hle/hle_threadman.cpp:63` |
| 258 | `ThreadManForUser` | `0xEDBA5844` | `sceKernelDeleteCallback` | **REGISTERED** | `host/hle/hle_threadman.cpp:662` |
| 259 | `ThreadManForUser` | `0xEF9E4C70` | `sceKernelDeleteEventFlag` | **REGISTERED** | `host/hle/hle_threadman.cpp:443` |
| 260 | `ThreadManForUser` | `0xF3986382` | `sceKernelReceiveMbxCB` | **REGISTERED** | `host/hle/hle_threadman.cpp:883` |
| 261 | `ThreadManForUser` | `0xF475845D` | `sceKernelStartThread` | **REGISTERED** | `host/hle/hle_threadman.cpp:31` |
| 262 | `ThreadManForUser` | `0xF8170FBE` | `sceKernelDeleteMutex` | **REGISTERED** | `host/hle/hle_threadman.cpp:556` |
| 263 | `ThreadManForUser` | `0x278C0DF5` | `sceKernelWaitThreadEnd` | **REGISTERED** | `host/hle/hle_threadman.cpp:105` |
| 264 | `ThreadManForUser` | `0x28B6489C` | `sceKernelDeleteSema` | **REGISTERED** | `host/hle/hle_threadman.cpp:336` |
| 265 | `ThreadManForUser` | `0x33BE4024` | `sceKernelReferMsgPipeStatus` | **REGISTERED** | `host/hle/hle_threadman.cpp:1732` |
| 266 | `ThreadManForUser` | `0x349D6D6C` | `sceKernelCheckCallback` | **REGISTERED** | `host/hle/hle_threadman.cpp:271` |
| 267 | `ThreadManForUser` | `0x369ED59D` | `sceKernelGetSystemTimeLow` | **REGISTERED** | `host/hle/hle_threadman.cpp:298` |
| 268 | `ThreadManForUser` | `0x369EEB6B` | `sceKernelReferThreadEventHandlerStatus` | **MISSING** | — |
| 269 | `ThreadManForUser` | `0x383F7BCC` | `sceKernelTerminateDeleteThread` | **REGISTERED** | `host/hle/hle_threadman.cpp:48` |
| 270 | `ThreadManForUser` | `0x39810265` | `sceKernelReferVplStatus` | **REGISTERED** | `host/hle/hle_threadman.cpp:1326` |
| 271 | `ThreadManForUser` | `0x0D81716A` | `sceKernelPollMbx` | **REGISTERED** | `host/hle/hle_threadman.cpp:844` |
| 272 | `ThreadManForUser` | `0x3F53E640` | `sceKernelSignalSema` | **REGISTERED** | `host/hle/hle_threadman.cpp:346` |
| 273 | `ThreadManForUser` | `0x402FCF22` | `sceKernelWaitEventFlag` | **REGISTERED** | `host/hle/hle_threadman.cpp:541` |
| 274 | `ThreadManForUser` | `0x446D8DE6` | `sceKernelCreateThread` | **REGISTERED** | `host/hle/hle_threadman.cpp:26` |
| 275 | `ThreadManForUser` | `0x4C145944` | `sceKernelReferLwMutexStatusByID` | **MISSING** | — |
| 276 | `ThreadManForUser` | `0x4E3A1105` | `sceKernelWaitSema` | **REGISTERED** | `host/hle/hle_threadman.cpp:388` |
| 277 | `ThreadManForUser` | `0x55C20A00` | `sceKernelCreateEventFlag` | **REGISTERED** | `host/hle/hle_threadman.cpp:409` |
| 278 | `ThreadManForUser` | `0x5F32BEAA` | `sceKernelReferVTimerStatus` | **MISSING** | — |
| 279 | `ThreadManForUser` | `0x60107536` | `sceKernelDeleteLwMutex` | **REGISTERED** | `host/hle/hle_threadman.cpp:925` |
| 280 | `ThreadManForUser` | `0x616403BA` | `sceKernelTerminateThread` | **REGISTERED** | `host/hle/hle_threadman.cpp:45` |
| 281 | `ThreadManForUser` | `0x6652B8CA` | `sceKernelSetAlarm` | **REGISTERED** | `host/hle/hle_threadman.cpp:684` |
| 282 | `ThreadManForUser` | `0x68DA9E36` | `sceKernelDelayThreadCB` | **REGISTERED** | `host/hle/hle_threadman.cpp:274` |
| 283 | `ThreadManForUser` | `0x6B30100F` | `sceKernelUnlockMutex` | **REGISTERED** | `host/hle/hle_threadman.cpp:632` |
| 284 | `ThreadManForUser` | `0x71BC9871` | `sceKernelChangeThreadPriority` | **REGISTERED** | `host/hle/hle_threadman.cpp:60` |
| 285 | `ThreadManForUser` | `0x721067F3` | `sceKernelReferTlsplStatus` | **MISSING** | — |
| 286 | `ThreadManForUser` | `0x730ED8BC` | `sceKernelReferCallbackStatus` | **MISSING** | — |
| 287 | `ThreadManForUser` | `0x7E65B999` | `sceKernelCancelAlarm` | **REGISTERED** | `host/hle/hle_threadman.cpp:696` |
| 288 | `ThreadManForUser` | `0x809CE29B` | `sceKernelExitDeleteThread` | **REGISTERED** | `host/hle/hle_threadman.cpp:37` |
| 289 | `ThreadManForUser` | `0x812346E4` | `sceKernelClearEventFlag` | **REGISTERED** | `host/hle/hle_threadman.cpp:467` |
| 290 | `ThreadManForUser` | `0x8125221D` | `sceKernelCreateMbx` | **REGISTERED** | `host/hle/hle_threadman.cpp:811` |
| 291 | `ThreadManForUser` | `0x8218B4DD` | `sceKernelReferGlobalProfiler` | **REGISTERED** | `host/hle/hle_threadman.cpp:1065` |
| 292 | `ThreadManForUser` | `0x82BC5777` | `sceKernelGetSystemTimeWide` | **REGISTERED** | `host/hle/hle_threadman.cpp:295` |
| 293 | `ThreadManForUser` | `0x17C1684E` | `sceKernelReferThreadStatus` | **REGISTERED** | `host/hle/hle_threadman.cpp:215` |
| 294 | `ThreadManForUser` | `0x86255ADA` | `sceKernelDeleteMbx` | **REGISTERED** | `host/hle/hle_threadman.cpp:823` |
| 295 | `ThreadManForUser` | `0x94416130` | `sceKernelGetThreadmanIdList` | **MISSING** | — |
| 296 | `ThreadManForUser` | `0x94AA61EE` | `sceKernelGetThreadCurrentPriority` | **REGISTERED** | `host/hle/hle_threadman.cpp:56` |
| 297 | `ThreadManForUser` | `0x9ACE131E` | `sceKernelSleepThread` | **REGISTERED** | `host/hle/hle_threadman.cpp:132` |
| 298 | `ThreadManForUser` | `0x9FA03CD3` | `sceKernelDeleteThread` | **REGISTERED** | `host/hle/hle_threadman.cpp:40` |
| 299 | `ThreadManForUser` | `0xA66B0120` | `sceKernelReferEventFlagStatus` | **REGISTERED** | `host/hle/hle_threadman.cpp:419` |
| 300 | `ThreadManForUser` | `0xA8E8C846` | `sceKernelReferMbxStatus` | **MISSING** | — |
| 301 | `ThreadManForUser` | `0xA9C2CB9A` | `sceKernelReferMutexStatus` | **MISSING** | — |
| 302 | `ThreadManForUser` | `0xAA73C935` | `sceKernelExitThread` | **REGISTERED** | `host/hle/hle_threadman.cpp:34` |
| 303 | `ThreadManForUser` | `0x19CFF145` | `sceKernelCreateLwMutex` | **REGISTERED** | `host/hle/hle_threadman.cpp:913` |
| 304 | `ThreadManForUser` | `0xB011B11F` | `sceKernelLockMutex` | **REGISTERED** | `host/hle/hle_threadman.cpp:566` |
| 305 | `ThreadManForUser` | `0xB7D098C6` | `sceKernelCreateMutex` | **REGISTERED** | `host/hle/hle_threadman.cpp:549` |
| 306 | `ThreadManForUser` | `0xBC6FEBC5` | `sceKernelReferSemaStatus` | **MISSING** | — |
| 307 | `UtilsForUser` | `0x6AD345D7` | `sceKernelSetGPO` | **MISSING** | — |
| 308 | `UtilsForUser` | `0x79D1C3FA` | `sceKernelDcacheWritebackAll` | **MISSING** | — |
| 309 | `UtilsForUser` | `0xB435DEC5` | `sceKernelDcacheWritebackInvalidateAll` | **MISSING** | — |
| 310 | `UtilsForUser` | `0xBFA98062` | `sceKernelDcacheInvalidateRange` | **MISSING** | — |
| 311 | `UtilsForUser` | `0x34B9FA9E` | `sceKernelDcacheWritebackInvalidateRange` | **MISSING** | — |
| 312 | `UtilsForUser` | `0x37FB5C42` | `sceKernelGetGPI` | **REGISTERED** | `host/hle/hle_system.cpp:256` |
| 313 | `UtilsForUser` | `0x3EE30821` | `sceKernelDcacheWritebackRange` | **MISSING** | — |
| 314 | `UtilsForUser` | `0x71EC4271` | `sceKernelLibcGettimeofday` | **REGISTERED** | `host/hle/hle_system.cpp:273` |
| 315 | `UtilsForUser` | `0x91E4F6A7` | `sceKernelLibcClock` | **REGISTERED** | `host/hle/hle_system.cpp:285` |
| 316 | `UtilsForUser` | `0x27CC57F0` | `sceKernelLibcTime` | **REGISTERED** | `host/hle/hle_system.cpp:268` |
| 317 | `sceSuspendForUser` | `0x090CCB3F` | `sceKernelPowerTick` | **REGISTERED** | `host/hle/hle_sysmem.cpp:100` |
| 318 | `sceSuspendForUser` | `0xA14F40B2` | `sceKernelVolatileMemTryLock` | **REGISTERED** | `host/hle/hle_sysmem.cpp:127` |
| 319 | `sceSuspendForUser` | `0xA569E425` | `sceKernelVolatileMemUnlock` | **REGISTERED** | `host/hle/hle_sysmem.cpp:135` |

## Important semantic caveats

- The SAS group now has matching registrations for all 30 imports, including ATRAC3 voice binding/concatenation/unset. The existing reverb operations are success-returning no-op shims; do not call the entire SAS library complete.
- `sceJpeg` has only `sceJpegCsc` registered; the 11 MJPEG operations remain missing.
- `sceMpeg` imports are all registered, but some library lifecycle functions are success-returning shims and decode/data paths still need game-level validation.
- `sceLibFont` imports are registered, but the backend substitutes host fonts and does not parse the embedded PSP font data; missing glyphs remain possible.
- `sceNetAdhocctl` is an offline implementation; registration does not imply working PSP ad-hoc networking.
- Other registered APIs may still be partial, unverified, or success-only. Inspect source comments and runtime traces before treating them as complete.
- The latest Android trace previously showed `UtilsForUser::sceKernelGetGPI` as the first stub. It remains unregistered in this source audit. The next runtime priority is therefore `sceKernelGetGPI`, followed by the first actual calls among the missing imports.

## Recommended priorities

1. Implement or deliberately handle the six missing `UtilsForUser` cache/GPO calls; these are especially relevant because `sceKernelGetGPI` was the first logged runtime stub (also missing).
2. Implement the missing ThreadMan status/query calls with correct PSP structures and validation; do not return success without filling guest output buffers.
3. Add UMD and power APIs according to actual runtime use and documented return behavior.
4. Add the missing RTC helpers if the game calls them on the boot path.
5. Implement MJPEG only if runtime traces show the game uses these APIs; do not add success-only decode stubs.
6. Resolve the four unknown NIDs from a second trusted NID database or the SDK/import metadata before assigning names.
7. Run `GEBURST_LIST_STUBS=1` and then `GEBURST_STRICT_HLE=1` on device to distinguish imported-only functions from boot-path requirements.

## Reproducibility

- Inventory artifact was retrieved from the user's Library as `ge_burst_imports.csv`.
- NID names: `https://github.com/pspdev/psp-ghidra-scripts/blob/master/ppsspp_niddb.xml`.
- No executable bytes were added to the repository.
- No build or device runtime test was performed as part of this static audit.
