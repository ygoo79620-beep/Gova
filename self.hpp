#pragma once


#include "../settings.hpp"
#include "../memory/classes.hpp"
#include "../encryptfunction.hpp"
//#include "../content/imgui_notify.h"
//#include "../content/tahoma.h" // <-- Required font!





namespace self {

    void options(void) {

        if (settings::self::enabled) {

            hk_World* World = (hk_World*)*(uint64_t*)(FiveM::World);
            if (!World)
                return;

            hk_Ped* LocalPlayer = World->LocalPlayer();
            if (!LocalPlayer)
                return;

            hk_ObjectNavigationPed* Nav = LocalPlayer->ObjectNavigation();
            if (!LocalPlayer)
                return;

            if (settings::self::godmode) {

                *(bool*)(LocalPlayer + 0x189) = true;
            }
            else {
                *(bool*)(LocalPlayer + 0x189) = false;
            }
        }
    }

    void noclip(void) {
        hk_World* World = (hk_World*)*(uint64_t*)(FiveM::World);
        if (!World)
            return;

     

        hk_Ped* LocalPlayer = World->LocalPlayer();
        if (!LocalPlayer)
            return;

        hk_ObjectNavigationPed* Nav = LocalPlayer->ObjectNavigation();
        if (!Nav)
            return;

        hk_ReplayInterface* ReplayInterface = (hk_ReplayInterface*)*(uint64_t*)(FiveM::ReplayInterface);
        if (!ReplayInterface)
            return;

        hk_VehicleInterface* VehicleInterface = ReplayInterface->VehicleInterface();
        if (!VehicleInterface)
            return;

        

        Vector3 ActualPos = LocalPlayer->GetCoordinate();

        if (settings::self::fastrun) {
            if (FiveM::FiveM_b2944) {
                *(float*)(*(uint64_t*)(LocalPlayer + FiveM::PlayerInfo) + 0x0D40) = settings::self::RunSpeedToSet;
            }
            else {
                *(float*)(*(uint64_t*)(LocalPlayer + FiveM::PlayerInfo) + 0x0CF0) = settings::self::RunSpeedToSet;
            }
        }

        if (LocalPlayer->IsInAVehicule() == true) {
            return;
        }
        if (LocalPlayer->GetHealth() < 100)
            return;

        if (SAFE_CALL(GetAsyncKeyState)(VK_LSHIFT))
            settings::self::noclipspeed = true;
        else
            settings::self::noclipspeed = false;

        if (settings::self::noclipspeed) {
            settings::self::noclipspeed = 1.0f;
        }
        else {
            settings::self::noclipspeed = 0.1f;
        }

        if (settings::self::noclip) {
            DWORD64 addr = FiveM::GetCamera();
            *(float*)(*(uint64_t*)(LocalPlayer + 0x30) + 0x30) = 1;
            Vector3 TPSangles = *(Vector3*)(addr + 0x03D0);
            if (TPSangles == Vector3(0, 0, 0)) {
                TPSangles = *(Vector3*)(addr + 0x40);
            }

            Vector3 newpos = ActualPos;

            // Calculate the intermediate position (lerp) between the current and new position
            if (SAFE_CALL(GetAsyncKeyState)(settings::self::hotkey_forward) & 0x8000) {
                newpos.x += (TPSangles.x * settings::self::noclipspeedlol);
                newpos.y += (TPSangles.y * settings::self::noclipspeedlol);
                newpos.z += (TPSangles.z * settings::self::noclipspeedlol);
            }
            if (SAFE_CALL(GetAsyncKeyState)(settings::self::hotkey_backward) & 0x8000) {
                newpos.x -= (TPSangles.x * settings::self::noclipspeedlol);
                newpos.y -= (TPSangles.y * settings::self::noclipspeedlol);
                newpos.z -= (TPSangles.z * settings::self::noclipspeedlol);
            }
            if (SAFE_CALL(GetAsyncKeyState)(settings::self::hotkey_left) & 0x8000) {
                newpos.x -= (TPSangles.y * settings::self::noclipspeedlol);
                newpos.y += (TPSangles.x * settings::self::noclipspeedlol);
                newpos.z += (TPSangles.z * settings::self::noclipspeedlol);
            }
            if (SAFE_CALL(GetAsyncKeyState)(settings::self::hotkey_right) & 0x8000) {
                newpos.x += (TPSangles.y * settings::self::noclipspeedlol);
                newpos.y -= (TPSangles.x * settings::self::noclipspeedlol);
                newpos.z -= (TPSangles.z * settings::self::noclipspeedlol);
            }
            if (SAFE_CALL(GetAsyncKeyState)(VK_SPACE) & 0x8000) {
                newpos.z += (TPSangles.z * settings::self::noclipspeedlol);
            }

            
            float lerpAmount = 0.5f; 
            newpos = ActualPos + (newpos - ActualPos) * lerpAmount;

            LocalPlayer->SetCoordinate(newpos);
            Nav->SetCoordinate(newpos);
        }
        else {
            
            LocalPlayer->SetVelocity();

            if (settings::self::fastrun) {
                if (FiveM::FiveM_b2944) {
                    *(float*)(*(uint64_t*)(LocalPlayer + FiveM::PlayerInfo) + 0x0D40) = settings::self::RunSpeedToSet;
                }
                else {
                    *(float*)(*(uint64_t*)(LocalPlayer + FiveM::PlayerInfo) + 0x0CF0) = settings::self::RunSpeedToSet;
                }
            }
        }
    }




    void autofarm(void) {
        if (settings::self::autoarm) {

            if (SAFE_CALL(GetAsyncKeyState)(settings::self::hotkey_autofarm)) {
                hk_World* World = (hk_World*)*(uint64_t*)(FiveM::World);
                if (!World)
                    return;

                hk_Ped* LocalPlayer = World->LocalPlayer( );
                if (!LocalPlayer)
                    return;

                hk_ObjectNavigationPed* Nav = LocalPlayer->ObjectNavigation( );
                if (!LocalPlayer)
                    return;

                hk_ReplayInterface* ReplayInterface = (hk_ReplayInterface*)*(uint64_t*)(FiveM::ReplayInterface);
                hk_PedInterface* PedInterface = ReplayInterface->PedInterface( );
                if (PedInterface) {
                    for (int index = 0; index < PedInterface->PedMaximum( ); index++) {
                        hk_Ped* Peds = PedInterface->PedList( )->Ped(index);
                        if (!Peds)
                            continue;
                        if (Peds == LocalPlayer)
                            continue;

                        bool IsPed = false;

                        auto ped_type = Peds->GetPedType( );
                        if (!ped_type) {
                            continue;
                        }
                        ped_type = ped_type << 11 >> 25;

                        if (ped_type != 2) {
                            IsPed = true;
                        }
                        if (!IsPed)
                            continue;

                        auto mypos = LocalPlayer->GetCoordinate( );
                        *(Vector3*)(Peds + 0x90) = Vector3(mypos.x, mypos.y + 1, mypos.z);
                        *(float*)(Peds + 0x280) = 0.1f;
                    }
                }
            }
        }
    }
}
              
