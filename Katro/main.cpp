/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									Jeu de Katro Malgache
//                         -----------------------------------------
// Tactiques:
// -Sur la deuxième ligne ou on arrive à choper les pions, il faut mettre un pions dans chaque trou et mettre le reste en sécurité derrière
//
// TODO:
// - Music et sons
// - Jeu traditionnel ou avec bonus: les bonus sont tiré tous les x tour et peuvent être utilisés quand on veut
//
// BONUS:
// - Billes en plus
// - Tour en plus
// - Inverser plateaux
// - Billes imprenable
//
// BUGS:
// -Lorsque on change la résolution, l'indicateur du dessus se place faux
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <dxerror.h>
#include <cmsgreporter.h>
#include <cconfigfile.h>
#include <canimation.h>
#include <csceneslinker.h>
#include <ckey.h>
#include <ckeyboardlistener.h>
#include <csound.h>
#include "cplayer.h"
#include "ctitlescene.h"
#include "cmainmenu.h"
#include "crulesmenu.h"
#include "cgamemenu.h"
#include "cplayermenu.h"
#include "cgamescene.h"
#include "resource.h"
#include "main.h"
#include "../KaRes/resource.h"

// Prototypes des fonctions 
/////////////////////////////////

void CheckUpdate();
void DestroyDirectDraw();
void DestroyDirectInput();
void DestroyDirectMusic();
void DestroyAll();
void InitDirectDraw();
void InitDirectInput();
void InitDirectMusic();
void InitObjects();
HWND InitWindow(HINSTANCE hInst, int nCmdShow);
bool LoadConfig();
void PlayScene();
bool ProcessKbInput();
bool ProcessMouseInput();
void RestoreDirectDraw();
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Variables globales 
////////////////////////////////

//Variables et structures
bool running, reinit;

//Objets de base nécessaire à l'application
Display *Screen;
Input *keyboard, *mouse;

//Objets
ConfigFile config;
KeyboardListener keybListener;
Visual *background;
Cursor *cursor;
ScenesLinker scenario;
List sounds, musics;

// Surface textes pour testes
Surface testText[2];

// Définitions des fonctions
/////////////////////////////////////

void CheckUpdate()
{
	DWORD exitCode;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

	CreateProcess(NULL, "UpdateClient.exe CHECK", NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	WaitForSingleObject(pi.hProcess, 15000);
	GetExitCodeProcess(pi.hProcess, &exitCode);

	switch (exitCode)
	{
	case 0:
		break;
	case 1:
		if (MessageBox(NULL, "Le programme doit fermer, voulez vous continuer? Relancez-le après la mise à jour.", "Mise à jour disponible!", MB_YESNO) == IDYES)
		{
			ShellExecute(NULL, "open", "UpdateClient.exe", "UPDATE", NULL, SW_SHOW);
			exit(0);
		}
		break;
	default:
		TerminateProcess(pi.hProcess, 0);
		MessageBox(NULL, "Impossible de vérifier la mise à jour, si le problème persiste, lancez le programme de mis à jour par vous-même.", "Erreur inconnue", MB_ICONERROR);
		break;
	}
}

void DestroyAll()
{
	DestroyDirectDraw();
	DestroyDirectInput();
	DestroyDirectMusic();

	scenario.destroyAll();

	delete background;
	delete cursor;
	delete Screen;
	delete keyboard;
	delete mouse;

	sounds.RemoveAllElement(0, true);
	musics.RemoveAllElement(0, true);
}

void DestroyDirectDraw()
{
	Scene *scene;
	LinkedList<Scene*>::Iterator iterator(scenario.getScenes());

	background->destroyAni();
	cursor->destroyAni();

	scene = iterator.first();

	while (scene)
	{
		scene->destroyAni();
		scene = iterator.next();
	}
	
	Screen->Destroy();
}

void DestroyDirectInput()
{
	keyboard->Destroy();
	mouse->Destroy();
}

void DestroyDirectMusic()
{
	int i;

	for (i = 0; i < sounds.GetElementCount(); i++)
		Sound::getSoundElement(i, &sounds)->destroy();

	for (i = 0; i < musics.GetElementCount(); i++)
		Sound::getSoundElement(i, &musics)->destroy();

	Sound::destroyPlayer();
}

void InitDirectDraw()
{
	Scene *scene;
	LinkedList<Scene*>::Iterator iterator(scenario.getScenes());

	Screen->SetDisplayMode();

	// TODO: Corriger le problème de performance sur XP et Vista
	// Pour l'instant on optimise les perfs en release pour Vista et en debug pour XP
#ifdef _DEBUG
	background->setMosaic(true);
#else
	background->setMosaic(false);
#endif

	background->addAni(IDB_SCENE_BKG, "", 1, 16, 16, -1, 90);

	cursor->addAni(IDB_HAND, "", 1, 48, 48, RGB(255, 255, 255), 90);

	scene = iterator.first();

	while (scene)
	{
		scene->initAni();
		scene = iterator.next();
	}
}

void InitDirectInput()
{
	keyboard->CreateDevice(GUID_SysKeyboard, &c_dfDIKeyboard, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	mouse->CreateDevice(GUID_SysMouse, &c_dfDIMouse, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
}

void InitDirectMusic()
{
	Sound::initPlayer(Screen->GetDxWnd(), DMUS_APATH_DYNAMIC_STEREO, 128, DMUS_AUDIOF_ALL);

	sounds.Initialize(3);
	musics.Initialize(6);

	sounds.AddElement(BALL_DROP, new Sound());
	Sound::getSoundElement(BALL_DROP, &sounds)->load("Sons/ball_drop.wav");

	sounds.AddElement(BALLS_DROP, new Sound());
	Sound::getSoundElement(BALLS_DROP, &sounds)->load("Sons/balls_drop.wav");

	sounds.AddElement(BALLS_GRAB, new Sound());
	Sound::getSoundElement(BALLS_GRAB, &sounds)->load("Sons/balls_grab.wav");

	musics.AddElement(MANDRY_A, new Sound());
	Sound::getSoundElement(MANDRY_A, &musics)->load("Sons/mandry_a.mid", true);

	musics.AddElement(LAIDAMA, new Sound());
	Sound::getSoundElement(LAIDAMA, &musics)->load("Sons/laidama.mid", true);

	musics.AddElement(PAULINE, new Sound());
	Sound::getSoundElement(PAULINE, &musics)->load("Sons/pauline3.mid", true);

	musics.AddElement(SOUL_OF_MBIRA, new Sound());
	Sound::getSoundElement(SOUL_OF_MBIRA, &musics)->load("Sons/soul_of_mbira.mid", true);

	musics.AddElement(TWIST_AND_SHOUT, new Sound());
	Sound::getSoundElement(TWIST_AND_SHOUT, &musics)->load("Sons/twist_and_shout.mid", true);

	musics.AddElement(SEGA, new Sound());
	Sound::getSoundElement(SEGA, &musics)->load("Sons/sega.mid", true);
}

void InitObjects()
{
	*background->getDimension() = Axe(Screen->GetCurrentMode()->GetWidth(), Screen->GetCurrentMode()->GetHeight());
	
	*cursor->getDimension() = Axe(10, 10);
	*cursor->getMover()->getDimension() = Axe(1, 1);
	cursor->getMover()->SetAcceleration(0.1);
}

HWND InitWindow(HINSTANCE hInst, int nCmdShow)
{
	HWND hWnd;
	WNDCLASS wc; //Structure contenant les informations de la fenêtre
	
	//----------Enregistrement d'une classe window----------

	//Assignation de valeure pour chaques champs de la structure wc
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor = LoadCursor((HINSTANCE)NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "MainWndClass";

	//Enregistrement
	if (!RegisterClass(&wc))
		ReportDxError(S_FALSE, WIN32_, ERR_REGISTERCLASS);
	
	//----------Création de la fenêtre----------

	hWnd = CreateWindow(wc.lpszClassName, "Katro", WS_OVERLAPPEDWINDOW, 0, 0, 1024, 768, (HWND)NULL, (HMENU)NULL, hInst, (LPVOID)NULL);

	if (!hWnd)
		ReportDxError(S_FALSE, WIN32_, ERR_CREATEWIN);

	ShowWindow(hWnd, nCmdShow); //Afficher la fenêtre
	UpdateWindow(hWnd);

	return hWnd;
}

bool LoadConfig()
{
	int i;
	bool b;

	if (!config.Open(true, false, false))
		return false;

	config.ReadGraphic(Screen->GetCurrentMode());
	config.ReadKeys();

	b = config.PlaySounds();

	for (i = 0; i < sounds.GetElementCount(); i++)
		if (b)
			Sound::getSoundElement(i, &sounds)->setMuted(0);
		else
		{
			Sound::getSoundElement(i, &sounds)->setMuted(2);
			Sound::getSoundElement(i, &sounds)->mute();
		}

	b = config.PlayMusic();

	for (i = 0; i < musics.GetElementCount(); i++)
		if (b)
			Sound::getSoundElement(i, &musics)->setMuted(0);
		else
		{
			Sound::getSoundElement(i, &musics)->setMuted(2);
			Sound::getSoundElement(i, &musics)->mute();
		}

	config.Close();

	return true;
}

void PlayScene()
{
	// On affiche le fond noir
	background->runAnimations();

	// On joue la scene en cours
	scenario.currentScene()->runAnimations();
	scenario.currentScene()->doMouseEvents(cursor);
	scenario.currentScene()->play();

	// On affiche le curseur
	cursor->setVisibleZone(cursor->getPosition()->x, cursor->getPosition()->x + 48, cursor->getPosition()->y, cursor->getPosition()->y + 48);
	cursor->runAni(0, 0);
	cursor->stickToMover();

#ifdef _DEBUG
	// On affiche les textes pour testes
	//Screen->LoadText(&testText[0], NULL, keybListener.getFlowWaiter(0)->getCounter(), RGB(0, 0, 0), RGB(255, 255, 255));
	//Screen->LoadText(&testText[1], NULL, keybListener.getFlowStarter()->getCounter(), RGB(0, 0, 0), RGB(255, 255, 255));
	//Screen->Blit(Screen->GetCurrentMode()->GetWidth() - 80, 0, &testText[0], NULL, NULL, DDBLT_WAIT);
	//Screen->Blit(Screen->GetCurrentMode()->GetWidth() - 40, 0, &testText[1], NULL, NULL, DDBLT_WAIT);
#endif

	// Double buffering
	Screen->FlipFlap();
}

bool ProcessKbInput()
{
	GameScene *gameScene;

	if (!keybListener.startCaptureKeys(keyboard))
		return false;

	gameScene = dynamic_cast<GameScene*>(scenario.currentScene());

	if (gameScene)
	{
		if (keybListener.keyReleased(DIK_LSHIFT))
			keybListener.startListenInput();
		if (keybListener.keyReleased(DIK_LCONTROL))
		{
			if (keybListener.stopListenInput())
			{
				gameScene->currentPlayer()->setName(keybListener.getInputString());
				gameScene->updateTurnIndicator();
			}
		}
	}

	if (keybListener.keyPressed((Str)"Configuration", config.GetKeyMap()))
	{
		ShowWindow(Screen->GetDxWnd(), SW_MINIMIZE);
		ShellExecute(Screen->GetDxWnd(), "open", "GameConfig.exe", NULL, NULL, SW_SHOW);
	}

	if (keybListener.keyReleased((Str)"Plein écran / fenêtré", config.GetKeyMap()))
	{
		DestroyDirectDraw();
		DestroyDirectInput();
		Screen->GetCurrentMode()->Switch();
		RestoreDirectDraw();
		InitDirectInput();
	}

	if (keybListener.keyPressed((Str)"Quitter", config.GetKeyMap()))
		WndProc(Screen->GetDxWnd(), WM_QUIT, NULL, NULL);

	keybListener.captureKeys();

	return true;
}

bool ProcessMouseInput()
{
	DIMOUSESTATE DIms;
	HRESULT hr;
	CardinalPoint screenEdge;

	screenEdge = CardinalPoint(0, Screen->GetCurrentMode()->GetWidth(), 0, Screen->GetCurrentMode()->GetHeight());

	hr = mouse->GetDevice()->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&DIms);

	if (hr != DI_OK)
	{
		ReportDxError(hr, DIRECTINPUT, ERR_DI_GETDEVICESTATE);
		return false;
	}
	else
	{
		if (Player::access)
		{
			cursor->setClickState(DIms.rgbButtons);
			cursor->getMover()->GenerateMouseAngle(DIms.lX, DIms.lY);
			cursor->getMover()->Move(DIms.lX, DIms.lY, &screenEdge, true);
			cursor->stickToMover();
		}
	}

	return true;
}

void RestoreDirectDraw()
{
	Scene *scene;
	LinkedList<Scene*>::Iterator iterator(scenario.getScenes());

	Screen->SetDisplayMode();

	background->restoreAni();
	cursor->restoreAni();

	scene = iterator.first();

	while (scene)
	{
		scene->restoreAni();
		scene = iterator.next();
	}
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	HMODULE ResModule;
	HWND hWndMain;

#ifndef _DEBUG
	CheckUpdate();
#endif

	Clock::useRandom();

	reinit = false;
	hWndMain = InitWindow(hInst, nCmdShow);
	ResModule = LoadLibrary("KaRes.dll");

	if (!ResModule)
	{
		MessageBox(hWndMain, "Impossible de charger la librairie RLRes.dll", NULL, MB_ICONERROR);
		SendMessage(hWndMain, WM_QUIT, 0, NULL);
		return 0;
	}

	Screen = new Display(hWndMain, ResModule);
	keyboard = new Input(hWndMain, hInst);
	mouse = new Input(hWndMain, hInst);

	InitDXErrorMsg();
	InitDirectMusic();

	if (!LoadConfig())
	{
		MessageBox(NULL, "Impossible de charger la configuration", NULL, MB_ICONERROR);
		SendMessage(NULL, WM_QUIT, NULL, NULL);
		return 0;
	}

	background = new Visual();
	cursor = new Cursor();

	scenario.appendScene(new TitleScene());
	scenario.appendScene(new MainMenu());
	scenario.appendScene(new RulesMenu());
	scenario.appendScene(new PlayerMenu());

	InitDirectDraw();
	InitDirectInput();
	InitObjects();

	scenario.begin();

	while (true)
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if(!GetMessage(&msg, NULL, 0, 0 ))
				return (int)msg.wParam;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (running)
			{
				if (reinit)
				{
					reinit = false;
					DestroyDirectDraw();
					DestroyDirectInput();
					RestoreDirectDraw();
					InitDirectInput();
				}
				else
				{
					PlayScene();
					ProcessMouseInput();
					ProcessKbInput();
				}
			}
		}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	GameScene *gameScene;

	switch (msg)
	{
	case WM_QUIT:
		gameScene = (GameScene*)scenario.findScene(GAME_SCENE_ID);
		if (gameScene && gameScene->currentPlayer())
			gameScene->save();
		FreeLibrary(Screen->GetResInst());
		DestroyAll();
		PostQuitMessage(0); //Ferme la fenêtre
		return 0;
	case WM_SETCURSOR:
		SetCursor(NULL); //Définit la valeur du curseur
		return 0;
	case WM_ACTIVATE:
		//Switché sur une autre application
		if (LOWORD(wParam) == WA_INACTIVE || (BOOL)HIWORD(wParam))
		{
			running = false;
			reinit = true;
		}
		else
			running = true;
		return 0;
	}

	if (msg == RegisterWindowMessage("GabGamesConfig"))
	{
		if (wParam == 1 && LoadConfig())
		{
			InitObjects();
			scenario.currentScene()->init();
		}
		ShowWindow(Screen->GetDxWnd(), SW_RESTORE);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
