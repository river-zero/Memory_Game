#include "Card.h"
#include <string>

using namespace Gdiplus;

Memory::Card::Card(HWND hwnd, int index, Type type, int x, int y) :
	// ��� ���� ��� �ʱ�ȭ
	mHwnd(hwnd),
	mIndex(index),
	mType(type),
	mX(x),
	mY(y),
	mIsFront(false)
{
	// �ո� ������ ���� �̹��� ��� ����
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
	// Gdiplus::Image �����ڴ� C ��Ÿ���� ���ڿ� �ʿ�
	mBack = std::make_unique<Image>(L"Images/Back.png");
	// �޸��� �׻� ����
}

bool Memory::Card::CheckClicked(int x, int y) {
	// ���콺 ��ġ�� �̹��� ������ ���ϴ��� Ȯ��
	if (x >= mX && y >= mY &&
		x <= static_cast<int>(mX + mFront->GetWidth()) &&
		y <= static_cast<int>(mY + mFront->GetHeight())) {
		// �ո��� �ƴ϶�� ������
		Flip(!mIsFront);
		return true;
	}

	// GDI+�� ����� ����Ѵٸ� �̷��Ե� ����
	/*Rect rct(mX, mY, mFront->GetWidth(), mFront->GetHeight());
	if (rct.Contains(x, y)) {
		Flip(!mIsFront);
		return true;
	}*/

	return false;
}

void Memory::Card::Flip(bool isFront) {
	// ���� ���� �� ȭ�� ������Ʈ�� ��û
	mIsFront = isFront;
	Invalidate();
}

void Memory::Card::Draw(Gdiplus::Graphics& graphics) {
	// mIsFront ���� ���� �ո� �Ǵ� �޸� �̹����� �׸�
	if (mIsFront) {
		graphics.DrawImage(mFront.get(), mX, mY, mFront->GetWidth(), mFront->GetHeight());
	}
	else {
		graphics.DrawImage(mBack.get(), mX, mY, mFront->GetWidth(), mFront->GetHeight());
	}
}

void Memory::Card::Invalidate() {
	// ī�� �̹����� �ٽ� �׸����� ���� �����쿡 ��û
	// rct�� ������ ������ ��ȿȭ
	RECT rct{ mX, mY,
		static_cast<LONG>(mX + mFront->GetWidth()),
		static_cast<LONG>(mY + mFront->GetHeight()) };
	InvalidateRect(mHwnd, &rct, false);
}