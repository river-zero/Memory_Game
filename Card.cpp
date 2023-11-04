#include "Card.h"
#include <string>

using namespace Gdiplus;

Memory::Card::Card(HWND hwnd, int index, Type type, int x, int y) :
	// 멤버 변수 모두 초기화
	mHwnd(hwnd),
	mIndex(index),
	mType(type),
	mX(x),
	mY(y),
	mIsFront(false) 
{
	// 앞면 종류에 따른 이미지 경로 설정
	std::wstring filename = L"Images/";
	switch (mType) {
	case Type::Ace:
		filename += L"Ace.png";
		break;
	case Type::Joker:
		filename += L"Joker.png";
		break;
	case Type::King:
		filename += L"King.png";
		break;
	case Type::Seven:
		filename += L"7.png";
		break;
	}

	mFront = std::make_unique<Image>(filename.c_str());
	// Gdiplus::Image 생성자는 C 스타일의 문자열 필요
	mBack = std::make_unique<Image>(L"Images/Back.png");
	// 뒷면은 항상 동일
}

bool Memory::Card::CheckClicked(int x, int y) {
	// 마우스 위치가 이미지 범위에 속하는지 확인
	if (x >= mX && y >= mY &&
		x <= static_cast<int>(mX + mFront->GetWidth()) &&
		y <= static_cast<int>(mY + mFront->GetHeight())) {
		// 앞면이 아니라면 뒤집기
		Flip(!mIsFront);
		return true;
	}

	// GDI+의 기능을 사용한다면 이렇게도 가능
	/*Rect rct(mX, mY, mFront->GetWidth(), mFront->GetHeight());
	if (rct.Contains(x, y)) {
		Flip(!mIsFront);
		return true;
	}*/

	return false;
}

void Memory::Card::Flip(bool isFront) {
	// 상태 변경 및 화면 업데이트가 요청
	mIsFront = isFront;
	Invalidate();
}

void Memory::Card::Draw(Gdiplus::Graphics& graphics) {
	// mIsFront 값에 따라 앞면 또는 뒷면 이미지를 그림
	if (mIsFront) {
		graphics.DrawImage(mFront.get(), mX, mY, mFront->GetWidth(), mFront->GetHeight());
	} else {
		graphics.DrawImage(mBack.get(), mX, mY, mBack->GetWidth(), mBack->GetHeight());
	}
}

void Memory::Card::Invalidate() {
	// rct로 지정된 영역을 무효화
	RECT rct{ mX, mY, 
		static_cast<LONG>(mX + mFront->GetWidth()), 
		static_cast<LONG>(mY + mFront->GetHeight()) };
	InvalidateRect(mHwnd, &rct, false);
}