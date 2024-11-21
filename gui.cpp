#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "amber.h"

#include <string>
#include <iostream>
#include <thread>
#include <string.h>
#include "pack.h"

enum State {
	SELECT,
	PACK,
	DONE
};

void startPack(const char* path, const char* launchCommand, int compressionLevel, unsigned long long* packedCount, State* state){

	pack(path, launchCommand, compressionLevel, packedCount);
	*state = DONE;
	*packedCount = 0;

}

void GuiLabelWrapper(std::string text, float y, float screenWidth, Font& fontTtf, float fontSize){

	float textWidth = MeasureTextEx(fontTtf, text.c_str(), fontSize, 1).x;
	if(textWidth > screenWidth){
		textWidth = screenWidth;
	}
	GuiLabel((Rectangle){screenWidth/2 - textWidth/2, y, screenWidth, fontSize}, text.c_str());

}

void gui(){

	const int screenWidth = 450;
    const int screenHeight = 340;
	const int fontSize = 25;
	const int MAXPATH = 260;
    
    InitWindow(screenWidth, screenHeight, "Packer");
	GuiLoadStyleAmber();

	Font fontTtf = LoadFontEx("C:/Windows/Fonts/arial.ttf", fontSize, 0, 250);
	GuiSetFont(fontTtf);
	GuiSetStyle(DEFAULT, TEXT_SIZE, fontSize);

    SetTargetFPS(60);
	
	State state = SELECT;

	std::string folderPath = "-";
	unsigned long long packedCount = 0;
	char launchCmd[MAXPATH + 1] = { 0 };
	int level = 0;

    while (!WindowShouldClose())
    {

        BeginDrawing();

			ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
			GuiLabelWrapper(folderPath, 10, screenWidth, fontTtf, fontSize);
			if (GuiButton((Rectangle){screenWidth/2 - 150/2, 45, 150, 50 }, "Select folder")){

				std::string path = selectFolder();
				if(path.length() > 0){

					folderPath = path;

				}

			}

			GuiLabelWrapper("Enter launch command", 105, screenWidth, fontTtf, fontSize);

			GuiTextBox((Rectangle){screenWidth/2 - 300/2, 140, 300, fontSize }, launchCmd, MAXPATH, true);
			if(strlen(launchCmd) >= MAXPATH-1){
				strncpy(launchCmd, launchCmd, MAXPATH-1);
				launchCmd[MAXPATH] = '\0';
			}

			GuiLabelWrapper("Compression level", 175, screenWidth, fontTtf, fontSize);
			GuiSpinner((Rectangle){screenWidth/2 - 150/2, 210, 150, 50}, "", &level, 0, 10, false);

			if(state == SELECT){

				if (GuiButton((Rectangle){screenWidth/2 - 150/2, 270, 150, 50 }, "Pack")){

					state = PACK;
					std::thread t(startPack, folderPath.c_str(), launchCmd, level, &packedCount, &state);
					t.detach();

				}

			}

			if(state == PACK){
				
				GuiLabelWrapper("Processing... (" + std::to_string(packedCount) + " files compressed)", 270, screenWidth, fontTtf, fontSize);

			}

			if(state == DONE){

				if (GuiButton((Rectangle){screenWidth/2 - 150/2, 270, 150, 50 }, "Done")){

					state = SELECT;

				}

			}


        EndDrawing();

    }

    CloseWindow();

}
