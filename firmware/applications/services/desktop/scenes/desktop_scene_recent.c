#include <furi.h>
#include <gui/modules/submenu.h>
#include <loader/loader.h>

#include "../desktop_i.h"
#include "desktop_scene.h"

// R0N1N Recent apps (docs/UX_DESIGN.md), reached by holding OK. Tracking is
// populated in desktop.c (desktop_recent_apps_push, fed from the Loader's
// LoaderEventTypeApplicationBeforeLoad -- see loader.h/loader.c for the
// event.name field this needed). In-memory only for Stage 1: the list is
// empty again after a reboot, no persistence yet.
static void desktop_scene_recent_submenu_callback(void* context, uint32_t index) {
    Desktop* desktop = context;
    view_dispatcher_send_custom_event(desktop->view_dispatcher, index);
}

void desktop_scene_recent_on_enter(void* context) {
    Desktop* desktop = context;
    Submenu* submenu = desktop->recent_submenu;
    submenu_reset(submenu);

    if(desktop->recent_apps_count == 0) {
        submenu_add_item(submenu, "No recent apps yet", 0, NULL, NULL);
    } else {
        for(uint8_t i = 0; i < desktop->recent_apps_count; i++) {
            submenu_add_item(
                submenu,
                desktop->recent_apps[i],
                i,
                desktop_scene_recent_submenu_callback,
                desktop);
        }
    }

    view_dispatcher_switch_to_view(desktop->view_dispatcher, DesktopViewIdRecent);
}

bool desktop_scene_recent_on_event(void* context, SceneManagerEvent event) {
    Desktop* desktop = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        if(desktop->recent_apps_count > 0) {
            furi_assert(event.event < desktop->recent_apps_count);
            loader_start_detached_with_gui_error(
                desktop->loader, desktop->recent_apps[event.event], NULL);
        }
        scene_manager_previous_scene(desktop->scene_manager);
        consumed = true;
    }

    return consumed;
}

void desktop_scene_recent_on_exit(void* context) {
    Desktop* desktop = context;
    submenu_reset(desktop->recent_submenu);
}
