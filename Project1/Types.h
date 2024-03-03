/*
 * 작성자: 윤정도
 * 생성일: 2/3/2024 8:11:54 PM
 * =====================
 *
 */


#pragma once

// 분석을 위해 정의한다.
#define _XM_NO_INTRINSICS_

#include <Windows.h>
#include "DirectXMath.h"
#include "SimpleMath.h"

using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;

using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

// using Vec2 = DirectX::XMFLOAT2;
// using Vec3 = DirectX::XMFLOAT3;
// using Vec4 = DirectX::XMFLOAT4;
using Color = DirectX::XMFLOAT4;

// 벡터는 확장기능이 추가된 녀석들로 변경
using Vec2 = DirectX::SimpleMath::Vector2;
using Vec3 = DirectX::SimpleMath::Vector3;
using Vec4 = DirectX::SimpleMath::Vector4;
using Matrix = DirectX::SimpleMath::Matrix;

