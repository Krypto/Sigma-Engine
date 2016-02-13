#ifndef SIGMA_IMGUI_SDL
#define SIGMA_IMGUI_SDL

#include "imgui.h"
#include <SDL2/SDL.h>

IMGUI_API void		  ImGui_ImplSdl_RenderDrawLists(ImDrawData* draw_data);
IMGUI_API bool        ImGui_ImplSdl_Init(SDL_Window *window);
IMGUI_API void        ImGui_ImplSdl_Shutdown();
IMGUI_API void        ImGui_ImplSdl_NewFrame(SDL_Window *window);
IMGUI_API bool        ImGui_ImplSdl_ProcessEvent(SDL_Event* event);

// Use if you want to reset your rendering device without losing ImGui state.
IMGUI_API void        ImGui_ImplSdl_InvalidateDeviceObjects();
IMGUI_API bool        ImGui_ImplSdl_CreateDeviceObjects();

#endif // SIGMA_IMGUI_SDL