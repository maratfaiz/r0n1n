#include <furi.h>
#include <gui/modules/submenu.h>
#include <loader/loader.h>

#include "../desktop_i.h"
#include "desktop_scene.h"

// R0N1N Quick Actions (docs/UX_DESIGN.md): reuses the existing FavoriteApp
// slots (configured today under Settings > Desktop) as a single list reached
// from Up-short, instead of each slot only being reachable by its own D-pad
// shortcut. Stage 1 deliberately doesn't add a separate/customizable
// quick-action storage -- it lists the same five slots that already exist.
static const char* const desktop_favorites_labels[FavoriteAppNumber] = {
    [FavoriteAppLeftShort] = "Left",
    [FavoriteAppLeftLong] = "Left (hold)",
    [FavoriteAppRightShort] = "Right",
    [FavoriteAppRightLong] = "Right (hold)",
    [FavoriteAppOkLong] = "OK (hold)",
};

static inline bool desktop_scene_favorites_check_none(const char* str) {
    return (str[1] == '\0' && str[0] == '?');
}

static void desktop_scene_favorites_submenu_callback(void* context, uint32_t index) {
    Desktop* desktop = context;
    view_dispatcher_send_custom_event(desktop->view_dispatcher, index);
}

void desktop_scene_favorites_on_enter(void* context) {
    Desktop* desktop = context;
    Submenu* submenu = desktop->favorites_submenu;
    submenu_reset(submenu);

    FuriString* app_name = furi_string_alloc();
    for(size_t i = 0; i < FavoriteAppNumber; i++) {
        const char* path = desktop->settings.favorite_apps[i].name_or_path;
        desktop_app_display_name(path, app_name);
        FuriString* label = furi_string_alloc_printf(
            "%s: %s",
            desktop_favorites_labels[i],
            (strlen(path) == 0)                      ? "Apps Menu" :
            desktop_scene_favorites_check_none(path) ? "(none)" :
                                                       furi_string_get_cstr(app_name));
        submenu_add_item(
            submenu,
            furi_string_get_cstr(label),
            i,
            desktop_scene_favorites_submenu_callback,
            desktop);
        furi_string_free(label);
    }
    furi_string_free(app_name);

    view_dispatcher_switch_to_view(desktop->view_dispatcher, DesktopViewIdFavorites);
}

bool desktop_scene_favorites_on_event(void* context, SceneManagerEvent event) {
    Desktop* desktop = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        furi_assert(event.event < FavoriteAppNumber);
        const char* path = desktop->settings.favorite_apps[event.event].name_or_path;
        if(strlen(path) == 0) {
            loader_start_detached_with_gui_error(desktop->loader, LOADER_APPLICATIONS_NAME, NULL);
        } else if(!desktop_scene_favorites_check_none(path)) {
            loader_start_detached_with_gui_error(desktop->loader, path, NULL);
        }
        scene_manager_previous_scene(desktop->scene_manager);
        consumed = true;
    }

    return consumed;
}

void desktop_scene_favorites_on_exit(void* context) {
    Desktop* desktop = context;
    submenu_reset(desktop->favorites_submenu);
}
