#include <Windows.h>
#include "graphic.h"
#include "physics.h"
#include "player.h"

int main(int argc, char *argv[]) {
	SetProcessDPIAware();  // 这样即便开启窗口缩放也不影响分辨率
	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);
	graphic::Initial(width, height);
	player::Initial(width, height);
	world::Initial(1328886154);
	physics::Initial(10);
	graphic::RenderLoop();
	return 0;
}