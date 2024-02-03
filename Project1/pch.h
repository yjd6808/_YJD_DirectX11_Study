#pragma once


#include "Types.h"
#include "Values.h"
#include "Struct.h"

// STL
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
using namespace std;

// WIN
#include <Windows.h>
#include <assert.h>

// DirectX
// �������� ���̷�ƮX ���̺귯���� ��ũ���ֵ��� ������ ���� �߰�������ߴµ�
// ������10 ���Ŀ��� ���� ���̷�Ʈ�� ���뼺 �ְ� ���Ǵ� ���̺귯���� �ƿ� SDK�� ���Խ��״�.
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <DirectXMath.h>

// �ؽ��ĸ� �ε����ִ� ���̺귯���� DirectTex
// https://github.com/microsoft/DirectXTex �������� ��  DirectXTex_Desktop_2022.sln �����ؼ� ���̺귯�� �̴´�.
// DirectXTex.h, DirectXTex.ini ������ ��Ŭ��� ��������̹Ƿ� �����ؼ� �����ͼ� ��
#include "DirectXTex/DirectXTex.h"
#include "DirectXTex/DirectXTex.inl"
using namespace DirectX;
using namespace Microsoft::WRL;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "DirectXTex.lib")
