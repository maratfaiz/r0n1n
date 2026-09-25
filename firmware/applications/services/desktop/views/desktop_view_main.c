#include <gui/gui_i.h>
#include <gui/view.h>
#include <gui/elements.h>
#include <gui/canvas.h>
#include <furi.h>
#include <input/input.h>
#include <dolphin/dolphin.h>
#include <locale/locale.h>

#include "desktop_view_main.h"

// R0N1N Home dashboard: below the persistent status bar (battery/BT/SD are
// already drawn there system-wide by power/bt/storage -- see docs/HARDWARE.md,
// no need to duplicate them here), show a big clock, the date, and the active
// profile.
#define DASHBOARD_CLOCK_Y   38
#define DASHBOARD_DATE_Y    50
#define DASHBOARD_PROFILE_Y 62

typedef struct {
    DateTime datetime;
    char profile_name[16];
    bool has_datetime;
} DesktopMainViewModel;

struct DesktopMainView {
    View* view;
    // R0N1N Home dashboard: a *separate* draw-only View, stacked above the
    // dolphin animation in desktop.c (main_view_stack) so the clock/date/
    // profile aren't drawn over. It's kept separate from `view` (input
    // handling) rather than just reordering `view` in the stack, because
    // ViewStack's input dispatch walks the same array in reverse and stops
    // at the first consumer (view_stack.c) -- `view`'s input callback always
    // returns true, so moving it later would make it swallow input before
    // the dolphin's own view (the right-button "poke" interaction) ever
    // sees it. A View with no input callback is always skipped by that
    // dispatch regardless of position, so this one can safely sit on top.
    View* dashboard_view;
    DesktopMainViewCallback callback;
    void* context;
    FuriTimer* poweroff_timer;
    bool dummy_mode;
};

#define DESKTOP_MAIN_VIEW_POWEROFF_TIMEOUT 1300

static void desktop_main_poweroff_timer_callback(void* context) {
    DesktopMainView* main_view = context;
    main_view->callback(DesktopMainEventOpenPowerOff, main_view->context);
}

void desktop_main_set_callback(
    DesktopMainView* main_view,
    DesktopMainViewCallback callback,
    void* context) {
    furi_assert(main_view);
    furi_assert(callback);
    main_view->callback = callback;
    main_view->context = context;
}

View* desktop_main_get_view(DesktopMainView* main_view) {
    furi_assert(main_view);
    return main_view->view;
}

View* desktop_main_get_dashboard_view(DesktopMainView* main_view) {
    furi_assert(main_view);
    return main_view->dashboard_view;
}

void desktop_main_set_dummy_mode_state(DesktopMainView* main_view, bool dummy_mode) {
    furi_assert(main_view);
    main_view->dummy_mode = dummy_mode;
}

void desktop_main_update_dashboard(
    DesktopMainView* main_view,
    const DateTime* datetime,
    const char* profile_name) {
    furi_assert(main_view);
    furi_assert(datetime);
    furi_assert(profile_name);
    with_view_model(
        main_view->dashboard_view,
        DesktopMainViewModel * model,
        {
            model->datetime = *datetime;
            model->has_datetime = true;
            strlcpy(model->profile_name, profile_name, sizeof(model->profile_name));
        },
        true);
}

static void desktop_main_draw_callback(Canvas* canvas, void* model) {
    DesktopMainViewModel* m = model;
    if(!m->has_datetime) return;

    canvas_set_color(canvas, ColorBlack);

    FuriString* time_str = furi_string_alloc();
    locale_format_time(time_str, &m->datetime, locale_get_time_format(), false);
    canvas_set_font(canvas, FontBigNumbers);
    canvas_draw_str_aligned(
        canvas, 64, DASHBOARD_CLOCK_Y, AlignCenter, AlignBottom, furi_string_get_cstr(time_str));
    furi_string_free(time_str);

    FuriString* date_str = furi_string_alloc();
    locale_format_date(date_str, &m->datetime, locale_get_date_format(), "/");
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str_aligned(
        canvas, 64, DASHBOARD_DATE_Y, AlignCenter, AlignBottom, furi_string_get_cstr(date_str));
    furi_string_free(date_str);

    if(m->profile_name[0] != '\0') {
        canvas_draw_str_aligned(
            canvas, 2, DASHBOARD_PROFILE_Y, AlignLeft, AlignBottom, m->profile_name);
    }
}

bool desktop_main_input_callback(InputEvent* event, void* context) {
    furi_assert(event);
    furi_assert(context);

    DesktopMainView* main_view = context;

    if(main_view->dummy_mode == false) {
        if(event->type == InputTypeShort) {
            if(event->key == InputKeyOk) {
                main_view->callback(DesktopMainEventOpenMenu, main_view->context);
            } else if(event->key == InputKeyUp) {
                // R0N1N navigation law (docs/UX_DESIGN.md): Up = Quick Actions.
                main_view->callback(DesktopMainEventOpenFavorites, main_view->context);
            } else if(event->key == InputKeyDown) {
                // Down = Control Center (the stock lock menu + quick
                // settings, unchanged, just reached from a different button).
                main_view->callback(DesktopMainEventOpenLockMenu, main_view->context);
            } else if(event->key == InputKeyLeft) {
                main_view->callback(DesktopMainEventOpenFavoriteLeftShort, main_view->context);
            }
            // Right key short is handled by animation manager
        } else if(event->type == InputTypeLong) {
            if(event->key == InputKeyUp) {
                main_view->callback(DesktopMainEventLock, main_view->context);
            } else if(event->key == InputKeyDown) {
                main_view->callback(DesktopMainEventOpenDebug, main_view->context);
            } else if(event->key == InputKeyLeft) {
                main_view->callback(DesktopMainEventOpenFavoriteLeftLong, main_view->context);
            } else if(event->key == InputKeyRight) {
                main_view->callback(DesktopMainEventOpenFavoriteRightLong, main_view->context);
            } else if(event->key == InputKeyOk) {
                if(furi_hal_rtc_is_flag_set(FuriHalRtcFlagDebug)) {
                    main_view->callback(DesktopAnimationEventNewIdleAnimation, main_view->context);
                } else {
                    // R0N1N navigation law: hold OK = Recent.
                    main_view->callback(DesktopMainEventOpenRecent, main_view->context);
                }
            }
        }
    } else {
        if(event->type == InputTypeShort) {
            if(event->key == InputKeyOk) {
                main_view->callback(DesktopDummyEventOpenOk, main_view->context);
            } else if(event->key == InputKeyUp) {
                main_view->callback(DesktopMainEventOpenLockMenu, main_view->context);
            } else if(event->key == InputKeyDown) {
                main_view->callback(DesktopDummyEventOpenDown, main_view->context);
            } else if(event->key == InputKeyLeft) {
                main_view->callback(DesktopDummyEventOpenLeft, main_view->context);
            }
            // Right key short is handled by animation manager
        } else if(event->type == InputTypeLong) {
            if(event->key == InputKeyOk) {
                // Not working in DummyMode
                // if(furi_hal_rtc_is_flag_set(FuriHalRtcFlagDebug)) {
                //     main_view->callback(DesktopAnimationEventNewIdleAnimation, main_view->context);
                // }
                main_view->callback(DesktopDummyEventOpenOkLong, main_view->context);
            } else if(event->key == InputKeyUp) {
                main_view->callback(DesktopDummyEventOpenUpLong, main_view->context);
            } else if(event->key == InputKeyDown) {
                main_view->callback(DesktopDummyEventOpenDownLong, main_view->context);
            } else if(event->key == InputKeyLeft) {
                main_view->callback(DesktopDummyEventOpenLeftLong, main_view->context);
            } else if(event->key == InputKeyRight) {
                main_view->callback(DesktopDummyEventOpenRightLong, main_view->context);
            }
        }
    }

    if(event->key == InputKeyBack) {
        if(event->type == InputTypePress) {
            furi_timer_start(main_view->poweroff_timer, DESKTOP_MAIN_VIEW_POWEROFF_TIMEOUT);
        } else if(event->type == InputTypeRelease) {
            furi_timer_stop(main_view->poweroff_timer);
        }
    }

    return true;
}

DesktopMainView* desktop_main_alloc(void) {
    DesktopMainView* main_view = malloc(sizeof(DesktopMainView));

    main_view->view = view_alloc();
    view_set_context(main_view->view, main_view);
    view_set_input_callback(main_view->view, desktop_main_input_callback);

    // Draw-only, deliberately not given an input callback -- see the
    // dashboard_view comment on the struct above.
    main_view->dashboard_view = view_alloc();
    view_allocate_model(
        main_view->dashboard_view, ViewModelTypeLocking, sizeof(DesktopMainViewModel));
    view_set_draw_callback(main_view->dashboard_view, desktop_main_draw_callback);

    main_view->poweroff_timer =
        furi_timer_alloc(desktop_main_poweroff_timer_callback, FuriTimerTypeOnce, main_view);

    return main_view;
}

void desktop_main_free(DesktopMainView* main_view) {
    furi_assert(main_view);
    view_free(main_view->view);
    view_free(main_view->dashboard_view);
    furi_timer_free(main_view->poweroff_timer);
    free(main_view);
}
