#include "Core/CGame.h"
#include "Core/Math.h"

LRESULT WINAPI WndProc(_In_ HWND hWnd, _In_ UINT Msg, _In_ WPARAM wParam, _In_ LPARAM lParam);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	int SmallStarCount{ 1000 };
	int LargePlanetCount{ 2 };
	int MiddlePlanetCount{ 3 };
	int SmallPlanetCount{ 5 };

	srand((unsigned int)GetTickCount64());

	CGame Game{1280, 720};

	Game.Create(hInstance, WndProc, "GameBase");

	Game.AddObject2D("Player", DirectX::XMFLOAT2(220, 220), "Asset/ship_210_anim.png");

	Game.AddObject2D("BG", DirectX::XMFLOAT2(1280.0f, 1280.0f), "Asset/BackGround.png");

	Game.CreateStarPool();

	Game.SetPlayerObject2D("Player");

	MSG Msg{};

	float BackgroundColor[]{ 0.0f, 0.0f, 0.0f, 1.0f };

	int iFrame{};
	while (true)
	{
		if (PeekMessage(&Msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (Msg.message == WM_QUIT)
			{
				break;
			}

			TranslateMessage(&Msg);

			DispatchMessage(&Msg);
		}
		else
		{
			DirectX::Keyboard::State KeyboardState{ Game.GetKeyboardState() };

			CObject2D* const PlayerObject2D{ Game.GetPlayerObject2D() };

			if (KeyboardState.Up) { PlayerObject2D->Translate(DirectX::XMVectorSet(0, +3, 0, 0)); }
			if (KeyboardState.Down) { PlayerObject2D->Translate(DirectX::XMVectorSet(0, -3, 0, 0)); }
			if (KeyboardState.Right) { PlayerObject2D->Translate(DirectX::XMVectorSet(+3, 0, 0, 0)); }
			if (KeyboardState.Left) { PlayerObject2D->Translate(DirectX::XMVectorSet(-3, 0, 0, 0)); }
			   
			Game.BeginRendering(BackgroundColor);

			Game.GetPlayerObject2D()->SetVisibleArea(DirectX::XMFLOAT2((float)(iFrame * 220), 0));

			Game.Draw();

			Game.EndRendering();

			++iFrame;

			if (iFrame >= 6) iFrame = 0;
		}
	}

	return 0;
}

LRESULT WINAPI WndProc(_In_ HWND hWnd, _In_ UINT Msg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	switch (Msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);

		return 0;
	case WM_ACTIVATEAPP:
		DirectX::Keyboard::ProcessMessage(Msg, wParam, lParam);
		break;
	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		DirectX::Mouse::ProcessMessage(Msg, wParam, lParam);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			DestroyWindow(hWnd);
		}
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		DirectX::Keyboard::ProcessMessage(Msg, wParam, lParam);
		break;
	default:
		break;
	}


	return DefWindowProc(hWnd, Msg, wParam, lParam);
}