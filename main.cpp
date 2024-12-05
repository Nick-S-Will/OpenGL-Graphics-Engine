#include "GameController.h"
#include "ToolWindow.h"

int main()
{
	OpenGL::ToolWindow^ toolWindow = gcnew OpenGL::ToolWindow();
	toolWindow->Show();
	GameController::GetInstance().Initialize();
	GameController::GetInstance().RunGame();

	return 0;
}