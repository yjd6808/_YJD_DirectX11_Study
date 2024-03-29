#pragma once

#include "Component.h"

class Transform : public Component
{
public:
	Transform();
	~Transform() override;

	void Init() override;
	void Update() override;

	void UpdateTransform();

	// Local
	Vec3 GetLocalScale() { return _localScale; }
	void SetLocalScale(const Vec3& localScale) { _localScale = localScale; UpdateTransform(); }

	Vec3 GetLocalRotation() { return _localRotation; }
	void SetLocalRotation(const Vec3& localRotation) { _localRotation = localRotation;  UpdateTransform(); }

	Vec3 GetLocalPosition() { return _localPosition; }
	void SetLocalPosition(const Vec3& localPosition) { _localPosition = localPosition; UpdateTransform(); }

	// World
	Vec3 GetScale() { return _scale; }
	void SetScale(const Vec3& scale);

	Vec3 GetRotation() { return _rotation; }
	void SetRotation(const Vec3& rotation);

	Vec3 GetPosition() { return _position; }
	void SetPosition(const Vec3& position);

	Matrix GetWorldMatrix() { return _matWorld; }

	// 계층
	bool HasParent() { return _parent != nullptr; }
	shared_ptr<Transform> GetParent() { return _parent; }
	void SetParent(shared_ptr<Transform> parent) { _parent = parent; }

	const vector<shared_ptr<Transform>>& GetChildren() { return _children; }
	void AddChild(const shared_ptr<Transform>& child) { _children.push_back(child); }
private:
	Vec3 _localPosition = { 0.f, 0.f, 0.f };
	Vec3 _localRotation = { 0.f, 0.f, 0.f };
	Vec3 _localScale = { 1.f, 1.f, 1.f };

	Matrix _matLocal = Matrix::Identity; // 나의 부모 좌표를 기준으로 하는 나의 변환 행렬
	Matrix _matWorld = Matrix::Identity; // 월드 행렬

	// Cache
	Vec3 _scale;
	Vec3 _rotation;
	Vec3 _position;

	Vec3 _right;
	Vec3 _up;
	Vec3 _look;
private:
	shared_ptr<Transform> _parent;
	vector<shared_ptr<Transform>> _children;
};



