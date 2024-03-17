#include "pch.h"
#include "Component.h"

Component::Component(const shared_ptr<GameObject>& owner)
	: _owner(owner)
{
}

Component::~Component() {
}
