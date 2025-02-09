/*
Copyright(c) 2016-2022 Panos Karabelas

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is furnished
to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

//= INCLUDES ==============================
#include "Toolbar.h"
#include "Profiler.h"
#include "ResourceViewer.h"
#include "ShaderEditor.h"
#include "RenderOptions.h"
#include "TextureViewer.h"
#include "CameraBookmarkViewer.h"
#include "Core/Engine.h"
#include "Rendering/Model.h"
#include "../ImGuiExtension.h"
#include "../ImGui/Source/imgui_internal.h"
#include "../Editor.h"
//=========================================

//= NAMESPACES ===============
using namespace std;
using namespace Spartan::Math;
//============================

Toolbar::Toolbar(Editor* editor) : Widget(editor)
{
    m_title     = "Toolbar";
    m_is_window = false;

    m_flags =
        ImGuiWindowFlags_NoCollapse      |
        ImGuiWindowFlags_NoResize        |
        ImGuiWindowFlags_NoMove          |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoScrollbar     |
        ImGuiWindowFlags_NoTitleBar;

    m_widgets[IconType::Button_Profiler]        = m_editor->GetWidget<Profiler>();
    m_widgets[IconType::Button_ResourceCache]   = m_editor->GetWidget<ResourceViewer>();
    m_widgets[IconType::Component_Script]       = m_editor->GetWidget<ShaderEditor>();
    m_widgets[IconType::Component_Options]      = m_editor->GetWidget<RenderOptions>();
    m_widgets[IconType::Directory_File_Texture] = m_editor->GetWidget<TextureViewer>();
    m_widgets[IconType::Button_BookMarkViewer]  = m_editor->GetWidget<CameraBookmarkViewer>();

    m_context->m_engine->EngineMode_Disable(Spartan::Engine_Mode::Engine_Game);
}

void Toolbar::TickAlways()
{
    auto show_button = [this](IconType icon_type, const function<bool()>& get_visibility, const function<void()>& make_visible)
    {
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, get_visibility() ? ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] : ImGui::GetStyle().Colors[ImGuiCol_Button]);
        if (ImGuiEx::ImageButton(icon_type, m_button_size))
        {
            make_visible();
        }
        ImGui::PopStyleColor();
    };

    // Play button
    show_button(
        IconType::Button_Play,
        [this](){ return m_context->m_engine->EngineMode_IsSet(Spartan::Engine_Mode::Engine_Game); },
        [this](){ m_context->m_engine->EngineMode_Toggle(Spartan::Engine_Mode::Engine_Game); }
    );

    for (auto& widget_it : m_widgets)
    {
        Widget* widget             = widget_it.second;
        const IconType widget_icon = widget_it.first;

        show_button(widget_icon, [this, &widget](){ return widget->GetVisible(); }, [this, &widget]() { widget->SetVisible(true); });
    }
}
