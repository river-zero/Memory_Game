#include "GameLogic.h"
#include <string>
#include <vector>
#include <random>

using namespace Gdiplus;

void Memory::GameLogic::CreateCards() {
	// 카드 종류를 저장할 벡터
	std::vector<Type> types;

	// 모든 카드가 생성될 때까지 루프 실행
	while (types.size() < static_cast<size_t>(BOARD_COL * BOARD_ROW)) {
		// 균등하게 카드 종류를 섞기 위함
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
	// 무작위로 카드 순서를 섞기
	std::random_device rd;
	std::mt19937 gen(rd());
	std::shuffle(types.begin(), types.end(), gen);

	int index{};
	int posX{ 15 }, posY{ 15 };

	// 카드를 생성하고 게임 보드에 배치
	for (int x = 0; x < BOARD_COL; x++) {
		posY = 15;
		for (int y = 0; y < BOARD_ROW; y++) {
			mDeck.push_back(Card(mHwnd, index, types[index++], posX, posY));
			posY += 140 + 10; // 카드 사이 간격은 10
		}

		posX += 100 + 10;
	}
}

void Memory::GameLogic::SwitchPlayerTurn() {
	mPlayerTurn = (mPlayerTurn == 1) ? 2 : 1;
}

// 게임을 초기화하고 배경 이미지 로드
void Memory::GameLogic::Init(HWND hwnd) {
	mHwnd = hwnd;
	mBackground = std::make_unique<Image>(L"Images/Board.jpg");
	CreateCards();

	mPlayerTurn = 1;
	mPlayer1Score = 0;
	mPlayer2Score = 0;
}

// 게임 자원 해제
void Memory::GameLogic::Release() {
	mDeck.clear();
	mBackground.reset();
}

void Memory::GameLogic::Draw(Gdiplus::Graphics& graphics) {
	// 배경 이미지
	graphics.DrawImage(mBackground.get(), 0, 0, mBackground->GetWidth(), mBackground->GetHeight());

	// 카드 더미 이미지
	for (auto& card : mDeck) {
		card.Draw(graphics);
	}

	// 플레이어1 클릭수 표시
	Gdiplus::PointF pos1(895.0f, 25.0f);
	Gdiplus::SolidBrush brush(Gdiplus::Color(0, 0, 0));
	Gdiplus::Font font(L"맑은고딕", 25);
	graphics.DrawString(L"P1", -1, &font, pos1, &brush);
	Gdiplus::StringFormat format;
	graphics.DrawString(std::to_wstring(mPlayer1Score).c_str(), -1, &font, mCountRect1, &format, &brush);

	// 플레이어2 클릭수 표시
	Gdiplus::PointF pos2(895.0f, 75.0f);
	graphics.DrawString(L"P2", -1, &font, pos2, &brush);
	graphics.DrawString(std::to_wstring(mPlayer2Score).c_str(), -1, &font, mCountRect2, &format, &brush);

	// 모든 카드가 사라지면 게임 결과 확인
	if (mDeck.empty()) {
		std::wstring winnerMessage;
		if (mPlayer1Score > mPlayer2Score) {
			winnerMessage = L"승자는 P1입니다!";
		} else if (mPlayer1Score < mPlayer2Score) {
			winnerMessage = L"승자는 P2입니다!";
		} else {
			winnerMessage = L"비겼습니다!";
		}

		Gdiplus::Font winnerFont(L"맑은고딕", 30, Gdiplus::FontStyleBold);
		format.SetAlignment(StringAlignmentCenter);
		format.SetLineAlignment(StringAlignmentCenter);
		graphics.DrawString(winnerMessage.c_str(), -1, &winnerFont, mWinnerRect, &format, &brush);
	}
}

void Memory::GameLogic::OnClick(int x, int y) {
	Card* pCard{};

	// 클릭한 카드가 있는지 확인
	for (auto& card : mDeck) {
		if (card.CheckClicked(x, y)) {
			pCard = &card;
			break;
		}
	}

	if (pCard) {
		// 점수 업데이트
		RECT rct;
		if (mPlayerTurn == 1) {
			rct = {
				static_cast<LONG>(mCountRect1.GetLeft()),
				static_cast<LONG>(mCountRect1.GetTop()),
				static_cast<LONG>(mCountRect1.GetRight()),
				static_cast<LONG>(mCountRect1.GetBottom())
			};
		} else {
			rct = {
				static_cast<LONG>(mCountRect2.GetLeft()),
				static_cast<LONG>(mCountRect2.GetTop()),
				static_cast<LONG>(mCountRect2.GetRight()),
				static_cast<LONG>(mCountRect2.GetBottom())
			};
		}
		InvalidateRect(mHwnd, &rct, true);
	}

	if (mpPrevious == nullptr) {
		// 클릭한 이전 카드가 없다면 지금 카드를 선택
		mpPrevious = pCard;
	}
	else {
		// 같은 카드를 선택한 것이 아니라면
		if (pCard != nullptr && mpPrevious != nullptr && mpPrevious != pCard) {
			// 짝이 맞는 경우
			if (pCard->GetType() == mpPrevious->GetType()) {
				mpPrevious->Invalidate();
				pCard->Invalidate();

				// 해당 플레이어의 점수 증가
				if (mPlayerTurn == 1) {
					mPlayer1Score++;
				} else {
					mPlayer2Score++;
				}

				mDeck.remove_if([&](Card& card) {return card.GetIndex() == pCard->GetIndex(); });
				mDeck.remove_if([&](Card& card) {return card.GetIndex() == mpPrevious->GetIndex(); });

				if (mDeck.empty()) {
					// 화면 정중앙에 해당 메시지 표시
					RECT rct;
					rct = {
						static_cast<LONG>(mWinnerRect.GetLeft()),
						static_cast<LONG>(mWinnerRect.GetTop()),
						static_cast<LONG>(mWinnerRect.GetRight()),
						static_cast<LONG>(mWinnerRect.GetBottom())
					};
					InvalidateRect(mHwnd, &rct, true);
				}
			}
			else {
				UpdateWindow(mHwnd);
				Sleep(300);
				pCard->Flip(false);
				mpPrevious->Flip(false);

				// 짝 맞추기 실패 시 플레이어 턴 교체
				SwitchPlayerTurn();
			}
		}

		mpPrevious = nullptr;
	}
}