#include "overlay.h"
#include <sstream>
#include "million.h"
#include "../Hotkey/hotkey.h"
#include "IconsFontAwesome6.h"
#include "IconsFontAwesome6Brands.h"
#include <process.h>
#include "../xor.h"
#include "../Encryption.h"
#include "../settings.hpp"
#include "imgui/imguinotify.hpp"
#include "../encryptstrings.hpp"
//pero ara dale otra vez 
int selectedHeal = 200;
int selectedArmor = 100;
i_overlay& i_overlay::here()
{
	static i_overlay ins;
	return ins;
}
void DrawHelpText(const char* helpText) {
	if (ImGui::IsItemHovered()) {
		ImGui::BeginTooltip();
		ImGui::Text("%s", helpText);
		ImGui::EndTooltip();
	}
}
DWORD weapon_current[] = {
	0x93E220BD, 0xA0973D5E, 0x24B17070, 0x2C3731D9, 0xAB564B93, 0x787F0BB, 0xBA45E8B8, 0x23C9F95C, 0xFDBC8A50, 0x497FACC3, //Throwables
	0x34A67B97, 0xFBAB5776, 0x060EC506, //Miscellaneous
	0xB1CA77B1, 0xA284510B, 0x42BF8A85, 0x7F7497E5, 0x6D544C99, 0x63AB0442, 0x0781FE4A, 0xB62D1F67, //Heavy Weapons
	0x05FC3C11, 0x0C472FE2, 0xA914799, 0xC734385A, 0x6A6C02E0, //Sniper Rifles
	0x9D07F764, 0x7FD62962, 0xDBBD7280, 0x61012683, //Light Machine Guns
	0xBFEFFF6D, 0x394F415C, 0x83BF0278, 0xFAD1F1C9, 0xAF113F99, 0xC0A3098D, 0x969C3D67, 0x7F229F94, 0x84D6FAFD, 0x624FE830, //Assault Rifles
	0x13532244, 0x2BE6766B, 0x78A97CD0, 0xEFE7E2DF, 0x0A3D4D34, 0xDB1AA450, 0xBD248B55, 0x476BF155, //Submachine Guns
	0x1B06D571, 0xBFE256D4, 0x5EF9FEC4, 0x22D8FE39, 0x3656C8C1, 0x99AEEB3B, 0xBFD21232, 0x88374054, 0xD205520E, 0x83839C4, 0x47757124, 0xDC4DB296, 0xC1B3C3D1, 0xCB96392F, 0x97EA20B8, 0xAF3696A1, //Pistols
	0x92A27487, 0x958A4A8F, 0xF9E6AA4B, 0x84BD7BFD, 0x8BB05FD7, 0x440E4788, 0x4E875F73, 0xF9DCBF2D, 0xD8DF3C3C, 0x99B507EA, 0xDD5DF8D9, 0xDFE37640, 0x678B81B1, 0x19044EE0, 0xCD274149, 0x94117305, 0x3813FC08, //Mele
	0x1D073A89, 0x555AF99A, 0x7846A318, 0xE284C527, 0x9D61E50F, 0xA89CB99E, 0x3AABBBAA, 0xEF951FBB, 0x12E82D3D
};

const char* weaponType[]{ "Throwable", "Misc", "Heavy", "Sniper", "Pistol", "Melee", "Light", "SMG", "Rifle", "Shotgun", "Unknown" };

bool i_overlay::LoadTextureFromFile(const char* filename, PDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height)
{
	// Load texture from disk
	PDIRECT3DTEXTURE9 texture;
	HRESULT hr = D3DXCreateTextureFromFileA(p_Device, filename, &texture);
	if (hr != S_OK)
		return false;

	// Retrieve description of the texture surface so we can access its size
	D3DSURFACE_DESC my_image_desc;
	texture->GetLevelDesc(0, &my_image_desc);
	*out_texture = texture;
	*out_width = (int)my_image_desc.Width;
	*out_height = (int)my_image_desc.Height;
	return true;
}

std::int64_t __stdcall i_overlay::wndproc(HWND hwnd, unsigned int message, WPARAM wparam, LPARAM lparam)
{

	std::int64_t ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	ImGui_ImplWin32_WndProcHandler(hwnd, message, wparam, lparam);
	if (i_overlay::here().showmenu)
		return 1;

	return DefWindowProcA(i_overlay::here().MyWnd, message, wparam, lparam);
}


void styledark()
{



	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
	static const ImWchar icons_ranges_brands[] = { ICON_MIN_FAB, ICON_MAX_16_FAB, 0 };
	ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
	ImFont* FontAwesome = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(fa6_solid_compressed_data, fa6_solid_compressed_size, 14.f, &icons_config, icons_ranges);
	ImFont* FontAwesomeBrands = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(fa_brands_400_compressed_data, fa_brands_400_compressed_size, 14.f, &icons_config, icons_ranges_brands);

}

HRESULT i_overlay::init_wndparams(HWND hwnd)
{
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_Object)))
		return E_FAIL;

	ZeroMemory(&p_Params, sizeof(p_Params));
	p_Params.Windowed = TRUE;
	p_Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	p_Params.hDeviceWindow = hwnd;
	p_Params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	p_Params.BackBufferFormat = D3DFMT_A8R8G8B8;
	p_Params.BackBufferWidth = Width;
	p_Params.BackBufferHeight = Height;
	p_Params.EnableAutoDepthStencil = TRUE;
	p_Params.AutoDepthStencilFormat = D3DFMT_D16;
	p_Params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(p_Object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &p_Params, 0, &p_Device))) {
		p_Object->Release();
		return E_FAIL;
	}

	ImGui::CreateContext();

	// LoadTextureFromFileAsync("unknown2.png", &my_texture, &my_image_width, &my_image_height);
	// Load the texture asynchronously to prevent potential UI freezes

	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = NULL;

	char windows_directory[MAX_PATH];

	// std::string segoeui_dir = (std::string)windows_directory + "\\Fonts\\segoeui.ttf";

	// global34 = io.Fonts->AddFontFromFileTTF(segoeui_dir.c_str(), 18.0f, &font_config, io.Fonts->GetGlyphRangesCyrillic());
	// ico_combo2 = io.Fonts->AddFontFromMemoryTTF(&icon, sizeof icon, 19, NULL, io.Fonts->GetGlyphRangesCyrillic());

	global3 = io.Fonts->AddFontFromMemoryTTF(segoui, sizeof(segoui), 17, NULL, io.Fonts->GetGlyphRangesCyrillic());

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX9_Init(p_Device);

	return S_OK;
}

void i_overlay::cleanup_d3d()
{
	if (p_Device != NULL) {
		p_Device->EndScene();
		p_Device->Release();
	}
	if (p_Object != NULL) {
		p_Object->Release();
	}
}

void set_window_target()
{
	i_overlay& overlay = i_overlay::here();
	i_memory& memory = i_memory::reeq();

	while (true) {
		overlay.GameWnd = memory.get_process_wnd(memory.pid);

		if (overlay.GameWnd) {
			ZeroMemory(&overlay.GameRect, sizeof(overlay.GameRect));
			GetWindowRect(overlay.GameWnd, &overlay.GameRect);

			DWORD dwStyle = GetWindowLong(overlay.GameWnd, GWL_STYLE);
			if (dwStyle & WS_BORDER) {
				overlay.GameRect.top += 32;
				overlay.Height -= 39;
			}

			overlay.ScreenCenterX = overlay.Width / 2;
			overlay.ScreenCenterY = overlay.Height / 2;

			MoveWindow(overlay.MyWnd, overlay.GameRect.left, overlay.GameRect.top, overlay.Width, overlay.Height, true);
		}
	}
}

char Licence[50] = "";
void i_overlay::setup_window()
{
	(HANDLE)_beginthreadex(nullptr, 0, reinterpret_cast<_beginthreadex_proc_type>(&set_window_target), 0, 0, 0);
	WNDCLASSEXA wcex = {
		sizeof(WNDCLASSEXA),
		0,
		wndproc,
		0,
		0,
		nullptr,
		LoadIcon(nullptr, IDI_APPLICATION),
		LoadCursor(nullptr, IDC_ARROW),
		nullptr,
		nullptr,
		("Chrome"),
		LoadIcon(nullptr, IDI_APPLICATION)
	};

	RECT Rect;
	GetWindowRect(GetDesktopWindow(), &Rect);

	RegisterClassExA(&wcex);

	MyWnd = CreateWindowA(("Chrome"), ("Chrome"), WS_POPUP, Rect.left, Rect.top, Rect.right, Rect.bottom, NULL, NULL, wcex.hInstance, NULL);
	SetWindowLong(MyWnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT);
	SetLayeredWindowAttributes(MyWnd, RGB(0, 0, 0), 255, LWA_ALPHA);

	MARGINS margin = { -1 };
	DwmExtendFrameIntoClientArea(MyWnd, &margin);

	ShowWindow(MyWnd, SW_SHOW);
}


void i_overlay::style()
{
	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();

	style.WindowRounding = 6;

	style.Colors[ImGuiCol_Text] = ImColor(230, 230, 230);         // Text color
	style.Colors[ImGuiCol_TextDisabled] = ImColor(0.40f, 0.40f, 0.40f, 1.00f); // Disabled text color
	style.Colors[ImGuiCol_WindowBg] = ImColor(13, 13, 13);     // Window background color
	style.Colors[ImGuiCol_ChildBg] = ImColor(16, 16, 16);      // Blue child window background color with reduced alpha
	style.Colors[ImGuiCol_PopupBg] = ImColor(26, 26, 26);        // Popup background color
	style.Colors[ImGuiCol_Border] = ImColor(38, 38, 38);        // Blue window border color
	style.Colors[ImGuiCol_BorderShadow] = ImColor(230, 230, 230);    // Window border shadow color
	style.Colors[ImGuiCol_FrameBg] = ImColor(26, 26, 26);      // Frame background color
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.00f, 0.40f, 0.75f, 0.60f);// Blue frame background color when hovered
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.35f, 0.35f, 0.35f, 0.70f); // Frame background color when active
	style.Colors[ImGuiCol_TitleBg] = ImColor(13, 13, 13);        // Title background color
	style.Colors[ImGuiCol_TitleBgActive] = ImColor(13, 13, 13);  // Title background color when active
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImColor(13, 13, 13);  // Title background color when collapsed
	style.Colors[ImGuiCol_MenuBarBg] = ImColor(26, 26, 26);    // Menu bar background color
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);   // Scrollbar background color
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f); // Scrollbar grab color
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f); // Scrollbar grab color when hovered
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.80f, 0.80f, 0.80f, 0.70f);  // Scrollbar grab color when active
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.78f, 0.78f, 0.78f, 1.00f);    // Checkbox checkmark color
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);   // Slider grab color
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.90f, 0.90f, 0.90f, 0.70f);  // Slider grab color when active
	style.Colors[ImGuiCol_Button] = ImColor(13, 13, 13);        // Button color
	style.Colors[ImGuiCol_ButtonHovered] = ImColor(13, 13, 13);    // Blue button color when hovered
	style.Colors[ImGuiCol_ButtonActive] = ImColor(13, 13, 13);    // Button color when active
	style.Colors[ImGuiCol_Header] = ImVec4(0.30f, 0.30f, 0.30f, 0.70f);        // Header color
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.80f); // Header color when hovered
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.50f, 0.50f, 0.50f, 0.80f);  // Header color when active
	style.Colors[ImGuiCol_Separator] = ImVec4(0.50f, 143.0f, 179.50f, 0.79f);     // Separator 
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.50f, 143.0f, 179.50f, 0.79f); // Separator color when hovered
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.50f, 143.0f, 179.50f, 0.79f);// Separator color when active
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.50f, 0.50f, 0.50f, 0.50f);     // Resize grip color
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.60f, 0.60f, 0.60f, 0.70f); // Resize grip color when hovered
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);// Resize grip color when active
	style.Colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.15f, 0.15f, 0.86f);           // Tab color
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.00f, 0.40f, 0.75f, 0.80f);    // Blue tab color when hovered
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.23f, 0.23f, 0.23f, 0.85f);     // Tab color when active
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.08f, 0.08f, 0.08f, 0.97f);   // Tab color when unfocused
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.13f, 0.13f, 0.13f, 0.97f); // Tab color when unfocused and active
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);     // Plot lines color
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.00f, 0.40f, 0.75f, 1.00f); // Blue plot lines color when hovered
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f); // Plot histogram color
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.00f, 0.40f, 0.75f, 1.00f); // Blue plot histogram color when hovered
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f); // Selected text background color


	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
	static const ImWchar icons_ranges_brands[] = { ICON_MIN_FAB, ICON_MAX_16_FAB, 0 };
	ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
	ImFont* FontAwesome = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(fa6_solid_compressed_data, fa6_solid_compressed_size, 14.f, &icons_config, icons_ranges);
	ImFont* FontAwesomeBrands = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(fa_brands_400_compressed_data, fa_brands_400_compressed_size, 14.f, &icons_config, icons_ranges_brands);

}

void i_overlay::CustomStyleColor()
{
	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();

	style.WindowRounding = 6;

	style.Colors[ImGuiCol_Text] = ImColor(230, 230, 230);         // Text color
	style.Colors[ImGuiCol_TextDisabled] = ImColor(0.40f, 0.40f, 0.40f, 1.00f); // Disabled text color
	style.Colors[ImGuiCol_WindowBg] = ImColor(13, 13, 13);     // Window background color
	style.Colors[ImGuiCol_ChildBg] = ImColor(16, 16, 16);      // Blue child window background color with reduced alpha
	style.Colors[ImGuiCol_PopupBg] = ImColor(26, 26, 26);        // Popup background color
	style.Colors[ImGuiCol_Border] = ImColor(18, 18, 18);        // Blue window border color
	style.Colors[ImGuiCol_BorderShadow] = ImColor(230, 230, 230);    // Window border shadow color
	style.Colors[ImGuiCol_FrameBg] = ImColor(26, 26, 26);      // Frame background color
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.00f, 0.40f, 0.75f, 0.60f);// Blue frame background color when hovered
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.35f, 0.35f, 0.35f, 0.70f); // Frame background color when active
	style.Colors[ImGuiCol_TitleBg] = ImColor(13, 13, 13);        // Title background color
	style.Colors[ImGuiCol_TitleBgActive] = ImColor(13, 13, 13);  // Title background color when active
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImColor(13, 13, 13);  // Title background color when collapsed
	style.Colors[ImGuiCol_MenuBarBg] = ImColor(26, 26, 26);    // Menu bar background color
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);   // Scrollbar background color
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f); // Scrollbar grab color
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f); // Scrollbar grab color when hovered
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.80f, 0.80f, 0.80f, 0.70f);  // Scrollbar grab color when active
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.78f, 0.78f, 0.78f, 1.00f);    // Checkbox checkmark color
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);   // Slider grab color
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.90f, 0.90f, 0.90f, 0.70f);  // Slider grab color when active
	style.Colors[ImGuiCol_Button] = ImColor(13, 13, 13);        // Button color
	style.Colors[ImGuiCol_ButtonHovered] = ImColor(13, 13, 13);    // Blue button color when hovered
	style.Colors[ImGuiCol_ButtonActive] = ImColor(13, 13, 13);    // Button color when active
	style.Colors[ImGuiCol_Header] = ImVec4(0.30f, 0.30f, 0.30f, 0.70f);        // Header color
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.80f); // Header color when hovered
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.50f, 0.50f, 0.50f, 0.80f);  // Header color when active
	style.Colors[ImGuiCol_Separator] = ImVec4(0.50f, 143.0f, 179.50f, 0.79f);     // Separator
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.50f, 143.0f, 179.50f, 0.79f); // Separator color when hovered
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.50f, 143.0f, 179.50f, 0.79f);// Separator color when active
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.50f, 0.50f, 0.50f, 0.50f);     // Resize grip color
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.60f, 0.60f, 0.60f, 0.70f); // Resize grip color when hovered
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);// Resize grip color when active
	style.Colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.15f, 0.15f, 0.86f);           // Tab color
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.00f, 0.40f, 0.75f, 0.80f);    // Blue tab color when hovered
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.23f, 0.23f, 0.23f, 0.85f);     // Tab color when active
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.08f, 0.08f, 0.08f, 0.97f);   // Tab color when unfocused
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.13f, 0.13f, 0.13f, 0.97f); // Tab color when unfocused and active
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);     // Plot lines color
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.00f, 0.40f, 0.75f, 1.00f); // Blue plot lines color when hovered
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f); // Plot histogram color
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.00f, 0.40f, 0.75f, 1.00f); // Blue plot histogram color when hovered
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f); // Selected text background color

}
int slider_int = 0;
int slider_int2 = 0;
int slider_int3s = 0;
bool test = false;
bool test2 = false;
bool test3 = false;
bool test4 = false;
const char* items[3] = { "head","chest", "legs" };
int selected_cfg = 0;
const char* cfg[]{ "Slot 1", "Slot 2", "Slot 3", "Slot 4", "Slot 5", "Slot 6", "Slot 7", "Slot 8", "Slot 9", "Slot 10" };

int current_tabs = 1;

char font[200] = "";

float Height_Y = 0;
float Width_X = 0;
char filterText[128];
char filterVehText[128];
char filterConText[128];
char filterModText[128];
char filterSearchText[128];
const char* VehicleName;
const char* ModelName;


/*
const char* const KeyNames[] = {
	"VK_RBUTTON",
	"VK_LBUTTON",
	"VK_CANCEL",
	"VK_MBUTTON",
	"VK_XBUTTON1",
	"VK_XBUTTON2",
	"VK_BACK",
	"VK_TAB",
	"VK_CLEAR",
	"VK_RETURN",
	"VK_SHIFT",
	"VK_CONTROL",
	"VK_ALT",
	"VK_PAUSE",
	"VK_CAPITAL",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
};
*/

const char* const KeyNames[] = {
	"Right Click",
	"ALT",
	"Control",
	"Middle Mouse",
	"Mouse 5",
	"Mouse 4",
	"Left Click",
};
// Main loop
bool loaderOpen = true;
bool aimbot = false;
bool esp = false;
bool autotarget = false;
bool autoshot = false;
bool drawfov = false;

ImFont* LexendRegular;
ImFont* Icons = nullptr;
ImFont* big;
ImFont* Normale;
ImFont* InterMedium;
IDirect3DTexture9* logo = nullptr;
IDirect3DTexture9* foto = nullptr;
auto s = ImVec2{}, p = ImVec2{}, gs = ImVec2{ 1020, 718 };



int fov = 0;
int smooth = 0;

int Hotkey = 2;
bool active = true;
bool active2 = false;
int subtab2 = 0;
int tab = 0;
bool enable = false;
int subtab3 = 0;
int subtab = 0;
int tabs = false;
bool selfignore = false;
uint64_t selectedped = 0;
ImVec2 watermarkPos = ImVec2(10, 5);
ImFont* segu = nullptr;
void i_overlay::menu()
{
	styledark();
	ImDrawList* draw;



	ImGuiWindowFlags main_window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
	ImVec2 pos;
	ImGui::SetNextWindowSize(ImVec2(689, 390));
	ImGui::Begin(Encrypt("KX&UNEX"), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	{
		if (settings::visuals::watermark) {
			ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[3]);
			auto draw_list = ImGui::GetBackgroundDrawList();

			auto watermark = ("KX Softwares [Owner Kaneki | 7946 | B2699]");
			auto watermark_size = ImGui::CalcTextSize(watermark);

			auto yOffset = 0;
			auto offset = 4; 

			draw_list->AddRectFilled(ImVec2(6, 4 + yOffset), ImVec2((4 * 2) + watermark_size.x + 6, 6 + yOffset), ImColor(0, 94, 255, 255));
			draw_list->AddRectFilled(ImVec2(6, 6 + yOffset), ImVec2((4 * 2) + watermark_size.x + 6, 25 + yOffset), ImColor(0.0f, 0.0f, 0.0f));
			draw_list->AddText(ImVec2(10, 6 + yOffset), ImColor(255, 255, 255, 255), watermark);
			ImGui::PopFont();
		}
		const auto& pWindowDrawList = ImGui::GetWindowDrawList();
		ImGui::PushFont(big);
		ImGui::SameLine();
		ImGui::TextColored(ImColor(255, 255, 255), (_xor_("   KX Softwares").c_str()));
		ImGui::SameLine();
		ImGui::TextColored(ImColor(0, 143, 179), (_xor_("[ALPHA]").c_str()));
		ImGui::PopFont();

		//ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(p.x + 40, p.y + 642 + 32), 17, ImColor(178, 182, 255), 50); //ôîí àâàòàðêè

		//ImGui::GetWindowDrawList()->AddImage(foto, ImVec2(p.x + 24 + 1, p.y + 642 + 32 - 16 + 1), ImVec2(p.x + 24 + 30 + 1, p.y + 642 + 32 + 30 - 16 + 1)); //àâàòàðêà

		//ImGui::GetWindowDrawList()->AddText(InterMedium, 17, ImVec2(p.x + 67, p.y + 660), ImColor(255, 255, 255), "Credits: Mister_Byman");//Nickname
		//ImGui::GetWindowDrawList()->AddText(InterMedium, 13, ImVec2(p.x + 67, p.y + 676), ImColor(127, 130, 135), "Expiry date: 21.12.2024");//date

		ImGui::SameLine();
		ImGui::SetCursorPosX(550);
		ImGui::Text("By Kaneki [9449]");
		ImGui::SetCursorPosX(570);
		ImGui::SetCursorPosY(30);
		ImGui::TextColored(ImVec4(0.0f, 0.56f, 0.7f, 1.0f), "TILL 10/20");
		ImGui::Separator();

		ImGui::SetCursorPosX(20);
		ImGui::SetCursorPosY(80);
		ImGui::PushFont(Normale);
		ImGui::TextColored(ImColor(0, 143, 179), (_xor_("Players").c_str()));
		ImGui::PopFont();

		ImGui::SetCursorPosX(20);
		ImGui::SetCursorPosY(110);
		if (ImGui::Button("SELF", ImVec2()))
		{
			tabs = 1;
		}
		ImGui::SetCursorPosX(20);
		ImGui::SetCursorPosY(140);
		if (ImGui::Button("PLAYERS", ImVec2()))
		{
			tabs = 2;
		}

		ImGui::SetCursorPosX(20);
		ImGui::SetCursorPosY(170);
		if (ImGui::Button("VEHICLES", ImVec2()))
		{
			tabs = 3;
		}

		ImGui::SetCursorPosX(20);
		ImGui::SetCursorPosY(200);
		ImGui::PushFont(Normale);
		ImGui::TextColored(ImColor(0, 143, 179), (_xor_("Visuals").c_str()));
		ImGui::PopFont();


		ImGui::SetCursorPosX(20);
		ImGui::SetCursorPosY(230);
		if (ImGui::Button("ESP VISUAL", ImVec2()))
		{
			tabs = 4;
		}

		ImGui::SetCursorPosX(20);
		ImGui::SetCursorPosY(265);
		ImGui::PushFont(Normale);
		ImGui::TextColored(ImColor(0, 143, 179), (_xor_("Miscellaneous").c_str()));
		ImGui::PopFont();

		ImGui::SetCursorPosX(20);
		ImGui::SetCursorPosY(295);
		if (ImGui::Button("COMBAT", ImVec2()))
		{
			tabs = 5;
		}

		ImGui::SetCursorPosX(20);
		ImGui::SetCursorPosY(325);
		if (ImGui::Button("MODULES", ImVec2()))
		{
			tabs = 6;
		}

		ImGui::SetCursorPosX(20);
		ImGui::SetCursorPosY(355);
		if (ImGui::Button("WEAPON SPAWNER", ImVec2()))
		{
			tabs = 7;
		}
		ImGui::SetCursorPosX(20);
		ImGui::SetCursorPosY(390);
		ImGui::PushFont(Normale);
		ImGui::TextColored(ImColor(0, 143, 179), (_xor_("Settings").c_str()));
		ImGui::PopFont();

		ImGui::SetCursorPosX(20);
		ImGui::SetCursorPosY(420);
		if (ImGui::Button("MISC", ImVec2()))
		{
			tabs = 8;
		}


		if (tabs == 1)
		{
			ImGui::SetCursorPosX(250);
			ImGui::SetCursorPosY(32);
			ImGui::PushFont(Normale);
			ImGui::TextColored(ImColor(255, 255, 255), (_xor_("Players >").c_str()));
			ImGui::PopFont();
			ImGui::SetCursorPosX(320);
			ImGui::SetCursorPosY(32);
			ImGui::PushFont(Normale);
			ImGui::TextColored(ImColor(0, 143, 179), (_xor_("Self").c_str()));
			ImGui::PopFont();
			ImGuiStyle& style = ImGui::GetStyle();
			style.Colors[ImGuiCol_ButtonHovered] = ImColor(0, 143, 179);    // Blue button color when hovered
			style.Colors[ImGuiCol_ButtonActive] = ImColor(77, 77, 77);
			ImGui::SetCursorPosY(85);
			ImGui::SetCursorPosX(180);
			ImGui::BeginChild("Self", ImVec2(200, 320), TRUE);
			ImGui::Text("Self Option");
			ImGui::Separator();
			ImGui::SetCursorPosY(40);
			ImGui::Checkbox("GodMode", &i_exploits::exp().godmodee);
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text("Semi-Godmode Will Be Sometimes Detected By other Anticheats [USE IT BY YOUR OWN RISK].");
				ImGui::EndTooltip();
			}
			ImGui::SetCursorPosY(70);
			ImGui::Checkbox("Infinite Stamina", &i_exploits::exp().infinitestamina);
			ImGui::SetCursorPosY(100);
			ImGui::Checkbox(("Fov Changer"), &i_exploits::exp().customfov);
			ImGui::SetCursorPosY(130);
			ImGui::Checkbox("Run-Speed", &i_exploits::exp().customspeed);
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text("Fast Run is Sometimes Risky To Use Because The Anticheats detecting the Option And Will Let You Ban [SEMI-UD SEMI RISK].");
				ImGui::EndTooltip();
			}
			ImGui::SetCursorPosY(160);
			ImGui::SetCursorPosY(190);
			ImGui::SliderInt("Run Speed", &i_exploits::exp().playerspeed, 1.0f, 100);
			ImGui::SliderInt((("Swim Speed")), &i_exploits::exp().swimspeed2, 1.0f, 100);
			ImGui::SliderInt((("Fov")), &i_exploits::exp().fovvalue, 50.0f, 150);
			ImGui::EndChild();
			ImGui::SetCursorPosX(430);
			ImGui::SetCursorPosY(85);
			ImGui::BeginChild("Preview", ImVec2(250, 320), TRUE);
			ImGui::SetCursorPosY(15);
			ImGui::SetCursorPosY(15);
				if (ImGui::Button("Give Health", ImVec2(230, 25)))
				{
					i_memory::reeq().Write<float>(i_dupa::skid().i_localplayer + 0x280, static_cast<float>(selectedHeal));
				}
				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::Text("Heal Risk Bannads Anti Cheats [SET ARMOR RISK].");
					ImGui::EndTooltip();
				}
				ImGui::SetCursorPosY(55);
				if (ImGui::Button("Give Armor", ImVec2(230, 25)))
				{
					uintptr_t armadura = i_dupa::skid().armoroffset;
					i_memory::reeq().Write<float>(i_dupa::skid().i_localplayer + armadura, static_cast<float>(selectedArmor));
				}
				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::Text("Heal Risk Bannads Anti Cheats [SET HEAL RISK].");
					ImGui::EndTooltip();
				}
			//ImGui::SetCursorPosY(115);
			//ImGui::SetCursorPosX(45);
			//ImGui::SliderFloat("", &noclip, 0.0f, 0.5f);
			//ImGui::SetCursorPosY(155);
			//ImGui::SetCursorPosX(45);
			//ImGui::SliderFloat("", &honrizontal, 0.0f, 0.5f);

			//ImGui::SetCursorPosY(100);
			//ImGui::SetCursorPosX(45);
			//ImGui::Text("Noclip Speed");

			//ImGui::SetCursorPosY(140);
			//ImGui::SetCursorPosX(45);
			//ImGui::Text("Noclip H Speed");
			//ImGui::EndChild();
		}

		if (tabs == 2)
		{
			ImGui::SetCursorPosX(250);
			ImGui::SetCursorPosY(32);
			ImGui::PushFont(Normale);
			ImGui::TextColored(ImColor(255, 255, 255), (_xor_("Miscellaneous >").c_str()));
			ImGui::PopFont();
			ImGui::SetCursorPosX(370);
			ImGui::SetCursorPosY(32);
			ImGui::PushFont(Normale);
			ImGui::TextColored(ImColor(0, 143, 179), (_xor_("Player List").c_str()));
			ImGui::PopFont();

			ImGui::SetCursorPosX(180);
			ImGui::SetCursorPosY(85);
			ImGui::BeginChild("List", ImVec2(250, 320), TRUE);
			ImGui::Text("Player List");
			ImGui::Separator();
			{
				static char searchQuery[256];

				ImGui::PushFont(global3);
				{
					ImGui::InputText("Search", searchQuery, sizeof(searchQuery));

					uintptr_t ClosestPlayer = 0;
					uintptr_t ReplayInterface = i_dupa::skid().i_replay;
					uintptr_t localplayer = i_dupa::skid().i_localplayer;
					if (ReplayInterface)
					{
						uintptr_t PedReplayInterface = i_memory::reeq().Read<DWORD64>(ReplayInterface + 0x18);
						uintptr_t PedList = i_memory::reeq().Read<DWORD64>(PedReplayInterface + 0x100);
						int entitylist = i_memory::reeq().Read<int>(PedReplayInterface + 0x108);

						bool firstSeparator = true;

						for (int i = 0U; i < entitylist; i++)
						{
							if (!PedList) continue;

							uintptr_t Ped = i_memory::reeq().Read<uintptr_t>(PedList + (i * 0x10));
							if (!Ped) continue;

							if (!i_memory::reeq().Read<uintptr_t>(Ped + i_dupa::skid().playerinfo))
								continue;

							uint64_t playerinfos = i_memory::reeq().Read<uint64_t>(Ped + i_dupa::skid().playerinfo);
							int playeridaddr = i_memory::reeq().Read<int>(playerinfos + 0x88);
							std::string playeridstr = std::to_string(playeridaddr);

							if (!searchQuery[0] || playeridstr.find(searchQuery) != std::string::npos)
							{
								if (!firstSeparator)
								{
									ImGui::Separator();
								}
								firstSeparator = false;

								ImGui::Text(playeridstr.c_str());
								if (ImGui::IsItemClicked())
								{
									selectedped = Ped;
								}
							}
						}
					}
				}
			}
			ImGui::EndChild();

			ImGui::SetCursorPosX(460);
			ImGui::SetCursorPosY(85);
			ImGui::BeginChild("Config", ImVec2(200, 320), TRUE);
			ImGui::Text("Trolling");
			ImGui::Separator();

			ImGui::Text("ID:");
			ImGui::SameLine();
			uint64_t playerinfos = i_memory::reeq().Read<uint64_t>(selectedped + i_dupa::skid().playerinfo);
			int playeridaddr = i_memory::reeq().Read<int>(playerinfos + 0x88);
			std::string playeridstr = std::to_string(playeridaddr);

			ImGui::Text(playeridstr.c_str());
			auto it = std::find(i_esp::esp().friends.begin(), i_esp::esp().friends.end(), playeridstr);

			if (it != i_esp::esp().friends.end())
				ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Friend: True");
			else
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Friend: False");

			if (ImGui::Button(skCrypt("Teleport To Player")))
			{
				D3DXVECTOR3 pos = i_memory::reeq().Read<D3DXVECTOR3>(selectedped + 0x90);
				uintptr_t localplayer = i_dupa::skid().i_localplayer;
				uintptr_t navig = i_memory::reeq().Read<uintptr_t>(localplayer + 0x30);
				i_memory::reeq().Write<D3DXVECTOR3>(navig + 0x30, pos);
				i_memory::reeq().Write<D3DXVECTOR3>(localplayer + 0x90, pos);

			}

			if (it != i_esp::esp().friends.end())
			{
				if (ImGui::Button(skCrypt("Remove Friend")))
				{

					int index = (it - i_esp::esp().friends.begin());
					i_esp::esp().friends.erase(i_esp::esp().friends.begin() + index);
				}
			}
			else
			{
				if (ImGui::Button(skCrypt("Add Friend")))
				{
					i_esp::esp().friends.push_back(playeridstr);
				}
			}
			ImGui::EndChild();
		}

		if (tabs == 3)
		{
			ImGuiStyle& style = ImGui::GetStyle();
			style.Colors[ImGuiCol_ButtonHovered] = ImColor(0, 143, 179);    // Blue button color when hovered
			style.Colors[ImGuiCol_ButtonActive] = ImColor(77, 77, 77);

			ImGui::SetCursorPosX(250);
			ImGui::SetCursorPosY(32);
			ImGui::PushFont(Normale);
			ImGui::TextColored(ImColor(255, 255, 255), (_xor_("Miscellaneous >").c_str()));
			ImGui::PopFont();
			ImGui::SetCursorPosX(370);
			ImGui::SetCursorPosY(32);
			ImGui::PushFont(Normale);
			ImGui::TextColored(ImColor(0, 143, 179), (_xor_("Vehicles Spawner").c_str()));
			ImGui::PopFont();
			ImGui::SetCursorPosX(180);
			ImGui::SetCursorPosY(85);
			ImGui::BeginChild("arrr", ImVec2(250, 320), TRUE);
			ImGui::Text("Vehicles");
			ImGui::Separator();
			ImGui::Checkbox(skCrypt("Sort by Model"), &settings::vehicle::vehicle_spawning::SortByType);
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "(!)"); // Rotes Symbol

			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text("Chackbox One View Vehicle Spwaner RISK .[SORT BY MODEL SYSTEM].");
				ImGui::EndTooltip();
			}
			if (settings::vehicle::vehicle_spawning::SortByType)
			{
				ImGui::Text("Filter Vehicle");
				ImGui::InputText(skCrypt("##ffff"), filterVehText, sizeof(filterVehText));

				int selectedItem = -1;

				ImGui::ListBoxHeader(skCrypt("##VehicleList"), ImVec2(ImGui::GetWindowWidth() - ImGui::GetStyle().ItemSpacing.x, ImGui::GetContentRegionAvail().y - 50));

				for (int i = 0; i < IM_ARRAYSIZE(settings::vehicle::vehicle_spawning::allVehicleNames); i++) {
					const char* weaponName = settings::vehicle::vehicle_spawning::allVehicleNames[i];

					// Vérifier si l'élément correspond au filtre
					if (strstr(weaponName, filterVehText) != nullptr) {
						bool isSelected = (settings::vehicle::vehicle_spawning::Selected == i);
						if (ImGui::Selectable(weaponName, isSelected)) {
							selectedItem = i;
						}
					}
				}
				VehicleName = settings::vehicle::vehicle_spawning::allVehicleNames[settings::vehicle::vehicle_spawning::Selected];
				settings::vehicle::vehicle_spawning::VehicleToSpawn = rage::joaat(VehicleName);
				ImGui::ListBoxFooter();

				if (selectedItem != -1) {
					settings::vehicle::vehicle_spawning::Selected = selectedItem;
				}
				if (ImGui::Button(skCrypt("Spawm Vehicle"))) {
					settings::vehicle::spawnvehicule = true;
				}
				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::Text("Spwan Vehicle Dont Working Wait Fixs Ready [Spwan Vehicle SYSTEM].");
					ImGui::EndTooltip();
				}
				{

				}
			}
			ImGui::EndChild();
			ImGui::SetCursorPosX(460);
			ImGui::SetCursorPosY(85);
			ImGui::BeginChild("Vh", ImVec2(200, 320), TRUE);
			ImGui::Text("Action");
			ImGui::Separator();
			ImGui::SetCursorPosY(50);
			ImGui::Button("Warp", ImVec2(200, 25));
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text("Warp Checkbox Dont Working Wait Fixs Ready [Warp SYSTEM].");
				ImGui::EndTooltip();
			}
			ImGui::SetCursorPosY(90);
			ImGui::Button("Unlock", ImVec2(200, 25));
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text("Unlock Checkbox Dont Working Wait Fixs Ready [Unlock SYSTEM].");
				ImGui::EndTooltip();
			}
			ImGui::Checkbox(skCrypt("Vehicle Speed"), &i_exploits::exp().boostvehicle);
			ImGui::SliderFloat(skCrypt(("Speed")), &i_exploits::exp().boostvalue, 0.0f, 100);
			if (ImGui::Button("Repair Vehicle", ImVec2(200, 25)))
			{
				uintptr_t localplayer = i_dupa::skid().i_localplayer;
				uintptr_t actualcar = i_memory::reeq().Read<uintptr_t>(localplayer + i_dupa::skid().i_veh);
				i_memory::reeq().Write<float>(actualcar + i_dupa::skid().i_enginehealth, 1000.f);
			}
			if (ImGui::Button("Break Vehicle", ImVec2(200, 25)))
			{
				uintptr_t localplayer = i_dupa::skid().i_localplayer;
				uintptr_t actualcar = i_memory::reeq().Read<uintptr_t>(localplayer + i_dupa::skid().i_veh);
				i_memory::reeq().Write<float>(actualcar + i_dupa::skid().i_enginehealth, 0.f); //  nice , do it wait
			}
			if (ImGui::Button("Red Vehicle", ImVec2(200, 25)))
			{
				uintptr_t localplayer = i_dupa::skid().i_localplayer;
				uintptr_t cur_vehicle = i_memory::reeq().Read<uintptr_t>(localplayer + i_dupa::skid().i_veh);
				auto vehicle_mods_ptr = i_memory::reeq().Read<uintptr_t>(cur_vehicle + 0x48);
				if (vehicle_mods_ptr) {
					auto model_info = i_memory::reeq().Read<uintptr_t>(vehicle_mods_ptr + 0x0020);
					if (model_info) {
						float red = 1.0f;
						i_memory::reeq().Write<float>(model_info + 0xA4, red);
						i_memory::reeq().Write<float>(model_info + 0xA8, red);
					}
				}
			}
			{

			}
			ImGui::EndChild();
		}

		if (tabs == 4)
		{

			ImGuiStyle& style = ImGui::GetStyle();
			style.Colors[ImGuiCol_ButtonHovered] = ImColor(0, 143, 179);    // Blue button color when hovered
			style.Colors[ImGuiCol_ButtonActive] = ImColor(77, 77, 77);
			ImGui::SetCursorPosX(250);
			ImGui::SetCursorPosY(32);
			ImGui::PushFont(Normale);
			ImGui::TextColored(ImColor(255, 255, 255), (_xor_("ESP >").c_str()));
			ImGui::PopFont();
			ImGui::SetCursorPosX(295);
			ImGui::SetCursorPosY(32);
			ImGui::PushFont(Normale);
			ImGui::TextColored(ImColor(0, 143, 179), (_xor_("Visual").c_str()));
			ImGui::PopFont();

			ImGui::SetCursorPosX(180);
			ImGui::SetCursorPosY(85);
			ImGui::BeginChild("espvisual", ImVec2(230, 320), TRUE);
			ImGui::Text("Visual");
			ImGui::Separator();
			ImGui::SetCursorPosY(40);
			ImGui::Checkbox(skCrypt("Box"), &i_esp::esp().box);
			//ImGui::SetCursorPosY(130);
			ImGui::Checkbox(skCrypt("Weapon"), &i_esp::esp().Weapon);
			//ImGui::SetCursorPosY(160);
			ImGui::Checkbox("Distance", &i_esp::esp().distance);
			//ImGui::SetCursorPosY(190);
			ImGui::Checkbox("Health Bar", &i_esp::esp().heathbar);
			//ImGui::SetCursorPosY(220);
			ImGui::Checkbox("Armor Bar", &i_esp::esp().armorbar);
			ImGui::Checkbox(("Skeleton"), &i_esp::esp().skeleton);
			ImGui::Checkbox(("Lines"), &i_esp::esp().lines);
			ImGui::Checkbox("CrossHair", &i_esp::esp().crosshair);
			ImGui::Checkbox(("Ignore Ped"), &i_esp::esp().ignoreped);
			ImGui::SliderInt((("Distance")), &i_esp::esp().distanceint, 0.0f, 500);
			//ImGui::SetCursorPosY(250);
			//ImGui::Text("Text Type");
			//const char* items[] = { "DropShadow", "Outlined" };
			//static int selectedItem = 0;
			//ImGui::Combo("", &selectedItem, items, IM_ARRAYSIZE(items));

			//ImGui::Text("Font Type");
			//const char* type[] = { "Modern", "Original" };
			//static int font = 0;
			//ImGui::Combo("", &font, type, IM_ARRAYSIZE(type));

			ImGui::SetCursorPosY(310);
			ImGui::EndChild();
			ImGui::SetCursorPosX(430);
			ImGui::SetCursorPosY(85);
			ImGui::BeginChild("Setings", ImVec2(250, 320), TRUE);
			ImGui::Text("PREVIEW");
			ImGui::Separator();

			//crosshair preview
			if (i_esp::esp().crosshair)
			{

				ImVec2 previewWindowPos = ImGui::GetWindowPos();
				ImVec2 previewWindowSize = ImGui::GetWindowSize();
				float center_x = previewWindowPos.x + (previewWindowSize.x / 2.f);
				float center_y = previewWindowPos.y + (previewWindowSize.y / 2.f);

				ImVec2 lineStart(center_x - 6, center_y);
				ImVec2 lineEnd(center_x + 7, center_y);
				ImVec2 lineStartVertical(center_x, center_y - 6);
				ImVec2 lineEndVertical(center_x, center_y + 7);
				ImU32 lineColor = IM_COL32(255, 255, 255, 255);

				ImGui::GetForegroundDrawList()->AddLine(lineStart, lineEnd, lineColor);
				ImGui::GetForegroundDrawList()->AddLine(lineStartVertical, lineEndVertical, lineColor);
			}

			//Corner Box preview
			if (i_esp::esp().box)
			{
				// Calcular el centro de la ventana de preview
				ImVec2 previewWindowPos = ImGui::GetWindowPos();
				ImVec2 previewWindowSize = ImGui::GetWindowSize();
				float center_x = previewWindowPos.x + (previewWindowSize.x / 2.f);
				float center_y = previewWindowPos.y + (previewWindowSize.y / 2.f);


				float Width = 100.0f;
				float Height = 100.0f;

				float boxMargin = 5.0f;

				ImVec2 topLeft(center_x - Width / 2 + boxMargin, center_y - Height / 2 + boxMargin);
				ImVec2 bottomRight(center_x + Width / 2 - boxMargin, center_y + Height / 2 - boxMargin);

				float cornerSize = 10.0f;

				ImGui::GetForegroundDrawList()->AddLine(ImVec2(topLeft.x, topLeft.y), ImVec2(topLeft.x + cornerSize, topLeft.y), i_esp::esp().boxcolor, 1.0f);
				ImGui::GetForegroundDrawList()->AddLine(ImVec2(topLeft.x, topLeft.y), ImVec2(topLeft.x, topLeft.y + cornerSize), i_esp::esp().boxcolor, 1.0f);
				ImGui::GetForegroundDrawList()->AddLine(ImVec2(bottomRight.x - cornerSize, topLeft.y), ImVec2(bottomRight.x, topLeft.y), i_esp::esp().boxcolor, 1.0f);
				ImGui::GetForegroundDrawList()->AddLine(ImVec2(bottomRight.x, topLeft.y), ImVec2(bottomRight.x, topLeft.y + cornerSize), i_esp::esp().boxcolor, 1.0f);
				ImGui::GetForegroundDrawList()->AddLine(ImVec2(topLeft.x, bottomRight.y - cornerSize), ImVec2(topLeft.x, bottomRight.y), i_esp::esp().boxcolor, 1.0f);
				ImGui::GetForegroundDrawList()->AddLine(ImVec2(topLeft.x, bottomRight.y), ImVec2(topLeft.x + cornerSize, bottomRight.y), i_esp::esp().boxcolor, 1.0f);
				ImGui::GetForegroundDrawList()->AddLine(ImVec2(bottomRight.x - cornerSize, bottomRight.y), ImVec2(bottomRight.x, bottomRight.y), i_esp::esp().boxcolor, 1.0f);
				ImGui::GetForegroundDrawList()->AddLine(ImVec2(bottomRight.x, bottomRight.y - cornerSize), ImVec2(bottomRight.x, bottomRight.y), i_esp::esp().boxcolor, 1.0f);

				ImGui::GetForegroundDrawList()->AddRectFilled(topLeft, bottomRight, IM_COL32(70, 70, 70, 70));
			}
			//Normal Box preview
			if (i_esp::esp().boxnormal)
			{
				ImVec2 previewWindowPos = ImGui::GetWindowPos();
				ImVec2 previewWindowSize = ImGui::GetWindowSize();
				float center_x = previewWindowPos.x + (previewWindowSize.x / 2.f);
				float center_y = previewWindowPos.y + (previewWindowSize.y / 2.f);

				float Width = 100.0f;
				float Height = 100.0f;

				float boxMargin = 5.0f;

				ImVec2 topLeft(center_x - Width / 2 + boxMargin, center_y - Height / 2 + boxMargin);
				ImVec2 bottomRight(center_x + Width / 2 - boxMargin, center_y + Height / 2 - boxMargin);

				ImU32 lineColor = i_esp::esp().boxcolor;
				ImU32 fillColor = IM_COL32(70, 70, 70, 50);

				ImGui::GetForegroundDrawList()->AddRect(topLeft, bottomRight, lineColor, 0);

				ImGui::GetForegroundDrawList()->AddRectFilled(topLeft, bottomRight, fillColor);
			}
			//Health Bar Preview
			if (i_esp::esp().heathbar)
			{
				ImVec2 previewWindowPos = ImGui::GetWindowPos();
				ImVec2 previewWindowSize = ImGui::GetWindowSize();
				float center_x = previewWindowPos.x + (previewWindowSize.x / 2.f);
				float center_y = previewWindowPos.y + (previewWindowSize.y / 2.f);

				float Width = 92.0f;
				float Height = 5.0f;

				float HealthPed = 200;
				float HealthDistance = 0;

				float HealthPercentage = (HealthPed - 100) / 100.0f; // Ajusta esta lógica según tus necesidades

				ImColor healthColor = ImColor(14, 110, 17, 255);
				ImColor backgroundColor = ImColor(0, 0, 0, 255);

				float barX = center_x + (Width / 2) + HealthDistance + 5; // Ajusta la posición X aquí
				float barY = center_y + Height / 2 - 2; // Ajusta la posición Y aquí, por ejemplo, agregamos 10 píxeles hacia abajo

				i_overlay::here().DrawHealthBarhorizontal(ImVec2(barX, barY), ImVec2(barX, barY), ImVec2(Width, Height), backgroundColor);
				i_overlay::here().DrawHealthBarhorizontal(ImVec2(barX, barY), ImVec2(barX, barY), ImVec2(Width * HealthPercentage, Height), healthColor);
			}


			//Armor Bar Preview
			if (i_esp::esp().armorbar)
			{
				ImVec2 previewWindowPos = ImGui::GetWindowPos();
				ImVec2 previewWindowSize = ImGui::GetWindowSize();
				float center_x = previewWindowPos.x + (previewWindowSize.x / 2.f);
				float center_y = previewWindowPos.y + (previewWindowSize.y / 2.f);

				float Width = 92.0f;
				float Height = 5.0f;

				float ArmorPed = 200;
				float ArmorDistance = 0;

				float ArmorPercentage = (ArmorPed - 100) / 100.0f; // Ajusta esta lógica según tus necesidades

				ImColor armorColor = ImColor(10, 145, 255, 255);
				ImColor backgroundColor = ImColor(0, 0, 0, 255);

				float barX = center_x + (Width / 2) + ArmorDistance - 102; // Ajusta la posición X aquí
				float barY = center_y + Height / 2 - 2; // Ajusta la posición Y aquí, por ejemplo, agregamos 10 píxeles hacia abajo

				i_overlay::here().DrawHealthBarhorizontal(ImVec2(barX, barY), ImVec2(barX, barY), ImVec2(Width, Height), backgroundColor);
				i_overlay::here().DrawHealthBarhorizontal(ImVec2(barX, barY), ImVec2(barX, barY), ImVec2(Width * ArmorPercentage, Height), armorColor);
			}

			ImVec2 cursorPos = ImGui::GetCursorPos();

			if (i_esp::esp().distance)
			{
				float offsetX = 55.0f;

				float offsetY = 190.0f;

				ImGui::SetCursorPos(ImVec2(cursorPos.x + offsetX, cursorPos.y + offsetY));

				ImGui::Text("ID: 1 33m");

				ImGui::SetCursorPos(cursorPos);
			}

			if (i_esp::esp().Weapon)
			{
				float offsetX = 77.0f;

				float offsetY = 170.0f;

				ImGui::SetCursorPos(ImVec2(cursorPos.x + offsetX, cursorPos.y + offsetY));

				ImGui::Text("pistol");

				ImGui::SetCursorPos(cursorPos);
			}

			ImGui::EndChild();
		}

		if (tabs == 5)
		{
			ImGuiStyle& style = ImGui::GetStyle();
			style.Colors[ImGuiCol_ButtonHovered] = ImColor(0, 143, 179);    // Blue button color when hovered
			style.Colors[ImGuiCol_ButtonActive] = ImColor(77, 77, 77);
			ImGui::SetCursorPosX(250);
			ImGui::SetCursorPosY(32);
			ImGui::PushFont(Normale);
			ImGui::TextColored(ImColor(255, 255, 255), (_xor_("Miscellaneous >").c_str()));
			ImGui::PopFont();
			ImGui::SetCursorPosX(370);
			ImGui::SetCursorPosY(32);
			ImGui::PushFont(Normale);
			ImGui::TextColored(ImColor(0, 143, 179), (_xor_("Combat").c_str()));
			ImGui::PopFont();

			ImGui::SetCursorPosX(180);
			ImGui::SetCursorPosY(85);
			ImGui::BeginChild("Weaponcategori", ImVec2(230, 320), TRUE);
			ImGui::Text("Weapon");
			ImGui::Separator();
			ImGui::SetCursorPosY(40);
			ImGui::Checkbox("Aimbot", &i_aim::aim().aimenable);
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text("May You Will Get Banned From Some Anticheats [USE IT AT OWN RISK].");
				ImGui::EndTooltip();
			}
			ImGui::SetCursorPosY(70);
			ImGui::Checkbox(skCrypt("Draw Fov"), &i_aim::aim().showfov);
			ImGui::SetCursorPosY(100);
			ImGui::Checkbox("Remove Weapon Recoil", &i_exploits::exp().norecoil);
			ImGui::SetCursorPosY(130);
			ImGui::Checkbox("Weapon Set Ammo", &i_exploits::exp().infiniteammo);
			ImGui::SetCursorPosY(160);
			ImGui::Checkbox(("Weapon No Spread"), &i_exploits::exp().nospread);
			ImGui::SetCursorPosY(190);
			ImGui::Checkbox("Weapon Reload Speed", &i_exploits::exp().noreload);
			ImGui::SetCursorPosY(220);
			ImGui::Checkbox("Weapon damage Boost", &i_exploits::exp().damageboos);
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text("May You Will Get Banned From Some Anticheats [USE IT AT OWN RISK].");
				ImGui::EndTooltip();
			}
			//ImGui::SetCursorPosY(220);
			//ImGui::Checkbox("Remove Weapon Spray", &sprayweapon);
			ImGui::SetCursorPosY(250);
			if (ImGui::Button("Refill Ammo", ImVec2(180, 22)))
			{
				uintptr_t WeaponManager = i_memory::reeq().Read<uintptr_t>(i_dupa::skid().i_localplayer + i_dupa::skid().weaponmanager);
				uintptr_t weaponinfo = i_memory::reeq().Read<uintptr_t>(WeaponManager + 0x20);
				uintptr_t AmmoInfo = i_memory::reeq().Read<uintptr_t>(weaponinfo + 0x60);

				uintptr_t AmmoCount = i_memory::reeq().Read<uintptr_t>(AmmoInfo + 0x8);

				uintptr_t AmmoCount2 = i_memory::reeq().Read<uintptr_t>(AmmoCount + 0x0);
				int actualammo = i_memory::reeq().Read<uintptr_t>(AmmoCount2 + 0x18);

				i_memory::reeq().Write<float>(AmmoCount2 + 0x18, 30);
			}
			ImGui::EndChild();
			ImGui::SetCursorPosX(430);
			ImGui::SetCursorPosY(85);
			ImGui::BeginChild("WeaponcaEZRZEi", ImVec2(230, 320), TRUE);
			ImGui::Text("Option");
			ImGui::Separator();
			ImGui::Checkbox(skCrypt("Target Ped"), &i_aim::aim().ignoreped);
			ImGui::Checkbox(skCrypt("Target Dead"), &i_aim::aim().ignoredeath);

			ImGui::SliderInt(skCrypt(("DrawFov")), &i_aim::aim().fov, 2.0f, 300);

			ImGui::SliderInt(skCrypt(("Smooth")), &i_aim::aim().smooth, 2, 100);

			ImGui::SliderInt(skCrypt(("Distance")), &i_aim::aim().distance, 0.0f, 1000);

			const char* Boness[]{ "Head","Neck","Chest" };
			ImGui::SetCursorPosX(10);
			ImGui::Combo((((""))), &i_aim::aim().boneid, Boness, IM_ARRAYSIZE(Boness), ImGuiComboFlags_NoArrowButton);

			ImGui::EndChild();
		}


		if (tabs == 6)
		{
			ImGui::SetCursorPosX(250);
			ImGui::SetCursorPosY(32);
			ImGui::PushFont(Normale);
			ImGui::TextColored(ImColor(255, 255, 255), (_xor_("Miscellaneous >").c_str()));
			ImGui::PopFont();
			ImGui::SetCursorPosX(370);
			ImGui::SetCursorPosY(32);
			ImGui::PushFont(Normale);
			ImGui::TextColored(ImColor(0, 143, 179), (_xor_("Modules").c_str()));
			ImGui::PopFont();

			ImGui::SetCursorPosX(180);
			ImGui::SetCursorPosY(85);
			ImGui::BeginChild("Moduless", ImVec2(250, 320), TRUE);
			ImGui::Text("Model Spwan");
			ImGui::Separator();
			ImGui::Text("Filter Model");
			ImGui::InputText(skCrypt("##ffff"), filterModText, sizeof(filterModText));

			int selectedItem = -1;

			ImGui::ListBoxHeader(skCrypt("##WeaponList"), ImVec2(ImGui::GetWindowWidth() - ImGui::GetStyle().ItemSpacing.x, ImGui::GetContentRegionAvail().y - 80));

			for (int i = 0; i < IM_ARRAYSIZE(settings::self::model_changer::Model); i++) {
				const char* weaponName = settings::self::model_changer::Model[i];

				// Vérifier si l'élément correspond au filtre
				if (strstr(weaponName, filterModText) != nullptr) {
					bool isSelected = (settings::self::model_changer::ModelPos == i);
					if (ImGui::Selectable(weaponName, isSelected)) {
						selectedItem = i;
					}
				}
			}

			ImGui::ListBoxFooter();

			if (selectedItem != -1) {
				settings::self::model_changer::ModelPos = selectedItem;
			}
			if (ImGui::Button(skCrypt("Change Model"))) {
				ModelName = settings::self::model_changer::Model[settings::self::model_changer::ModelPos];
				settings::self::model_changer::ModelJoat = rage::joaat(ModelName);
				settings::self::model_changer::ModelChange = true;
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text("Change Model Dont Working Wait Fixs Ready [CHANGE MODEL Risk]");
				ImGui::EndTooltip();
			}
			ImGui::EndChild();

			ImGui::SetCursorPosX(460);
			ImGui::SetCursorPosY(85);
			ImGui::BeginChild("configmodules", ImVec2(200, 320), TRUE);
			ImGui::Text("Spwan Model");
			//ImGui::Separator();
			ImGui::SetCursorPosY(70);
			//ImGui::Text("Customs");
			//ImGui::Separator();
			//ImGui::SetCursorPosY(100);
			//ImGui::Checkbox("Vehicle Turbo", &turbo);
			//ImGui::SetCursorPosY(130);
			//ImGui::Checkbox("Stick Players", &stick);
			ImGui::SetCursorPosY(170);
			ImGuiStyle& style = ImGui::GetStyle();
			style.Colors[ImGuiCol_ButtonHovered] = ImColor(0, 143, 179);    // Blue button color when hovered
			style.Colors[ImGuiCol_ButtonActive] = ImColor(77, 77, 77);
			ImGui::SetCursorPos({ 55, 33 });
			{
			}
			ImGui::EndChild();

		}

		if (tabs == 7)
		{
			ImGuiStyle& style = ImGui::GetStyle();
			style.Colors[ImGuiCol_ButtonHovered] = ImColor(0, 143, 179);    // Blue button color when hovered
			style.Colors[ImGuiCol_ButtonActive] = ImColor(77, 77, 77);

			ImGui::SetCursorPosX(250);
			ImGui::SetCursorPosY(32);
			ImGui::PushFont(Normale);
			ImGui::TextColored(ImColor(255, 255, 255), (_xor_("Miscellaneous >").c_str()));
			ImGui::PopFont();
			ImGui::SetCursorPosX(370);
			ImGui::SetCursorPosY(32);
			ImGui::PushFont(Normale);
			ImGui::TextColored(ImColor(0, 143, 179), (_xor_("Weapon Spawner").c_str()));
			ImGui::PopFont();

			ImGui::SetCursorPosX(180);
			ImGui::SetCursorPosY(85);
			ImGui::BeginChild("allahexistepas", ImVec2(250, 320), TRUE);
			ImGui::Text("Choose Weapon");
			ImGui::Separator();

			ImGui::Checkbox(skCrypt("Enable"), &settings::weapon::weapon_spawning::weapon_spawning);

			ImGui::Text("Filter Weapon");
			ImGui::InputText(skCrypt("##ffff"), filterText, sizeof(filterText));

			int selectedItem = -1;

			ImGui::ListBoxHeader(skCrypt("##WeaponList"), ImVec2(ImGui::GetWindowWidth() - ImGui::GetStyle().ItemSpacing.x, ImGui::GetContentRegionAvail().y - 80));

			for (int i = 0; i < IM_ARRAYSIZE(settings::weapon::weapon_spawning::weapon_list); i++) {
				const char* weaponName = settings::weapon::weapon_spawning::weapon_list[i];

				// Vérifier si l'élément correspond au filtre
				if (strstr(weaponName, filterText) != nullptr) {
					bool isSelected = (settings::weapon::weapon_spawning::weapon_current == i);
					if (ImGui::Selectable(weaponName, isSelected)) {
						selectedItem = i;
					}
				}
			}

			ImGui::ListBoxFooter();

			if (selectedItem != -1) {
				settings::weapon::weapon_spawning::weapon_current = selectedItem;
			}
			if (ImGui::Button(skCrypt("Give Weapon", ImVec2(200, 25)))) {
				settings::weapon::weapon_spawning::give = true;
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text("The Weapons Force In Hand Dont Woking Wait Fixs Ready [FORCE IN HAND SYSTEM].");
				ImGui::EndTooltip();
			}
			ImGui::EndChild();

			ImGui::SetCursorPosX(460);
			ImGui::SetCursorPosY(85);
			ImGui::BeginChild("je sait toujour pas", ImVec2(200, 320), TRUE);
			ImGui::Text("Action");
			ImGui::Separator();
			ImGui::Checkbox(skCrypt("Force In Hand"), &settings::weapon::weapon_spawning::in_hand);
			ImGui::SetCursorPosY(130);
			ImGui::EndChild();

		}
		if (tabs == 8)
		{
			ImGui::SetCursorPosX(250);
			ImGui::SetCursorPosY(32);
			ImGui::PushFont(Normale);
			ImGui::TextColored(ImColor(255, 255, 255), (_xor_("Miscellaneous >").c_str()));
			ImGui::PopFont();
			ImGui::SetCursorPosX(370);
			ImGui::SetCursorPosY(32);
			ImGui::PushFont(Normale);
			ImGui::TextColored(ImColor(0, 143, 179), (_xor_("Modules").c_str()));
			ImGui::PopFont();

			ImGui::SetCursorPosX(180);
			ImGui::SetCursorPosY(85);
			ImGui::BeginChild("Moduless", ImVec2(250, 320), TRUE);
			ImGui::Text("Miscellaneous");
			ImGui::Separator();
			const char* BoxStyle[]{ "2D","Rounded" };
			ImGui::SetCursorPosX(15);
			ImGui::PushItemWidth(125);
			ImGui::Combo(skCrypt(((("Box")))), &i_esp::esp().boxtype, BoxStyle, 2, ImGuiComboFlags_NoArrowButton);
			const char* HealthbarStyle[]{ "Right","Left","Up" };
			ImGui::SetCursorPosX(15);
			ImGui::PushItemWidth(125);
			ImGui::Combo(skCrypt(((("Health")))), &i_esp::esp().HealthBar_type, HealthbarStyle, 3, ImGuiComboFlags_NoArrowButton);
			ImGui::Text("Skeleton");
			ImGui::ColorPicker4(skCrypt(" 4 "), (float*)&i_esp::esp().skeletoncolor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoSidePreview);


			ImGui::Text("Box");
			ImGui::SameLine;
			ImGui::ColorPicker4(skCrypt(("    2     ")), (float*)&i_esp::esp().boxcolor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoSidePreview);

			ImGui::Text("Fov");
			ImGui::SameLine;
			ImGui::ColorPicker4(skCrypt(("      f            ")), (float*)&i_esp::esp().FovColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoSidePreview);

			ImGui::Text("Snaplines");
			ImGui::SameLine;
			ImGui::ColorPicker4(skCrypt(("          1       ")), (float*)&i_esp::esp().LineColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoSidePreview);
			{

			}
			ImGui::EndChild();

			ImGui::SetCursorPosX(460);
			ImGui::SetCursorPosY(85);
			ImGui::BeginChild("Config", ImVec2(200, 320), TRUE);
			ImGui::Text("Config");
			//ImGui::Separator();
			ImGui::SetCursorPosY(70);
			//ImGui::Text("Customs");
			//ImGui::Separator();
			//ImGui::SetCursorPosY(100);
			//ImGui::Checkbox("Vehicle Turbo", &turbo);
			//ImGui::SetCursorPosY(130);
			//ImGui::Checkbox("Stick Players", &stick);
			ImGui::SetCursorPosY(170);
			ImGuiStyle& style = ImGui::GetStyle();
			style.Colors[ImGuiCol_ButtonHovered] = ImColor(0, 143, 179);    // Blue button color when hovered
			style.Colors[ImGuiCol_ButtonActive] = ImColor(77, 77, 77);
			ImGui::SetCursorPos({ 55, 33 });
			{
				if (ImGui::Button(E("Load"))) {

					FILE* p_stream;

					// open file for (extended) reading
					fopen_s(&p_stream, cfg[selected_cfg], "r+");

					// set file pointer to 0
					fseek(p_stream, 0, SEEK_SET);

					// Aimbot
					fread_s(&settings::aimbot::enabled, sizeof(bool), sizeof(bool), 1, p_stream);

					fread_s(&settings::aimbot::aimbot_fov, sizeof(bool), sizeof(bool), 1, p_stream);
					fread_s(&settings::aimbot::smoothhorizontal, sizeof(bool), sizeof(bool), 1, p_stream);
					fread_s(&settings::aimbot::smoothvertical, sizeof(bool), sizeof(bool), 1, p_stream);
					fread_s(&settings::aimbot::aimbot_distance, sizeof(bool), sizeof(bool), 1, p_stream);
					fread_s(&settings::aimbot::aimbot_target, sizeof(bool), sizeof(bool), 1, p_stream);
					fread_s(&settings::aimbot::hotkey, sizeof(bool), sizeof(bool), 1, p_stream);
					fread_s(&settings::aimbot::aimbot_bone, sizeof(bool), sizeof(bool), 1, p_stream);
					fread_s(&settings::aimbot::aimbot_type, sizeof(bool), sizeof(bool), 1, p_stream);
					fread_s(&settings::aimbot::aimbot_selection, sizeof(bool), sizeof(bool), 1, p_stream);

					// Visuals
					fread_s(&settings::visuals::enabled, sizeof(bool), sizeof(bool), 1, p_stream);

					fread_s(&settings::visuals::master_switch, sizeof(bool), sizeof(bool), 1, p_stream);
					fread_s(&settings::visuals::box, sizeof(bool), sizeof(bool), 1, p_stream);
					fread_s(&settings::visuals::box_round, sizeof(bool), sizeof(bool), 1, p_stream);
					fread_s(&settings::visuals::filled, sizeof(bool), sizeof(bool), 1, p_stream);
					fread_s(&settings::visuals::playername, sizeof(bool), sizeof(bool), 1, p_stream);
					fread_s(&settings::visuals::playername, sizeof(bool), sizeof(bool), 1, p_stream);
					fread_s(&settings::visuals::weapon_name, sizeof(bool), sizeof(bool), 1, p_stream);
					fread_s(&settings::visuals::snapline, sizeof(bool), sizeof(bool), 1, p_stream);
					fread_s(&settings::visuals::healthbar, sizeof(bool), sizeof(bool), 1, p_stream);
					fread_s(&settings::visuals::armorbar, sizeof(bool), sizeof(bool), 1, p_stream);
					fread_s(&settings::visuals::dont_draw_distance, sizeof(bool), sizeof(bool), 1, p_stream);
					fread_s(&settings::visuals::circle_at_middle, sizeof(bool), sizeof(bool), 1, p_stream);
					fread_s(&settings::visuals::skeleton, sizeof(bool), sizeof(bool), 1, p_stream);
					fread_s(&settings::visuals::watermark, sizeof(bool), sizeof(bool), 1, p_stream);
					fread_s(&settings::visuals::render_distance, sizeof(bool), sizeof(bool), 1, p_stream);
					fread_s(&settings::visuals::draw_dead, sizeof(bool), sizeof(bool), 1, p_stream);
					fread_s(&settings::visuals::draw_ped, sizeof(bool), sizeof(bool), 1, p_stream);
					fread_s(&settings::visuals::draw_self, sizeof(bool), sizeof(bool), 1, p_stream);
					fread_s(&settings::visuals::preview_target, sizeof(bool), sizeof(bool), 1, p_stream);
					fread_s(&settings::visuals::draw_fov, sizeof(bool), sizeof(bool), 1, p_stream);
					fread_s(&settings::visuals::filled_fov, sizeof(bool), sizeof(bool), 1, p_stream);
					fread_s(&settings::visuals::crosshair, sizeof(bool), sizeof(bool), 1, p_stream);
					fread_s(&settings::visuals::crossahir_style, sizeof(bool), sizeof(bool), 1, p_stream);

					// Vehicle

					// Object
					fread_s(&settings::object::object_distance, sizeof(bool), sizeof(bool), 1, p_stream);
					fread_s(&settings::object::object_esp, sizeof(bool), sizeof(bool), 1, p_stream);
					fread_s(&settings::object::object_snapline, sizeof(bool), sizeof(bool), 1, p_stream);

					// Weapon
					fread_s(&settings::weapon::enabled, sizeof(bool), sizeof(bool), 1, p_stream);

					fread_s(&settings::weapon::no_recoil, sizeof(bool), sizeof(bool), 1, p_stream);
					fread_s(&settings::weapon::no_reload, sizeof(bool), sizeof(bool), 1, p_stream);
					fread_s(&settings::weapon::no_spread, sizeof(bool), sizeof(bool), 1, p_stream);
					fread_s(&settings::weapon::infinite_range, sizeof(bool), sizeof(bool), 1, p_stream);
					fread_s(&settings::weapon::infite_ammo, sizeof(bool), sizeof(bool), 1, p_stream);
					fread_s(&settings::weapon::damage_boost, sizeof(bool), sizeof(bool), 1, p_stream);
					fread_s(&settings::weapon::recoil_value, sizeof(bool), sizeof(bool), 1, p_stream);
					fread_s(&settings::weapon::spread_value, sizeof(bool), sizeof(bool), 1, p_stream);
					fread_s(&settings::weapon::weapon_range, sizeof(bool), sizeof(bool), 1, p_stream);
					fread_s(&settings::weapon::ammo_to_set, sizeof(bool), sizeof(bool), 1, p_stream);
					fread_s(&settings::weapon::int_damage, sizeof(bool), sizeof(bool), 1, p_stream);

					fread_s(&font, sizeof(bool), sizeof(bool), 1, p_stream);

					fclose(p_stream);
				}

				ImGui::SameLine();

				if (ImGui::Button(E("Save"))) {

					FILE* p_stream;

					// open file for (extended) reading
					fopen_s(&p_stream, cfg[selected_cfg], "w+");

					// set file pointer to 0
					fseek(p_stream, 0, SEEK_SET);

					// Aimbot
					fwrite(&settings::aimbot::enabled, sizeof(bool), 1, p_stream);

					fwrite(&settings::aimbot::aimbot_fov, sizeof(bool), 1, p_stream);
					fwrite(&settings::aimbot::smoothhorizontal, sizeof(bool), 1, p_stream);
					fwrite(&settings::aimbot::smoothvertical, sizeof(bool), 1, p_stream);
					fwrite(&settings::aimbot::aimbot_distance, sizeof(bool), 1, p_stream);
					fwrite(&settings::aimbot::aimbot_target, sizeof(bool), 1, p_stream);
					fwrite(&settings::aimbot::hotkey, sizeof(bool), 1, p_stream);
					fwrite(&settings::aimbot::aimbot_bone, sizeof(bool), 1, p_stream);
					fwrite(&settings::aimbot::aimbot_type, sizeof(bool), 1, p_stream);
					fwrite(&settings::aimbot::aimbot_selection, sizeof(bool), 1, p_stream);

					// Visuals
					fwrite(&settings::visuals::enabled, sizeof(bool), 1, p_stream);

					fwrite(&settings::visuals::master_switch, sizeof(bool), 1, p_stream);
					fwrite(&settings::visuals::box, sizeof(bool), 1, p_stream);
					fwrite(&settings::visuals::box_round, sizeof(bool), 1, p_stream);
					fwrite(&settings::visuals::filled, sizeof(bool), 1, p_stream);
					fwrite(&settings::visuals::playername, sizeof(bool), 1, p_stream);
					fwrite(&settings::visuals::playername, sizeof(bool), 1, p_stream);
					fwrite(&settings::visuals::weapon_name, sizeof(bool), 1, p_stream);
					fwrite(&settings::visuals::snapline, sizeof(bool), 1, p_stream);
					fwrite(&settings::visuals::healthbar, sizeof(bool), 1, p_stream);
					fwrite(&settings::visuals::armorbar, sizeof(bool), 1, p_stream);
					fwrite(&settings::visuals::dont_draw_distance, sizeof(bool), 1, p_stream);
					fwrite(&settings::visuals::circle_at_middle, sizeof(bool), 1, p_stream);
					fwrite(&settings::visuals::skeleton, sizeof(bool), 1, p_stream);
					fwrite(&settings::visuals::watermark, sizeof(bool), 1, p_stream);
					fwrite(&settings::visuals::render_distance, sizeof(bool), 1, p_stream);
					fwrite(&settings::visuals::draw_dead, sizeof(bool), 1, p_stream);
					fwrite(&settings::visuals::draw_ped, sizeof(bool), 1, p_stream);
					fwrite(&settings::visuals::draw_self, sizeof(bool), 1, p_stream);
					fwrite(&settings::visuals::preview_target, sizeof(bool), 1, p_stream);
					fwrite(&settings::visuals::draw_fov, sizeof(bool), 1, p_stream);
					fwrite(&settings::visuals::filled_fov, sizeof(bool), 1, p_stream);
					fwrite(&settings::visuals::crosshair, sizeof(bool), 1, p_stream);
					fwrite(&settings::visuals::crossahir_style, sizeof(bool), 1, p_stream);

					// Vehicle

					// Object
					fwrite(&settings::object::object_distance, sizeof(bool), 1, p_stream);
					fwrite(&settings::object::object_esp, sizeof(bool), 1, p_stream);
					fwrite(&settings::object::object_snapline, sizeof(bool), 1, p_stream);

					// Weapon
					fwrite(&settings::weapon::enabled, sizeof(bool), 1, p_stream);

					fwrite(&settings::weapon::no_recoil, sizeof(bool), 1, p_stream);
					fwrite(&settings::weapon::no_reload, sizeof(bool), 1, p_stream);
					fwrite(&settings::weapon::no_spread, sizeof(bool), 1, p_stream);
					fwrite(&settings::weapon::infinite_range, sizeof(bool), 1, p_stream);
					fwrite(&settings::weapon::infite_ammo, sizeof(bool), 1, p_stream);
					fwrite(&settings::weapon::damage_boost, sizeof(bool), 1, p_stream);
					fwrite(&settings::weapon::recoil_value, sizeof(bool), 1, p_stream);
					fwrite(&settings::weapon::spread_value, sizeof(bool), 1, p_stream);
					fwrite(&settings::weapon::weapon_range, sizeof(bool), 1, p_stream);
					fwrite(&settings::weapon::ammo_to_set, sizeof(bool), 1, p_stream);
					fwrite(&settings::weapon::int_damage, sizeof(bool), 1, p_stream);

					fwrite(&font, sizeof(bool), 1, p_stream);

					fclose(p_stream);
				}
			}
			if (ImGui::Button("Sent Feedback", ImVec2(200, 30)))
			{
				system("start https://discord.com/channels/1226435512877711391/1231879130438438983");
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text("Cleck Goto Channel Feedback ad sent text +Rep KX Cheats One Top. [RISK SENTFEEDBACK]");
				ImGui::EndTooltip();
			}
			if (ImGui::Button("Panic (Exit Cheat)", ImVec2(200, 30)))
			{
				exit(0);
			}
			if (ImGui::Button("KX TERM", ImVec2(200, 30)))
			{
				system("start https://discord.com/terms");
			}
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "(!)"); // Symbol in Rot neben der Checkbox

			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text("KX TERM. [KX TERM.]");
				ImGui::EndTooltip();		
			}
			ImGui::EndChild();
		}
		//ImGui::SetCursorPosY(475);
		//ImGui::SetCursorPosX(420);
		//ImGui::Text("KX Softwares https://discord.gg/f699zmAN ");

	}
	ImGui::End();
}

void i_overlay::signal()
{
	ZeroMemory(&Message, sizeof(MSG));
	style();
	static RECT old_rc;
	while (Message.message != WM_QUIT)
	{
		if (PeekMessage(&Message, MyWnd, 0, 0, PM_REMOVE)) {
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		HWND hwnd_active = GetForegroundWindow();


		if (hwnd_active == GameWnd) {
			HWND hwndtest = GetWindow(hwnd_active, GW_HWNDPREV);
			SetWindowPos(MyWnd, hwndtest, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

		}
		if (showmenu)
		{

		}
		RECT rc;
		POINT xy;

		ZeroMemory(&rc, sizeof(RECT));
		ZeroMemory(&xy, sizeof(POINT));
		GetClientRect(GameWnd, &rc);
		ClientToScreen(GameWnd, &xy);
		rc.left = xy.x;
		rc.top = xy.y;

		ImGuiIO& io = ImGui::GetIO();
		io.ImeWindowHandle = GameWnd;
		io.DeltaTime = 1.0f / 60.0f;

		POINT p;
		GetCursorPos(&p);
		io.MousePos.x = p.x - xy.x;
		io.MousePos.y = p.y - xy.y;
		if (GetAsyncKeyState(0x1)) {
			io.MouseDown[0] = true;
			io.MouseClicked[0] = true;
			io.MouseClickedPos[0].x = io.MousePos.x;
			io.MouseClickedPos[0].x = io.MousePos.y;
		}
		else io.MouseDown[0] = false;

		if (rc.left != old_rc.left || rc.right != old_rc.right || rc.top != old_rc.top || rc.bottom != old_rc.bottom) {

			old_rc = rc;

			Width = rc.right;
			Height = rc.bottom;

			p_Params.BackBufferWidth = Width;
			p_Params.BackBufferHeight = Height;
			SetWindowPos(MyWnd, (HWND)0, xy.x, xy.y, Width, Height, SWP_NOREDRAW);
			p_Device->Reset(&p_Params);
		}
		i_overlay::here().render();

		Sleep(10);
	}
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	i_overlay::here().cleanup_d3d();
	DestroyWindow(i_overlay::here().MyWnd);


}

std::wstring MBytesToWString(const char* lpcszString)
{
	int len = strlen(lpcszString);
	int unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, lpcszString, -1, NULL, 0);
	wchar_t* pUnicode = new wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
	::MultiByteToWideChar(CP_ACP, 0, lpcszString, -1, (LPWSTR)pUnicode, unicodeLen);
	std::wstring wString = (wchar_t*)pUnicode;
	delete[] pUnicode;
	return wString;
}

std::string WStringToUTF8(const wchar_t* lpwcszWString)
{
	char* pElementText;
	int iTextLen = ::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)lpwcszWString, -1, NULL, 0, NULL, NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, (iTextLen + 1) * sizeof(char));
	::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)lpwcszWString, -1, pElementText, iTextLen, NULL, NULL);
	std::string strReturn(pElementText);
	delete[] pElementText;
	return strReturn;
}

void i_overlay::RectFilled(float x, float y, float x1, float y1, ImColor color, float rounding, int rounding_corners_flags)
{
	ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x1, y1), color, rounding, rounding_corners_flags);
}

void i_overlay::DrawHealthBarhorizontal(ImVec2 pos, ImVec2 pos2, ImVec2 dim, ImColor col)
{

	ImGui::GetForegroundDrawList()->AddLine(ImVec2(pos.x + dim.y, pos.y), ImVec2(pos2.x, pos.y), col, dim.x);

}

void i_overlay::DrawRectImGui(int x, int y, int w, int h, ImVec4 color, float thickness)
{
	ImGui::GetForegroundDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(color), 0, 0, thickness);
}

void i_overlay::DrawLeftProgressBar(int x, int y, int w, int h, int thick, int m_health)
{
	int G = (255 * m_health / 100);
	int R = 255 - G;
	RGBA healthcol = { R, G, 0, 255 };

	DrawFilledRect(x - (w / 2) - 3, y, thick, (h)*m_health / 100, &healthcol);
}

void i_overlay::DrawFilledRect(int x, int y, int w, int h, RGBA* color)
{
	ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 153.0, color->B / 51.0, color->A / 255.0)), 0, 0);
}
void i_overlay::DrawShieldbar(int x, int y, int w, int h, int thick, int m_health)
{
	int G = (255 * m_health / 100);
	int R = 255 - G;

	DrawFilledRect(x - (w / 2) - 3, y, thick, (h)*m_health / 100, &Col.darkblue);
}

void i_overlay::DrawHealthBar(ImVec2 pos, ImVec2 dim, ImColor col)
{


	ImGui::GetForegroundDrawList()->AddLine(pos, ImVec2(pos.x, pos.y - dim.y), col, dim.x);

}
void i_overlay::DrawLine(const ImVec2& x, const ImVec2 y, ImU32 color, const FLOAT width)
{
	ImGui::GetForegroundDrawList()->AddLine(x, y, color, width);
}

void i_overlay::DrawOutlinedText(ImFont* pFont, const std::string& text, const ImVec2& pos, float size, ImU32 color, bool center)
{
	std::stringstream stream(text);
	std::string line;

	float y = 0.0f;
	int index = 0;

	while (std::getline(stream, line))
	{
		ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());

		if (center)
		{

			ImGui::GetForegroundDrawList()->AddText(pFont, size, ImVec2(pos.x - textSize.x / 2.0f, pos.y + textSize.y * index), ImGui::GetColorU32(color), line.c_str());
		}
		else
		{//

			ImGui::GetForegroundDrawList()->AddText(pFont, size, ImVec2(pos.x, pos.y + textSize.y * index), ImGui::GetColorU32(color), line.c_str());
		}

		y = pos.y + textSize.y * (index + 1);
		index++;
	}
}

void i_overlay::DrawString(float fontSize, int x, int y, bool bCenter, bool stroke, const char* pText, ...)
{
	va_list va_alist;
	char buf[1024] = { 0 };
	va_start(va_alist, pText);
	_vsnprintf_s(buf, sizeof(buf), pText, va_alist);
	va_end(va_alist);
	std::string text = WStringToUTF8(MBytesToWString(buf).c_str());
	if (bCenter)
	{
		ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
		x = x - textSize.x / 4;
		y = y - textSize.y;
	}
	if (stroke)
	{
		ImGui::GetForegroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x + 1, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		ImGui::GetForegroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x - 1, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		ImGui::GetForegroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x + 1, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		ImGui::GetForegroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x - 1, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
	}
	ImGui::GetForegroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x, y), ImColor(189, 175, 255), text.c_str());
}

static const char* keyNames[] = {
	"",
	"Left Mouse",
	"Right Mouse",
	"Cancel",
	"Middle Mouse",
	"Mouse 5",
	"Mouse 4",
	"",
	"Backspace",
	"Tab",
	"",
	"",
	"Clear",
	"Enter",
	"",
	"",
	"Shift",
	"Control",
	"Alt",
	"Pause",
	"Caps",
	"",
	"",
	"",
	"",
	"",
	"",
	"Escape",
	"",
	"",
	"",
	"",
	"Space",
	"Page Up",
	"Page Down",
	"End",
	"Home",
	"Left",
	"Up",
	"Right",
	"Down",
	"",
	"",
	"",
	"Print",
	"Insert",
	"Delete",
	"",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"",
	"",
	"",
	"",
	"",
	"Numpad 0",
	"Numpad 1",
	"Numpad 2",
	"Numpad 3",
	"Numpad 4",
	"Numpad 5",
	"Numpad 6",
	"Numpad 7",
	"Numpad 8",
	"Numpad 9",
	"Multiply",
	"Add",
	"",
	"Subtract",
	"Decimal",
	"Divide",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",
};

static bool Items_ArrayGetter(void* data, int idx, const char** out_text)
{
	const char* const* items = (const char* const*)data;
	if (out_text)
		*out_text = items[idx];
	return true;
}

void i_overlay::HotkeyButton(int aimkey, void* changekey, int status)
{
	const char* preview_value = NULL;
	if (aimkey >= 0 && aimkey < IM_ARRAYSIZE(keyNames))
		Items_ArrayGetter(keyNames, aimkey, &preview_value);

	std::string aimkeys;
	if (preview_value == NULL)
		aimkeys = ("Select Key");
	else
		aimkeys = preview_value;

	if (status == 1)
	{
		aimkeys = ("Press key to bind");
	}
	if (ImGui::Button(aimkeys.c_str(), ImVec2(100, 20)))
	{
		if (status == 0)
		{
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)changekey, nullptr, 0, nullptr);
		}
	}
}


void i_overlay::initialize()
{

	// some stuff xyzus
	setup_window();
	init_wndparams(MyWnd);
	Sleep(2000);
	signal();
}


