#pragma once

#include "../sdk/sdk.h"
#include <Windows.h>
#include "../main/fivem-external.h"
#include "../overlay/overlay.h"



class i_esp {
public:
    static i_esp& esp();

    void esp_init();
    bool IsOnScreen(D3DXVECTOR2 coords);
    void start();
    void draw_skeleton(uintptr_t ped_base);
    void vehicleesp();
    bool ignoreped = true;
    bool ignoredead = true;

    bool box = false;
    bool cornered = false;
    bool heathbar = false;
    bool armorbar = false;
    bool distance = false;
    bool Weapon = false;
    bool playerid = false;
    bool lines = false;

    bool skeleton = false;
    bool fixedesp = true;
    bool vehiclesnapline = false;
    bool boxnormal = false;
    bool esp_showdistance;
    bool crosshair = false;
    int distanceint = 90;
    int boxtype = 0;
    int HealthBar_type = 0;
    std::vector<std::string> friends;

    ImColor boxcolor = ImColor(235, 230, 230);
    ImColor FovColor = ImColor(0, 0, 0);
    ImColor SilFovColor = ImColor(0, 0, 0);
    ImColor LineColor = ImColor(255, 255, 255, 255);
    ImColor skeletoncolor = ImColor(255, 255, 255, 255);

    int ArmorBar_type = 1;

};