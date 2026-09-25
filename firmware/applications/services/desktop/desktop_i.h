#pragma once

#include "desktop.h"
#include "desktop_settings.h"

#include "animations/animation_manager.h"
#include "views/desktop_view_pin_timeout.h"
#include "views/desktop_view_pin_input.h"
#include "views/desktop_view_locked.h"
#include "views/desktop_view_main.h"
#include "views/desktop_view_lock_menu.h"
#include "views/desktop_view_debug.h"
#include "views/desktop_view_slideshow.h"

#include <gui/gui.h>
#include <gui/view_stack.h>
#include <gui/view_dispatcher.h>
#include <gui/modules/popup.h>
#include <gui/modules/submenu.h>
#include <gui/scene_manager.h>

#include <loader/loader.h>
#include <notification/notification_app.h>
#include <toolbox/path.h>

#define STATUS_BAR_Y_SHIFT 13

// R0N1N Home dashboard placeholder (docs/UX_DESIGN.md, docs/ROADMAP.md Stage 2):
// no Profile Manager exists yet, so the dashboard always shows this fixed name.
#define DASHBOARD_DEFAULT_PROFILE_NAME "Everyday"

// R0N1N Recent apps (docs/UX_DESIGN.md): most-recently-launched first, no
// persistence across reboot in Stage 1 -- see desktop.c/desktop_scene_recent.c.
// Entries are what the Loader was asked to start: an internal app's name or
// a full .fap path, so the length matches FavoriteApp.name_or_path -- a
// shorter buffer would truncate paths into something that can't relaunch.
#define DESKTOP_RECENT_APPS_COUNT   6
#define DESKTOP_RECENT_APP_NAME_LEN sizeof(((FavoriteApp*)NULL)->name_or_path)

typedef enum {
    DesktopViewIdMain,
    DesktopViewIdLockMenu,
    DesktopViewIdLocked,
    DesktopViewIdDebug,
    DesktopViewIdPopup,
    DesktopViewIdPinInput,
    DesktopViewIdPinTimeout,
    DesktopViewIdSlideshow,
    // R0N1N navigation law (docs/UX_DESIGN.md): Up-short opens Favorites
    // ("Quick Actions"), long-press-OK opens Recent.
    DesktopViewIdFavorites,
    DesktopViewIdRecent,
    DesktopViewIdTotal,
} DesktopViewId;

typedef struct {
    uint8_t hour;
    uint8_t minute;
    bool format_12; // 1 - 12 hour, 0 - 24H
} DesktopClock;

struct Desktop {
    FuriThread* scene_thread;

    Gui* gui;
    ViewDispatcher* view_dispatcher;
    SceneManager* scene_manager;

    Popup* popup;
    DesktopLockMenuView* lock_menu;
    DesktopDebugView* debug_view;
    DesktopViewLocked* locked_view;
    DesktopMainView* main_view;
    DesktopViewPinTimeout* pin_timeout_view;
    DesktopSlideshowView* slideshow_view;
    DesktopViewPinInput* pin_input_view;
    Submenu* favorites_submenu;
    Submenu* recent_submenu;

    ViewStack* main_view_stack;
    ViewStack* locked_view_stack;

    ViewPort* lock_icon_viewport;
    ViewPort* dummy_mode_icon_viewport;
    ViewPort* clock_viewport;
    ViewPort* stealth_mode_icon_viewport;

    Loader* loader;
    Storage* storage;
    NotificationApp* notification;

    FuriPubSub* status_pubsub;
    FuriPubSub* input_events_pubsub;
    FuriPubSubSubscription* input_events_subscription;

    FuriTimer* auto_lock_timer;
    FuriTimer* update_clock_timer;
    // R0N1N Home dashboard (desktop_view_main.c): runs only while the Main
    // scene is on screen, started/stopped in desktop_scene_main_on_enter/exit.
    FuriTimer* dashboard_update_timer;

    AnimationManager* animation_manager;
    FuriSemaphore* animation_semaphore;

    DesktopClock clock;
    DesktopSettings settings;

    // R0N1N Recent apps (docs/UX_DESIGN.md). pending_app_name is a
    // cross-thread scratch field: desktop_loader_callback (Loader's thread)
    // writes it, then blocks on animation_semaphore until the
    // DesktopGlobalBeforeAppStarted handler (ViewDispatcher's thread) has
    // copied it into launched_app_name. That copy is only pushed into
    // recent_apps on DesktopGlobalAppStopped, so launches that fail (app not
    // found, bad .fap) never show up in Recent -- the Loader only publishes
    // ApplicationStopped for an app that actually ran.
    char pending_app_name[DESKTOP_RECENT_APP_NAME_LEN];
    char launched_app_name[DESKTOP_RECENT_APP_NAME_LEN];
    char recent_apps[DESKTOP_RECENT_APPS_COUNT][DESKTOP_RECENT_APP_NAME_LEN];
    uint8_t recent_apps_count;

    bool in_transition;
    bool app_running;
    bool locked;
};

// R0N1N Quick Actions/Recent: .fap paths are too long for a 128px-wide
// list, so show just the file name without extension; internal app names
// (no leading '/') are shown as-is.
static inline void desktop_app_display_name(const char* name_or_path, FuriString* label) {
    if(name_or_path[0] == '/') {
        path_extract_filename_no_ext(name_or_path, label);
    } else {
        furi_string_set(label, name_or_path);
    }
}

void desktop_lock(Desktop* desktop);
void desktop_unlock(Desktop* desktop);
void desktop_set_dummy_mode_state(Desktop* desktop, bool enabled);
void desktop_set_stealth_mode_state(Desktop* desktop, bool enabled);
void desktop_run_archive(Desktop* desktop);
