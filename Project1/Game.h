/*
 * 작성자: 윤정도
 * 생성일: 2/3/2024 9:23:29 PM
 * =====================
 *
 */


#pragma once

class Game
{
public:
	Game();
	~Game();

	void Init(HWND hwnd);
	void Render();
	void Update();
private:
	HWND _hwnd{};
	uint32 _width{};
	uint32 _height{};

private:
	// DirectX

};