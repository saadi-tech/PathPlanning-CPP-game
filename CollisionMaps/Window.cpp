#include "Window.h"
#include "CollisionMap.h"
#include "CollisionMapCollection.h"
#include "SampleMonsters.h"

#include <iostream>
#include <vector>
#include "glm/gtc/matrix_transform.hpp"
#include <algorithm>

#define CAM_MOVE_SPD 2.f
#define SCENE_ROTATION -90.f

glm::mat4x4 matProjection{ glm::mat4x4(1.f) };
glm::mat4x4 matModel{ glm::mat4x4(1.f) };
int iWidth{ STARTING_WIDTH }, iHeight{ STARTING_HEIGHT };
float fZoom{ 50.f };
float fPosX{ 0.f };
float fPosY{ 0.f };

CollisionMapCollection collision_map_collection;
SampleMonsters sample_monsters;

int iPreviousKeyState_P = GLFW_RELEASE;
int iPreviousKeyState_O = GLFW_RELEASE;

void UpdateProjection() {

    float fAspect = (float)iWidth / iHeight;

    if (fAspect >= 1.f)
        matProjection = glm::ortho(-fZoom * fAspect + fPosX, fZoom * fAspect + fPosX, -fZoom + fPosY, fZoom + fPosY, 1.f, -1.f);
    else
        matProjection = glm::ortho(-fZoom + fPosX, fZoom + fPosX, -fZoom / fAspect + fPosY, fZoom / fAspect + fPosY, 1.f, -1.f);
}

Window::EXIT_REASON Window::Begin()
{
   int x, y;
   std::cout << "Please enter a X Coordinate, enter 0 for default selection." << std::endl;
   std::cin >> x;

   std::cout << "Please enter a Y Coordinate, enter 0 for default selection." << std::endl;
   std::cin >> y;

   if (x == 0 && y == 0)
   {
      x = -455163;
      y = 195077;
   }

   const CollisionMap* collision_map = collision_map_collection.GetCollisionMap(x, y);
   if (collision_map)
   {

      // Init GLFW
      if (!glfwInit())
         return WINDOW_GLFW_FAIL;

      // Init window object
      w = glfwCreateWindow(STARTING_WIDTH, STARTING_HEIGHT, "CollisionMap Test", nullptr, nullptr);

      if (w == NULL)
      {
         glfwTerminate();
         return WINDOW_INIT_FAIL;
      }

      glfwMakeContextCurrent(w);
      
      auto _on_resize = [](GLFWwindow* w, int wi, int he)
      {static_cast<Window*>(glfwGetWindowUserPointer(w))->OnResize(wi, he); };
      auto _on_mouse_click = [](GLFWwindow* w, int btn, int a, int mods)
      {static_cast<Window*>(glfwGetWindowUserPointer(w))->OnMouseClick(btn, a, mods); };
      auto _on_scroll = [](GLFWwindow* w, double xoffset, double yoffset)
      {static_cast<Window*>(glfwGetWindowUserPointer(w))->OnScroll(xoffset, yoffset); };
      auto _on_mouse_move = [](GLFWwindow* w, double xpos, double ypos)
      { static_cast<Window*>(glfwGetWindowUserPointer(w))->OnMouseMove(w, static_cast<float>(xpos), static_cast<float>(ypos)); };

      glfwSetFramebufferSizeCallback(w, _on_resize);
      glfwSetMouseButtonCallback(w, _on_mouse_click);
      glfwSetCursorPosCallback(w, _on_mouse_move);
      glfwSetScrollCallback(w, _on_scroll);
      glfwSwapInterval(1);

      if (glewInit() != GLEW_OK) {

         glfwTerminate();
         return WINDOW_GLEW_FAIL;
      }

      sample_monsters.OutputInitialLocationInfo();

      mCellGrid.Load(collision_map->m_cell_map);
      mCellComplex.Load(*collision_map);

      //CellMonster tmp1; tmp1.Load(sample_monsters.GetMonsterByName("ExpectedVillage")); mCellMonsters.push_back(tmp1);
      CellMonster tmp2; tmp2.Load(sample_monsters.GetMonsterByName("ExpectedValid1")); mCellMonsters.push_back(tmp2);
      CellMonster tmp3; tmp3.Load(sample_monsters.GetMonsterByName("ExpectedValid2")); mCellMonsters.push_back(tmp3);
      //CellMonster tmp4; tmp4.Load(sample_monsters.GetMonsterByName("ExpectedMaybedInValid")); mCellMonsters.push_back(tmp4);
      //CellMonster tmp5; tmp5.Load(sample_monsters.GetMonsterByName("ExpectedInValid")); mCellMonsters.push_back(tmp5);
      //CellMonster tmp6; tmp6.Load(sample_monsters.GetMonsterByName("ExpectedNone")); mCellMonsters.push_back(tmp6);

      
      matModel = glm::mat4x4(1.f);
      matModel *= glm::rotate(matModel, glm::radians(SCENE_ROTATION), glm::vec3(0.0f, 0.0f, 1.0f));

      UpdateProjection();
      const Monster* target_monster = sample_monsters.GetMonsterByName("ExpectedValid2");
      
      const Monster* actual_monster = sample_monsters.GetMonsterByName("ExpectedValid1");
      std::cout << actual_monster->GetCellX() << "," << actual_monster->GetCellY() << std::endl;
      std::cout << target_monster->GetCellX() << "," << target_monster->GetCellY() << std::endl;
      std::cout << "Visible: " << actual_monster->CanView(target_monster->GetLocation()) << std::endl;

      StartLoop();

      glfwDestroyWindow(w);
      w = nullptr;

      
   }
   else
   {
      std::cout << "Collision Map not found at coordinates (" + std::to_string(x) + ", " + std::to_string(y) + ")." << std::endl;
   }

   return WINDOW_FINISHED;
}

void Window::StartLoop()
{
    double dLastTime = glfwGetTime(), dTimer = dLastTime;
    double dDeltaTime = 0, dNowTime = 0;
    int iFrames = 0, iUpdates = 0;

    while (!glfwWindowShouldClose(w)) {

        dNowTime = glfwGetTime();
        dDeltaTime += (dNowTime - dLastTime) / (1.f / 60.f);
        dLastTime = dNowTime;

        // Monsters only move once per second...
        while (dDeltaTime >= 1.0) {

            OnUpdate(dNowTime);
            iUpdates++;
            dDeltaTime--;
        }

        OnRender();
        iFrames++;

        if (glfwGetTime() - dTimer > 1.0) {

            dTimer++;
            iUpdates = 0, iFrames = 0;
        }
    }
}

void Window::OnUpdate(double dTime)
{
    KeyListener();

    const Monster* target_monster = sample_monsters.GetMonsterByName("ExpectedValid2");


   
    // For demo purposes, ExpectedValid1 chases ExpectedValid2, and all others Roam or try to.
    // When they can see each other or can no longer see each other, a notification is printed to console.
    for (CellMonster& monster : mCellMonsters)
    {
        Monster* actual_monster = monster.mMonster;
        
        
        if (actual_monster->m_name == "ExpectedValid1" && target_monster)
        {
           
           actual_monster->Chase(target_monster->GetLocation());
        }
        else
        {
           actual_monster->Roam();
        }
    }
}

void Window::OnRender()
{
    glfwPollEvents();

    glClearColor(0.1f, 0.5f, 0.1f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);


    mCellComplex.UpdateMatrices(matProjection, matModel);
    mCellComplex.Draw();

    mCellGrid.UpdateMatrices(matProjection, matModel);
    mCellGrid.Draw();

    for (CellMonster& cm : mCellMonsters)
    {
        if (!cm.mMonster->CanDraw())
            continue;

        cm.UpdateMatrices(matProjection, matModel);
        cm.Draw();
    }


    glfwSwapBuffers(w);
}

void Window::OnResize(int wi, int he)
{
    if (wi < 1 || he < 1)
        return;

    iWidth = wi;
    iHeight = he;
    glViewport(0, 0, iWidth, iHeight);

    UpdateProjection();
}

void Window::OnMouseClick(int iBtn, int iAction, int iMods)
{

}

void Window::OnMouseMove(GLFWwindow* w, float x, float y)
{
    float vp[4];
    glGetFloatv(GL_VIEWPORT, vp);

    double screen_x, screen_y;
    glfwGetCursorPos(w, &screen_x, &screen_y);
    glm::vec3 screen_pos = glm::vec3(static_cast<float>(screen_x), static_cast<float>(screen_y), 0.f);

    glm::vec3 world_pos = glm::unProject(screen_pos, glm::mat4x4(1.f), matProjection, glm::vec4(vp[0], vp[1], vp[2], vp[3]));

    //std::cout << world_pos.x << " / " << world_pos.y << std::endl;
}

void Window::KeyListener()
{
    bool bMove{ false };

    if (glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS) {

        fPosY += CAM_MOVE_SPD;
        bMove = true;
    }
    else if (glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS) {

        fPosY -= CAM_MOVE_SPD;
        bMove = true;
    }

    if (glfwGetKey(w, GLFW_KEY_D) == GLFW_PRESS) {

        fPosX += CAM_MOVE_SPD;
        bMove = true;
    }
    else if (glfwGetKey(w, GLFW_KEY_A) == GLFW_PRESS) {

        fPosX -= CAM_MOVE_SPD;
        bMove = true;
    }
    else if (glfwGetKey(w, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
       glfwSetWindowShouldClose(w, 1);
    }

    if (bMove)
        UpdateProjection();


    int current_keystate_p = glfwGetKey(w, GLFW_KEY_P);
    int current_keystate_o = glfwGetKey(w, GLFW_KEY_O);

    if (current_keystate_p == GLFW_PRESS && iPreviousKeyState_P == GLFW_RELEASE)
        CellMonster::fTickTime -= .01f;
    else if (current_keystate_o == GLFW_PRESS && iPreviousKeyState_O == GLFW_RELEASE)
       CellMonster::fTickTime += .01f;

    CellMonster::fTickTime = glm::clamp(CellMonster::fTickTime, .005f, 2.f);

    iPreviousKeyState_P = current_keystate_p;
    iPreviousKeyState_O = current_keystate_o;
}

void Window::OnScroll(double xoffset, double yoffset)
{
    if (yoffset > 0)
        fZoom -= 2.5f;
    else
        fZoom += 2.5f;

    fZoom = std::max(5.f, std::min(fZoom, 19999.f));

    UpdateProjection();
}
