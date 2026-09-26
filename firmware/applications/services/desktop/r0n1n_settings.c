#include "r0n1n_settings.h"
#include "r0n1n_catalog.h"

#include <furi.h>
#include <saved_struct.h>
#include <storage/storage.h>

#define TAG "R0n1nSettings"

#define R0N1N_SETTINGS_PATH  INT_PATH(".r0n1n.settings")
#define R0N1N_SETTINGS_MAGIC (0x52)
#define R0N1N_SETTINGS_VER   (1)

static const char* const r0n1n_quick_defaults[R0N1N_QUICK_SLOTS] = {
    "NFC",
    "Sub-GHz",
    "Infrared",
    "Bad USB",
    R0N1N_APP_ARCHIVE,
    R0N1N_APP_SETTINGS,
};

void r0n1n_settings_load(R0n1nSettings* settings) {
    furi_assert(settings);
    if(!saved_struct_load(
           R0N1N_SETTINGS_PATH,
           settings,
           sizeof(R0n1nSettings),
           R0N1N_SETTINGS_MAGIC,
           R0N1N_SETTINGS_VER)) {
        FURI_LOG_I(TAG, "No settings, using defaults");
        memset(settings, 0, sizeof(R0n1nSettings));
        settings->profile = R0n1nProfileEveryday;
        for(size_t i = 0; i < R0N1N_QUICK_SLOTS; i++) {
            strlcpy(settings->quick[i], r0n1n_quick_defaults[i], R0N1N_SLOT_NAME_SIZE);
        }
    }
    if(settings->profile >= R0n1nProfileCount) settings->profile = R0n1nProfileEveryday;
}

void r0n1n_settings_save(const R0n1nSettings* settings) {
    furi_assert(settings);
    if(!saved_struct_save(
           R0N1N_SETTINGS_PATH,
           settings,
           sizeof(R0n1nSettings),
           R0N1N_SETTINGS_MAGIC,
           R0N1N_SETTINGS_VER)) {
        FURI_LOG_E(TAG, "Failed to save settings");
    }
}
