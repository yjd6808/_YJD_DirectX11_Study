﻿/*
 * 작성자: 윤정도
 * 생성일: 2/3/2024 8:11:44 PM
 * =====================
 * 공용으로 사용할 녀석들
 */


#pragma once


struct Vertex
{
	Vec3 Position;
	// Color Color;
	Vec2 UV;
};

struct TransformData
{
	Vec3 offset;
	float dummy;	// float4를 맞춰주기 위함
};