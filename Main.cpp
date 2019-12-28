#include "Core/CGame.h"
#include "Core/Math.h"

LRESULT WINAPI WndProc(_In_ HWND hWnd, _In_ UINT Msg, _In_ WPARAM wParam, _In_ LPARAM lParam);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	using namespace DirectX;

	static constexpr float KWidth{ 1280.0f };
	static constexpr float KHeight{ 720.0f };

	srand((unsigned int)GetTickCount64());

	CGame Game{ KWidth, KHeight };

	Game.Create(hInstance, WndProc, "GameBase");

	Game.AddObject2D("Player", DirectX::XMFLOAT2(220, 220), "Asset/dalb_anim.png");
	Game.GetObject2DPtr("Player")->ScaleTo(DirectX::XMVectorSet(0.5f, 0.5f, 1, 0));

	Game.AddObject2D("BG", DirectX::XMFLOAT2(1280.0f, 1280.0f), "Asset/BackGround.png");

	Game.CreateStarPool();

	Game.SetPlayerObject2D("Player");

	MSG Msg{};

	float BackgroundColor[]{ 0.0f, 0.0f, 0.0f, 1.0f };

	int iFrame{};

	bool IsZKeyDown{};

	while (true)
	{
		if (PeekMessage(&Msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (Msg.message == WM_QUIT)
			{
				break;
			}

			if (Msg.message == WM_CHAR)
			{
				if (Msg.wParam == 'z')
				{
					IsZKeyDown = true;
				}
			}

			TranslateMessage(&Msg);

			DispatchMessage(&Msg);
		}
		else
		{
			Game.Update();

			float DeltaTime{ Game.GetDeltaTime() };

			DirectX::Keyboard::State KeyboardState{ Game.GetKeyboardState() };

			CObject2D* const PlayerObject2D{ Game.GetPlayerObject2D() };

			static float PlayerSpeed{ 400.0f };

			float PlayerX{ DirectX::XMVectorGetX(PlayerObject2D->GetTranslation()) };
			float PlayerY{ DirectX::XMVectorGetY(PlayerObject2D->GetTranslation()) };

			if (KeyboardState.Up && PlayerY < +KHeight * 0.5f) { PlayerObject2D->Translate(DirectX::XMVectorSet(0, +PlayerSpeed, 0, 0) * DeltaTime); }
			if (KeyboardState.Down && PlayerY > -KHeight * 0.5f) { PlayerObject2D->Translate(DirectX::XMVectorSet(0, -PlayerSpeed, 0, 0) * DeltaTime); }
			if (KeyboardState.Right && PlayerX < +KWidth * 0.5f) { PlayerObject2D->Translate(DirectX::XMVectorSet(+PlayerSpeed, 0, 0, 0) * DeltaTime); }
			if (KeyboardState.Left && PlayerX > -KWidth * 0.5f) { PlayerObject2D->Translate(DirectX::XMVectorSet(-PlayerSpeed, 0, 0, 0) * DeltaTime); }
			if (IsZKeyDown) { Game.SpawnPlayerBullet(); IsZKeyDown = false; }

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