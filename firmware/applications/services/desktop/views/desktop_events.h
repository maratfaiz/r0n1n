#pragma once

typedef enum {
    DesktopMainEventLock,
    // R0N1N navigation law (docs/UX_DESIGN.md): Down opens this same scene,
    // now framed as "Control Center" -- BT/silent/dummy-mode toggles paging
    // sideways into brightness/volume/vibro, unchanged from the stock lock
    // menu + quick settings, just reached from a different button.
    DesktopMainEventOpenLockMenu,
    DesktopMainEventOpenFavoriteLeftShort,
    DesktopMainEventOpenFavoriteLeftLong,
    DesktopMainEventOpenFavoriteRightShort,
    DesktopMainEventOpenFavoriteRightLong,
    // R0N1N Quick Actions (Up) and Recent (hold OK) -- see
    // desktop_scene_favorites.c / desktop_scene_recent.c. These replace
    // Archive's old dedicated Down-short shortcut (still reachable from the
    // app launcher) and the old Ok-long-launches-favorite-5 shortcut (that
    // same favorite is now one of the five entries Quick Actions lists).
    DesktopMainEventOpenFavorites,
    DesktopMainEventOpenRecent,
    DesktopMainEventOpenMenu,
    DesktopMainEventOpenDebug,
    DesktopMainEventOpenPowerOff,

    DesktopDummyEventOpenLeft,
    DesktopDummyEventOpenDown,
    DesktopDummyEventOpenOk,
    DesktopDummyEventOpenUpLong,
    DesktopDummyEventOpenDownLong,
    DesktopDummyEventOpenLeftLong,
    DesktopDummyEventOpenRightLong,
    DesktopDummyEventOpenOkLong,

    DesktopLockedEventUnlocked,
    DesktopLockedEventUpdate,
    DesktopLockedEventShowPinInput,
    DesktopLockedEventDoorsClosed,

    DesktopPinInputEventResetWrongPinLabel,
    DesktopPinInputEventUnlocked,
    DesktopPinInputEventUnlockFailed,
    DesktopPinInputEventBack,

    DesktopPinTimeoutExit,

    DesktopDebugEventToggleDebugMode,
    DesktopDebugEventExit,

    //DesktopLockMenuEventLock,
    DesktopLockMenuEventBt,
    DesktopLockMenuEventDummyModeOn,
    DesktopLockMenuEventDummyModeOff,
    DesktopLockMenuEventStealthModeOn,
    DesktopLockMenuEventStealthModeOff,
    DesktopLockMenuEventOpenQuickSettings,

    DesktopQuickSettingsEventBrightnessChanged,
    DesktopQuickSettingsEventVolumeChanged,
    DesktopQuickSettingsEventVibroChanged,
    DesktopQuickSettingsEventSave,
    DesktopQuickSettingsEventClose,

    DesktopAnimationEventCheckAnimation,
    DesktopAnimationEventNewIdleAnimation,
    DesktopAnimationEventInteractAnimation,

    DesktopSlideshowCompleted,
    DesktopSlideshowPoweroff,

    DesktopHwMismatchExit,

    DesktopEnclaveExit,

    // Global events
    DesktopGlobalBeforeAppStarted,
    DesktopGlobalAfterAppFinished,
    DesktopGlobalAutoLock,
    DesktopGlobalApiUnlock,
    DesktopGlobalSaveSettings,
    DesktopGlobalReloadSettings,
    // R0N1N Recent apps: the app announced by the last BeforeAppStarted
    // actually ran and has now exited -- see desktop.c.
    DesktopGlobalAppStopped,
} DesktopEvent;
