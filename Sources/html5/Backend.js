function initGL(width, height) {
    const canvas = document.querySelector("#canvas");
    let gl = GL.createContext(canvas,{majorVersion: 1});
    GL.makeContextCurrent(gl);
    canvas.width  = width;
    canvas.height = height;
}

function step() {
    // UpdateUI();
    _drawfunc();
    window.requestAnimationFrame(step);
}

function startloop(){
    window.requestAnimationFrame(step);
}

