#pragma once

#include "Card.h"
#include <list>
#include <string>

namespace Memory {
	class GameLogic {
	private:
		// 카드의 행과 열 정의
		const int BOARD_ROW = 5;
		const int BOARD_COL = 8;

		// 클릭수를 표현하기 위한 직사각형 좌표 영역
		const Gdiplus::RectF mCountRect1{ 945.0f, 25.0f, 120.0f, 50.0f };
		const Gdiplus::RectF mCountRect2{ 945.0f, 75.0f, 120.0f, 50.0f };

		// 승자를 표시하기 위한 직사각형 좌표 영역
		const Gdiplus::RectF mWinnerRect{ 250.0f, 220.0f, 500.0f, 300.0f };

		// 배경 이미지를 기리킬 포인터
		std::unique_ptr<Gdiplus::Image> mBackground;

		// 카드 더미
		std::list<Card> mDeck;

		// 윈도우 핸들
		HWND mHwnd;

		// 비교를 위해 이전에 클릭한 카드를 가리킬 포인터
		Card* mpPrevious;

		// 2인용 게임으로 구현하기 위해 사용할 변수들
		int mPlayerTurn;
		int mPlayer1Score;
		int mPlayer2Score;

	private:
		// 카드 더미를 생성
		void CreateCards();

		// 플레이어 턴 교체
		void SwitchPlayerTurn();

	public:
		// 생성자와 소멸자 대신 아래 함수를 제공
		// 객체의 생성과 해제를 직접 제어 가능
		void Init(HWND hwnd);
		void Release();

		// 배경, 카드 더미, 클릭수를 그림
		void Draw(Gdiplus::Graphics& graphics);

		// 어떤 카드가 클릭되었는지 판단
		void OnClick(int x, int y);
	};
}