#ifndef CGAMESCENE
#define CGAMESCENE

#include <cscene.h>
#include <clinkedlist.h>
#include <cpanel.h>
#include "cplayer.h"

class GameScene : public Scene
{
public:
	enum PlayerMode
	{
		ONE_PLAYER_VS_ONE_CPU,
		ONE_PLAYER_VS_ONE_PLAYER
	};
	enum GameMode
	{
		TWO_PLAYERS,
		FOUR_PLAYERS
	};
	GameScene();
	GameScene(int holesTotal, PlayerMode playerMode, GameMode gameMode, Mover *hand);
	~GameScene();
	void abandon(Player* player);
	Player* currentPlayer();
	void destroy();
	void destroyAni();
	void doMouseEvents(Cursor *cursor);
	void faceToFace(Player *player1, Player *player2);
	Player* findPlayer(int number);
	GameMode getGameMode() {return gameMode;}
	List* getNameIndicators() {return &nameIndicators;}
	PlayerMode getPlayerMode() {return playerMode;}
	Visual* getSeparator() {return separator;}
	List* getScoreIndicators() {return &scoreIndicators;}
	LinkedList<Player*>* getPlayers() {return &players;}
	void init();
	void initAni();
	bool initMusicTracks();
	static bool load(Mover *hand);
	void nextTurn();
	void play();
	void playTrack();
	void restoreAni();
	void runAnimations();
	void setCpuLevel(int cpuLevel) {this->cpuLevel = cpuLevel;}
	void setCurrentPlayer(int number);
	void setGameMode(GameMode gameMode) {this->gameMode = gameMode;}
	void setPlayerMode(PlayerMode playerMode) {this->playerMode = playerMode;}
	void setBoards(Panel* boards) {this->boards = boards;}
	void setHolesTotal(int holesTotal) {this->holesTotal = holesTotal;}
	bool save();
	static bool saveExists();
	void start();
	void updateTurnIndicator();
private:
	static const int TOTAL_BOTTOM_DECO = 5;
	static const int TOTAL_SHELL = 3;
	int holesTotal, cpuLevel, winner, currentTrack;
	PlayerMode playerMode;
	GameMode gameMode;
	Surface turnIndicator;
	Visual *title, *backButton, *shell[TOTAL_SHELL], *bottomDeco[TOTAL_BOTTOM_DECO], *separator;
	Panel *boards;
	List nameIndicators, scoreIndicators, musicTracks;
	LinkedList<Player*> players;
	LinkedList<Player*>::Iterator playersIter;
};

#endif
