#pragma once

#include "Card.h"
#include <list>
#include <string>

namespace Memory {
	class GameLogic {
	private:
		// ī���� ��� �� ����
		const int BOARD_ROW = 5;
		const int BOARD_COL = 8;

		// Ŭ������ ǥ���ϱ� ���� ���簢�� ��ǥ ����
		const Gdiplus::RectF mCountRect1{ 945.0f, 25.0f, 120.0f, 50.0f };
		const Gdiplus::RectF mCountRect2{ 945.0f, 75.0f, 120.0f, 50.0f };

		// ���ڸ� ǥ���ϱ� ���� ���簢�� ��ǥ ����
		const Gdiplus::RectF mWinnerRect{ 250.0f, 220.0f, 500.0f, 300.0f };

		// ��� �̹����� �⸮ų ������
		std::unique_ptr<Gdiplus::Image> mBackground;

		// ī�� ����
		std::list<Card> mDeck;

		// ������ �ڵ�
		HWND mHwnd;

		// �񱳸� ���� ������ Ŭ���� ī�带 ����ų ������
		Card* mpPrevious;

		// 2�ο� �������� �����ϱ� ���� ����� ������
		int mPlayerTurn;
		int mPlayer1Score;
		int mPlayer2Score;

	private:
		// ī�� ���̸� ����
		void CreateCards();

		// �÷��̾� �� ��ü
		void SwitchPlayerTurn();

	public:
		// �����ڿ� �Ҹ��� ��� �Ʒ� �Լ��� ����
		// ��ü�� ������ ������ ���� ���� ����
		void Init(HWND hwnd);
		void Release();

		// ���, ī�� ����, Ŭ������ �׸�
		void Draw(Gdiplus::Graphics& graphics);

		// � ī�尡 Ŭ���Ǿ����� �Ǵ�
		void OnClick(int x, int y);
	};
}