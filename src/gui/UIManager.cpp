#include "UIManager.h"
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "../thirdparty/stb_image.h"

// ─── Instagram Color Palette ────────────────────────────────────────
// Background White:   #FFFFFF  (1.00, 1.00, 1.00)
// Card/Surface:       #FAFAFA  (0.98, 0.98, 0.98)
// Primary Text:       #262626  (0.15, 0.15, 0.15)
// Secondary Text:     #8E8E8E  (0.56, 0.56, 0.56)
// Border/Divider:     #DBDBDB  (0.86, 0.86, 0.86)
// Instagram Blue:     #0095F6  (0.00, 0.58, 0.96)
// Link Blue:          #00376B  (0.00, 0.22, 0.42)
// Error Red:          #ED4956  (0.93, 0.29, 0.34)
// Like Red:           #ED4956  (0.93, 0.29, 0.34)
// Gradient Start:     #F58529  (0.96, 0.52, 0.16) orange
// Gradient Mid:       #DD2A7B  (0.87, 0.16, 0.48) pink
// Gradient End:       #515BD4  (0.32, 0.36, 0.83) purple

static const ImVec4 IG_WHITE = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
static const ImVec4 IG_SURFACE = ImVec4(0.98f, 0.98f, 0.98f, 1.00f);
static const ImVec4 IG_TEXT = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
static const ImVec4 IG_TEXT_SEC = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
static const ImVec4 IG_BORDER = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
static const ImVec4 IG_BLUE = ImVec4(0.00f, 0.58f, 0.96f, 1.00f);
static const ImVec4 IG_BLUE_HOVER = ImVec4(0.10f, 0.65f, 1.00f, 1.00f);
static const ImVec4 IG_BLUE_DARK = ImVec4(0.00f, 0.22f, 0.42f, 1.00f);
static const ImVec4 IG_RED = ImVec4(0.93f, 0.29f, 0.34f, 1.00f);
static const ImVec4 IG_ORANGE = ImVec4(0.96f, 0.52f, 0.16f, 1.00f);
static const ImVec4 IG_PINK = ImVec4(0.87f, 0.16f, 0.48f, 1.00f);
static const ImVec4 IG_PURPLE = ImVec4(0.32f, 0.36f, 0.83f, 1.00f);
static const ImVec4 IG_TRANSPARENT = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

// ─── Texture Loader ─────────────────────────────────────────────────
bool UIManager::LoadTextureFromFile(const char *filename, GLuint *out_texture,
                                    int *out_width, int *out_height) {
  int image_width = 0, image_height = 0;
  unsigned char *image_data =
      stbi_load(filename, &image_width, &image_height, NULL, 4);
  if (image_data == NULL)
    return false;

  GLuint image_texture;
  glGenTextures(1, &image_texture);
  glBindTexture(GL_TEXTURE_2D, image_texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, image_data);
  stbi_image_free(image_data);

  *out_texture = image_texture;
  *out_width = image_width;
  *out_height = image_height;
  return true;
}

// ─── Constructor ────────────────────────────────────────────────────
UIManager::UIManager() { SetupInstagramTheme(); }

// ─── Instagram Theme Setup ──────────────────────────────────────────
void UIManager::SetupInstagramTheme() {
  ImGuiStyle &style = ImGui::GetStyle();
  ImVec4 *colors = style.Colors;

  // Backgrounds
  colors[ImGuiCol_WindowBg] = IG_SURFACE;
  colors[ImGuiCol_ChildBg] = IG_WHITE;
  colors[ImGuiCol_PopupBg] = IG_WHITE;

  // Text
  colors[ImGuiCol_Text] = IG_TEXT;
  colors[ImGuiCol_TextDisabled] = IG_TEXT_SEC;

  // Borders
  colors[ImGuiCol_Border] = IG_BORDER;
  colors[ImGuiCol_BorderShadow] = IG_TRANSPARENT;

  // Frames (input fields)
  colors[ImGuiCol_FrameBg] = IG_SURFACE;
  colors[ImGuiCol_FrameBgHovered] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
  colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);

  // Title bars
  colors[ImGuiCol_TitleBg] = IG_WHITE;
  colors[ImGuiCol_TitleBgActive] = IG_WHITE;
  colors[ImGuiCol_TitleBgCollapsed] = IG_WHITE;

  // Buttons – Instagram Blue
  colors[ImGuiCol_Button] = IG_BLUE;
  colors[ImGuiCol_ButtonHovered] = IG_BLUE_HOVER;
  colors[ImGuiCol_ButtonActive] = IG_BLUE_DARK;

  // Input cursor
  colors[ImGuiCol_InputTextCursor] = IG_TEXT;

  // Scrollbar
  colors[ImGuiCol_ScrollbarBg] = IG_WHITE;
  colors[ImGuiCol_ScrollbarGrab] = IG_BORDER;
  colors[ImGuiCol_ScrollbarGrabHovered] = IG_TEXT_SEC;
  colors[ImGuiCol_ScrollbarGrabActive] = IG_TEXT;

  // Checkmarks/Sliders
  colors[ImGuiCol_CheckMark] = IG_BLUE;
  colors[ImGuiCol_SliderGrab] = IG_BLUE;
  colors[ImGuiCol_SliderGrabActive] = IG_BLUE_HOVER;

  // Headers (selectable, menu)
  colors[ImGuiCol_Header] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
  colors[ImGuiCol_HeaderHovered] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
  colors[ImGuiCol_HeaderActive] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);

  // Separator
  colors[ImGuiCol_Separator] = IG_BORDER;
  colors[ImGuiCol_SeparatorHovered] = IG_BORDER;
  colors[ImGuiCol_SeparatorActive] = IG_BORDER;

  // Rounding – Instagram uses soft corners
  style.WindowRounding = 0.0f;
  style.ChildRounding = 3.0f;
  style.FrameRounding = 6.0f;
  style.PopupRounding = 8.0f;
  style.ScrollbarRounding = 8.0f;
  style.GrabRounding = 6.0f;
  style.TabRounding = 6.0f;

  // Borders
  style.WindowBorderSize = 0.0f;
  style.FrameBorderSize = 1.0f;
  style.ChildBorderSize = 1.0f;

  // Spacing
  style.FramePadding = ImVec2(10.0f, 8.0f);
  style.ItemSpacing = ImVec2(8.0f, 6.0f);
  style.ItemInnerSpacing = ImVec2(6.0f, 4.0f);
  style.ScrollbarSize = 10.0f;
}

// ─── UI Helpers ─────────────────────────────────────────────────────

void UIManager::DrawAvatar(const char *letter, ImVec4 bgColor, float size) {
  ImVec2 p = ImGui::GetCursorScreenPos();
  ImDrawList *draw_list = ImGui::GetWindowDrawList();
  float radius = size * 0.5f;
  ImVec2 center = ImVec2(p.x + radius, p.y + radius);

  draw_list->AddCircleFilled(center, radius, ImGui::GetColorU32(bgColor), 32);
  draw_list->AddCircle(center, radius, ImGui::GetColorU32(IG_BORDER), 32, 1.5f);

  // Draw the letter centered
  ImVec2 textSize = ImGui::CalcTextSize(letter);
  draw_list->AddText(
      ImVec2(center.x - textSize.x * 0.5f, center.y - textSize.y * 0.5f),
      ImGui::GetColorU32(IG_WHITE), letter);

  ImGui::Dummy(ImVec2(size, size));
}

void UIManager::DrawStoryCircle(const char *username, ImVec4 ringColor) {
  float radius = 24.0f;
  float totalSize = radius * 2 + 8;

  ImGui::BeginGroup();

  // Draw circle using draw list overlay
  ImVec2 p = ImGui::GetCursorScreenPos();
  ImDrawList *draw_list = ImGui::GetWindowDrawList();
  ImVec2 center = ImVec2(p.x + totalSize * 0.5f, p.y + totalSize * 0.5f);

  // Outer gradient ring
  draw_list->AddCircle(center, radius + 3, ImGui::GetColorU32(ringColor), 32,
                       2.5f);
  draw_list->AddCircle(center, radius + 1, ImGui::GetColorU32(IG_WHITE), 32,
                       2.0f);

  // Inner circle background
  draw_list->AddCircleFilled(
      center, radius - 1, ImGui::GetColorU32(ImVec4(0.90f, 0.90f, 0.93f, 1.0f)),
      32);

  // Letter
  char firstLetter[2] = {username[0], '\0'};
  if (firstLetter[0] >= 'a' && firstLetter[0] <= 'z')
    firstLetter[0] -= 32;
  ImVec2 textSize = ImGui::CalcTextSize(firstLetter);
  draw_list->AddText(
      ImVec2(center.x - textSize.x * 0.5f, center.y - textSize.y * 0.5f),
      ImGui::GetColorU32(IG_TEXT), firstLetter);

  // Reserve space for the circle
  ImGui::Dummy(ImVec2(totalSize, totalSize));

  // Username below, centered via draw list
  char shortName[10];
  int len = (int)strlen(username);
  if (len > 8) {
    memcpy(shortName, username, 7);
    shortName[7] = '.';
    shortName[8] = '.';
    shortName[9] = '\0';
  } else {
    memcpy(shortName, username, len + 1);
  }
  ImVec2 nameSize = ImGui::CalcTextSize(shortName);
  float nameX = p.x + (totalSize - nameSize.x) * 0.5f;
  float nameY = p.y + totalSize + 2;
  draw_list->AddText(ImVec2(nameX, nameY), ImGui::GetColorU32(IG_TEXT_SEC),
                     shortName);

  // Reserve space for the text too
  ImGui::Dummy(ImVec2(totalSize, nameSize.y + 2));

  ImGui::EndGroup();
}

// ─── Main Render ────────────────────────────────────────────────────
void UIManager::Render() {
  auto viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->Pos);
  ImGui::SetNextWindowSize(viewport->Size);

  ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration |
                           ImGuiWindowFlags_NoMove |
                           ImGuiWindowFlags_NoSavedSettings;

  ImGui::PushStyleColor(ImGuiCol_WindowBg, IG_SURFACE);
  if (ImGui::Begin("Main Window", nullptr, flags)) {
    switch (currentState) {
    case AppState::Login:
      RenderLogin();
      break;
    case AppState::Register:
      RenderRegister();
      break;
    case AppState::HomeFeed:
      RenderHomeFeed();
      break;
    case AppState::Profile:
      RenderProfile();
      break;
    case AppState::Messages:
      RenderMessages();
      break;
    case AppState::Search:
      RenderSearch();
      break;
    }
  }
  ImGui::End();
  ImGui::PopStyleColor();
}

// ─── Login Screen ───────────────────────────────────────────────────
void UIManager::RenderLogin() {
  auto windowWidth = ImGui::GetWindowSize().x;
  auto windowHeight = ImGui::GetWindowSize().y;

  // Center the login card
  ImGui::SetCursorPos(
      ImVec2(windowWidth * 0.5f - 175, windowHeight * 0.5f - 200));

  // Card with subtle border
  ImGui::PushStyleColor(ImGuiCol_ChildBg, IG_WHITE);
  ImGui::PushStyleColor(ImGuiCol_Border, IG_BORDER);
  ImGui::BeginChild("LoginCard", ImVec2(350, 400), true,
                    ImGuiWindowFlags_NoScrollbar);

  ImGui::Spacing();
  ImGui::Spacing();
  ImGui::Spacing();

  // "Instagram" branding
  const char *brandName = "Instagram";
  ImGui::PushStyleColor(ImGuiCol_Text, IG_TEXT);
  ImVec2 brandSize = ImGui::CalcTextSize(brandName);
  ImGui::SetCursorPosX((350 - brandSize.x) * 0.5f);
  ImGui::Text("%s", brandName);
  ImGui::PopStyleColor();

  ImGui::Spacing();
  ImGui::Spacing();
  ImGui::Spacing();
  ImGui::Spacing();

  // Error message
  if (!errorMessage.empty()) {
    ImGui::PushStyleColor(ImGuiCol_Text, IG_RED);
    ImVec2 errSize = ImGui::CalcTextSize(errorMessage.c_str());
    ImGui::SetCursorPosX((350 - errSize.x) * 0.5f);
    ImGui::TextWrapped("%s", errorMessage.c_str());
    ImGui::PopStyleColor();
    ImGui::Spacing();
  }

  // Input fields with Instagram-style hints
  ImGui::SetCursorPosX(25);
  ImGui::PushItemWidth(300);
  ImGui::PushStyleColor(ImGuiCol_FrameBg, IG_SURFACE);
  ImGui::InputTextWithHint("##LoginUser", "Phone number, username, or email",
                           usernameBuffer, IM_ARRAYSIZE(usernameBuffer));
  ImGui::Spacing();
  ImGui::SetCursorPosX(25);
  ImGui::InputTextWithHint("##LoginPass", "Password", passwordBuffer,
                           IM_ARRAYSIZE(passwordBuffer),
                           ImGuiInputTextFlags_Password);
  ImGui::PopStyleColor();
  ImGui::PopItemWidth();

  ImGui::Spacing();
  ImGui::Spacing();

  // "Log in" button – Instagram Blue, white text
  ImGui::SetCursorPosX(25);
  ImGui::PushStyleColor(ImGuiCol_Button, IG_BLUE);
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IG_BLUE_HOVER);
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, IG_BLUE_DARK);
  ImGui::PushStyleColor(ImGuiCol_Text, IG_WHITE);
  if (ImGui::Button("Log in", ImVec2(300, 36))) {
    if (Database::GetInstance().VerifyUser(usernameBuffer, passwordBuffer,
                                           currentUser)) {
      currentState = AppState::HomeFeed;
      errorMessage = "";
    } else {
      errorMessage = "Sorry, your password was incorrect. Please double-check "
                     "your password.";
    }
  }
  ImGui::PopStyleColor(4);

  ImGui::Spacing();
  ImGui::Spacing();

  // OR divider
  ImGui::SetCursorPosX(25);
  float separatorY = ImGui::GetCursorPosY() + 8;
  ImGui::GetWindowDrawList()->AddLine(
      ImVec2(ImGui::GetWindowPos().x + 25,
             ImGui::GetWindowPos().y + separatorY),
      ImVec2(ImGui::GetWindowPos().x + 150,
             ImGui::GetWindowPos().y + separatorY),
      ImGui::GetColorU32(IG_BORDER), 1.0f);
  ImGui::GetWindowDrawList()->AddLine(
      ImVec2(ImGui::GetWindowPos().x + 200,
             ImGui::GetWindowPos().y + separatorY),
      ImVec2(ImGui::GetWindowPos().x + 325,
             ImGui::GetWindowPos().y + separatorY),
      ImGui::GetColorU32(IG_BORDER), 1.0f);
  ImVec2 orSize = ImGui::CalcTextSize("OR");
  ImGui::SetCursorPosX((350 - orSize.x) * 0.5f);
  ImGui::PushStyleColor(ImGuiCol_Text, IG_TEXT_SEC);
  ImGui::Text("OR");
  ImGui::PopStyleColor();

  ImGui::Spacing();
  ImGui::Spacing();
  ImGui::Spacing();

  // "Forgot password?" link
  const char *forgotText = "Forgot password?";
  ImVec2 forgotSize = ImGui::CalcTextSize(forgotText);
  ImGui::SetCursorPosX((350 - forgotSize.x) * 0.5f);
  ImGui::PushStyleColor(ImGuiCol_Text, IG_BLUE_DARK);
  ImGui::Text("%s", forgotText);
  ImGui::PopStyleColor();

  ImGui::EndChild();

  // "Don't have an account? Sign up" card below
  ImGui::SetCursorPosX(windowWidth * 0.5f - 175);
  ImGui::Spacing();
  ImGui::BeginChild("SignupLink", ImVec2(350, 60), true,
                    ImGuiWindowFlags_NoScrollbar);
  const char *signupPre = "Don't have an account?";
  const char *signupLink = "Sign up";
  ImVec2 preSize = ImGui::CalcTextSize(signupPre);
  ImVec2 linkSize = ImGui::CalcTextSize(signupLink);
  float totalW = preSize.x + 6 + linkSize.x;
  ImGui::SetCursorPos(ImVec2((350 - totalW) * 0.5f, 20));
  ImGui::PushStyleColor(ImGuiCol_Text, IG_TEXT_SEC);
  ImGui::Text("%s", signupPre);
  ImGui::PopStyleColor();
  ImGui::SameLine(0, 6);
  ImGui::PushStyleColor(ImGuiCol_Text, IG_BLUE);
  if (ImGui::Selectable(signupLink, false, 0, linkSize)) {
    currentState = AppState::Register;
    errorMessage = "";
  }
  ImGui::PopStyleColor();
  ImGui::EndChild();

  ImGui::PopStyleColor(2); // ChildBg and Border
}

// ─── Register Screen ────────────────────────────────────────────────
void UIManager::RenderRegister() {
  auto windowWidth = ImGui::GetWindowSize().x;
  auto windowHeight = ImGui::GetWindowSize().y;

  ImGui::SetCursorPos(
      ImVec2(windowWidth * 0.5f - 175, windowHeight * 0.5f - 240));

  ImGui::PushStyleColor(ImGuiCol_ChildBg, IG_WHITE);
  ImGui::PushStyleColor(ImGuiCol_Border, IG_BORDER);
  ImGui::BeginChild("RegisterCard", ImVec2(350, 480), true,
                    ImGuiWindowFlags_NoScrollbar);

  ImGui::Spacing();
  ImGui::Spacing();
  ImGui::Spacing();

  // Branding
  const char *brandName = "Instagram";
  ImVec2 brandSize = ImGui::CalcTextSize(brandName);
  ImGui::SetCursorPosX((350 - brandSize.x) * 0.5f);
  ImGui::Text("%s", brandName);

  ImGui::Spacing();

  const char *subtitle = "Sign up to see photos and videos from your friends.";
  ImGui::PushStyleColor(ImGuiCol_Text, IG_TEXT_SEC);
  ImGui::SetCursorPosX(25);
  ImGui::PushTextWrapPos(325);
  ImGui::TextWrapped("%s", subtitle);
  ImGui::PopTextWrapPos();
  ImGui::PopStyleColor();

  ImGui::Spacing();
  ImGui::Spacing();

  // Error
  if (!errorMessage.empty()) {
    ImGui::PushStyleColor(ImGuiCol_Text, IG_RED);
    ImGui::SetCursorPosX(25);
    ImGui::TextWrapped("%s", errorMessage.c_str());
    ImGui::PopStyleColor();
    ImGui::Spacing();
  }

  // Fields
  ImGui::PushItemWidth(300);
  ImGui::PushStyleColor(ImGuiCol_FrameBg, IG_SURFACE);
  ImGui::SetCursorPosX(25);
  ImGui::InputTextWithHint("##RegUser", "Username", usernameBuffer,
                           IM_ARRAYSIZE(usernameBuffer));
  ImGui::Spacing();
  ImGui::SetCursorPosX(25);
  ImGui::InputTextWithHint("##RegPass", "Password", passwordBuffer,
                           IM_ARRAYSIZE(passwordBuffer),
                           ImGuiInputTextFlags_Password);
  ImGui::Spacing();
  ImGui::SetCursorPosX(25);
  ImGui::InputTextWithHint("##RegCity", "City", cityBuffer,
                           IM_ARRAYSIZE(cityBuffer));
  ImGui::PopStyleColor();
  ImGui::PopItemWidth();

  ImGui::Spacing();
  ImGui::Spacing();

  // Sign up button
  ImGui::SetCursorPosX(25);
  ImGui::PushStyleColor(ImGuiCol_Button, IG_BLUE);
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IG_BLUE_HOVER);
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, IG_BLUE_DARK);
  ImGui::PushStyleColor(ImGuiCol_Text, IG_WHITE);
  if (ImGui::Button("Sign up", ImVec2(300, 36))) {
    if (strlen(usernameBuffer) == 0 || strlen(passwordBuffer) == 0 ||
        strlen(cityBuffer) == 0) {
      errorMessage = "Please fill in all fields.";
    } else if (Database::GetInstance().CreateUser(usernameBuffer,
                                                  passwordBuffer, cityBuffer)) {
      Database::GetInstance().VerifyUser(usernameBuffer, passwordBuffer,
                                         currentUser);
      currentState = AppState::HomeFeed;
      errorMessage = "";
    } else {
      errorMessage = "This username isn't available. Please try another.";
    }
  }
  ImGui::PopStyleColor(4);

  ImGui::Spacing();
  ImGui::Spacing();

  // Terms
  ImGui::PushStyleColor(ImGuiCol_Text, IG_TEXT_SEC);
  ImGui::SetCursorPosX(25);
  ImGui::PushTextWrapPos(325);
  ImGui::TextWrapped("By signing up, you agree to our Terms, Privacy Policy "
                     "and Cookies Policy.");
  ImGui::PopTextWrapPos();
  ImGui::PopStyleColor();

  ImGui::EndChild();

  // "Have an account? Log in" card
  ImGui::SetCursorPosX(windowWidth * 0.5f - 175);
  ImGui::Spacing();
  ImGui::BeginChild("LoginLink", ImVec2(350, 60), true,
                    ImGuiWindowFlags_NoScrollbar);
  const char *loginPre = "Have an account?";
  const char *loginLink = "Log in";
  ImVec2 preSize = ImGui::CalcTextSize(loginPre);
  ImVec2 linkSize = ImGui::CalcTextSize(loginLink);
  float totalW = preSize.x + 6 + linkSize.x;
  ImGui::SetCursorPos(ImVec2((350 - totalW) * 0.5f, 20));
  ImGui::PushStyleColor(ImGuiCol_Text, IG_TEXT_SEC);
  ImGui::Text("%s", loginPre);
  ImGui::PopStyleColor();
  ImGui::SameLine(0, 6);
  ImGui::PushStyleColor(ImGuiCol_Text, IG_BLUE);
  if (ImGui::Selectable(loginLink, false, 0, linkSize)) {
    currentState = AppState::Login;
    errorMessage = "";
  }
  ImGui::PopStyleColor();
  ImGui::EndChild();

  ImGui::PopStyleColor(2);
}

// ─── Sidebar Navigation ────────────────────────────────────────────
void UIManager::RenderNavBar() {
  ImGui::PushStyleColor(ImGuiCol_ChildBg, IG_WHITE);
  ImGui::PushStyleColor(ImGuiCol_Border, IG_BORDER);
  ImGui::BeginChild("Sidebar", ImVec2(220, 0), true);

  ImGui::Spacing();
  ImGui::Spacing();

  // Instagram branding
  ImGui::SetCursorPosX(20);
  ImGui::PushStyleColor(ImGuiCol_Text, IG_TEXT);
  ImGui::Text("Instagram");
  ImGui::PopStyleColor();

  ImGui::Spacing();
  ImGui::Spacing();
  ImGui::Separator();
  ImGui::Spacing();
  ImGui::Spacing();

  // Navigation items with active highlighting
  auto DrawNavButton = [&](const char *icon, const char *label, AppState target,
                           bool hasAction = true) {
    bool isActive = (currentState == target);

    // Highlight active item
    if (isActive) {
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.94f, 0.94f, 0.94f, 1.0f));
      ImGui::PushStyleColor(ImGuiCol_Text, IG_TEXT);
    } else {
      ImGui::PushStyleColor(ImGuiCol_Button, IG_TRANSPARENT);
      ImGui::PushStyleColor(ImGuiCol_Text, IG_TEXT);
    }
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                          ImVec4(0.94f, 0.94f, 0.94f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                          ImVec4(0.90f, 0.90f, 0.90f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));

    char buttonLabel[64];
    snprintf(buttonLabel, sizeof(buttonLabel), "  %s  %s", icon, label);

    ImGui::SetCursorPosX(8);
    if (ImGui::Button(buttonLabel, ImVec2(204, 48)) && hasAction) {
      currentState = target;
    }

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(4);
    ImGui::Spacing();
  };

  DrawNavButton("H", "Home", AppState::HomeFeed);
  DrawNavButton("Q", "Search", AppState::Search);
  DrawNavButton("M", "Messages", AppState::Messages);
  DrawNavButton("P", "Profile", AppState::Profile);

  // Separator before create and logout
  ImGui::Spacing();
  ImGui::Separator();
  ImGui::Spacing();

  // Create post button
  ImGui::PushStyleColor(ImGuiCol_Button, IG_TRANSPARENT);
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                        ImVec4(0.94f, 0.94f, 0.94f, 1.0f));
  ImGui::PushStyleColor(ImGuiCol_Text, IG_TEXT);
  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));
  ImGui::SetCursorPosX(8);
  ImGui::Button("  +  Create", ImVec2(204, 48));
  ImGui::PopStyleVar(2);
  ImGui::PopStyleColor(3);

  // Logout at bottom
  ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 56);
  ImGui::Separator();
  ImGui::Spacing();
  ImGui::PushStyleColor(ImGuiCol_Button, IG_TRANSPARENT);
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                        ImVec4(0.96f, 0.96f, 0.96f, 1.0f));
  ImGui::PushStyleColor(ImGuiCol_Text, IG_TEXT);
  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));
  ImGui::SetCursorPosX(8);
  if (ImGui::Button("  ->  Log out", ImVec2(204, 40))) {
    currentState = AppState::Login;
    memset(usernameBuffer, 0, sizeof(usernameBuffer));
    memset(passwordBuffer, 0, sizeof(passwordBuffer));
    memset(postBuffer, 0, sizeof(postBuffer));
  }
  ImGui::PopStyleVar(2);
  ImGui::PopStyleColor(3);

  ImGui::EndChild();
  ImGui::PopStyleColor(2);
}

// ─── Post Card Helper ───────────────────────────────────────────────
void UIManager::DrawPostCard(const Post &post) {
  ImGui::PushStyleColor(ImGuiCol_ChildBg, IG_WHITE);
  ImGui::PushStyleColor(ImGuiCol_Border, IG_BORDER);
  ImGui::BeginChild((std::string("Post_") + std::to_string(post.id)).c_str(),
                    ImVec2(0, 0),
                    ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_Borders,
                    ImGuiWindowFlags_NoScrollbar);

  // ── Post Header: Avatar + Username + Timestamp ──
  User author;
  Database::GetInstance().GetUserById(post.user_id, author);

  char firstLetter[2] = {author.username.empty() ? '?' : author.username[0],
                         '\0'};
  if (firstLetter[0] >= 'a' && firstLetter[0] <= 'z')
    firstLetter[0] -= 32;

  DrawAvatar(firstLetter, IG_PURPLE, 32.0f);
  ImGui::SameLine();
  ImGui::BeginGroup();
  ImGui::PushStyleColor(ImGuiCol_Text, IG_TEXT);
  ImGui::Text("%s", author.username.c_str());
  ImGui::PopStyleColor();
  ImGui::PushStyleColor(ImGuiCol_Text, IG_TEXT_SEC);
  ImGui::Text("%s", post.timestamp.c_str());
  ImGui::PopStyleColor();
  ImGui::EndGroup();

  ImGui::Spacing();

  // ── Post Image (if available) ──
  if (!postImageLoaded) {
    postImageLoaded = LoadTextureFromFile("../assets/images/sample_post.jpg",
                                          &postImageTexture, &postImageWidth,
                                          &postImageHeight);
  }

  if (postImageLoaded && postImageTexture != 0) {
    float availWidth = ImGui::GetContentRegionAvail().x;
    float ratio = availWidth / (float)postImageWidth;
    float displayHeight = (float)postImageHeight * ratio;
    ImGui::Image((void *)(intptr_t)postImageTexture,
                 ImVec2(availWidth, displayHeight));
    ImGui::Spacing();
  }

  // ── Action Row: Like, Comment, Share ... Save ──
  ImGui::PushStyleColor(ImGuiCol_Button, IG_TRANSPARENT);
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                        ImVec4(0.96f, 0.96f, 0.96f, 1.0f));
  ImGui::PushStyleColor(ImGuiCol_Text, IG_TEXT);
  ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);

  ImGui::Button("Like");
  ImGui::SameLine();
  ImGui::Button("Comment");
  ImGui::SameLine();
  ImGui::Button("Share");

  float saveW = ImGui::CalcTextSize("Save").x + 20;
  ImGui::SameLine(ImGui::GetContentRegionAvail().x - saveW +
                  ImGui::GetCursorPosX());
  ImGui::Button("Save");

  ImGui::PopStyleVar();
  ImGui::PopStyleColor(3);

  ImGui::Spacing();

  // ── Like count ──
  ImGui::PushStyleColor(ImGuiCol_Text, IG_TEXT);
  ImGui::Text("0 likes");
  ImGui::PopStyleColor();

  // ── Caption ──
  ImGui::PushStyleColor(ImGuiCol_Text, IG_TEXT);
  ImGui::Text("%s", author.username.c_str());
  ImGui::PopStyleColor();
  ImGui::SameLine(0, 6);
  ImGui::PushStyleColor(ImGuiCol_Text, IG_TEXT);
  ImGui::TextWrapped("%s", post.content.c_str());
  ImGui::PopStyleColor();

  ImGui::Spacing();
  ImGui::Spacing();

  ImGui::EndChild();
  ImGui::PopStyleColor(2);
}

// ─── Home Feed ──────────────────────────────────────────────────────
void UIManager::RenderHomeFeed() {
  RenderNavBar();
  ImGui::SameLine();

  auto windowWidth = ImGui::GetWindowSize().x;
  float remainingWidth = windowWidth - 220;
  float feedWidth = 470.0f;
  ImGui::SetCursorPosX(220 + (remainingWidth - feedWidth) * 0.5f);

  ImGui::BeginChild("FeedScroll", ImVec2(feedWidth, 0), false);

  ImGui::Spacing();
  ImGui::Spacing();

  // ── Stories Bar (Horizontal) ──
  ImGui::PushStyleColor(ImGuiCol_ChildBg, IG_WHITE);
  ImGui::PushStyleColor(ImGuiCol_Border, IG_BORDER);
  ImGui::BeginChild("StoriesBar", ImVec2(0, 100), true,
                    ImGuiWindowFlags_HorizontalScrollbar);

  ImGui::SetCursorPos(ImVec2(12, 6));
  DrawStoryCircle(currentUser.username.c_str(), IG_PINK);
  ImGui::SameLine(0, 10);
  DrawStoryCircle("explore", IG_ORANGE);
  ImGui::SameLine(0, 10);
  DrawStoryCircle("friends", IG_PURPLE);
  ImGui::SameLine(0, 10);
  DrawStoryCircle("trending", IG_BLUE);
  ImGui::SameLine(0, 10);
  DrawStoryCircle("music", IG_PINK);
  ImGui::SameLine(0, 10);
  DrawStoryCircle("food", IG_ORANGE);
  ImGui::SameLine(0, 10);
  DrawStoryCircle("travel", IG_PURPLE);
  ImGui::SameLine(0, 10);
  DrawStoryCircle("tech", IG_BLUE);

  ImGui::EndChild();
  ImGui::PopStyleColor(2);

  ImGui::Spacing();
  ImGui::Spacing();

  // ── Create Post ──
  ImGui::PushStyleColor(ImGuiCol_ChildBg, IG_WHITE);
  ImGui::PushStyleColor(ImGuiCol_Border, IG_BORDER);
  ImGui::BeginChild("CreatePost", ImVec2(0, 140), true,
                    ImGuiWindowFlags_NoScrollbar);

  // Mini avatar + input
  char myLetter[2] = {
      currentUser.username.empty() ? '?' : currentUser.username[0], '\0'};
  if (myLetter[0] >= 'a' && myLetter[0] <= 'z')
    myLetter[0] -= 32;
  DrawAvatar(myLetter, IG_BLUE, 32.0f);
  ImGui::SameLine();
  ImGui::PushStyleColor(ImGuiCol_FrameBg, IG_SURFACE);
  ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
  ImGui::InputTextMultiline("##NewPost", postBuffer, IM_ARRAYSIZE(postBuffer),
                            ImVec2(-1, 50));
  ImGui::PopItemWidth();
  ImGui::PopStyleColor();

  ImGui::Spacing();

  // Share button
  float btnW = 80;
  ImGui::SetCursorPosX(ImGui::GetWindowWidth() - btnW - 16);
  ImGui::PushStyleColor(ImGuiCol_Button, IG_BLUE);
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IG_BLUE_HOVER);
  ImGui::PushStyleColor(ImGuiCol_Text, IG_WHITE);
  if (ImGui::Button("Share", ImVec2(btnW, 30))) {
    if (strlen(postBuffer) > 0) {
      Database::GetInstance().CreatePost(currentUser.id, postBuffer, "now");
      memset(postBuffer, 0, sizeof(postBuffer));
    }
  }
  ImGui::PopStyleColor(3);

  ImGui::EndChild();
  ImGui::PopStyleColor(2);

  ImGui::Spacing();

  // ── Feed Posts ──
  auto posts = Database::GetInstance().GetFeedPosts(currentUser.id);
  for (const auto &post : posts) {
    DrawPostCard(post);
    ImGui::Spacing();
  }

  ImGui::EndChild();
}

// ─── Profile Page ───────────────────────────────────────────────────
void UIManager::RenderProfile() {
  RenderNavBar();
  ImGui::SameLine();

  auto windowWidth = ImGui::GetWindowSize().x;
  float remainingWidth = windowWidth - 220;
  float profileWidth = 600.0f;
  ImGui::SetCursorPosX(220 + (remainingWidth - profileWidth) * 0.5f);

  ImGui::PushStyleColor(ImGuiCol_ChildBg, IG_WHITE);
  ImGui::PushStyleColor(ImGuiCol_Border, IG_BORDER);
  ImGui::BeginChild("ProfileContent", ImVec2(profileWidth, 0), true);

  ImGui::Spacing();
  ImGui::Spacing();

  // ── Profile Header ──
  ImGui::SetCursorPosX(30);
  char avatarLetter[2] = {
      currentUser.username.empty() ? '?' : currentUser.username[0], '\0'};
  if (avatarLetter[0] >= 'a' && avatarLetter[0] <= 'z')
    avatarLetter[0] -= 32;
  DrawAvatar(avatarLetter, IG_PURPLE, 80.0f);

  ImGui::SameLine(0, 30);
  ImGui::BeginGroup();

  // Username + Edit Profile button
  ImGui::PushStyleColor(ImGuiCol_Text, IG_TEXT);
  ImGui::Text("%s", currentUser.username.c_str());
  ImGui::PopStyleColor();
  ImGui::SameLine(0, 16);

  ImGui::PushStyleColor(ImGuiCol_Button, IG_SURFACE);
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IG_BORDER);
  ImGui::PushStyleColor(ImGuiCol_Text, IG_TEXT);
  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
  ImGui::Button("Edit profile");
  ImGui::PopStyleVar();
  ImGui::PopStyleColor(3);

  ImGui::Spacing();

  // Stats row
  auto userPosts = Database::GetInstance().GetUserPosts(currentUser.id);
  auto friends = Database::GetInstance().GetFriends(currentUser.id);
  ImGui::PushStyleColor(ImGuiCol_Text, IG_TEXT);
  ImGui::Text("%d posts", (int)userPosts.size());
  ImGui::SameLine(0, 24);
  ImGui::Text("%d followers", (int)friends.size());
  ImGui::SameLine(0, 24);
  ImGui::Text("0 following");
  ImGui::PopStyleColor();

  ImGui::Spacing();

  // Location
  ImGui::PushStyleColor(ImGuiCol_Text, IG_TEXT_SEC);
  ImGui::Text("%s", currentUser.city.c_str());
  ImGui::PopStyleColor();

  ImGui::EndGroup();

  ImGui::Spacing();
  ImGui::Spacing();
  ImGui::Separator();
  ImGui::Spacing();
  ImGui::Spacing();

  // ── Posts Section Title ──
  ImGui::PushStyleColor(ImGuiCol_Text, IG_TEXT);
  ImVec2 postsLabel = ImGui::CalcTextSize("POSTS");
  ImGui::SetCursorPosX((profileWidth - postsLabel.x) * 0.5f);
  ImGui::Text("POSTS");
  ImGui::PopStyleColor();
  ImGui::Spacing();
  ImGui::Separator();
  ImGui::Spacing();

  // ── User's Posts ──
  for (const auto &post : userPosts) {
    ImGui::PushStyleColor(ImGuiCol_ChildBg, IG_SURFACE);
    ImGui::BeginChild(
        (std::string("ProfilePost_") + std::to_string(post.id)).c_str(),
        ImVec2(0, 80), true);
    ImGui::PushStyleColor(ImGuiCol_Text, IG_TEXT);
    ImGui::TextWrapped("%s", post.content.c_str());
    ImGui::PopStyleColor();
    ImGui::PushStyleColor(ImGuiCol_Text, IG_TEXT_SEC);
    ImGui::Text("%s", post.timestamp.c_str());
    ImGui::PopStyleColor();
    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::Spacing();
  }

  if (userPosts.empty()) {
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::PushStyleColor(ImGuiCol_Text, IG_TEXT_SEC);
    const char *noPostsText = "No posts yet.";
    ImVec2 npSize = ImGui::CalcTextSize(noPostsText);
    ImGui::SetCursorPosX((profileWidth - npSize.x) * 0.5f);
    ImGui::Text("%s", noPostsText);
    ImGui::PopStyleColor();
  }

  ImGui::EndChild();
  ImGui::PopStyleColor(2);
}

// ─── Messages / DM Page ─────────────────────────────────────────────
void UIManager::RenderMessages() {
  RenderNavBar();
  ImGui::SameLine();

  auto windowWidth = ImGui::GetWindowSize().x;
  float remainingWidth = windowWidth - 220;
  float msgWidth = 600.0f;
  ImGui::SetCursorPosX(220 + (remainingWidth - msgWidth) * 0.5f);

  ImGui::PushStyleColor(ImGuiCol_ChildBg, IG_WHITE);
  ImGui::PushStyleColor(ImGuiCol_Border, IG_BORDER);
  ImGui::BeginChild("MessagesContent", ImVec2(msgWidth, 0), true);

  ImGui::Spacing();
  ImGui::Spacing();

  // Header
  ImGui::PushStyleColor(ImGuiCol_Text, IG_TEXT);
  ImGui::SetCursorPosX(20);
  ImGui::Text("%s", currentUser.username.c_str());
  ImGui::PopStyleColor();
  ImGui::SameLine();
  ImGui::PushStyleColor(ImGuiCol_Text, IG_TEXT_SEC);
  ImGui::Text("  Messages");
  ImGui::PopStyleColor();

  ImGui::Spacing();
  ImGui::Separator();
  ImGui::Spacing();

  // Search bar
  ImGui::SetCursorPosX(20);
  ImGui::PushStyleColor(ImGuiCol_FrameBg, IG_SURFACE);
  ImGui::PushItemWidth(msgWidth - 40);
  ImGui::InputTextWithHint("##SearchDM", "Search", searchBuffer,
                           IM_ARRAYSIZE(searchBuffer));
  ImGui::PopItemWidth();
  ImGui::PopStyleColor();

  ImGui::Spacing();
  ImGui::Separator();
  ImGui::Spacing();

  // Chat area
  ImGui::BeginChild("ChatArea", ImVec2(0, -70), false);

  ImGui::PushStyleColor(ImGuiCol_Text, IG_TEXT_SEC);
  ImGui::Spacing();
  ImGui::Spacing();
  const char *emptyMsg = "Send a message to start a chat.";
  ImVec2 emSize = ImGui::CalcTextSize(emptyMsg);
  ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - emSize.x) * 0.5f);
  ImGui::Text("%s", emptyMsg);
  ImGui::PopStyleColor();

  ImGui::EndChild();

  // Message input bar
  ImGui::Separator();
  ImGui::Spacing();
  ImGui::SetCursorPosX(20);
  ImGui::PushStyleColor(ImGuiCol_FrameBg, IG_SURFACE);
  ImGui::PushItemWidth(msgWidth - 120);
  ImGui::InputTextWithHint("##MsgInput", "Message...", messageBuffer,
                           IM_ARRAYSIZE(messageBuffer));
  ImGui::PopItemWidth();
  ImGui::PopStyleColor();

  ImGui::SameLine();
  ImGui::PushStyleColor(ImGuiCol_Button, IG_BLUE);
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IG_BLUE_HOVER);
  ImGui::PushStyleColor(ImGuiCol_Text, IG_WHITE);
  if (ImGui::Button("Send", ImVec2(60, 0))) {
    memset(messageBuffer, 0, sizeof(messageBuffer));
  }
  ImGui::PopStyleColor(3);

  ImGui::EndChild();
  ImGui::PopStyleColor(2);
}

// ─── Search Page ────────────────────────────────────────────────────
void UIManager::RenderSearch() {
  RenderNavBar();
  ImGui::SameLine();

  auto windowWidth = ImGui::GetWindowSize().x;
  float remainingWidth = windowWidth - 220;
  float searchWidth = 500.0f;
  ImGui::SetCursorPosX(220 + (remainingWidth - searchWidth) * 0.5f);

  ImGui::PushStyleColor(ImGuiCol_ChildBg, IG_WHITE);
  ImGui::PushStyleColor(ImGuiCol_Border, IG_BORDER);
  ImGui::BeginChild("SearchContent", ImVec2(searchWidth, 0), true);

  ImGui::Spacing();
  ImGui::Spacing();

  // Search header
  ImGui::PushStyleColor(ImGuiCol_Text, IG_TEXT);
  ImGui::SetCursorPosX(20);
  ImGui::Text("Search");
  ImGui::PopStyleColor();

  ImGui::Spacing();

  // Search input bar with rounded styling
  ImGui::SetCursorPosX(16);
  ImGui::PushStyleColor(ImGuiCol_FrameBg, IG_SURFACE);
  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);
  ImGui::PushItemWidth(searchWidth - 32);
  bool searchChanged = ImGui::InputTextWithHint(
      "##SearchInput", "Search", searchBuffer, IM_ARRAYSIZE(searchBuffer));
  ImGui::PopItemWidth();
  ImGui::PopStyleVar();
  ImGui::PopStyleColor();

  ImGui::Spacing();
  ImGui::Separator();
  ImGui::Spacing();

  // Search results
  if (strlen(searchBuffer) > 0) {
    auto results = Database::GetInstance().SearchUsers(searchBuffer);

    if (results.empty()) {
      ImGui::Spacing();
      ImGui::Spacing();
      ImGui::PushStyleColor(ImGuiCol_Text, IG_TEXT_SEC);
      const char *noResults = "No results found.";
      ImVec2 nrSize = ImGui::CalcTextSize(noResults);
      ImGui::SetCursorPosX((searchWidth - nrSize.x) * 0.5f);
      ImGui::Text("%s", noResults);
      ImGui::PopStyleColor();
    } else {
      ImGui::PushStyleColor(ImGuiCol_Text, IG_TEXT_SEC);
      ImGui::SetCursorPosX(20);
      ImGui::Text("Results");
      ImGui::PopStyleColor();
      ImGui::Spacing();

      for (const auto &user : results) {
        ImGui::PushStyleColor(ImGuiCol_ChildBg, IG_WHITE);
        ImGui::BeginChild(
            (std::string("UserResult_") + std::to_string(user.id)).c_str(),
            ImVec2(0, 60), false);

        ImGui::SetCursorPos(ImVec2(16, 10));
        char letter[2] = {user.username.empty() ? '?' : user.username[0], '\0'};
        if (letter[0] >= 'a' && letter[0] <= 'z')
          letter[0] -= 32;
        DrawAvatar(letter, IG_PURPLE, 36.0f);
        ImGui::SameLine(0, 12);

        ImGui::BeginGroup();
        ImGui::PushStyleColor(ImGuiCol_Text, IG_TEXT);
        ImGui::Text("%s", user.username.c_str());
        ImGui::PopStyleColor();
        ImGui::PushStyleColor(ImGuiCol_Text, IG_TEXT_SEC);
        ImGui::Text("%s", user.city.c_str());
        ImGui::PopStyleColor();
        ImGui::EndGroup();

        ImGui::EndChild();
        ImGui::PopStyleColor();

        // Divider line
        ImGui::PushStyleColor(ImGuiCol_Separator, IG_BORDER);
        ImGui::Separator();
        ImGui::PopStyleColor();
      }
    }
  } else {
    // Empty state
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::PushStyleColor(ImGuiCol_Text, IG_TEXT_SEC);
    const char *hint1 = "Search for people";
    ImVec2 h1Size = ImGui::CalcTextSize(hint1);
    ImGui::SetCursorPosX((searchWidth - h1Size.x) * 0.5f);
    ImGui::Text("%s", hint1);

    ImGui::Spacing();

    const char *hint2 = "Type a username to find people on Instagram.";
    ImVec2 h2Size = ImGui::CalcTextSize(hint2);
    ImGui::SetCursorPosX((searchWidth - h2Size.x) * 0.5f);
    ImGui::Text("%s", hint2);
    ImGui::PopStyleColor();
  }

  ImGui::EndChild();
  ImGui::PopStyleColor(2);
}
