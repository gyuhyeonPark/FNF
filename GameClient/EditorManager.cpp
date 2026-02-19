#include "pch.h"
#include "EditorManager.h"
#include "Inspector.h"
#include "Outliner.h"
#include "Menu.h"
#include "ListUI.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "KeyManager.h"

EditorManager::EditorManager()
    :m_ShowDemo(false)
{
}

EditorManager::~EditorManager()
{
    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

LRESULT CALLBACK  WndProc(HWND, UINT, WPARAM, LPARAM);

void EditorManager::Init(HWND _hwnd)
{
    // Make process DPI aware and obtain main monitor scale
    ImGui_ImplWin32_EnableDpiAwareness();
    float main_scale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;
    //io.ConfigDockingAlwaysTabBar = true;
    //io.ConfigDockingTransparentPayload = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale;        // Set initial font scale. (in docking branch: using io.ConfigDpiScaleFonts=true automatically overrides this for every window depending on the current monitor)
    io.ConfigDpiScaleFonts = true;          // [Experimental] Automatically overwrite style.FontScaleDpi in Begin() when Monitor DPI changes. This will scale fonts but _NOT_ scale sizes/padding for now.
    io.ConfigDpiScaleViewports = true;      // [Experimental] Scale Dear ImGui and Platform Windows when Monitor DPI changes.

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(_hwnd);
    ImGui_ImplDX11_Init(DEVICE.Get(), DEVICE_CONTEXT.Get());

    CreateEditorUI();
}

void EditorManager::Progress()
{
    Tick();
    Render();
}

void EditorManager::AddUI(const string& _UIName, Ptr<EditorUI> _UI)
{
    Ptr<EditorUI> pUI = FindUI(_UIName);
    assert(nullptr == pUI);
    m_uiMap.insert(make_pair(_UIName, _UI));
}

Ptr<EditorUI> EditorManager::FindUI(const string& _UIName)
{
    map<string, Ptr<EditorUI>>::iterator iter = m_uiMap.find(_UIName);

    if (iter == m_uiMap.end())
        return nullptr;

    return iter->second;
}

void EditorManager::Tick()
{
    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    if (KEY_TAP(KEY::ENTER))
        ImGui::SetWindowFocus(nullptr);

    if (KEY_TAP(KEY::F8))
        m_ShowDemo = !m_ShowDemo;

    if (m_ShowDemo)
        ImGui::ShowDemoWindow(&m_ShowDemo);

    for (const auto& pair : m_uiMap)
    {
        if (pair.second->IsActive())
            pair.second->Tick();
    }


}

void EditorManager::Render()
{
    // Rendering
    ImGui::Render();

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

void EditorManager::CreateEditorUI()
{
    Ptr<EditorUI> pUI = nullptr;
    pUI = new Inspector;
    AddUI(pUI->GetUIName(), pUI);

    pUI = new Outliner;
    AddUI(pUI->GetUIName(), pUI);

    pUI = new Menu;
    AddUI(pUI->GetUIName(), pUI);
   
    pUI = new ListUI;
    pUI->SetActive(false);
    AddUI(pUI->GetUIName(), pUI);
}
