# Moving Picture Studio

<p align="left">
  <img src="https://olekolek1000.com/mps/github/logo.png" width="350" height="350">
</p>

**[MAIN WEBPAGE](http://olekolek1000.com/mps/)**

This is a stop-motion animation program which is made to be **MODERN**, **EASY**, **MULTIPLATFORM** and **FAST**.

![img_screenshot](https://olekolek1000.com/mps/github/screenshot.png)

The application is written in C++, based on OpenGL 2.1/ES 2.0 and SDL.  
Currently supported/tested platforms:  

 - Linux x64
 - Windows x64
 
Works, but it's unstable:
 - MacOS x64

This application also works on ARM processors (Raspberry Pi and other).  
	
Please report any bugs and problems in this program. It helps a lot!
Ideas are also welcome ;)

### Done:
 - Open/Save Project
 - Export to GIF
 - Frame selector (Create/remove frame, duplicate frame)
 - History (Undo/redo)
 - Tool: Pencil
 - Tool: Line
 - Tool: Spray
 - Tool: Bucket fill
 - Tool: Pipette
 - Tool: Eraser
 - Tool: Rectangle
 - Tool: Resize (still WIP)
 - Zoomable viewport
 - Dynamic Rotatable viewport
 - Scalable GUI
 - Theme manager with config files (still WIP)

### Partially done (Works fine, but might be changed in the future):<br>
 - Layer support
 - Toolbox
 - Color selector
 - Ghosting (more options)

 - Menu

### To do:
 - Import animation
 - Export PNG strip
 - use FFMPEG (avcodec) for everything
 - "Play animation" button
 - Tool: Brush (draw using any image)
 - Tool: Circle
 - Tool: Select
 - Copy/paste
 - Compressed history (less RAM usage, more history actions)
 - Compressed frames not in use
 - Change key bindings
 - GUI settings (in menu)

### In the distant future:
 - Tablet pressure support (I'm waiting for the SDL library)


## Controls:
 - Left Shift: Rotate viewport

### Drawing area:
 - LMB: Use tool (draw)
 - MMB: Pick color
 - RMB: Drag viewport
 - Mouse wheel: Zoom in/out
 - 1/2/3/4/5/6/7/8/9: Scale viewport to (1:1, 2:1, 3:1 etc.) and reset viewport rotation

 - Space: Toggle ghosting visibility
 - CTRL+Z: Undo
 - CTRL+Y: Redo
 - Left/Right: Change frames

 - F3: Show FPS
 - F4: Toggle fullscreen
 - F5: Toggle VSync
 - F6: Toggle FPS limit
 - F7: Decrease FPS limit
 - F8: Increase FPS limit
 - Esc: Open menu
 - -/+: Scale GUI

 - End: Restart application (for testing purposes)
