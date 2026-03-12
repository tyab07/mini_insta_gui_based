#pragma once
#include "../db/Database.h"
#include "imgui.h"
// For texture ID (using OpenGL)
#include <GL/gl.h>

enum class AppState { Login, Register, HomeFeed, Profile, Messages, Search };

class UIManager {
public:
  UIManager();
  void Render();

  // Utility to load images
  bool LoadTextureFromFile(const char *filename, GLuint *out_texture,
                           int *out_width, int *out_height);

private:
  void RenderLogin();
  void RenderRegister();
  void RenderNavBar();
  void RenderHomeFeed();
  void RenderProfile();
  void RenderMessages();
  void RenderSearch();

  // Style configuration
  void SetupInstagramTheme();

  // UI helpers
  void DrawAvatar(const char *letter, ImVec4 bgColor, float size = 32.0f);
  void DrawPostCard(const Post &post);
  void DrawStoryCircle(const char *username, ImVec4 ringColor);

  AppState currentState = AppState::Login;
  User currentUser;

  // Input buffers
  char usernameBuffer[256] = "";
  char passwordBuffer[256] = "";
  char cityBuffer[256] = "";
  char postBuffer[512] = "";
  char searchBuffer[256] = "";
  char messageBuffer[512] = "";

  std::string errorMessage;

  // Cached textures
  bool postImageLoaded = false;
  GLuint postImageTexture = 0;
  int postImageWidth = 0;
  int postImageHeight = 0;
};
