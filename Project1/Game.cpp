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
	_graphics = make_shared<Graphics>(hwnd);
	ComPtr<ID3D11Device> device = _graphics->GetDevice();
	ComPtr<ID3D11DeviceContext> deviceContext = _graphics->GetDeviceContext();

	_pipeline = make_shared<Pipeline>(deviceContext);
	_gameObject = make_shared<GameObject>(device, deviceContext);
}

void Game::Update() {
	_gameObject->Update();
}

void Game::Render() {

	// 준비작업을 먼저한다.
	_graphics->RenderBegin();

	// 여기서 여러가지 물체들을 그린다.
	{
		_gameObject->Render(_pipeline);
	}

	// 그림을 다 그렸으니 제출한다.
	_graphics->RenderEnd();
}

