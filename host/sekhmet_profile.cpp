// God Eater Burst — US PSP release ULUS10563, APP_VER 01.00.
#include "profile.hpp"

#include <array>

namespace portablekit {
namespace {

constexpr GameProfile kProfile{
    .app_name = "SekhmetNative",
    .project_name = "God Eater Burst",
    .env_prefix = "GEBURST",
    .data_organization = "eldaxe",
    .data_application = "Sekhmet",

    .disc_id = "ULUS10563",
    .disc_id_display = "ULUS-10563",
    .game_title = "GOD EATER BURST",
    .executable_path_on_disc = "PSP_GAME/SYSDIR/EBOOT.BIN",
    .param_sfo_path_on_disc = "PSP_GAME/PARAM.SFO",
    .release_name = "the US release",
    .encrypted_executable_sha256 =
        "aa78e5d222340a84532600eb30d5694f91f7750493529af584e4813c4392a6e9",
    .executable_sha256 =
        "1c6d7966fdc93e4b78a1b4807bbb3c7eb9c3030986790a521224d44b59ab34c4",
    .decryption_tag = 0xD91613F0,
    .decryption_key = {
        0xEB, 0xFF, 0x40, 0xD8, 0xB4, 0x1A, 0xE1, 0x66,
        0x91, 0x3B, 0x8F, 0x64, 0xB6, 0xFC, 0xB7, 0x12,
    },

    .load_base = 0x08804000,
    .guest_ram_bytes = 32u * 1024u * 1024u,
    .boot_path = "disc0:/PSP_GAME/SYSDIR/EBOOT.BIN",
    .overlay_slots = {},

    // Save directory naming and ad-hoc product code are provisional until
    // verified against the extracted ULUS10563 filesystem and runtime trace.
    .save_game_name = "ULUS10563",
    .save_folders = {},
    .adhoc_product_code = "ULUS10563",

    .frame_vblanks = 2,
    .system_language = 1,
    .confirm_button = 1,
};

} // namespace

const GameProfile &game() { return kProfile; }

} // namespace portablekit
