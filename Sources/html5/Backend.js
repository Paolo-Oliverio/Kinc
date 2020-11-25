const kjs_canvas = document.querySelector("#canvas");
var kjs_screen = { x:0|0, y:0|0 };

function kjs_init(width, height) 
{
    kjs_canvas.width  = width;
    kjs_canvas.height = height;
    window.addEventListener('resize', kjs_event_resize, false);
    kjs_event_resize(null);

    {//Webgl initialization leveraging functions in gl.h that doesnt get optimized out anyway.
        let gl = GL.createContext(kjs_canvas,{majorVersion: 1});
        GL.makeContextCurrent(gl);
        
    }
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
    document.addEventListener('keydown', (e) => {
        if (!e.repeat){
            _js_event_key(0, e.keyCode);
        }
        if (e.keyCode === 8 /* backspace */ || e.keyCode === 9 /* tab */) {
            e.preventDefault();
        }
    });
    document.addEventListener('keyup', (e) => { _js_event_key(1, e.keyCode); });
    document.addEventListener('keypress', (e) => { _js_event_key(2, e.keyCode); });
    //Mouse Input Callbacks
    document.addEventListener('mousedown', e => 
    { _js_event_mouse(0, e.button, kjs_pointer_xform_X(e.clientX), kjs_pointer_xform_Y(e.clientY)); });

    document.addEventListener('mouseup', e => 
    { _js_event_mouse(1, e.button, kjs_pointer_xform_X(e.clientX), kjs_pointer_xform_Y(e.clientY)); });

    document.addEventListener('mousemove', e => 
    { _js_event_mouse(2, 0, kjs_pointer_xform_X(e.clientX), kjs_pointer_xform_Y(e.clientY)); });
    
    document.addEventListener("wheel", e =>
    {
        var dir = Math.sign(e.deltaY);
        _js_event_wheel(dir);
    });
    //Touch Input Callbacks

    /*if(kjs_isiOS()) //IOS uses nonstandard touch identifiers.
    {
        var touchIDs = [];
        canvas.addEventListener('touchstart', e => {
            e.stopPropagation();
            e.preventDefault();
            var touch;
            for (var i=0; i < e.changedTouches.length; i++)
            {
                touch =  e.changedTouches[i];
                var id  = touchIDs.indexOf(-1);
                if (id == -1) id = touchIDs.length;
                touchIds[id] = touch.identifier;
                _js_event_touch(0, id, kjs_pointer_xform_X(touch.clientX), kjs_pointer_xform_Y(touch.clientY));
            }
        });
        canvas.addEventListener('touchend', e => {
            var touch;
            for (var i=0; i < e.changedTouches.length; i++)
            {
                touch =  e.changedTouches[i];
                var id = touchIDs.indexOf(touch.identifier);
                touchIDs[id] = -1;
                _js_event_touch(1,id, kjs_pointer_xform_X(touch.clientX), kjs_pointer_xform_Y(touch.clientY));
            }
        });
        canvas.addEventListener('touchmove', e => {
            var touch;
            for (var i=0; i < e.changedTouches.length; i++)
            {
                touch =  e.changedTouches[i];
                let id = touchIDs.indexOf(id);
                _js_event_touch(2, id, kjs_pointer_xform_X(touch.clientX), kjs_pointer_xform_Y(touch.clientY));
            }
        });
    } else  {*/
        document.addEventListener('touchstart', e => {
            e.stopPropagation();
            e.preventDefault();
            var touch;
            for (var i=0; i < e.changedTouches.length; i++)
            {
                touch =  e.changedTouches[i];
                //console.log(touch);
                _js_event_touch(0, touch.identifier, kjs_pointer_xform_X(touch.clientX), kjs_pointer_xform_Y(touch.clientY));
            }
        });
        document.addEventListener('touchend', e => {
            var touch;
            for (var i=0; i < e.changedTouches.length; i++)
            {
                touch =  e.changedTouches[i];
                //console.log(touch);
                _js_event_touch(1, touch.identifier, kjs_pointer_xform_X(touch.clientX), kjs_pointer_xform_Y(touch.clientY));
            }
        });
        document.addEventListener('touchmove', e => {
            var touch;
            for (var i=0; i < e.changedTouches.length; i++)
            {
                touch =  e.changedTouches[i];
                _js_event_touch(2, touch.identifier, kjs_pointer_xform_X(touch.clientX), kjs_pointer_xform_Y(touch.clientY));
            }
        });
    //}// end of if(kjs_isiOS())
}

//Request Animation Frame Infinite Loop.
function kjs_start_loop()
{
    _js_loop();
    window.requestAnimationFrame(kjs_start_loop);
}

function kjs_pointer_xform_X(x){
    return (x - kjs_screen.x);
}

function kjs_pointer_xform_Y(y){
    return (y - kjs_screen.y);
}

function kjs_event_resize(e){
    _js_event_resize(window.innerWidth, window.innerHeight);
    kjs_screen.x = ((Math.max(window.innerWidth, kjs_canvas.width) - kjs_canvas.width) * 0.5)|0;
    kjs_screen.y = ((Math.max(window.innerHeight, kjs_canvas.height) - kjs_canvas.height) * 0.5)|0;
}

/*
//Detect ios platform for touch management.
function kjs_isiOS() {
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
*/