# Renderer Application

## 1. Develop log

> **2023 1.13**
>
> * Add Renderer Application which is the encapsulation of Window(Platform specific, OpenGL DX11 etc.)
> * Add Event System
> * Add Event Callback to Renderer Application
> * ~~Set ImGui context in Renderer Application.~~ Actually the interfaces for createImGuiContext and destroyImGuiContext should in Window.h
>
> <img src="https://raw.githubusercontent.com/lxcug/imgs/main/imgs20230113211840.png" style="zoom:50%;" />
>
> * Input Polling
> * RendererCamera
> * Loading .obj model
>
> <img src="https://raw.githubusercontent.com/lxcug/imgs/main/imgs20230114013007.png" style="zoom:50%;" />
>
> 

> **2023 1.14**
>
> * Add FrameBuffer(Attach Color and Depth)
>
> ---
>
> - [x] Fixed depth buffer render error
> - [x] Fixed frame buffer Texture Attachment error
>
> <img src="https://raw.githubusercontent.com/lxcug/imgs/main/imgs20230115001002.png" style="zoom:50%;" />

> **2023 1.15**
>
> - [x] Fixed Timer.h error
> - [x] Fixed FrameBuffer Resize Error

> **2023 1.16**
>
> * Add Global Settings 
>
> <img src="https://raw.githubusercontent.com/lxcug/imgs/main/imgs20230116210652.png" style="zoom:50%;" />