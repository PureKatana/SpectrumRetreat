#include "UI_Manager.h"
#include <iostream>

void UI_Manager::drawPhone(const Phone& phone, float radius)
{
    ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
    ImVec2 window_size = ImGui::GetIO().DisplaySize;
    ImVec2 center(window_size.x - radius - 10.0f, window_size.y - radius - 10.0f); // 10px margin
    ImU32 color; 

    switch (phone.getState())
    {
    case ColorState::E_INACTIVE:

        color = IM_COL32(0, 0, 0, 255);
        break;

    case ColorState::E_BLUE:
        color = IM_COL32(0, 0, 255, 255);
        break;

    case ColorState::E_RED:
        color = IM_COL32(255, 0, 0, 255);
        break;

    case ColorState::E_YELLOW:
        color = IM_COL32(255, 255, 0 , 255);
        break;

    default:
        color = IM_COL32(255, 255, 255, 255);
        break;
    }

    draw_list->AddCircleFilled(center, radius, color, 32);
}

void UI_Manager::drawCursor(float radius)
{
    ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
    ImVec2 window_size = ImGui::GetIO().DisplaySize;
    ImVec2 center(window_size.x * 0.5f, window_size.y * 0.5f);

    // Force ImGui's internal mouse position to the center
    ImGui::GetIO().MousePos = center;

    ImU32 color = IM_COL32(255, 255, 255, 255); // White color for the cursor
    draw_list->AddCircleFilled(center, radius, color, 32);
}

void UI_Manager::drawMenu(GLFWwindow* window, std::function<void()> onRestartClicked, const char* text)
{
    // Get the display size
    ImVec2 window_size = ImGui::GetIO().DisplaySize;
    ImVec2 menu_size(600, 200);

    // Calculate centered position for the window itself
    ImVec2 center_pos(
        (window_size.x - menu_size.x) * 0.5f,
        (window_size.y - menu_size.y) * 0.5f
    );

    ImGui::SetNextWindowPos(center_pos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(menu_size, ImGuiCond_Always);

    ImGui::Begin("Game Menu", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    // Calculate available width for content within the current window
    float content_width = ImGui::GetContentRegionAvail().x;

    // --- Center "Pause Menu" text ---
    // Calculate the width of the text
    float text_width = ImGui::CalcTextSize(text).x;
    // Set the cursor position to center the text
    ImGui::SetCursorPosX((content_width - text_width) * 0.5f);
    ImGui::Text(text);

    // Add some spacing before the buttons
    ImGui::Spacing();
    ImGui::Spacing(); // Added an extra spacing for better visual separation
    ImGui::Spacing();

    // --- Center "Restart" button ---
    // Button width is fixed at 300
    float button_width = 300.0f;
    // Set the cursor position to center the button
    ImGui::SetCursorPosX((content_width - button_width) * 0.5f);
    if (ImGui::Button("Restart", ImVec2(button_width, 60)))
    {
        if (onRestartClicked)
        {
            onRestartClicked(); // Execute the callback
        }
    }

    ImGui::Spacing();

    // --- Center "Quit" button ---
    // Set the cursor position to center the button
    ImGui::SetCursorPosX((content_width - button_width) * 0.5f);
    if (ImGui::Button("Quit", ImVec2(button_width, 60)))
    {
		glfwSetWindowShouldClose(window, true); // Close the window
    }

    ImGui::End();
}

