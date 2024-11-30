#include "StorehouseLib.h"
#include <sstream>

using namespace NSStorehouseLib;

static std::vector<std::string> split(const std::string& s, char delim)
{
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;

    while (getline(ss, item, delim))
    {
        result.push_back(item);
    }

    return result;
}

void StorehouseLib::Init(IFont* font,
                         ISoundEffect* SE,
                         ISprite* sprCursor,
                         ISprite* sprBackground,
                         ISprite* sprPanel,
                         ISprite* sprPanelTop)
{
    m_font = font;
    m_SE = SE;
    m_sprCursor = sprCursor;
    m_sprBackground = sprBackground;
    m_sprPanel = sprPanel;
    m_sprPanelTop = sprPanelTop;
}

void NSStorehouseLib::StorehouseLib::SetInventoryList(const std::vector<std::string>& arg)
{
    m_leftList = arg;
}

void NSStorehouseLib::StorehouseLib::SetStorehouseList(const std::vector<std::string>& arg)
{
    m_rightList = arg;
}

std::string StorehouseLib::Up()
{
    if (m_eFocus == eFocus::LEFT)
    {
        if (m_leftSelect >= 1)
        {
            m_leftSelect--;
            m_SE->PlayMove();
        }
        // �J�[�\������ԏ�ɂ���Ƃ��ɏ�{�^���������ꂽ��
        // �J�[�\���͂��̂܂܂Ń��X�g�����Ɉړ�����
        if (m_leftCursor != 0)
        {
            m_leftCursor--;
        }
        else if (m_leftCursor == 0)
        {
            if (m_leftSelect != 0)
            {
                m_leftBegin--;
            }
        }
    }
    else if (m_eFocus == eFocus::RIGHT)
    {
        if (m_rightSelect >= 1)
        {
            m_rightSelect--;
            m_SE->PlayMove();
        }
        // �J�[�\������ԏ�ɂ���Ƃ��ɏ�{�^���������ꂽ��
        // �J�[�\���͂��̂܂܂Ń��X�g�����Ɉړ�����
        if (m_rightCursor != 0)
        {
            m_rightCursor--;
        }
        else if (m_rightCursor == 0)
        {
            if (m_rightSelect != 0)
            {
                m_rightBegin--;
            }
        }
    }
    return "";
}

std::string StorehouseLib::Down()
{
    if (m_eFocus == eFocus::LEFT)
    {
        if (m_leftSelect <= (int)m_leftList.size() - 2)
        {
            m_leftSelect++;
            m_SE->PlayMove();
        }

        // �J�[�\������ԉ��ɂ���Ƃ��ɉ��{�^���������ꂽ��
        // �J�[�\���͂��̂܂܂Ń��X�g����Ɉړ�����
        if (m_leftCursor != PANEL_ROW_MAX - 1)
        {
            m_leftCursor++;
        }
        else if (m_leftCursor == PANEL_ROW_MAX - 1)
        {
            if (m_leftSelect != (int)m_leftList.size() - 1)
            {
                m_leftBegin++;
            }
        }
    }
    else if (m_eFocus == eFocus::RIGHT)
    {
        if (m_rightSelect <= (int)m_rightList.size() - 2)
        {
            m_rightSelect++;
            m_SE->PlayMove();
        }

        // �J�[�\������ԉ��ɂ���Ƃ��ɉ��{�^���������ꂽ��
        // �J�[�\���͂��̂܂܂Ń��X�g����Ɉړ�����
        if (m_rightCursor != PANEL_ROW_MAX - 1)
        {
            m_rightCursor++;
        }
        else if (m_rightCursor == PANEL_ROW_MAX - 1)
        {
            if (m_rightSelect != (int)m_rightList.size() - 1)
            {
                m_rightBegin++;
            }
        }
    }
    return "";
}

std::string StorehouseLib::Right()
{
    if (m_eFocus == eFocus::LEFT)
    {
        m_eFocus = eFocus::RIGHT;
        m_SE->PlayMove();
    }
    return std::string();
}

std::string StorehouseLib::Left()
{
    if (m_eFocus == eFocus::RIGHT)
    {
        m_eFocus = eFocus::LEFT;
        m_SE->PlayMove();
    }
    return std::string();
}

std::string StorehouseLib::Into()
{
    std::string result;
    if (m_eFocus == eFocus::LEFT)
    {
        result = m_leftList.at(m_leftSelect);
        m_SE->PlayClick();
    }
    else if (m_eFocus == eFocus::RIGHT)
    {
        result = m_rightList.at(m_rightSelect);
        m_SE->PlayClick();
    }
    return result;
}

std::string StorehouseLib::Back()
{
    std::string result;
    result = "EXIT";
    m_SE->PlayBack();

    return result;
}

std::string NSStorehouseLib::StorehouseLib::Next()
{
    if (m_eFocus == eFocus::LEFT)
    {
        if (m_leftSelect <= (int)m_leftList.size() - 2)
        {
            m_leftSelect++;
            m_SE->PlayMove();
        }

        // �J�[�\������ԉ��ɂ���Ƃ��ɉ��{�^���������ꂽ��
        // �J�[�\���͂��̂܂܂Ń��X�g����Ɉړ�����
        if (m_leftCursor != PANEL_ROW_MAX - 1)
        {
            m_leftCursor++;
        }
        else if (m_leftCursor == PANEL_ROW_MAX - 1)
        {
            if (m_leftSelect != (int)m_leftList.size() - 1)
            {
                m_leftBegin++;
            }
        }
    }
    else if (m_eFocus == eFocus::RIGHT)
    {
        if (m_rightSelect <= (int)m_rightList.size() - 2)
        {
            m_rightSelect++;
            m_SE->PlayMove();
        }

        // �J�[�\������ԉ��ɂ���Ƃ��ɉ��{�^���������ꂽ��
        // �J�[�\���͂��̂܂܂Ń��X�g����Ɉړ�����
        if (m_rightCursor != PANEL_ROW_MAX - 1)
        {
            m_rightCursor++;
        }
        else if (m_rightCursor == PANEL_ROW_MAX - 1)
        {
            if (m_rightSelect != (int)m_rightList.size() - 1)
            {
                m_rightBegin++;
            }
        }
    }
    return "";
}

std::string NSStorehouseLib::StorehouseLib::Previous()
{
    if (m_eFocus == eFocus::LEFT)
    {
        if (m_leftSelect >= 1)
        {
            m_leftSelect--;
            m_SE->PlayMove();
        }
        // �J�[�\������ԏ�ɂ���Ƃ��ɏ�{�^���������ꂽ��
        // �J�[�\���͂��̂܂܂Ń��X�g�����Ɉړ�����
        if (m_leftCursor != 0)
        {
            m_leftCursor--;
        }
        else if (m_leftCursor == 0)
        {
            if (m_leftSelect != 0)
            {
                m_leftBegin--;
            }
        }
    }
    else if (m_eFocus == eFocus::RIGHT)
    {
        if (m_rightSelect >= 1)
        {
            m_rightSelect--;
            m_SE->PlayMove();
        }
        // �J�[�\������ԏ�ɂ���Ƃ��ɏ�{�^���������ꂽ��
        // �J�[�\���͂��̂܂܂Ń��X�g�����Ɉړ�����
        if (m_rightCursor != 0)
        {
            m_rightCursor--;
        }
        else if (m_rightCursor == 0)
        {
            if (m_rightSelect != 0)
            {
                m_rightBegin--;
            }
        }
    }
    return "";
}

void StorehouseLib::CursorOn(const int x, const int y)
{
    int previousCursor = m_leftCursor;
    if (LEFT_PANEL_STARTX < x && x <= LEFT_PANEL_STARTX + PANEL_WIDTH)
    {
        if (LEFT_PANEL_STARTY < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 1)
        {
            m_eFocus = eFocus::LEFT;
            m_leftCursor = 0;
            m_leftSelect = 0 + m_leftBegin;
        }
        else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 1 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 2)
        {
            m_eFocus = eFocus::LEFT;
            m_leftCursor = 1;
            m_leftSelect = 1 + m_leftBegin;
        }
        else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 2 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 3)
        {
            m_eFocus = eFocus::LEFT;
            m_leftCursor = 2;
            m_leftSelect = 2 + m_leftBegin;
        }
        else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 3 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 4)
        {
            m_eFocus = eFocus::LEFT;
            m_leftCursor = 3;
            m_leftSelect = 3 + m_leftBegin;
        }
        else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 4 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 5)
        {
            m_eFocus = eFocus::LEFT;
            m_leftCursor = 4;
            m_leftSelect = 4 + m_leftBegin;
        }
        else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 5 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 6)
        {
            m_eFocus = eFocus::LEFT;
            m_leftCursor = 5;
            m_leftSelect = 5 + m_leftBegin;
        }
        else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 6 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 7)
        {
            m_eFocus = eFocus::LEFT;
            m_leftCursor = 6;
            m_leftSelect = 6 + m_leftBegin;
        }
        else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 7 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 8)
        {
            m_eFocus = eFocus::LEFT;
            m_leftCursor = 7;
            m_leftSelect = 7 + m_leftBegin;
        }
        else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 8 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 9)
        {
            m_eFocus = eFocus::LEFT;
            m_leftCursor = 8;
            m_leftSelect = 8 + m_leftBegin;
        }
        else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 9 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 10)
        {
            m_eFocus = eFocus::LEFT;
            m_leftCursor = 9;
            m_leftSelect = 9 + m_leftBegin;
        }
    }
    if (previousCursor != m_leftCursor)
    {
        m_SE->PlayMove();
    }

    int previousRightCursor = m_rightCursor;
    if (RIGHT_PANEL_STARTX < x && x <= RIGHT_PANEL_STARTX + PANEL_WIDTH)
    {
        if (RIGHT_PANEL_STARTY < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 1)
        {
            m_eFocus = eFocus::RIGHT;
            m_rightCursor = 0;
            m_rightSelect = 0 + m_rightBegin;
        }
        else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 1 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 2)
        {
            m_eFocus = eFocus::RIGHT;
            m_rightCursor = 1;
            m_rightSelect = 1 + m_rightBegin;
        }
        else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 2 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 3)
        {
            m_eFocus = eFocus::RIGHT;
            m_rightCursor = 2;
            m_rightSelect = 2 + m_rightBegin;
        }
        else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 3 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 4)
        {
            m_eFocus = eFocus::RIGHT;
            m_rightCursor = 3;
            m_rightSelect = 3 + m_rightBegin;
        }
        else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 4 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 5)
        {
            m_eFocus = eFocus::RIGHT;
            m_rightCursor = 4;
            m_rightSelect = 4 + m_rightBegin;
        }
        else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 5 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 6)
        {
            m_eFocus = eFocus::RIGHT;
            m_rightCursor = 5;
            m_rightSelect = 5 + m_rightBegin;
        }
        else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 6 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 7)
        {
            m_eFocus = eFocus::RIGHT;
            m_rightCursor = 6;
            m_rightSelect = 6 + m_rightBegin;
        }
        else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 7 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 8)
        {
            m_eFocus = eFocus::RIGHT;
            m_rightCursor = 7;
            m_rightSelect = 7 + m_rightBegin;
        }
        else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 8 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 9)
        {
            m_eFocus = eFocus::RIGHT;
            m_rightCursor = 8;
            m_rightSelect = 8 + m_rightBegin;
        }
        else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 9 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 10)
        {
            m_eFocus = eFocus::RIGHT;
            m_rightCursor = 9;
            m_rightSelect = 9 + m_rightBegin;
        }
    }
    if (previousRightCursor != m_rightCursor)
    {
        m_SE->PlayMove();
    }
}

std::string StorehouseLib::Click(const int x, const int y)
{
    std::string result;
    m_SE->PlayClick();
    if (m_eFocus == eFocus::LEFT)
    {
        if (LEFT_PANEL_STARTX < x && x <= LEFT_PANEL_STARTX + PANEL_WIDTH)
        {
            if (LEFT_PANEL_STARTY < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 1)
            {
                m_leftCursor = 0;
                m_leftSelect = 0 + m_leftBegin;
                result = Into();
            }
            else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 1 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 2)
            {
                m_leftCursor = 1;
                m_leftSelect = 1 + m_leftBegin;
                result = Into();
            }
            else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 2 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 3)
            {
                m_leftCursor = 2;
                m_leftSelect = 2 + m_leftBegin;
                result = Into();
            }
            else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 3 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 4)
            {
                m_leftCursor = 3;
                m_leftSelect = 3 + m_leftBegin;
                result = Into();
            }
            else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 4 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 5)
            {
                m_leftCursor = 4;
                m_leftSelect = 4 + m_leftBegin;
                result = Into();
            }
            else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 5 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 6)
            {
                m_leftCursor = 5;
                m_leftSelect = 5 + m_leftBegin;
                result = Into();
            }
            else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 6 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 7)
            {
                m_leftCursor = 6;
                m_leftSelect = 6 + m_leftBegin;
                result = Into();
            }
            else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 7 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 8)
            {
                m_leftCursor = 7;
                m_leftSelect = 7 + m_leftBegin;
                result = Into();
            }
            else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 8 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 9)
            {
                m_leftCursor = 8;
                m_leftSelect = 8 + m_leftBegin;
                result = Into();
            }
            else if (LEFT_PANEL_STARTY + PANEL_HEIGHT * 9 < y && y <= LEFT_PANEL_STARTY + PANEL_HEIGHT * 10)
            {
                m_leftCursor = 9;
                m_leftSelect = 9 + m_leftBegin;
                result = Into();
            }
        }
    }
    else if (m_eFocus == eFocus::RIGHT)
    {
        if (RIGHT_PANEL_STARTX < x && x <= RIGHT_PANEL_STARTX + PANEL_WIDTH)
        {
            if (RIGHT_PANEL_STARTY < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 1)
            {
                m_rightCursor = 0;
                m_rightSelect = 0 + m_rightBegin;
                result = Into();
            }
            else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 1 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 2)
            {
                m_rightCursor = 1;
                m_rightSelect = 1 + m_rightBegin;
                result = Into();
            }
            else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 2 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 3)
            {
                m_rightCursor = 2;
                m_rightSelect = 2 + m_rightBegin;
                result = Into();
            }
            else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 3 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 4)
            {
                m_rightCursor = 3;
                m_rightSelect = 3 + m_rightBegin;
                result = Into();
            }
            else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 4 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 5)
            {
                m_rightCursor = 4;
                m_rightSelect = 4 + m_rightBegin;
                result = Into();
            }
            else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 5 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 6)
            {
                m_rightCursor = 5;
                m_rightSelect = 5 + m_rightBegin;
                result = Into();
            }
            else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 6 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 7)
            {
                m_rightCursor = 6;
                m_rightSelect = 6 + m_rightBegin;
                result = Into();
            }
            else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 7 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 8)
            {
                m_rightCursor = 7;
                m_rightSelect = 7 + m_rightBegin;
                result = Into();
            }
            else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 8 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 9)
            {
                m_rightCursor = 8;
                m_rightSelect = 8 + m_rightBegin;
                result = Into();
            }
            else if (RIGHT_PANEL_STARTY + PANEL_HEIGHT * 9 < y && y <= RIGHT_PANEL_STARTY + PANEL_HEIGHT * 10)
            {
                m_rightCursor = 9;
                m_rightSelect = 9 + m_rightBegin;
                result = Into();
            }
        }
    }
    return result;
}

void StorehouseLib::Draw()
{
    // �w�i
    m_sprBackground->DrawImage(0, 0);

    // �㕔���̍��Ɂu�C���x���g���v�A�E���Ɂu�q�Ɂv�ƕ\������
    m_sprPanelTop->DrawImage(150, 100);
    m_font->DrawText_("�C���x���g��", 205, 110);

    m_sprPanelTop->DrawImage(750, 100);
    m_font->DrawText_("�q��", 855, 110);

    m_font->DrawText_("���E�Ő؂�ւ��A�㉺�ŃA�C�e����I��", 1085, 85);
    m_font->DrawText_("����{�^���ŃA�C�e���𔽑Α��Ɉړ�", 1085, 135);

    // ���̗�̔w�i
    if ((int)m_leftList.size() >= PANEL_ROW_MAX)
    {
        for (int i = 0; i < PANEL_ROW_MAX; ++i) {
            m_sprPanel->DrawImage(LEFT_PANEL_STARTX, LEFT_PANEL_STARTY + (PANEL_HEIGHT * (int)i));
        }
    }
    else
    {
        for (std::size_t i = 0; i < m_leftList.size(); ++i) {
            m_sprPanel->DrawImage(LEFT_PANEL_STARTX, LEFT_PANEL_STARTY + (PANEL_HEIGHT * (int)i));
        }
    }

    // �E�̗�̔w�i
    if ((int)m_rightList.size() >= PANEL_ROW_MAX)
    {
        for (int i = 0; i < PANEL_ROW_MAX; ++i) {
            m_sprPanel->DrawImage(RIGHT_PANEL_STARTX, RIGHT_PANEL_STARTY + (PANEL_HEIGHT * (int)i));
        }
    }
    else
    {
        for (std::size_t i = 0; i < m_rightList.size(); ++i) {
            m_sprPanel->DrawImage(RIGHT_PANEL_STARTX, RIGHT_PANEL_STARTY + (PANEL_HEIGHT * (int)i));
        }
    }

    // ���̗�̃C���x���g����\��
    if ((int)m_leftList.size() >= PANEL_ROW_MAX)
    {
        for (int i = m_leftBegin; i < m_leftBegin + PANEL_ROW_MAX; ++i)
        {
            m_font->DrawText_(m_leftList.at(i),
                              LEFT_PANEL_STARTX + PANEL_PADDINGX,
                              LEFT_PANEL_STARTY + PANEL_PADDINGY + ((i - m_leftBegin) * PANEL_HEIGHT));
        }
    }
    else
    {
        for (std::size_t i = 0; i < m_leftList.size(); ++i)
        {
            m_font->DrawText_(m_leftList.at(i),
                              LEFT_PANEL_STARTX + PANEL_PADDINGX,
                              LEFT_PANEL_STARTY + PANEL_PADDINGY + ((int)i * PANEL_HEIGHT));
        }
    }

    // �E�̗�̑q�ɂ�\��
    if ((int)m_rightList.size() >= PANEL_ROW_MAX)
    {
        for (int i = m_rightBegin; i < m_rightBegin + PANEL_ROW_MAX; ++i)
        {
            m_font->DrawText_(m_rightList.at(i),
                              RIGHT_PANEL_STARTX + PANEL_PADDINGX,
                              RIGHT_PANEL_STARTY + PANEL_PADDINGY + ((i - m_rightBegin) * PANEL_HEIGHT));
        }
    }
    else
    {
        for (std::size_t i = 0; i < m_rightList.size(); ++i)
        {
            m_font->DrawText_(m_rightList.at(i),
                              RIGHT_PANEL_STARTX + PANEL_PADDINGX,
                              RIGHT_PANEL_STARTY + PANEL_PADDINGY + ((int)i * PANEL_HEIGHT));
        }
    }

    // �J�[�\���̕\��
    if (m_eFocus == eFocus::LEFT)
    {
        m_sprCursor->DrawImage(80, 205 + (m_leftCursor * 60));
    }
    else if (m_eFocus == eFocus::RIGHT)
    {
        m_sprCursor->DrawImage(680, 205 + (m_rightCursor * 60));
    }


}
