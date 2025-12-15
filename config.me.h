/* Taken from https://github.com/djpohly/dwl/issues/466 */
#define COLOR(hex)    { ((hex >> 24) & 0xFF) / 255.0f, \
                        ((hex >> 16) & 0xFF) / 255.0f, \
                        ((hex >> 8) & 0xFF) / 255.0f, \
                        (hex & 0xFF) / 255.0f }
/* appearance */
static const int sloppyfocus               = 1;  /* focus follows mouse */
static const int bypass_surface_visibility = 0;  /* 1 means idle inhibitors will disable idle tracking even if it's surface isn't visible  */
static const int smartgaps                 = 0;  /* 1 means no outer gap when there is only one window */
static int gaps                            = 0;  /* 1 means gaps between windows are added */
static const unsigned int gappx            = 10; /* gap pixel between windows */
static const unsigned int borderpx         = 1;  /* border pixel of windows */
static const int draw_minimal_borders      = 1; /* merge adjacent borders */
static const int showbar                   = 1; /* 0 means no bar */
static const int topbar                    = 1; /* 0 means bottom bar */
static const char *fonts[]                 = {"Iosevka Nerd Font:size=14"};
static const float rootcolor[]             = COLOR(0x000000ff);
/* This conforms to the xdg-protocol. Set the alpha to zero to restore the old behavior */
static const float fullscreen_bg[]         = {0.1f, 0.1f, 0.1f, 0.0f}; /* You can also use glsl colors */
static const int respect_monitor_reserved_area = 0;  /* 1 to monitor center while respecting the monitor's reserved area, 0 to monitor center */
static const char *cursor_theme            = "Adwaita";
static const char cursor_size[]            = "24"; /* Make sure it's a valid integer, otherwise things will break */
static const int opacity = 1; /* flag to enable opacity */
static const float opacity_inactive = 0.97;
static const float opacity_active = 0.97;

static const int shadow = 1; /* flag to enable shadow */
static const int shadow_only_floating = 1; /* only apply shadow to floating windows */
static const float shadow_color[4] = COLOR(0x00000055);
static const float shadow_color_focus[4] = COLOR(0x00000055);
static const int shadow_blur_sigma = 10;
static const int shadow_blur_sigma_focus = 10;
static const char *const shadow_ignore_list[] = { NULL }; /* list of app-id to ignore */

static const int corner_radius = 8; /* 0 disables corner_radius */
static const int corner_radius_inner = 8; /* 0 disables corner_radius */
static const int corner_radius_only_floating = 1; /* only apply corner_radius and corner_radius_inner to floating windows */

static const int blur = 1; /* flag to enable blur */
static const int blur_xray = 0; /* flag to make transparent fs and floating windows display your background */
static const int blur_ignore_transparent = 1;
static const struct blur_data blur_data = {
    /* default */
	.radius = 5,
	.num_passes = 3,
	.noise = (float)0.02,
	.brightness = (float)0.9,
	.contrast = (float)0.9,
	.saturation = (float)1.1,
// 	.radius = 3,
// 	.num_passes = 3,
// 	.noise = (float)0.017,
// 	.brightness = (float)0.8172,
// 	.contrast = (float)0.8916,
// 	.saturation = (float)0.1696,
};
static uint32_t colors[][3]                = {
	/*               fg          bg          border    */
	[SchemeNorm] = { 0xbbbbbbff, 0x222222ff, 0x444444ff },
	[SchemeSel]  = { 0xeeeeeeff, 0x005577ff, 0x005577ff },
	[SchemeUrg]  = { 0,          0,          0x770000ff },
};

/* tagging */
static char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

/* logging */
static int log_level = WLR_ERROR;

static const Env envs[] = {
	/* variable			value */
    { "DISPLAY",             ":0"        },
    { "XDG_SESSION_TYPE",    "wayland"   },
    { "QT_QPA_PLATFORM",     "wayland"   },
    { "GDK_BACKEND",         "wayland"   },
	{ "XDG_CURRENT_DESKTOP", "wlroots"   },
	{ "XDG_SESSION_DESKTOP", "wlroots"   },
    { "INPUT_METHOD",        "fcitx"     },
    { "QT_IM_MODULE",        "fcitx"     },
    { "XMODIFIER",           "@im=fcitx" },
	{ "MOZ_ENABLE_WAYLAND",  "1"         },
};

/* window resizing */
/* resize_corner:
 * 0: top-left
 * 1: top-right
 * 2: bottom-left
 * 3: bottom-right
 * 4: closest to the cursor
 */
static const int resize_corner = 4;
static const int warp_cursor = 0;	/* 1: warp to corner, 0: don’t warp */
static const int lock_cursor = 0;	/* 1: lock cursor, 0: don't lock */

/* NOTE: ALWAYS keep a rule declared even if you don't use rules (e.g leave at least one example) */
static const Rule rules[] = {
	/* app_id             title       tags mask     isfloating   monitor */
	{ "com.mitchellh.ghostty", NULL,    0,            0,           -1, 0,   0,   0,   0,   0.94 }, /* Start on currently visible tags floating, not tiled */
	{ "satty",            NULL,         0,            1,           -1, 0.5,   0.5,   0.5,   0,   1.0 }, /* Start on currently visible tags floating, not tiled */
	{ "TUI.float",        NULL,         0,            1,           -1, 0.5, 0.5, 0.7, 0.7, 0.9 }, /* Start on currently visible tags floating, not tiled */
	{ "xdg-desktop-portal-gtk", NULL,   0,       1,           -1, 0.5, 0.5, 0.7, 0.7, 1.0 }, /* Start on currently visible tags floating, not tiled */
	{ NULL,               "Picture in picture",  0,   1,           -1, 0.99, 0.99, 0.25, 0.25, 1.0 }, /* Start on currently visible tags floating, not tiled */
};

/* layout(s) */
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* monitors */
/* (x=-1, y=-1) is reserved as an "autoconfigure" monitor position indicator
 * WARNING: negative values other than (-1, -1) cause problems with Xwayland clients
 * https://gitlab.freedesktop.org/xorg/xserver/-/issues/899
*/
/* NOTE: ALWAYS add a fallback rule, even if you are completely sure it won't be used */
static const MonitorRule monrules[] = {
	/* name       mfact  nmaster scale layout       rotate/reflect                x    y */
	/* example of a HiDPI laptop monitor:
	{ "eDP-1",    0.5f,  1,      2,    &layouts[0], WL_OUTPUT_TRANSFORM_NORMAL,   -1,  -1 },
	*/
	/* defaults */
	{ NULL,       0.5f, 1,      1.4,    &layouts[0], WL_OUTPUT_TRANSFORM_NORMAL,   -1,  -1 },
};

/* keyboard */
static const struct xkb_rule_names xkb_rules = {
	/* can specify fields: rules, model, layout, variant, options */
	/* example:
	.options = "ctrl:nocaps",
	*/
	.options = NULL,
};

static const int repeat_rate = 25;
static const int repeat_delay = 600;

/* Trackpad */
static const int tap_to_click = 1;
static const int tap_and_drag = 1;
static const int drag_lock = 1;
static const int natural_scrolling = 1;
static const int disable_while_typing = 1;
static const int left_handed = 0;
static const int middle_button_emulation = 0;
/* You can choose between:
LIBINPUT_CONFIG_SCROLL_NO_SCROLL
LIBINPUT_CONFIG_SCROLL_2FG
LIBINPUT_CONFIG_SCROLL_EDGE
LIBINPUT_CONFIG_SCROLL_ON_BUTTON_DOWN
*/
static const enum libinput_config_scroll_method scroll_method = LIBINPUT_CONFIG_SCROLL_2FG;

/* You can choose between:
LIBINPUT_CONFIG_CLICK_METHOD_NONE
LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS
LIBINPUT_CONFIG_CLICK_METHOD_CLICKFINGER
*/
static const enum libinput_config_click_method click_method = LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS;

/* You can choose between:
LIBINPUT_CONFIG_SEND_EVENTS_ENABLED
LIBINPUT_CONFIG_SEND_EVENTS_DISABLED
LIBINPUT_CONFIG_SEND_EVENTS_DISABLED_ON_EXTERNAL_MOUSE
*/
static const uint32_t send_events_mode = LIBINPUT_CONFIG_SEND_EVENTS_ENABLED;

/* You can choose between:
LIBINPUT_CONFIG_ACCEL_PROFILE_FLAT
LIBINPUT_CONFIG_ACCEL_PROFILE_ADAPTIVE
*/
static const enum libinput_config_accel_profile accel_profile = LIBINPUT_CONFIG_ACCEL_PROFILE_ADAPTIVE;
static const double accel_speed = 0.0;

/* You can choose between:
LIBINPUT_CONFIG_TAP_MAP_LRM -- 1/2/3 finger tap maps to left/right/middle
LIBINPUT_CONFIG_TAP_MAP_LMR -- 1/2/3 finger tap maps to left/middle/right
*/
static const enum libinput_config_tap_button_map button_map = LIBINPUT_CONFIG_TAP_MAP_LRM;

/* If you want to use the windows key for MODKEY, use WLR_MODIFIER_LOGO */
#define MODKEY WLR_MODIFIER_LOGO

#define TAGKEYS(KEY,SKEY,TAG) \
	{ MODKEY,                    KEY,            view,            {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_CTRL,  KEY,            toggleview,      {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_SHIFT, SKEY,           tag,             {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_CTRL|WLR_MODIFIER_SHIFT,SKEY,toggletag, {.ui = 1 << TAG} }

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *termcmd[] = { "ghostty", NULL };
static const char *volumeUp[] = {"/bin/sh", "-c", "wpctl set-volume @DEFAULT_AUDIO_SINK@ 5%+ \
    && current_volume=$(echo \"$(wpctl get-volume @DEFAULT_AUDIO_SINK@) 100\" | awk '{print $2 * $3}') \
    && notify-send -h string:x-canonical-private-synchronous:volume_notification -h int:value:\"$current_volume\" \"Volume set to ${current_volume}%\"",
             NULL };
static const char *volumeDown[] = {"/bin/sh", "-c", "wpctl set-volume @DEFAULT_AUDIO_SINK@ 5%- \
    && current_volume=$(echo \"$(wpctl get-volume @DEFAULT_AUDIO_SINK@) 100\" | awk '{print $2 * $3}') \
    && notify-send -h string:x-canonical-private-synchronous:volume_notification -h int:value:\"$current_volume\" \"Volume set to ${current_volume}%\"",
        NULL };
static const char *brightUp[] = {"/bin/sh", "-c", "brightnessctl s 5%+ \
    && max_brightness=$(brightnessctl max) \
    && current_brightness=$(brightnessctl gget) \
    && percentage=$((current_brightness * 100 / max_brightness)) \
    && notify-send -h string:x-canonical-private-synchronous:brightness_notification \
        -h int:value:\"$percentage\" \"Brightness set to ${percentage}%\"",
             NULL };
static const char *brightDown[] = {"/bin/sh", "-c", "brightnessctl s 5%- \
    && max_brightness=$(brightnessctl max) \
    && current_brightness=$(brightnessctl gget) \
    && percentage=$((current_brightness * 100 / max_brightness)) \
    && notify-send -h string:x-canonical-private-synchronous:brightness_notification \
        -h int:value:\"$percentage\" \"Brightness set to ${percentage}%\"",
             NULL };

static const Key keys[] = {
	/* Note that Shift changes certain key codes: c -> C, 2 -> at, etc. */
	/* modifier                  key                 function        argument */
	{ MODKEY,                    XKB_KEY_space,      spawn,          SHCMD("j4-dmenu-desktop --no-generic --dmenu='wmenu -i -f \"Iosevka Nerd Font 14\"'") },
	{ MODKEY,                    XKB_KEY_t,          spawn,          {.v = termcmd} },
	{ MODKEY,                    XKB_KEY_b,          spawn,          SHCMD("brave-browser-nightly")},
	{ MODKEY,                    XKB_KEY_d,          spawn,          SHCMD("brave-browser-nightly --new-window --app='https://discord.com/channels/@me'")},
	{ MODKEY,                    XKB_KEY_g,          spawn,          SHCMD("brave-browser-nightly --new-window --app='https://grok.com/'")},
	{ 0,                         XKB_KEY_Print,      spawn,          SHCMD("sh ~/.config/dwl/screenshot.sh")},
    { MODKEY,                    XKB_KEY_Print,      spawn,          SHCMD("sh ~/.config/dwl/screenrec.sh")},
	{ MODKEY,                    XKB_KEY_i,          spawn,          SHCMD("ghostty --class='TUI.float' --command='bluetui'")},
	{ MODKEY,                    XKB_KEY_a,          spawn,          SHCMD("ghostty --class='TUI.float' --command='wiremix'")},
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_L,          spawn,          SHCMD("hyprlock")},
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_M,          spawn,          SHCMD("wl-mirror eDP-1")},
	// { MODKEY,                    XKB_KEY_b,          togglebar,      {0} },
	{ MODKEY,                    XKB_KEY_j,          focusstack,     {.i = +1} },
	{ MODKEY,                    XKB_KEY_k,          focusstack,     {.i = -1} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_J,          movestack,      {.i = +1} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_K,          movestack,      {.i = -1} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_I,          incnmaster,     {.i = +1} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_D,          incnmaster,     {.i = -1} },
	{ MODKEY,                    XKB_KEY_h,          setmfact,       {.f = -0.05f} },
	{ MODKEY,                    XKB_KEY_l,          setmfact,       {.f = +0.05f} },
	{ MODKEY,                    XKB_KEY_minus,      moveresizekb,   {.v = (int []){ 0, 0, -80, 0 }}},
	{ MODKEY,                    XKB_KEY_equal,      moveresizekb,   {.v = (int []){ 0, 0, 80, 0 }}},
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_underscore, moveresizekb,   {.v = (int []){ 0, 0, 0, -45 }}},
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_plus,       moveresizekb,   {.v = (int []){ 0, 0, 0, 45 }}},
	{ MODKEY,                    XKB_KEY_z,          zoom,           {0} },
	{ MODKEY,                    XKB_KEY_Tab,        view,           {0} },
	{ MODKEY,                    XKB_KEY_q,          killclient,     {0} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_t,          setlayout,      {.v = &layouts[0]} },
	// { MODKEY,                    XKB_KEY_f,          setlayout,      {.v = &layouts[1]} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_m,          setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                    XKB_KEY_v,          togglefloating, {0} },
	{ MODKEY,                    XKB_KEY_f,          togglefullscreen, {0} },
	{ MODKEY,                    XKB_KEY_0,          view,           {.ui = ~0} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_parenright, tag,            {.ui = ~0} },
	{ MODKEY,                    XKB_KEY_comma,      focusmon,       {.i = WLR_DIRECTION_LEFT} },
	{ MODKEY,                    XKB_KEY_period,     focusmon,       {.i = WLR_DIRECTION_RIGHT} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_less,       tagmon,         {.i = WLR_DIRECTION_LEFT} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_greater,    tagmon,         {.i = WLR_DIRECTION_RIGHT} },
	TAGKEYS(          XKB_KEY_1, XKB_KEY_exclam,                     0),
	TAGKEYS(          XKB_KEY_2, XKB_KEY_at,                         1),
	TAGKEYS(          XKB_KEY_3, XKB_KEY_numbersign,                 2),
	TAGKEYS(          XKB_KEY_4, XKB_KEY_dollar,                     3),
	TAGKEYS(          XKB_KEY_5, XKB_KEY_percent,                    4),
	TAGKEYS(          XKB_KEY_6, XKB_KEY_asciicircum,                5),
	TAGKEYS(          XKB_KEY_7, XKB_KEY_ampersand,                  6),
	TAGKEYS(          XKB_KEY_8, XKB_KEY_asterisk,                   7),
	TAGKEYS(          XKB_KEY_9, XKB_KEY_parenleft,                  8),
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_E,          spawn,          SHCMD("wlogout")},
    { 0,              XKB_KEY_XF86AudioRaiseVolume,  spawn,          {.v = volumeUp }},
    { 0,              XKB_KEY_XF86AudioLowerVolume,  spawn,          {.v = volumeDown}},
    { 0,              XKB_KEY_XF86AudioMute,         spawn,          SHCMD("wpctl set-mute @DEFAULT_AUDIO_SINK@ toggle")},
    { 0,              XKB_KEY_XF86AudioMicMute,      spawn,          SHCMD("wpctl set-mute @DEFAULT_AUDIO_SOURCE@ toggle")},
    { 0,              XKB_KEY_XF86MonBrightnessUp,   spawn,          {.v = brightUp}},
    { 0,              XKB_KEY_XF86MonBrightnessDown, spawn,          {.v = brightDown}},
    { 0,              XKB_KEY_XF86AudioNext,         spawn,          SHCMD("playerctl next")},
    { 0,              XKB_KEY_XF86AudioPause,        spawn,          SHCMD("playerctl play-pause")},
    { 0,              XKB_KEY_XF86AudioPlay,         spawn,          SHCMD("playerctl play-pause")},
    { 0,              XKB_KEY_XF86AudioPrev,         spawn,          SHCMD("playerctl previous")},


	/* Ctrl-Alt-Backspace and Ctrl-Alt-Fx used to be handled by X server */
	{ WLR_MODIFIER_CTRL|WLR_MODIFIER_ALT,XKB_KEY_Terminate_Server, quit, {0} },
	/* Ctrl-Alt-Fx is used to switch to another VT, if you don't know what a VT is
	 * do not remove them.
	 */
#define CHVT(n) { WLR_MODIFIER_CTRL|WLR_MODIFIER_ALT,XKB_KEY_XF86Switch_VT_##n, chvt, {.ui = (n)} }
	CHVT(1), CHVT(2), CHVT(3), CHVT(4), CHVT(5), CHVT(6),
	CHVT(7), CHVT(8), CHVT(9), CHVT(10), CHVT(11), CHVT(12),
};

static const Button buttons[] = {
	{ ClkLtSymbol, 0,      BTN_LEFT,   setlayout,      {.v = &layouts[0]} },
	{ ClkLtSymbol, 0,      BTN_RIGHT,  setlayout,      {.v = &layouts[2]} },
	{ ClkTitle,    0,      BTN_MIDDLE, zoom,           {0} },
	{ ClkStatus,   0,      BTN_MIDDLE, spawn,          {.v = termcmd} },
	{ ClkClient,   MODKEY, BTN_LEFT,   moveresize,     {.ui = CurMove} },
	{ ClkClient,   MODKEY, BTN_MIDDLE, togglefloating, {0} },
	{ ClkClient,   MODKEY, BTN_RIGHT,  moveresize,     {.ui = CurResize} },
	{ ClkTagBar,   0,      BTN_LEFT,   view,           {0} },
	{ ClkTagBar,   0,      BTN_RIGHT,  toggleview,     {0} },
	{ ClkTagBar,   MODKEY, BTN_LEFT,   tag,            {0} },
	{ ClkTagBar,   MODKEY, BTN_RIGHT,  toggletag,      {0} },
};
