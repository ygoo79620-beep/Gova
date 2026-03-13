#include "imgui.h"

namespace c
{
	inline ImVec4 roll_color = ImColor(255, 255, 255);
	inline ImVec4 accent_color = ImColor(0, 13, 255);
	inline ImColor accent_colorx = ImColor(63, 0, 105);
	inline ImVec4 accent_low_color = ImColor(250, 132, 86, 255 / 2);
	inline ImVec4 accent_low = ImColor(174, 197, 255, 255 / 2);

	inline ImVec4 accent_text_color = ImColor(245, 245, 255);

	inline ImVec4 accent_text_low_color = ImColor(245, 245, 255, 255 / 2);

	inline ImVec4 notify = ImColor(43, 48, 54);

	inline float size = 0.5f;

	namespace bg
	{
		inline ImVec4 background = ImColor(14, 14, 15, 210);
		inline ImVec4 outline_background = ImColor(27, 29, 32, 210);
		inline ImVec2 size = ImVec2(1050, 640);
		inline float rounding = 12;
	}

	namespace child
	{
		inline ImVec4 background = ImColor(22, 23, 26, 250);
		inline ImVec4 outline_background = ImColor(27, 29, 32, 255);
		inline float rounding = 6;
	}

	namespace checkbox
	{
		inline ImVec4 circle_inactive = ImColor(43, 48, 54, 255);

		inline ImVec4 background = ImColor(27, 29, 32, 255);
		inline ImVec4 outline_background = ImColor(30, 32, 36, 255);
		inline float rounding = 30;
	}

	namespace slider
	{
		inline ImVec4 circle_inactive = ImColor(43, 48, 54, 255);

		inline ImVec4 background = ImColor(27, 29, 32, 255);
		inline ImVec4 outline_background = ImColor(30, 32, 36, 255);
		inline float rounding = 30;
	}

	namespace combo
	{
		inline ImVec4 background = ImColor(27, 29, 32, 255);
		inline ImVec4 outline_background = ImColor(30, 32, 36, 255);
		inline float rounding = 3;
	}

	namespace picker
	{
		inline ImVec4 background = ImColor(27, 29, 32, 255);
		inline ImVec4 outline_background = ImColor(30, 32, 36, 255);
		inline float rounding = 2;
	}

	namespace button
	{
		inline ImVec4 background_hov = ImColor(26, 26, 26, 200);
		inline ImVec4 background = ImColor(20, 20, 20, 200);
		inline ImVec4 background_active = ImColor(40, 40, 40, 200);

		inline ImVec4 outline_background = ImColor(55, 55, 55, 255);
		inline float rounding = 4;
		inline float bg_rounding = 5.f;
	}

	namespace input
	{
		inline ImVec4 accent = ImColor(235, 5, 12, 255);
		inline ImVec4 text_selected = ImColor(70, 70, 70, 100);
		inline ImVec4 text_hint = ImColor(180, 180, 180, 255);
		inline ImVec4 text = ImColor(255, 255, 255, 255);
		inline ImVec4 background_hov = ImColor(70, 70, 70, 60);
		inline ImVec4 background = ImColor(30, 30, 30, 20);
		inline ImVec4 background_active = ImColor(26, 27, 29, 255);
		inline ImVec4 background_inactive = ImColor(16, 17, 19, 255);

		inline ImVec4 outline = ImColor(22, 24, 26, 255);
		inline ImVec4 icon = ImColor(26, 27, 31, 255);

		inline float rounding = 2.f;

		/*
			inline ImVec4 background = ImColor(27, 29, 32, 255);
		inline ImVec4 outline_background = ImColor(30, 32, 36, 255);
		inline float rounding = 4;
		*/

	}

	namespace keybind
	{
		inline float rounding = 4.f * c::size;
		inline ImVec4 background = ImColor(25, 25, 25, 255);
		inline ImVec4 text_active = ImColor(255, 255, 255, 255);
		inline ImVec4 text_hov = ImColor(130, 130, 130, 255);
		inline ImVec4 text = ImColor(90, 93, 100, 255);
	}


	namespace text
	{
		inline ImVec4 text_active = ImColor(255, 255, 255, 255);
		inline ImVec4 text_hov = ImColor(245, 245, 255);
		inline ImVec4 text = ImColor(90, 93, 100);
		inline ImVec4 text2 = ImColor(90, 93, 100, 255);
		inline ImVec4 hide_text = ImColor(43, 48, 54, 255);

	}
}