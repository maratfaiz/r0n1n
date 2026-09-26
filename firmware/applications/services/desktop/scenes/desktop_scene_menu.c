#include <furi.h>
#include <assets_icons.h>

#include "desktop_scene.h"
#include "desktop_scene_r0n1n.h"

// R0N1N Applications menu (OK on Home, docs/UX_DESIGN.md): the sections as
// categories, then the shell's own screens -- everything is reachable from
// here even if a profile hides it from the Left/Right sections.

typedef enum {
    MenuFiles = R0n1nSectionCount,
    MenuCaptures,
    MenuSearch,
    MenuHub,
    MenuAllApps,
    MenuSettings,
} MenuItem;

void desktop_scene_menu_on_enter(void* context) {
    Desktop* desktop = context;
    R0n1nList* list = desktop->r0n1n_list;

    desktop_r0n1n_prepare_list(desktop);
    r0n1n_list_set_title(list, NULL, "Приложения");
    for(uint32_t i = 0; i < R0n1nSectionCount; i++) {
        r0n1n_list_add_item(
            list, r0n1n_sections[i].icon, r0n1n_sections[i].title, NULL, NULL, NULL, i);
    }
    r0n1n_list_add_item(list, &I_dir_10px, "Файлы", NULL, NULL, NULL, MenuFiles);
    r0n1n_list_add_item(list, &I_R_Clock_9x8, "Захваты", NULL, NULL, NULL, MenuCaptures);
    r0n1n_list_add_item(list, &I_R_Search_8x8, "Поиск", NULL, NULL, NULL, MenuSearch);
    r0n1n_list_add_item(list, &I_R_Download_8x8, "R0N1N Hub", NULL, NULL, NULL, MenuHub);
    r0n1n_list_add_item(list, &I_R_Star_9x7, "Все приложения", NULL, NULL, NULL, MenuAllApps);
    r0n1n_list_add_item(list, &I_settings_10px, "Настройки", NULL, NULL, NULL, MenuSettings);

    r0n1n_list_set_selected_item(
        list, scene_manager_get_scene_state(desktop->scene_manager, DesktopSceneMenu));
    view_dispatcher_switch_to_view(desktop->view_dispatcher, DesktopViewIdR0n1nList);
}

bool desktop_scene_menu_on_event(void* context, SceneManagerEvent event) {
    Desktop* desktop = context;
    if(event.type != SceneManagerEventTypeCustom) return false;
    if((event.event & R0N1N_EVT_KIND) != R0N1N_EVT_OK) return false;

    const uint32_t item = event.event & R0N1N_EVT_VALUE;
    scene_manager_set_scene_state(desktop->scene_manager, DesktopSceneMenu, item);

    if(item < R0n1nSectionCount) {
        desktop->section = item;
        scene_manager_next_scene(
            desktop->scene_manager,
            item == R0n1nSectionDev ? DesktopSceneDevTools : DesktopSceneSectionApps);
    } else if(item == MenuFiles) {
        desktop_r0n1n_launch(desktop, R0N1N_APP_ARCHIVE, NULL);
    } else if(item == MenuCaptures) {
        scene_manager_next_scene(desktop->scene_manager, DesktopSceneCaptures);
    } else if(item == MenuSearch) {
        scene_manager_next_scene(desktop->scene_manager, DesktopSceneSearch);
    } else if(item == MenuHub) {
        scene_manager_next_scene(desktop->scene_manager, DesktopSceneHub);
    } else if(item == MenuAllApps) {
        desktop_r0n1n_launch(desktop, R0N1N_APP_ALL, NULL);
    } else if(item == MenuSettings) {
        scene_manager_next_scene(desktop->scene_manager, DesktopSceneR0n1nSettings);
    }
    return true;
}

void desktop_scene_menu_on_exit(void* context) {
    UNUSED(context);
}
