#include "pch.h"
#include "CameraUI.h"
#include "GameObject.h"

CameraUI::CameraUI()
	:ComponentUI(COMPONENT_TYPE::CAMERA, "Camera")
{
}

CameraUI::~CameraUI()
{
}

void CameraUI::Tick_UI()
{
	OutputTitle("Camera");

    // 과제

    if (ImGui::TreeNode("Layer CheckBox"))
    {
        static bool layers[MAX_LAYER] = {};
        static ImGuiMultiSelectFlags flags = ImGuiMultiSelectFlags_NoAutoSelect | ImGuiMultiSelectFlags_NoAutoClear | ImGuiMultiSelectFlags_ClearOnEscape;

        UINT m_LayerCheck = GetTarget()->Camera()->GetCurrentCheckedLayer();

        if (ImGui::BeginChild("##Basket", ImVec2(-FLT_MIN, ImGui::GetFontSize() * 20), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeY))
        {
            ImGuiMultiSelectIO* ms_io = ImGui::BeginMultiSelect(flags, -1, IM_COUNTOF(layers));
            ImGuiSelectionExternalStorage storage_wrapper;
            storage_wrapper.UserData = (void*)layers;
            storage_wrapper.AdapterSetItemSelected =
                [](ImGuiSelectionExternalStorage* self, int n, bool selected)
                {
                    bool* array = (bool*)self->UserData; array[n] = selected;
                };
            storage_wrapper.ApplyRequests(ms_io);

            for (int n = 0; n < MAX_LAYER; n++)
            {
                char label[MAX_LAYER];
                sprintf_s(label, "Layer %d", n);

                for (int i = 0; i < MAX_LAYER; i++)
                {
                    if (m_LayerCheck & (1 << i))
                        layers[i] = true;
                }

                ImGui::SetNextItemSelectionUserData(n);
                ImGui::Checkbox(label, &layers[n]);

                if (layers[n] == true)
                    m_LayerCheck |= (1 << n);
                else
                    m_LayerCheck &= ~(1 << n);     // 비트 끄기
            }
            GetTarget()->Camera()->SetLayerCheck(m_LayerCheck);
            ms_io = ImGui::EndMultiSelect();
            storage_wrapper.ApplyRequests(ms_io);
        }
        ImGui::EndChild();

        ImGui::TreePop();
    }



    PROJ_TYPE ProjType = GetTarget()->Camera()->GetProjType();
    const char* items[] = { "ORTHOGRAPHIC", "PERSPECTIVE" };
    if (ImGui::Combo("combo", (int*)&ProjType, items, 2))
    {
        GetTarget()->Camera()->SetProjType(ProjType);
    }


    ImGui::BeginDisabled(ProjType == PROJ_TYPE::ORTHOGRAPHIC);

    ImGui::Text("FOV");
    ImGui::SameLine(100);
    float  FOV = GetTarget()->Camera()->GetFOV();
    if (ImGui::DragFloat("##FOV", &FOV, 0.1f))
    {
        GetTarget()->Camera()->SetFOV(FOV);
    }

    ImGui::EndDisabled();


    float       m_Far;          // 카메라 시야 최대거리
    float       m_Width;        // 직교투영 가로길이
    float       m_AspectRatio;  // 종횡비 (가로 / 세로), 세로대비 가로의 길이 비율   
    float       m_OrthoScale;   // 직교투영 배율

}
