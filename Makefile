all:
	g++ -Isrc/include -Lsrc/lib -o output/main main.cpp src/include/baseObject.cpp src/include/hangmanImage.cpp src/include/commonFunc.cpp src/include/playerPower.cpp src/include/Text.cpp src/include/stdafx.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
