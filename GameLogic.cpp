#include "GameLogic.h"
#include <string>
#include <vector>
#include <random>

using namespace Gdiplus;

void Memory::GameLogic::CreateCards() {
	// 카드 종류를 저장할 벡터
	std::vector<Type> types;

	while (types.size() < static_cast<size_t>(BOARD_COL * BOARD_ROW)) {
		int mod = types.size() % 8;

		// 카드 종류를 선택하고 짝이 맞도록 각 카드는 두 개씩 추가
		switch (mod) {
		case 0:
			types.push_back(Type::Ace);
			types.push_back(Type::Ace);
			break;
		case 2:
			types.push_back(Type::Joker);
			types.push_back(Type::Joker);
			break;
		case 4:
			types.push_back(Type::Seven);
			types.push_back(Type::Seven);
			break;
		case 6:
			types.push_back(Type::King);
			types.push_back(Type::King);
			break;
		}
	}

	// 균등한 분포를 가진 mt19937 랜덤
	// 카드 종류를 무작위로 섞기
	std::random_device rd;
	std::mt19937 gen(rd());
	std::shuffle(types.begin(), types.end(), gen);

	int index{};
	int posX{ 15 }, posY{ 15 };

	// 게임 보드에 카드를 배치
	for (int x = 0; x < BOARD_COL; ++x) {
		posY = 15;
		for (int y = 0; y < BOARD_ROW; ++y) {
			mDeck.push_back(Card(mHwnd, index, types[index++], posX, posY));
			posY += 150;
		}

		posX += 110;
	}
}

void Memory::GameLogic::Init(HWND hwnd) {
	mHwnd = hwnd;
	mBackground = std::make_unique<Image>(L"Images/Board.jpg");
	CreateCards();
}

void Memory::GameLogic::Release() {
	mDeck.clear();
	mBackground.reset();
}

void Memory::GameLogic::Draw(Gdiplus::Graphics& graphics) {
	// 배경 이미지
	graphics.DrawImage(mBackground.get(), 0, 0, 1024, 810);

	// 카드 더미 이미지
	for (auto& card : mDeck) {
		card.Draw(graphics);
	}

	// "클릭수:" 텍스트
	Gdiplus::PointF pos(895.0f, 25.0f);
	Gdiplus::SolidBrush brush(Gdiplus::Color(0, 0, 0));
	Gdiplus::Font font(L"상주다정다감체", 30);
	graphics.DrawString(L"클릭수:", -1, &font, pos, &brush);

	// 실제 클릭수
	Gdiplus::StringFormat format;
	format.SetAlignment(StringAlignmentCenter);
	format.SetLineAlignment(StringAlignmentCenter);
	graphics.DrawString(std::to_wstring(mClickCount).c_str(), -1, &font, mCountRect, &format, &brush);
}

void Memory::GameLogic::OnClick(int x, int y) {

}