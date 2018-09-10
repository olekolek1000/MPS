<h1><b>===Moving Picture Studio===</b></h1><br>

<img src="http://mps.olekolek1000.pl/github/logo.png">

<b><a href="http://mps.olekolek1000.pl/">MAIN WEBPAGE</a></b>

This is a stop-motion animation program which is made to be <b>MODERN</b>, <b>EASY</b>, <b>MULTIPLATFORM</b> and <b>FAST</b>.

![img_screenshot](http://mps.olekolek1000.pl/github/screenshot.png)

The application is written in C++, based on OpenGL 2.1/ES 2.0 and SDL.<br>
Currently supported/tested platforms:<br>
	-Linux x64<br>
	-Windows x64<br>
This application works also on ARM processors (Raspberry PI and other).<br>
	
Please report any bugs and problems in this program. It helps a lot!
The ideas are also welcome ;)

<b>===Done:</b><br>
-Open/Save Project<br>
-Export to GIF<br>
-Frame selector (Create/remove frame, duplicate frame)<br>
-History (Undo/redo)<br>
-Tool: Pencil<br>
-Tool: Line<br>
-Tool: Spray<br>
-Tool: Bucket fill<br>
-Tool: Pipette<br>
-Tool: Eraser<br>
-Tool: Rectangle<br>
-Tool: Resize (still WIP)<br>
-Zoomable viewport<br>
-Dynamic Rotatable viewport<br>
-Scalable GUI<br>
-Theme manager with config files (still WIP)<br>

<b>===Partially done (Works fine, but there may be a lot of changes in the future):</b><br>
-Layer support<br>
-Toolbox<br>
-Color selector<br>
-Ghosting (more options)<br>

-Menu

<b>===To do:</b><br>
-Import animation<br>
-Export PNG strip<br>
-use FFMPEG (avcodec) for everything<br>
-"Play animation" button<br>
-Tool: Brush (draw using any image)<br>
-Tool: Circle<br>
-Tool: Select<br>
-Copy/paste<br>
-Compressed history (less RAM usage, more history actions)<br>
-Compressed frames not in use<br>
-Change key bindings<br>
-GUI settings (in menu)<br>

<b>===In the distant future:</b><br>
-Tablet pressure support (I'm waiting for the SDL library)<br>
-Mac support<br>


<b>===Controls:</b><br>
Left Shift: Rotate viewport<br>

In drawing area:<br>
LMB: Use tool (draw)<br>
--MMB: Pick color<br>
--RMB: Drag viewport<br>
--Mouse wheel: Zoom in/out<br>
--1/2/3/4/5/6/7/8/9: Scale viewport to (1:1, 2:1, 3:1 etc.) and reset viewport rotation<br>

Space: Toggle ghosting visibility<br>
CTRL+Z: Undo<br>
CTRL+Y: Redo<br>
Left/Right: Change frames<br>
<br>
F3: Show FPS<br>
F4: Toggle fullscreen<br>
F5: Toggle VSync<br>
F6: Toggle FPS limit
F7: Decrease FPS limit
F8: Increase FPS limit
Esc: Open menu<br>
-/+ - Scale GUI<br>

End: Restart application (for testing purposes)<br>
