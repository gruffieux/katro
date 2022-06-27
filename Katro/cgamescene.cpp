#include <csceneslinker.h>
#include <canimation.h>
#include <cdatafilelist.h>
#include <csound.h>
#include "cindicator.h"
#include "cball.h"
#include "cgamescene.h"
#include "cplayeria.h"
#include "main.h"
#include "../KaRes/resource.h"

extern ScenesLinker scenario;
extern List musics;

GameScene::GameScene() : Scene(GAME_SCENE_ID)
{
	int i;

	holesTotal = cpuLevel = winner = 0;
	currentTrack = -1;
	playerMode = ONE_PLAYER_VS_ONE_CPU;
	gameMode = TWO_PLAYERS;
	
	title = new Visual();
	backButton = new Visual();
	separator = new Visual();
	boards = new Panel();

	for (i = 0; i < TOTAL_SHELL; i++)
		shell[i] = new Visual();

	for (i = 0; i < TOTAL_BOTTOM_DECO; i++)
		bottomDeco[i] = new Visual();

	playersIter.setList(&players);
}

GameScene::GameScene(int holesTotal, PlayerMode playerMode, GameMode gameMode, Mover *hand) :
Scene(GAME_SCENE_ID)
{
	int i;
	Player *player;
	LinkedList<Player*>::Iterator iterator(&players);

	cpuLevel = winner = 0;
	currentTrack = -1;
	this->holesTotal = holesTotal;
	this->playerMode = playerMode;
	this->gameMode = gameMode;

	title = new Visual();
	backButton = new Visual();
	separator = new Visual();

	for (i = 0; i < TOTAL_SHELL; i++)
		shell[i] = new Visual();

	for (i = 0; i < TOTAL_BOTTOM_DECO; i++)
		bottomDeco[i] = new Visual();

	switch (gameMode)
	{
	case TWO_PLAYERS:
		player = new Player(1, "");
		player->setHand(hand);
		players.append(player);
		switch (playerMode)
		{
		case GameScene::ONE_PLAYER_VS_ONE_CPU:
			player = new PlayerIA();
			break;
		case GameScene::ONE_PLAYER_VS_ONE_PLAYER:
			player = new Player(2, "");
			break;
		}
		player->setHand(hand);
		players.append(player);
		player = iterator.first();
		while (player)
		{
			nameIndicators.AddElement(new Indicator("..."));
			scoreIndicators.AddElement(new Indicator("00"));
			player = iterator.next();
		}
		boards = new Panel(0, 0, 0, 0, 41, false, Text(), 128*holesTotal/2-55, 128*4+32-55, Str());
		break;
	case FOUR_PLAYERS:
		break;
	default:
		break;
	}

	playersIter.setList(&players);
}

GameScene::~GameScene()
{
	int i;

	delete title;
	delete backButton;
	delete separator;
	delete boards;

	for (i = 0; i < TOTAL_SHELL; i++)
		delete shell[i];

	for (i = 0; i < TOTAL_BOTTOM_DECO; i++)
		delete bottomDeco[i];

	nameIndicators.RemoveAllElement(0, true);
	scoreIndicators.RemoveAllElement(0, true);
}

Player* GameScene::currentPlayer()
{
	if (playersIter.hasCurrent())
		return playersIter.currentData();

	return NULL;
}

void GameScene::destroy()
{
	Player *player;
	LinkedList<Player*>::Iterator iterator(&players);

	player = iterator.first();

	while (player)
	{
		player->getBoard()->destroy();
		delete player;
		player = iterator.next();
	}

	players.clear();
}

void GameScene::destroyAni()
{
	int i;
	Player *player;
	LinkedList<Player*>::Iterator iterator(&players);

	turnIndicator.Destroy();
	title->destroyAni();
	backButton->destroyAni();
	separator->destroyAni();
	boards->destroyAni();

	for (i = 0; i < TOTAL_SHELL; i++)
		shell[i]->destroyAni();

	for (i = 0; i < TOTAL_BOTTOM_DECO; i++)
		bottomDeco[i]->destroyAni();

	for (i = 0; i < nameIndicators.GetElementCount(); i++)
		Indicator::getIndicatorElement(i, &nameIndicators)->destroyAni();

	for (i = 0; i < scoreIndicators.GetElementCount(); i++)
		Indicator::getIndicatorElement(i, &scoreIndicators)->destroyAni();

	player = iterator.first();

	while (player)
	{
		player->getBoard()->destroyAni();
		player = iterator.next();
	}
}

void GameScene::doMouseEvents(Cursor *cursor)
{
	// On cible le trou sous la main du joueur
	if (cursor->getClickState() == Cursor::NO_CLICK)
		currentPlayer()->updateFocus();

	// Quand un bouton est relâché
	switch (cursor->buttonReleased())
	{
	case Cursor::LEFT_CLICK:
		if (cursor->collision(backButton))
		{
			Sound::stop(DMUS_SEGF_DEFAULT);
			scenario.goToScene(MAIN_MENU_ID);
		}
		else if (cursor->collision(currentPlayer()->getBoard()) && !winner)
			switch (currentPlayer()->getNumber())
			{
			case 1:
				if (currentPlayer()->getFocus()->getRank() == Hole::BACK)
					currentPlayer()->startTurn(Board::RightToLeft);
				else
					currentPlayer()->startTurn(Board::LeftToRight);
				break;
			case 2:
				if (currentPlayer()->getFocus()->getRank() == Hole::FRONT)
					currentPlayer()->startTurn(Board::LeftToRight);
				else
					currentPlayer()->startTurn(Board::RightToLeft);
				break;
			}
		break;
	case Cursor::RIGHT_CLICK:
		break;
	default:
		break;
	}
}

void GameScene::faceToFace(Player *player1, Player *player2)
{
	Hole *hole1, *hole2;
	LinkedList<Hole*>::Iterator iter1(player1->getBoard()->getHoles());
	LinkedList<Hole*>::Iterator iter2(player2->getBoard()->getHoles());

	hole1 = iter1.last();
	hole2 = iter2.first();

	while (hole1 && hole2)
	{
		if ((hole1->getRank() != Hole::FRONT) | (hole2->getRank() != Hole::BACK))
			break;
		hole1->setOpposite(hole2);
		hole2->setOpposite(hole1);
		hole1 = hole1->getPrevious();
		hole2 = hole2->getNext();
	}

	player1->setOpponent(player2);
	player2->setOpponent(player1);
}

Player* GameScene::findPlayer(int number)
{
	Player *player;
	LinkedList<Player*>::Iterator iterator(&players);

	player = iterator.first();

	while (player)
	{
		if (player->getNumber() == number)
			return player;
		player = iterator.next();
	}

	return NULL;
}

void GameScene::init()
{
	int i, x;
	bool b;
	Indicator *indicator;
	LinkedList<Player*>::Iterator iterator(&players);

	switch (gameMode)
	{
	case TWO_PLAYERS:
		// Titre
		title->getDimension()->x = Animation::getAnimationElement(0, title->getAnimations())->GetFrameDimension()->x;
		title->getDimension()->y = Animation::getAnimationElement(0, title->getAnimations())->GetFrameDimension()->y;
		title->rescale(40, 20);
		title->centerPosition(0, 10, displayer->GetCurrentMode()->GetWidth(), title->getDimension()->y);

		// Bouton de retour
		backButton->getDimension()->x = Animation::getAnimationElement(0, backButton->getAnimations())->GetFrameDimension()->x;
		backButton->getDimension()->y = Animation::getAnimationElement(0, backButton->getAnimations())->GetFrameDimension()->y;
		backButton->rescale(70, 70);
		backButton->getPosition()->x = 10;
		backButton->getPosition()->y = 10;

		// Coquillage au  coin en haut à droite du plateau
		shell[0]->getDimension()->x = Animation::getAnimationElement(0, shell[0]->getAnimations())->GetFrameDimension()->x;
		shell[0]->getDimension()->y = Animation::getAnimationElement(0, shell[0]->getAnimations())->GetFrameDimension()->y;
		shell[0]->rescale(70, 70);
		shell[0]->getPosition()->x = displayer->GetCurrentMode()->GetWidth() - shell[0]->getDimension()->x;
		shell[0]->getPosition()->y = 0;

		// Coquillage au  coin en bas à droite du plateau
		shell[1]->getDimension()->x = Animation::getAnimationElement(0, shell[1]->getAnimations())->GetFrameDimension()->x;
		shell[1]->getDimension()->y = Animation::getAnimationElement(0, shell[1]->getAnimations())->GetFrameDimension()->y;
		shell[1]->rescale(70, 70);
		shell[1]->getPosition()->x = displayer->GetCurrentMode()->GetWidth() - shell[1]->getDimension()->x;
		shell[1]->getPosition()->y = displayer->GetCurrentMode()->GetHeight() - shell[1]->getDimension()->y;

		// Coquillage au  coin en bas à gauche du plateau
		shell[2]->getDimension()->x = Animation::getAnimationElement(0, shell[2]->getAnimations())->GetFrameDimension()->x;
		shell[2]->getDimension()->y = Animation::getAnimationElement(0, shell[2]->getAnimations())->GetFrameDimension()->y;
		shell[2]->rescale(70, 70);
		shell[2]->getPosition()->x = 0;
		shell[2]->getPosition()->y = displayer->GetCurrentMode()->GetHeight() - shell[2]->getDimension()->y;

		// Plateaux 2 joueurs
		boards->centerPosition(0, 0, displayer->GetCurrentMode()->GetWidth(), displayer->GetCurrentMode()->GetHeight());
		boards->Refresh();
		iterator.last()->getBoard()->getPosition()->x = boards->getPosition()->x - 29;
		iterator.last()->getBoard()->getPosition()->y = boards->getPosition()->y  - 29;
		iterator.last()->getBoard()->update(holesTotal);
		separator->getPosition()->x = boards->getPosition()->x - 29;
		separator->getPosition()->y = boards->getPosition()->y + 256 - 29;
		separator->getDimension()->x = holesTotal * 128 / 2;
		separator->getDimension()->y = 32;
		iterator.first()->getBoard()->getPosition()->x = boards->getPosition()->x - 29;
		iterator.first()->getBoard()->getPosition()->y = boards->getPosition()->y + 256 + separator->getDimension()->y - 29;
		iterator.first()->getBoard()->update(holesTotal);

		// Déco en bas
		x = displayer->GetCurrentMode()->GetWidth() / 2 - (40 * (TOTAL_BOTTOM_DECO * 2 - 1) / 2);
		b = true;
		for (i = 0; i < TOTAL_BOTTOM_DECO; i++)
		{
			bottomDeco[i]->getDimension()->x = Animation::getAnimationElement(0, bottomDeco[i]->getAnimations())->GetFrameDimension()->x;
			bottomDeco[i]->getDimension()->y = Animation::getAnimationElement(0, bottomDeco[i]->getAnimations())->GetFrameDimension()->y;
			if (b)
				bottomDeco[i]->rescale(15, 15);
			else
				bottomDeco[i]->rescale(30, 30);
			bottomDeco[i]->getPosition()->x = x;
			bottomDeco[i]->getPosition()->y = displayer->GetCurrentMode()->GetHeight() - 70;
			x += 40 * 2;
			b = !b;
		}

		// Indicateur de nom du joueur 1
		i = iterator.first()->getNumber() - 1;
		indicator = Indicator::getIndicatorElement(i, &nameIndicators);
		indicator->Refresh();
		indicator->getPosition()->x = boards->getPosition()->x + 70;
		indicator->getPosition()->y = boards->getPosition()->y + boards->getDimension()->y + 67;

		// Indicateur de score du joueur 1
		indicator = Indicator::getIndicatorElement(i, &scoreIndicators);
		indicator->Refresh();
		indicator->getPosition()->x = boards->getPosition()->x - 83;
		indicator->getPosition()->y = boards->getPosition()->y + boards->getDimension()->y - 100;

		// Indicateur de nom du joueur 2
		i = iterator.last()->getNumber() - 1;
		indicator = Indicator::getIndicatorElement(i, &nameIndicators);
		indicator->Refresh();
		indicator->getPosition()->x = boards->getPosition()->x + boards->getDimension()->x - indicator->getDimension()->x - 70;
		indicator->getPosition()->y = boards->getPosition()->y - 67;

		// Indicateur de score du joueur 1
		indicator = Indicator::getIndicatorElement(i, &scoreIndicators);
		indicator->Refresh();
		indicator->getPosition()->x = boards->getPosition()->x + boards->getDimension()->x + 67;
		indicator->getPosition()->y = boards->getPosition()->y + 100;
		break;
	case FOUR_PLAYERS:
		break;
	default:
		break;
	}

	Sound::stop(DMUS_SEGF_DEFAULT);

	initMusicTracks();
}

void GameScene::initAni()
{
	int i;
	bool b = true;
	Player *player;
	LinkedList<Player*>::Iterator iterator(&players);

	title->addAni(IDB_TITLE, "", 1, 963, 447, -1, 0);
	backButton->addAni(IDB_BACK_BUTTON, "", 1, 368, 73, RGB(255, 255, 255), 0);

	separator->setMosaic(true);
	separator->addAni(IDB_BOARD_SEPARATOR, "", 1, 16, 16, -1, 0);

	shell[0]->addAni(IDB_SHELL1, "", 1, 279, 266, RGB(255, 255, 255), 0);
	shell[1]->addAni(IDB_SHELL2, "", 1, 210, 226, RGB(255, 255, 255), 0);
	shell[2]->addAni(IDB_SHELL3, "", 1, 284, 249, RGB(255, 255, 255), 0);

	for (i = 0; i < TOTAL_BOTTOM_DECO; i++)
	{
		if (b)
			bottomDeco[i]->addAni(IDB_STONES, "", 1, 736, 393, RGB(255, 255, 255), 0);
		else
			bottomDeco[i]->addAni(IDB_BUSH, "", 1, 360, 196, RGB(255, 255, 255), 0);
		b = !b;
	}

	boards->initBorder(0, IDB_BORDER_TOP, "", 8, 41, RGB(255, 255, 255), true);
	boards->initBorder(1, IDB_BORDER_RIGHT, "", 41, 8, RGB(255, 255, 255), true);
	boards->initBorder(2, IDB_BORDER_BOTTOM, "", 8, 41, RGB(255, 255, 255), true);
	boards->initBorder(3, IDB_BORDER_LEFT, "", 41, 8, RGB(255, 255, 255), true);
	boards->initBorderCorners(IDB_BORDER_CORNERS, "", 41, 41, RGB(255, 255, 255));

	player = iterator.first();

	while (player)
	{
		i = player->getNumber() - 1;
		Indicator::getIndicatorElement(i, &nameIndicators)->initAni();
		Indicator::getIndicatorElement(i, &scoreIndicators)->initAni();
		player = iterator.next();
	}
}

bool GameScene::initMusicTracks()
{
	int track;
	List randomTracks;

	randomTracks.AddElement(Sound::getSoundElement(LAIDAMA, &musics));
	randomTracks.AddElement(Sound::getSoundElement(PAULINE, &musics));
	randomTracks.AddElement(Sound::getSoundElement(SOUL_OF_MBIRA, &musics));

	musicTracks.RemoveAllElement();

	while (randomTracks.GetElementCount())
	{
		track = Clock::random(0, randomTracks.GetElementCount()-1);
		if (track == -1)
			return false;
		musicTracks.AddElement(Sound::getSoundElement(track, &randomTracks));
		randomTracks.RemoveElement(track, 1);
	}

	currentTrack = -1;

	return true;
}

bool GameScene::load(Mover *hand)
{
	int i, j, k, curPlayerNb, holesTotal, ballsCount;
	Str index;
	GameScene *gameScene;
	Player *player;
	Hole *hole;
	Indicator *indicator;
	List balls;
	LinkedList<Player*>::Iterator *playersIter;
	LinkedList<Hole*>::Iterator *holesIter;
	DataFileList file("savegame.dat");

	if (!file.Open(true, false, false))
		return false;

	gameScene = (GameScene*)scenario.findScene(GAME_SCENE_ID);

	if (gameScene)
	{
		gameScene->destroy();
		gameScene->destroyAni();
		scenario.removeScene(gameScene);
		delete gameScene;
	}

	gameScene = new GameScene();
	playersIter = new LinkedList<Player*>::Iterator(gameScene->getPlayers());

	// On charge les données globales
	curPlayerNb = file.Read("curPlayer.nb");
	holesTotal = file.Read("holesTotal");
	gameScene->setHolesTotal(holesTotal);
	gameScene->setPlayerMode((PlayerMode)file.Read("playerMode"));
	gameScene->setGameMode((GameMode)file.Read("gameMode"));

	// On créé les joueurs et le panneau principal pour les plateaux
	switch (gameScene->getGameMode())
	{
	case TWO_PLAYERS:
		player = new Player(1, "");
		player->setHand(hand);
		gameScene->getPlayers()->append(player);
		switch (gameScene->getPlayerMode())
		{
		case GameScene::ONE_PLAYER_VS_ONE_CPU:
			player = new PlayerIA();
			break;
		case GameScene::ONE_PLAYER_VS_ONE_PLAYER:
			player = new Player(2, "");
			break;
		}
		player->setHand(hand);
		gameScene->getPlayers()->append(player);
		gameScene->setBoards(new Panel(0, 0, 0, 0, 41, false, Text(), 128*holesTotal/2-55, 128*4+32-55, Str()));
		break;
	case FOUR_PLAYERS:
		break;
	default:
		break;
	}

	i = 0;
	player = playersIter->first();

	// On charge les données des joueurs et l'état de leurs plateaux
	while (player)
	{
		index = Str::Parse(i+1);
		player->setName(file.ReadString("player.name", false, index, true));
		player->getBoard()->init(holesTotal);
		player->getBoard()->initAni();
		j = 0;
		holesIter = new LinkedList<Hole*>::Iterator(player->getBoard()->getHoles());
		hole = holesIter->first();
		while (hole)
		{
			index = BuildString("%i%i", i+1, j+1);
			ballsCount = file.Read("hole.balls.count", false, index, true);
			for (k = 0; k < ballsCount; k++)
			{
				index = BuildString("%i%i%i", i+1, j+1, k+1);
				balls.AddElement(new Ball(file.Read("hole.ball.playerNb", false, index, true)));
			}
			hole->addBalls(&balls);
			balls.RemoveAllElement(0, true);
			hole = holesIter->next();
			j++;
		}
		delete holesIter;
		indicator = new Indicator(*player->getName());
		gameScene->getNameIndicators()->AddElement(indicator);
		indicator->Refresh();
		indicator = new Indicator(Str::Parse(player->getBoard()->ballsAmount()));
		gameScene->getScoreIndicators()->AddElement(indicator);
		indicator->Refresh();
		player = playersIter->next();
		i++;
	}

	file.Close();

	// On met les joueurs face à face
	switch (gameScene->getGameMode())
	{
	case TWO_PLAYERS:
		gameScene->faceToFace(playersIter->first(), playersIter->last());
		break;
	case FOUR_PLAYERS:
		break;
	default:
		break;
	}

	// On commence la scène
	gameScene->initAni();
	gameScene->setCurrentPlayer(curPlayerNb);
	gameScene->getSeparator()->setName(BuildString("C'était à %s de jouer...", gameScene->currentPlayer()->getName()->Get()));
	gameScene->updateTurnIndicator();
	scenario.insertScene(gameScene);
	scenario.goToScene(GAME_SCENE_ID);

	delete playersIter;

	return true;
}

void GameScene::nextTurn()
{
	if (playersIter.hasNext())
		playersIter.next();
	else
		playersIter.first();
}

void GameScene::play()
{
	int i;
	Str text;
	Indicator *indicator;
	Player *player;
	PlayerIA *playerIA;
	LinkedList<Player*>::Iterator iterator(&players);

	playerIA = dynamic_cast<PlayerIA*>(currentPlayer());

	// Le joueur artificiel joue son tour
	if (!winner)
		if (playerIA && playerIA->getActions()->isEmpty())
		{
			playerIA->think(holesTotal);
			if (playerIA->getFocus())
			{
				playerIA->getHand()->centerPosition(playerIA->getFocus());
				if (playerIA->getFocus()->getRank() == Hole::FRONT)
					playerIA->startTurn(Board::LeftToRight);
				else
					playerIA->startTurn(Board::RightToLeft);
			}
			else
			{
				winner = playerIA->getOpponent()->getNumber();
				separator->setName(BuildString("%s déclare forfait, %s a gagné la partie!", playerIA->getName()->Get(), playerIA->getOpponent()->getName()->Get()));
			}
		}

	// Le joueur en cours joue son tour
	switch (currentPlayer()->playTurn())
	{
	case Player::TURN_END:
		if (currentPlayer()->getOpponent()->getBoard()->ballsAmount() > 1)
		{
			nextTurn();
			separator->setName(BuildString("C'est à %s de jouer...", currentPlayer()->getName()->Get()));
		}
		else
			separator->setName(BuildString("%s a gagné la partie!", currentPlayer()->getName()->Get()));
		player = iterator.first();
		while (player)
		{
			i = player->getNumber() - 1;
			indicator = Indicator::getIndicatorElement(i, &scoreIndicators);
			indicator->setName(Str::Parse(player->getBoard()->ballsAmount()));
			indicator->updateContent();
			player = iterator.next();
		}
		updateTurnIndicator();
		break;
	default:
		break;
	}

	// Joue la musique de fin
	if (currentPlayer()->getOpponent()->getBoard()->ballsAmount() <= 1 && !winner)
	{
		winner = currentPlayer()->getNumber();
		Sound::stop(DMUS_SEGF_DEFAULT);
		if (playerIA)
			Sound::getSoundElement(SEGA, &musics)->play(DMUS_SEGF_DEFAULT, false);
		else
			Sound::getSoundElement(TWIST_AND_SHOUT, &musics)->play(DMUS_SEGF_DEFAULT, false);
	}
	else
		playTrack();
}

void GameScene::playTrack()
{
	if (!winner)
		if (currentTrack == -1 || Sound::getSoundElement(currentTrack, &musicTracks)->finished())
		{
			currentTrack++;
			if (currentTrack >= musicTracks.GetElementCount())
				currentTrack = 0;
			Sound::stop(DMUS_SEGF_DEFAULT);
			Sound::getSoundElement(currentTrack, &musicTracks)->play(DMUS_SEGF_DEFAULT, false);
		}
}

void GameScene::restoreAni()
{
	int i;
	Player *player;
	LinkedList<Player*>::Iterator iterator(&players);

	title->restoreAni();
	backButton->restoreAni();
	separator->restoreAni();
	boards->restoreAni();

	for (i = 0; i < TOTAL_SHELL; i++)
		shell[i]->restoreAni();

	for (i = 0; i < TOTAL_BOTTOM_DECO; i++)
		bottomDeco[i]->restoreAni();

	for (i = 0; i < nameIndicators.GetElementCount(); i++)
		Indicator::getIndicatorElement(i, &nameIndicators)->restoreAni();

	for (i = 0; i < scoreIndicators.GetElementCount(); i++)
		Indicator::getIndicatorElement(i, &scoreIndicators)->restoreAni();

	player = iterator.first();

	while (player)
	{
		player->getBoard()->restoreAni();
		player = iterator.next();
	}

	updateTurnIndicator();
}

void GameScene::runAnimations()
{
	int i;
	Player *player;
	Item item;
	LinkedList<Player*>::Iterator iterator(&players);

	title->runAnimations();

	boards->Run();

	separator->runAnimations();

	// Affiche l'indicateur de tour
	*item.getDimension() = Axe(turnIndicator.GetWidth(), turnIndicator.GetHeight());
	item.centerPosition(separator);
	displayer->Blit(separator->getPosition()->x + 50, item.getPosition()->y, &turnIndicator, NULL, NULL, DDBLT_WAIT | DDBLT_KEYSRC);

	player = iterator.first();

	while (player)
	{
		player->getBoard()->runAnimations();
		player = iterator.next();
	}

	for (i = 0; i < TOTAL_SHELL; i++)
		shell[i]->runAnimations();

	for (i = 0; i < nameIndicators.GetElementCount(); i++)
		Indicator::getIndicatorElement(i, &nameIndicators)->runAnimations();

	for (i = 0; i < scoreIndicators.GetElementCount(); i++)
		Indicator::getIndicatorElement(i, &scoreIndicators)->runAnimations();

	for (i = 0; i < TOTAL_BOTTOM_DECO; i++)
		bottomDeco[i]->runAnimations();

	backButton->runAnimations();
}

void GameScene::setCurrentPlayer(int number)
{
	int i;
	Player *player;

	player = playersIter.first();

	i = player->getNumber();

	while (i != number)
	{
		player = playersIter.next();
		i++;
	}
}

bool GameScene::save()
{
	int i, j, k;
	Str index;
	Player *player;
	Hole *hole;
	LinkedList<Player*>::Iterator playersIter(&players);
	LinkedList<Hole*>::Iterator *holesIter;
	DataFileList file("savegame.dat");

	if (!file.Open(false, true, true))
		return false;

	if (!currentPlayer()->getActions()->isEmpty())
		nextTurn();

	file.Write(currentPlayer()->getNumber(), "curPlayer.nb");
	file.Write(holesTotal, "holesTotal");
	file.Write(playerMode, "playerMode");
	file.Write(gameMode, "gameMode");

	i = 0;
	player = playersIter.first();

	while (player)
	{
		index = Str::Parse(i+1);
		file.WriteString(*player->getName(), "player.name", false, index, true);
		j = 0;
		holesIter = new LinkedList<Hole*>::Iterator(player->getBoard()->getHoles());
		hole = holesIter->first();
		while (hole)
		{
			index = BuildString("%i%i", i+1, j+1);
			file.Write(hole->getBalls()->GetElementCount(), "hole.balls.count", false, index, true);
			for (k = 0; k < hole->getBalls()->GetElementCount(); k++)
			{
				index = BuildString("%i%i%i", i+1, j+1, k+1);
				file.Write(Ball::getBallElement(k, hole->getBalls())->getPlayerNb(), "hole.ball.playerNb", false, index, true);
			}
			hole = holesIter->next();
			j++;
		}
		delete holesIter;
		player = playersIter.next();
		i++;
	}

	file.Close();

	return true;
}

bool GameScene::saveExists()
{
	DataFileList file("savegame.dat");

	if (file.Open(true, false, false))
	{
		file.Close();
		return true;
	}

	return false;
}

void GameScene::start()
{
	int i, playerNb;
	Player *player;
	PlayerIA *playerIA;
	Indicator *indicator;
	LinkedList<Player*>::Iterator iterator(&players);

	winner = 0;

	player = iterator.first();

	// On prépare les plateaux
	while (player)
	{
		player->getBoard()->init(holesTotal);
		player->getBoard()->initAni();
		player->prepareBoard();
		playerIA = dynamic_cast<PlayerIA*>(player);
		if (playerIA)
			playerIA->setLevel(cpuLevel);
		i = player->getNumber() - 1;
		indicator = Indicator::getIndicatorElement(i, &scoreIndicators);
		indicator->setName(Str::Parse(player->getBoard()->ballsAmount()));
		indicator->updateContent();
		indicator = Indicator::getIndicatorElement(i, &nameIndicators);
		indicator->setName(*player->getName());
		indicator->Refresh();
		player = iterator.next();
	}

	// On met les joueurs face à face
	switch (gameMode)
	{
	case TWO_PLAYERS:
		faceToFace(iterator.first(), iterator.last());
		playerNb = Clock::random(1, 2);
		break;
	case FOUR_PLAYERS:
		break;
	default:
		break;
	}

	// Définit le joueur qui commence
	setCurrentPlayer(playerNb);

	separator->setName(BuildString("%s commence...", currentPlayer()->getName()->Get()));

	updateTurnIndicator();
}

void GameScene::updateTurnIndicator()
{
	if (separator->getName()->GetLength())
	{
		displayer->LoadText(&turnIndicator, NULL, separator->getName()->Get(), RGB(0, 0, 0), RGB(255, 255, 255));
		turnIndicator.SetColorKey(RGB(0, 0, 0));
	}
}
