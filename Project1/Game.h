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
	void Update();
	void Render();
private:
	shared_ptr<Graphics> _graphics;
	shared_ptr<Pipeline> _pipeline;
	shared_ptr<GameObject> _gameObject;
};
