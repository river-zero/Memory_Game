#include "GameLogic.h"
#include <string>
#include <vector>
#include <random>

using namespace Gdiplus;

void Memory::GameLogic::CreateCards() {
	// ī�� ������ ������ ����
	std::vector<Type> types;

	// ��� ī�尡 ������ ������ ���� ����
	while (types.size() < static_cast<size_t>(BOARD_COL * BOARD_ROW)) {
		// �յ��ϰ� ī�� ������ ���� ����
		int mod = types.size() % 8;

		// ī�� ������ �����ϰ� ¦�� �µ��� �� ī��� �� ���� �߰�
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

	// �յ��� ������ ���� mt19937 ����
	// �������� ī�� ������ ����
	std::random_device rd;
	std::mt19937 gen(rd());
	std::shuffle(types.begin(), types.end(), gen);

	int index{};
	int posX{ 15 }, posY{ 15 };

	// ī�带 �����ϰ� ���� ���忡 ��ġ
	for (int x = 0; x < BOARD_COL; x++) {
		posY = 15;
		for (int y = 0; y < BOARD_ROW; y++) {
			mDeck.push_back(Card(mHwnd, index, types[index++], posX, posY));
			posY += 140 + 10; // ī�� ���� ������ 10
		}

		posX += 100 + 10;
	}
}

void Memory::GameLogic::SwitchPlayerTurn() {
	mPlayerTurn = (mPlayerTurn == 1) ? 2 : 1;
}

// ������ �ʱ�ȭ�ϰ� ��� �̹��� �ε�
void Memory::GameLogic::Init(HWND hwnd) {
	mHwnd = hwnd;
	mBackground = std::make_unique<Image>(L"Images/Board.jpg");
	CreateCards();

	mPlayerTurn = 1;
	mPlayer1Score = 0;
	mPlayer2Score = 0;
}

// ���� �ڿ� ����
void Memory::GameLogic::Release() {
	mDeck.clear();
	mBackground.reset();
}

void Memory::GameLogic::Draw(Gdiplus::Graphics& graphics) {
	// ��� �̹���
	graphics.DrawImage(mBackground.get(), 0, 0, mBackground->GetWidth(), mBackground->GetHeight());

	// ī�� ���� �̹���
	for (auto& card : mDeck) {
		card.Draw(graphics);
	}

	// �÷��̾�1 Ŭ���� ǥ��
	Gdiplus::PointF pos1(895.0f, 25.0f);
	Gdiplus::SolidBrush brush(Gdiplus::Color(0, 0, 0));
	Gdiplus::Font font(L"�������", 25);
	graphics.DrawString(L"P1", -1, &font, pos1, &brush);
	Gdiplus::StringFormat format;
	graphics.DrawString(std::to_wstring(mPlayer1Score).c_str(), -1, &font, mCountRect1, &format, &brush);

	// �÷��̾�2 Ŭ���� ǥ��
	Gdiplus::PointF pos2(895.0f, 75.0f);
	graphics.DrawString(L"P2", -1, &font, pos2, &brush);
	graphics.DrawString(std::to_wstring(mPlayer2Score).c_str(), -1, &font, mCountRect2, &format, &brush);

	// ��� ī�尡 ������� ���� ��� Ȯ��
	if (mDeck.empty()) {
		std::wstring winnerMessage;
		if (mPlayer1Score > mPlayer2Score) {
			winnerMessage = L"���ڴ� P1�Դϴ�!";
		} else if (mPlayer1Score < mPlayer2Score) {
			winnerMessage = L"���ڴ� P2�Դϴ�!";
		} else {
			winnerMessage = L"�����ϴ�!";
		}

		Gdiplus::Font winnerFont(L"�������", 30, Gdiplus::FontStyleBold);
		format.SetAlignment(StringAlignmentCenter);
		format.SetLineAlignment(StringAlignmentCenter);
		graphics.DrawString(winnerMessage.c_str(), -1, &winnerFont, mWinnerRect, &format, &brush);
	}
}

void Memory::GameLogic::OnClick(int x, int y) {
	Card* pCard{};

	// Ŭ���� ī�尡 �ִ��� Ȯ��
	for (auto& card : mDeck) {
		if (card.CheckClicked(x, y)) {
			pCard = &card;
			break;
		}
	}

	if (pCard) {
		// ���� ������Ʈ
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
		// Ŭ���� ���� ī�尡 ���ٸ� ���� ī�带 ����
		mpPrevious = pCard;
	}
	else {
		// ���� ī�带 ������ ���� �ƴ϶��
		if (pCard != nullptr && mpPrevious != nullptr && mpPrevious != pCard) {
			// ¦�� �´� ���
			if (pCard->GetType() == mpPrevious->GetType()) {
				mpPrevious->Invalidate();
				pCard->Invalidate();

				// �ش� �÷��̾��� ���� ����
				if (mPlayerTurn == 1) {
					mPlayer1Score++;
				} else {
					mPlayer2Score++;
				}

				mDeck.remove_if([&](Card& card) {return card.GetIndex() == pCard->GetIndex(); });
				mDeck.remove_if([&](Card& card) {return card.GetIndex() == mpPrevious->GetIndex(); });

				if (mDeck.empty()) {
					// ȭ�� ���߾ӿ� �ش� �޽��� ǥ��
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

				// ¦ ���߱� ���� �� �÷��̾� �� ��ü
				SwitchPlayerTurn();
			}
		}

		mpPrevious = nullptr;
	}
}