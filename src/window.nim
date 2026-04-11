import staticglfw as glfw
import opengl
import strformat

type
    Window* = ref object
        handle: glfw.Window
        parent: App

    App* = ref object of RootObj
        window: Window



proc resize(window: Window, width, height: int) =
    echo fmt"Resized to {width} x {height}"



proc sizeCallback(window: glfw.Window, width: cint, height: cint){.cdecl.} =
    let appWindow = cast[ptr Window](window.getWindowUserPointer())[]
    appWindow.resize((int)width, (int)height)



proc run(window: Window) =
    while glfw.windowShouldClose(window.handle) == 0:
        glfw.pollEvents()

        window.handle.swapBuffers()

    window.handle.destroyWindow()
    glfw.terminate()



proc newWindow(width, height: int, title: string, parent: App): Window =
    if glfw.init() == 0:
        raise newException(Exception, "GLFW initialization failed")

    glfw.windowHint(CONTEXT_VERSION_MAJOR, 3)
    glfw.windowHint(CONTEXT_VERSION_MINOR, 2)
    glfw.windowHint(OPENGL_PROFILE, OPENGL_CORE_PROFILE)

    result = Window(parent: parent)

    result.handle = glfw.createWindow((cint)width, (cint)height, (cstring)title, nil, nil)
    result.handle.makeContextCurrent()

    result.handle.setWindowUserPointer(addr result)
    discard result.handle.setWindowSizeCallback(sizeCallback)



proc newApp*(width, height: int, title: string): App =
    result = App()

    result.window = newWindow(width, height, title, result)

proc run*(app: App) =
    app.window.run()
