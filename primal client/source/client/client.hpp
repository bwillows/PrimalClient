#pragma once
#include "../sdk/sdk.h"

class client
{
	std::string  temporary_string;
	std::wstring temporary_wstring;
	int          frame_counter = 0;
	RECT         window_rect = { 0 };


	struct {
		struct
		{
			bool friendly = true;
			bool enemy    = true;
			bool sleeping = true;
			bool dead     = true;

			bool health   = true;
			bool tribe    = true;
			bool distance = true;
			bool level    = true;
		} player;
		struct
		{
			bool wild     = true;
			bool friendly = true;
			bool enemy    = true;
			bool dead     = true;

			bool health   = true;
			bool distance = true;
			bool level    = true;
		} dino;
		struct
		{

		} structure;
	} settings;

public:
	UCanvas* canvas;

	void draw_text(const std::string& text, const FVector2D& location, const FLinearColor& color);
	void tick(UWorld* uworld);
	void draw_menu();
};