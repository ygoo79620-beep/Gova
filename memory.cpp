#include "memory.h"
#include <iostream>
#include "skStr.h"
#include <conio.h> // para usar getch()
#include <fstream>
#include <string>
#include <windows.h>
#include <iostream>
#include <thread>
#include <fstream>
#include <string>

#include <direct.h>
#include <nlohmann/json.hpp>
#include "../utils.hpp"
#include "../xorstr.hpp"
#include "../Protection/Basic/Strings/xor.hpp"
using json = nlohmann::json;

i_memory& i_memory::reeq()
{
    static i_memory ins;
    return ins;
}

// Función para ocultar la contraseña
std::string getHiddenInput()
{
    std::string input;
    char ch;
    while (true)
    {
        ch = _getch(); // Obtener el siguiente carácter sin mostrarlo
        if (ch == '\r') // Si es la tecla Enter, terminar la entrada
        {
            std::cout << std::endl;
            break;
        }
        else if (ch == '\b') // Si es la tecla de retroceso, borrar el último carácter
        {
            if (!input.empty())
            {
                input.pop_back();
                std::cout << "\b \b"; // Borrar el carácter en la pantalla
            }
        }
        else
        {
            input += ch;
            std::cout << '*'; // Mostrar un asterisco en lugar del carácter
        }
    }
    return input;
}

static BOOL SetConsoleSize(int cols, int rows) {
    HWND hWnd;
    HANDLE hConOut;
    CONSOLE_FONT_INFO fi;
    CONSOLE_SCREEN_BUFFER_INFO bi;
    int w, h, bw, bh;
    RECT rect = { 0, 0, 0, 0 };
    COORD coord = { 0, 0 };
    hWnd = GetConsoleWindow();
    if (hWnd) {
        hConOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hConOut && hConOut != (HANDLE)-1) {
            if (GetCurrentConsoleFont(hConOut, FALSE, &fi)) {
                if (GetClientRect(hWnd, &rect)) {
                    w = rect.right - rect.left;
                    h = rect.bottom - rect.top;
                    if (GetWindowRect(hWnd, &rect)) {
                        bw = rect.right - rect.left - w;
                        bh = rect.bottom - rect.top - h;
                        if (GetConsoleScreenBufferInfo(hConOut, &bi)) {
                            coord.X = bi.dwSize.X;
                            coord.Y = bi.dwSize.Y;
                            if (coord.X < cols || coord.Y < rows) {
                                if (coord.X < cols) {
                                    coord.X = cols;
                                }
                                if (coord.Y < rows) {
                                    coord.Y = rows;
                                }
                                if (!SetConsoleScreenBufferSize(hConOut, coord)) {
                                    return FALSE;
                                }
                            }
                            return SetWindowPos(hWnd, NULL, rect.left, rect.top, cols * fi.dwFontSize.X + bw, rows * fi.dwFontSize.Y + bh, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
                        }
                    }
                }
            }
        }
    }
    return FALSE;
}
void Logf(const char* message) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 4);
    std::cout << xorstr_("\n                       [");
    SetConsoleTextAttribute(hConsole, 15);
    std::cout << xorstr_("+");
    SetConsoleTextAttribute(hConsole, 4);
    std::cout << xorstr_("]");
    SetConsoleTextAttribute(hConsole, 15);
    std::cout << message << std::endl;
}
void logo()
{
    using namespace KeyAuth;
    std::string name = skCrypt("X4valor").decrypt();
    std::string ownerid = skCrypt("md8zlBXYKj").decrypt();
    std::string secret = skCrypt("b31bcd1c90980f58834e66fb70015c2beaff3a16deb1e97bc0c35fc67b6d3256").decrypt();
    std::string version = skCrypt("1.0").decrypt();
    std::string url = skCrypt("https://keyauth.win/api/1.2/").decrypt(); // change if you're self-hosting

    SetConsoleSize(60, 20);

    api KeyAuthApp(name, ownerid, secret, version, url);
}


void saveCredentialsToJson(const std::string& username, const std::string& password) {
    std::string ruta = "C:/Settings";

    int resultado = _mkdir(ruta.c_str());
    json credentials;
    credentials["username"] = username;
    credentials["password"] = password;

    std::ofstream file("C:/Settings/logincheat.json");
    file << credentials.dump(4);  // dump(4) para formatear el JSON con sangrías
    file.close();
}

bool loadCredentialsFromJson(std::string& username, std::string& password) {
    std::ifstream file("C:/Settings/logincheat.json");
    if (!file.is_open()) {
        return false;  // El archivo JSON no existe
    }

    json credentials;
    file >> credentials;
    file.close();

    if (!credentials["username"].is_null() && !credentials["password"].is_null()) {
        username = credentials["username"];
        password = credentials["password"];
        return true;  // Credenciales cargadas con éxito desde el archivo JSON
    }

    return false;  // El archivo JSON no contiene las credenciales adecuadas
}
void slowPrint(const std::string& message, int milliseconds) {
    for (char c : message) {
        std::cout << c << std::flush;  // flush to ensure immediate output
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }
}
void i_memory::initialize()
{
    using namespace KeyAuth;
    std::string name = skCrypt("X4valor").decrypt();
    std::string ownerid = skCrypt("md8zlBXYKj").decrypt();
    std::string secret = skCrypt("b31bcd1c90980f58834e66fb70015c2beaff3a16deb1e97bc0c35fc67b6d3256").decrypt();
    std::string version = skCrypt("1.0").decrypt();
    std::string url = skCrypt("https://keyauth.win/api/1.2/").decrypt(); // change if you're self-hosting

    api KeyAuthApp(name, ownerid, secret, version, url);
    KeyAuthApp.init();
    int delay = 50;
    logo();
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 3);
    std::cout << xorstr_("\n\n\n") << R"(
                             __ ___  __
                            / //_/ |/_/
                           / ,< _>  <  
                          /_/|_/_/|_|  
             

                            )" << xorstr_("\n\n");
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    SetConsoleTextAttribute(hConsole, 4);
    std::string Welcome = (" Loading Cheat...\n\n");
    slowPrint(Welcome, delay);
    Sleep(2500);
    /*KeyAuthApp.init();
    if (!KeyAuthApp.data.success)
    {
        std::cout << skCrypt("\n Status: ") << KeyAuthApp.data.message;
        Sleep(1500);
        exit(0);
    }*/



    SetConsoleTextAttribute(hConsole, 10);

    Sleep(2000);  // Display for 2 seconds (2000 milliseconds)
    std::cout << "Successfully Connected.\n\n";
    SetConsoleTextAttribute(hConsole, 15);  // Reset color to default

    if (std::filesystem::exists("Settings"))
    {
        if (!CheckIfJsonKeyExists("Settings", "username"))
        {
            std::string key = ReadFromJson("Settings", "license");
            KeyAuthApp.license(key);
            if (!KeyAuthApp.data.success)
            {
                std::remove("Settings");
                std::cout << skCrypt("\n Status: ") << KeyAuthApp.data.message;
                Sleep(1500);
                exit(1);
            }
            std::cout << skCrypt("\n\n [Settings Loader] Automatically Logged In.\n");
        }
        else
        {
            std::string username = ReadFromJson("Settings", "username");
            std::string password = ReadFromJson("Settings", "password");
            KeyAuthApp.login(username, password);
            if (!KeyAuthApp.data.success)
            {
                std::remove("Settings");
                std::cout << skCrypt("\n Status: ") << KeyAuthApp.data.message;
                Sleep(1500);
                exit(1);
            }
            std::cout << skCrypt("\n\n Cheats confirmed!\n");
        }
    }
    else
    {

        std::cout << skCrypt("\n [1] Login\n ");
        std::cout << skCrypt("\n [2] Register\n ");
        SetConsoleTextAttribute(hConsole, 7);
        SetConsoleTextAttribute(hConsole, 4);

        std::cout << skCrypt("\n Selection: ");

        SetConsoleTextAttribute(hConsole, 7); //reset color

        int option;
        std::string username;
        std::string password;
        std::string key;

        std::cin >> option;
        switch (option)
        {
        case 1:
            std::cout << skCrypt("\n\n Enter username: ");
            std::cin >> username;
            std::cout << skCrypt("\n Enter password: ");
            std::cin >> password;
            KeyAuthApp.login(username, password);
            break;
        case 2:
            std::cout << skCrypt("\n\n Enter username: ");
            std::cin >> username;
            std::cout << skCrypt("\n Enter password: ");
            std::cin >> password;
            std::cout << skCrypt("\n Enter license key: ");
            std::cin >> key;
            KeyAuthApp.regstr(username, password, key);
            break;
        default:
            std::cout << skCrypt("\n\n Status: Failure: Invalid Selection");
            Sleep(3000);
            exit(0);
        }

        if (!KeyAuthApp.data.success)
        {
            std::cout << skCrypt("\n Status: ") << KeyAuthApp.data.message;
            Sleep(2500);
            exit(0);
        }
        if (username.empty() || password.empty())
        {
            WriteToJson("Settings", "license", key, false, "", "");
            Logf(xorstr_("License confirmed!"));
            Sleep(2000);
        }
        else
        {
            WriteToJson("Settings", "username", username, true, "password", password);
            Logf(xorstr_("License confirmed!"));
            Sleep(2000);
        }
    }
    {
        // Guardar las credenciales en el archivo JSON
        window = FindWindowA("grcWindow", 0);
        if (!window)
        {
            logo();
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            std::cout << "Fivem not Found!";
            Sleep(2000);
        }
        logo();
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        GetWindowThreadProcessId(window, &pid);
        hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
    }
}

bool i_memory::find_build(const wchar_t* processName) {

    bool exists = false;
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry))
        while (Process32Next(snapshot, &entry))
            if (!_wcsicmp(entry.szExeFile, processName))
                exists = true;

    CloseHandle(snapshot);
    return exists;
}



uintptr_t i_memory::get_base_address(std::string a1)
{
    std::wstring pname = std::wstring(a1.begin(), a1.end());
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
    if (hSnap == INVALID_HANDLE_VALUE)
        return 0;
    MODULEENTRY32 ent;
    ent.dwSize = sizeof(ent);
    if (Module32First(hSnap, &ent))
    {
        do
        {
            if (!_wcsicmp(ent.szModule, pname.c_str()))
            {
                CloseHandle(hSnap);
                return (uintptr_t)ent.modBaseAddr;
            }
        } while (Module32Next(hSnap, &ent));
    }
    CloseHandle(hSnap);
    return 0;
}


HWND i_memory::get_process_wnd(uint32_t pid)
{
    std::pair<HWND, uint32_t> params = { 0, pid };
    BOOL bResult = EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
        auto pParams = (std::pair<HWND, uint32_t>*)(lParam);
        uint32_t processId = 0;

        if (GetWindowThreadProcessId(hwnd, reinterpret_cast<LPDWORD>(&processId)) && processId == pParams->second) {
            SetLastError((uint32_t)-1);
            pParams->first = hwnd;
            return FALSE;
        }

        return TRUE;

        }, (LPARAM)&params);

    if (!bResult && GetLastError() == -1 && params.first)
        return params.first;

    return NULL;
}
