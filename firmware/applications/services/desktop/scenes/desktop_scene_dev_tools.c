#include <furi.h>
#include <assets_icons.h>

#include "desktop_scene.h"
#include "desktop_scene_r0n1n.h"

// R0N1N Dev section: a 4x2 grid of hardware tools. Built-in ones launch
// directly; catalog apps (I2C Tools, SPI Mem, DAP Link...) are looked up
// under /ext/apps, with a hint to install them if they're missing.

void desktop_scene_dev_tools_on_enter(void* context) {
    Desktop* desktop = context;
    R0n1nGrid* grid = desktop->r0n1n_grid;

    desktop_r0n1n_prepare_grid(desktop);
    r0n1n_grid_set_title(grid, &I_R_Chip_9x7, "Dev");
    r0n1n_grid_set_layout(grid, 4, 30, 19, 12);
    for(uint32_t i = 0; i < r0n1n_dev_tools_count; i++) {
        r0n1n_grid_add_item(grid, r0n1n_dev_tools[i].icon, r0n1n_dev_tools[i].caption, false, i);
    }
    r0n1n_grid_set_selected_item(
        grid, scene_manager_get_scene_state(desktop->scene_manager, DesktopSceneDevTools));
    view_dispatcher_switch_to_view(desktop->view_dispatcher, DesktopViewIdR0n1nGrid);
}

bool desktop_scene_dev_tools_on_event(void* context, SceneManagerEvent event) {
    Desktop* desktop = context;
    if(event.type != SceneManagerEventTypeCustom) return false;
    if((event.event & R0N1N_EVT_KIND) != R0N1N_EVT_OK) return false;

    const uint32_t i = event.event & R0N1N_EVT_VALUE;
    if(i >= r0n1n_dev_tools_count) return false;

    scene_manager_set_scene_state(desktop->scene_manager, DesktopSceneDevTools, i);
    const R0n1nDevTool* tool = &r0n1n_dev_tools[i];
    if(!tool->target || !desktop_r0n1n_launch(desktop, tool->target, NULL)) {
        desktop_r0n1n_show_info(desktop, tool->hint);
    }
    return true;
}

void desktop_scene_dev_tools_on_exit(void* context) {
    UNUSED(context);
}
