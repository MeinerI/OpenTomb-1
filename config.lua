-- LUA config file
screen =
{
    x = 50;
    y = 20;
    width = 1024;
    height = 640;
    fov = 75.0;
    debug_view_state = 0;
    fullscreen = 0;
    crosshair = 1;
}

audio =
{
    sound_volume = 0.8;
    music_volume = 0.9;
    use_effects = 1;
    listener_is_player = 0;
}

render =
{
    mipmap_mode = 3;
    mipmaps = 3;                                -- It's not recommended to set it higher than 3 to prevent border bleeding.
    lod_bias = 0;
    anisotropy = 4;                             -- Maximum depends and is limited by hardware capabilities.
    antialias = 1;
    antialias_samples = 4;                      -- Maximum depends and is limited by hardware capabilities.
    z_depth = 24;                               -- Maximum and recommended is 24.
    texture_border = 16;
    fog_color = {r = 255, g = 255, b = 255};
}

controls =
{
    mouse_sensitivity_x = 0.25;                 -- to inverse mouse axis use negative values
    mouse_sensitivity_y = 0.25;

    use_joy = 0;                                -- Use joystick - yes (1) or no (0)
    joy_number = 0;                             -- If you have one joystick in system, it will be 0.
    joy_rumble = 0;                             -- Force feedback

    joy_move_axis_x = 0;                        -- Movement axes options.
    joy_move_axis_y = 1;
    joy_move_invert_x = 0;
    joy_move_invert_y = 0;
    joy_move_sensitivity = 1.5;
    joy_move_deadzone = 1500;

    joy_look_axis_x = 2;                        -- Look axes options.
    joy_look_axis_y = 3;
    joy_look_invert_x = 0;
    joy_look_invert_y = 1;
    joy_look_sensitivity = 1.5;
    joy_look_deadzone = 1500;
}

console =
{
    background_color = {r = 0, g = 0, b = 0, a = 200};

    lines_count = 128;
    commands_count = 128;
    height = 400;

    spacing = 0.5;
    show_cursor_period = 0.5;
    show = 0;
}

-- Keys binding
-- Please note that on XInput game controllers (XBOX360 and such), triggers are NOT
-- coded as joystick buttons. Instead, they have unique names: JOY_TRIGGERLEFT and
-- JOY_TRIGGERRIGHT.

dofile(base_path .. "scripts/config/control_constants.lua");

bind(ACT_JUMP, KEY_SPACE, JOY_3);
bind(ACT_ACTION, KEY_LCTRL, JOY_1);
bind(ACT_DRAWWEAPON, KEY_F);
bind(ACT_ROLL, KEY_X, JOY_2);
bind(ACT_SPRINT, KEY_CAPSLOCK, JOY_TRIGGERRIGHT);
bind(ACT_CROUCH, KEY_V, JOY_TRIGGERLEFT);
bind(ACT_WALK, KEY_LSHIFT, JOY_11);

bind(ACT_UP, KEY_W);
bind(ACT_DOWN, KEY_S);
bind(ACT_LEFT, KEY_A);
bind(ACT_RIGHT, KEY_D);

bind(ACT_LOOK, KEY_O);
bind(ACT_LOOKUP, KEY_UP, JOY_POVUP);
bind(ACT_LOOKDOWN, KEY_DOWN, JOY_POVDOWN);
bind(ACT_LOOKLEFT, KEY_LEFT, JOY_POVLEFT);
bind(ACT_LOOKRIGHT, KEY_RIGHT, JOY_POVRIGHT);

bind(ACT_INVENTORY, KEY_ESCAPE);
bind(ACT_SCREENSHOT, KEY_PRINTSCREEN);
bind(ACT_CONSOLE, KEY_BACKQUOTE);
bind(ACT_SAVEGAME, KEY_F5);
bind(ACT_LOADGAME, KEY_F6);

bind(ACT_SMALLMEDI, KEY_9);
bind(ACT_BIGMEDI, KEY_0);
