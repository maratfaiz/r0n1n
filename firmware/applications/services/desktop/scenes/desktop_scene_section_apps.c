#include <furi.h>

#include "desktop_scene.h"
#include "desktop_scene_r0n1n.h"

// Apps of one section (desktop->section), opened from the sections carousel
// or the Applications menu.

static const char* const desktop_section_not_installed =
    "Приложения нет на SD.\nУстановите его из каталога\nFlipper Apps.";

void desktop_scene_section_apps_on_enter(void* context) {
    Desktop* desktop = context;
    const R0n1nSectionInfo* section = &r0n1n_sections[desktop->section];

    desktop_r0n1n_prepare_list(desktop);
    r0n1n_list_set_title(desktop->r0n1n_list, section->icon, section->title);
    for(uint32_t i = 0; i < section->app_count; i++) {
        r0n1n_list_add_item(
            desktop->r0n1n_list,
            section->apps[i].icon,
            section->apps[i].label,
            NULL,
            NULL,
            NULL,
            i);
    }
    view_dispatcher_switch_to_view(desktop->view_dispatcher, DesktopViewIdR0n1nList);
}

bool desktop_scene_section_apps_on_event(void* context, SceneManagerEvent event) {
    Desktop* desktop = context;
    if(event.type != SceneManagerEventTypeCustom) return false;
    if((event.event & R0N1N_EVT_KIND) != R0N1N_EVT_OK) return false;

    const R0n1nSectionInfo* section = &r0n1n_sections[desktop->section];
    const uint32_t i = event.event & R0N1N_EVT_VALUE;
    if(i < section->app_count) {
        const char* name = section->apps[i].name;
        // .fap entries (e.g. the HID remotes) live on SD and may be missing.
        if(name[0] == '/' && !storage_file_exists(desktop->storage, name)) {
            desktop_r0n1n_show_info(desktop, desktop_section_not_installed);
        } else {
            desktop_r0n1n_launch(desktop, name, NULL);
        }
    }
    return true;
}

void desktop_scene_section_apps_on_exit(void* context) {
    UNUSED(context);
}
