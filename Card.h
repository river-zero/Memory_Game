#pragma once

#include <windows.h>
#include <gdiplus.h>
#include <memory>

namespace Memory {
    // ī���� ���� ����
    enum class Type {
        Seven,
        King,
        Joker,
        Ace
    };

    class Card {
    private:
        // �ո�� �޸� �̹����� ����ų ������
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

        // ������ ��ǥ���� Ŭ���� Ȯ��
        bool CheckClicked(int x, int y);

        // ī�� ������
        void Flip(bool isFront);

        // ī�� �̹��� �׸���
        void Draw(Gdiplus::Graphics& graphics);

        // ī�� �̹����� �ٽ� �׸����� ���� �����쿡 ��û
        void Invalidate();

        Type GetType() const { return mType; }
        int GetIndex() const { return mIndex; }
    };
}