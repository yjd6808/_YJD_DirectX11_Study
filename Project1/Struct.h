/*
 * 작성자: 윤정도
 * 생성일: 2/3/2024 8:11:44 PM
 * =====================
 * 공용으로 사용할 녀석들
 */


#pragma once

struct TransformData
{
	Matrix MatWorld = Matrix::Identity;
	Matrix MatView = Matrix::Identity;
	Matrix MatProjection = Matrix::Identity;
};