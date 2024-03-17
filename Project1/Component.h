#pragma once

class Component
{
public:
	Component(const shared_ptr<GameObject>& owner);
	virtual ~Component();

	virtual void Init() = 0;
	virtual void Update() = 0;

	shared_ptr<GameObject> GetGameObject() { return _owner.lock(); }
protected:
	weak_ptr<GameObject> _owner;
};

