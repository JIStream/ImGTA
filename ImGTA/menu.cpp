#include "menu.hpp"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "../../ImGTA/UniversalHookX/console/console.hpp"

namespace ig = ImGui;

namespace Menu {
    void InitializeContext(HWND hwnd) {
        if (ig::GetCurrentContext( ))
            return;

        LOG("Init Context!");

        ImGui::CreateContext( );
        ImGui_ImplWin32_Init(hwnd);

        ImGuiIO& io = ImGui::GetIO( );
        io.IniFilename = io.LogFilename = nullptr;
    }

    void Render( ) {
        if (!bShowMenu)
            return;

        LOG("Drawing!");
        ig::ShowDemoWindow( );
    }
} // namespace Menu
