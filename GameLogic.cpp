#include "GameLogic.h"
#include <string>
#include <vector>
#include <random>

using namespace Gdiplus;

void Memory::GameLogic::CreateCards() {
	/*std::vector<Type> types;
	while (types.size() < static_cast<size_t>(BOARD_COL * BOARD_ROW)) {
		int mod = types.size() % 6;

		switch (mod) {
		case 0:
			types.push_back(Type::Bear);
			types.push_back(Type::Bear);
			break;
		case 2:
			types.push_back(Type::Wolf);
			types.push_back(Type::Wolf);
			break;
		case 4:
			types.push_back(Type::Dragon);
			types.push_back(Type::Dragon);
			break;
		}
	}

	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(types.begin(), types.end(), g);

	int index{};
	int posX{ 15 }, posY{ 15 };
	for (int x = 0; x < BOARD_COL; ++x) {
		posY = 15;
		for (int y = 0; y < BOARD_ROW; ++y) {
			mDeck.push_back(Card(mHwnd, index, types[index++], posX, posY));
			posY += 150;
		}

		posX += 110;
	}*/
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
	graphics.DrawImage(mBackground.get(), 0, 0, 1024, 768);

	// 카드 더미 이미지
	for (auto& card : mDeck) {
		card.Draw(graphics);
	}

	// "클릭수:" 텍스트
	Gdiplus::PointF pos(895.0f, 20.0f);
	Gdiplus::SolidBrush brush(Gdiplus::Color(0, 0, 0));
	Gdiplus::Font font(L"상주다정다감체", 29);
	graphics.DrawString(L"클릭수:", -1, &font, pos, &brush);

	// 실제 클릭수
	Gdiplus::StringFormat format;
	format.SetAlignment(StringAlignmentCenter);
	format.SetLineAlignment(StringAlignmentCenter);
	graphics.DrawString(std::to_wstring(mClickCount).c_str(), -1, &font, mCountRect, &format, &brush);
}

void Memory::GameLogic::OnClick(int x, int y) {
	/*Card* pCard{};

	for (auto& card : mDeck) {
		if (card.CheckClicked(x, y)) {
			pCard = &card;
			break;
		}
	}

	if (pCard) {
		mClickCount++;
		RECT rct{ static_cast<LONG>(mCountRect.GetLeft()),
			static_cast<LONG>(mCountRect.GetTop()),
			static_cast<LONG>(mCountRect.GetRight()),
			static_cast<LONG>(mCountRect.GetBottom()) };
		InvalidateRect(mHwnd, &rct, true);

		if (mpPrevious == nullptr) {
			mpPrevious = pCard;
		} else {
			if (mpPrevious != pCard) {
				if (pCard->GetType() == mpPrevious->GetType()) {
					mpPrevious->Invalidate();
					pCard->Invalidate();

					mDeck.remove_if([&](Card& card) {return card.GetIndex() == pCard->GetIndex(); });
					mDeck.remove_if([&](Card& card) {return card.GetIndex() == mpPrevious->GetIndex(); });

				} else {
					UpdateWindow(mHwnd);
					Sleep(500); // ms
					pCard->Flip(false);
					mpPrevious->Flip(false);
				}
			}

			mpPrevious = nullptr;
		}
	}*/
}