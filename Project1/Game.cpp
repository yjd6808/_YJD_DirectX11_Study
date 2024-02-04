/*
 * 작성자: 윤정도
 * 생성일: 2/3/2024 9:23:38 PM
 * =====================
 *
 */


#include "pch.h"
#include "Game.h"


Game::Game() {
	
}

Game::~Game() {
	
}

void Game::Init(HWND hwnd) {
	_hwnd = hwnd;
	_width = GMinSizeX;
	_height = GMinSizeY;

	CreateDeviceAndSwapChain();
}

void Game::Render() {
	
}

void Game::Update() {
	
}

void Game::CreateDeviceAndSwapChain() {
}
