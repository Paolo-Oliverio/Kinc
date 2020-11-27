const kjs_canvas = document.querySelector("#canvas");
let kjs_screen = { x:0, y:0 , scaleX:1, scaleY:1 };
let kjs_gpad = { 
    state : [],
    id : [],
}

/** @export */
function kjs_init(width, height)
{
    kjs_canvas.width  = width;
    kjs_canvas.height = height;
    let callback = window.addEventListener;
    callback('resize', kjs_event_resize, false);
    kjs_event_resize(null);
    /*
    function kjs_first_input(e){
    //TODO call _js_first_input on c to setup 
        kjs_unregister_first_input()
    }
    function kjs_register_first_input(canvas){
        canvas.addEventListener('mousedown',  kjs_first_input);
        document.addEventListener('keydown',  kjs_first_input);
        canvas.addEventListener('touchstart', kjs_first_input);
    }
    function kjs_unregister_first_input(canvas){
        canvas.removeEventListener('mousedown',  kjs_first_input);
        document.removeEventListener('keydown',  kjs_first_input);
        canvas.removeEventListener('touchstart', kjs_first_input);
    }
    kjs_register_first_input(glcanvas);*/

    //Keyboard Input Callbacks
    // e.keyCode has been deprecated the new parameter e.key should be used.
    // e.ksy is a string and should be mapped to kinc keyboard constants like map = { 'a': 65 , 'b' : 66....}
    callback('keydown', (e) => {
        if (!e.repeat){
            _js_event_key(0, e.keyCode);
        }
        if (e.keyCode === 8 /* backspace */ || e.keyCode === 9 /* tab */) {
            e.preventDefault();
        }
    });
    callback('keyup', (e) => { _js_event_key(1, e.keyCode);});
    callback('keypress', (e) => { _js_event_key(2, e.keyCode);});
    
    //Pointer Input Helpers.
    function kjs_pointer_xform_X(x){ return (x - kjs_screen.x) * kjs_screen.scaleX;}
    function kjs_pointer_xform_Y(y){ return (y - kjs_screen.y) * kjs_screen.scaleY;}

    //Mouse Input Callbacks
    callback('mousedown', e => 
    { _js_event_mouse(0, e.button, kjs_pointer_xform_X(e.clientX), kjs_pointer_xform_Y(e.clientY)); });

    callback('mouseup', e => 
    { _js_event_mouse(1, e.button, kjs_pointer_xform_X(e.clientX), kjs_pointer_xform_Y(e.clientY)); });

    callback('mousemove', e => 
    { _js_event_mouse(2, 0, kjs_pointer_xform_X(e.clientX), kjs_pointer_xform_Y(e.clientY)); });
    
    callback("wheel", e =>
    {
        let dir = Math.sign(e.deltaY);
        _js_event_wheel(dir);
    });

    //Touch Input Callbacks
    const KINC_IOS = kjs_isiOS();
    var IOS_touchIDs = [];//only used for ios.

    callback('touchstart', e => {
        e.stopPropagation();
        e.preventDefault();
        let touch;
        if(KINC_IOS)
        {
            let id;
            for (let i = 0; i < e.changedTouches.length; i++)
            {
                touch = e.changedTouches[i];
                id = IOS_touchIDs.indexOf(-1);
                if (id == -1) id = IOS_touchIDs.length;
                IOS_touchIDs[id] = touch.identifier;
                _js_event_touch(0, id, kjs_pointer_xform_X(touch.clientX), kjs_pointer_xform_Y(touch.clientY));
            }
            return
        }
        for (let i = 0; i < e.changedTouches.length; i++)
        {
            touch = e.changedTouches[i];
            //console.log(touch);
            _js_event_touch(0, touch.identifier, kjs_pointer_xform_X(touch.clientX), kjs_pointer_xform_Y(touch.clientY));
        }
    });
    callback('touchend', e => {
        e.preventDefault();
        let touch;
        if(KINC_IOS)
        {
            let id;
            for (let i = 0; i < e.changedTouches.length; i++)
            {
                touch = e.changedTouches[i];
                id = IOS_touchIDs.indexOf(touch.identifier);
                IOS_touchIDs[id] = -1;
                _js_event_touch(1,id, kjs_pointer_xform_X(touch.clientX), kjs_pointer_xform_Y(touch.clientY));
            }
            return
        }
        for (let i = 0; i < e.changedTouches.length; i++)
        {
            touch = e.changedTouches[i];
            //console.log(touch);
            _js_event_touch(1, touch.identifier, kjs_pointer_xform_X(touch.clientX), kjs_pointer_xform_Y(touch.clientY));
            }
        });
    callback('touchmove', e => {
        e.preventDefault();
        let touch;
        if(KINC_IOS)
        {
            let id;
            for (let i = 0; i < e.changedTouches.length; i++)
            {
                touch = e.changedTouches[i];
                id = IOS_touchIDs.indexOf(id);
                _js_event_touch(2, id, kjs_pointer_xform_X(touch.clientX), kjs_pointer_xform_Y(touch.clientY));
            }
            return
        }
        for (let i = 0; i < e.changedTouches.length; i++)
        {
            touch = e.changedTouches[i];
            _js_event_touch(2, touch.identifier, kjs_pointer_xform_X(touch.clientX), kjs_pointer_xform_Y(touch.clientY));
        }
    });
//Gamepad Api
    callback("gamepadconnected", function(e) {
        let g = kjs_getGamepads()[e.gamepad.index];
        kjs_gpad.state[e.gamepad.index] = new Float32Array(g.axes.length + g.buttons.length);
        kjs_gpad.id[e.gamepad.index] = g.id;
    });
    callback("gamepaddisconnected", function(e) {
        kjs_gpad.state[e.gamepad.index] = undefined;
        kjs_gpad.id[e.gamepad.index] = undefined;
    });
}

function kjs_getGamepads(){
    return navigator.getGamepads ? navigator.getGamepads() : (navigator.webkitGetGamepads ? navigator.webkitGetGamepads : []);
}

function kjs_gamepads_update()
{
    let pads = kjs_getGamepads()
    let state, g, axes, n, axisval, buttons, pressed, wasPressed;
    for (let i = 0; i < kjs_gpad.state.length; ++i)
    {
        state = kjs_gpad.state[i];
        if (state === undefined) continue;
        g = pads[i];
        axes = g.axes;
        n = axes.length;
        for (let a = 0; a < n; ++a)
        {
            //Introduced minimal deadzone and sensibility.
            axisval = Math.abs(axes[a]) > 0.05 ? axes[a] : 0;
            if (Math.abs(state[a] - axisval) > 0.02)
            {
                _js_event_gamepad_axis(i, a, axisval);
                state[a] = axisval;
            }
        }
        buttons = g.buttons;
        for (let b = 0; b < buttons.length; ++b)
        {
            wasPressed = state[n];
            pressed = buttons[b].pressed * buttons[b].value;
            if (wasPressed != pressed)
            {
                _js_event_gamepad_button(i, b, pressed);
                state[n] = pressed;
            }
            n++;
        }
    }
}

function kjs_gamepad_connected(id)
{
    return kjs_gpad.state[id] !== undefined;
}

function kjs_gamepad_product_name(id)
{
    let g = kjs_getGamepads()[id];
    if(g === undefined) return 0;
    return stringToC(g);
}

function kjs_vib(g,del,dur,min,max)
{
    return g.vibrationActuator.playEffect("dual-rumble", {
        startDelay : del,
        duration: dur,
        strongMagnitude: min,
        weakMagnitude: max
    });
}

//Request Animation Frame Infinite Loop.
function kjs_start_loop()
{
    kjs_gamepads_update();
    _js_loop();
    window.requestAnimationFrame(kjs_start_loop);
}

function kjs_event_resize(e)
{
    _js_update_display(0,0,)
    // from _js_event_resize user may change canvas size to adapt to the new available space.
    _js_event_resize(window.innerWidth, window.innerHeight);
    // canvas here has been potentially changed.
    const rect = kjs_canvas.getBoundingClientRect();
    kjs_screen.x = rect.left;
    kjs_screen.y = rect.top;
    kjs_screen.scaleX = kjs_canvas.width / rect.width;
    kjs_screen.scaleY = kjs_canvas.height / rect.height; 
}

//Detect ios platform for touch management.
function kjs_isiOS() 
{
    return [
    'iPad Simulator',
    'iPhone Simulator',
    'iPod Simulator',
    'iPad',
    'iPhone',
    'iPod'
    ].includes(navigator.platform)
    // iPad on iOS 13 detection
    || (navigator.userAgent.includes("Mac") && "ontouchend" in document)
}

function stringToC(str)
{
    var buffer = Module._malloc(str.length + 1);
    Module.writeStringToMemory(str, buffer);
    return buffer;
}