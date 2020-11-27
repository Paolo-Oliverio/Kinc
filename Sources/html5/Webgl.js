function kjs_init_gfx_api() 
{
    //Webgl initialization leveraging functions in gl.h that doesnt get optimized out anyway.
    let gl = GL.createContext(kjs_canvas,{majorVersion: 1});
    GL.makeContextCurrent(gl);
}