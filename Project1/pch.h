#pragma once


#include "Types.h"
#include "Values.h"
#include "Struct.h"

// STL
#include <vector>
#include <memory>
#include <list>
#include <map>
#include <unordered_map>
#include <string>
using namespace std;

// WIN
#include <Windows.h>
#include <assert.h>

// DirectX
// 옛날에는 다이렉트X 라이브러리를 링크해주도록 설정을 직접 추가해줘야했는데
// 윈도우10 이후에는 워낙 다이렉트가 범용성 있게 사용되는 라이브러리라서 아예 SDK에 포함시켰다.
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <DirectXMath.h>

// 텍스쳐를 로딩해주는 라이브러리는 DirectTex
// https://github.com/microsoft/DirectXTex 내려받은 후  DirectXTex_Desktop_2022.sln 실행해서 라이브러리 뽑는다.
// DirectXTex.h, DirectXTex.ini 파일은 인클루드 헤더파일이므로 복사해서 가져와서 씀
#include "DirectXTex/DirectXTex.h"
#include "DirectXTex/DirectXTex.inl"
using namespace DirectX;
using namespace Microsoft::WRL;

#define CHECK(p)		assert(SUCCEEDED(p))

#include "Graphics.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "InputLayout.h"
#include "VertexData.h"
#include "Geometry.h"
#include "GeometryHelper.h"