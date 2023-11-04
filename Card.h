#pragma once

#include <windows.h>
#include <gdiplus.h>
#include <memory>

namespace Memory {
    // 카드의 종류 정의
    enum class Type {
        Seven,
        King,
        Joker,
        Ace
    };

    class Card {
    private:
        // 앞면과 뒷면 이미지를 가리킬 포인터
        std::unique_ptr<Gdiplus::Image> mBack;
        std::unique_ptr<Gdiplus::Image> mFront;

        int mX;
        int mY;
        bool mIsFront;
        Type mType;
        HWND mHwnd;
        int mIndex;

    public:
        Card(HWND hwnd, int index, Type type, int x, int y);

        // 지정된 좌표에서 클릭을 확인
        bool CheckClicked(int x, int y);

        // 카드 뒤집기
        void Flip(bool isFront);

        // 카드 이미지 그리기
        void Draw(Gdiplus::Graphics& graphics);

        // 카드 이미지를 다시 그리도록 직접 윈도우에 요청
        void Invalidate();

        Type GetType() const { return mType; }
        int GetIndex() const { return mIndex; }
    };
}