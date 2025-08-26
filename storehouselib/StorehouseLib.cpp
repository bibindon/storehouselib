#include "StorehouseLib.h"
#include <sstream>
#include <algorithm>
#include <tchar.h>

using namespace NSStorehouseLib;

static std::vector<std::wstring> split(const std::wstring& s, wchar_t delim)
{
    std::vector<std::wstring> result;
    std::wstringstream ss(s);
    std::wstring item;

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
                         bool bEnglish)
{
    m_font = font;
    m_SE = SE;
    m_sprCursor = sprCursor;
    m_sprBackground = sprBackground;
    m_bEnglish = bEnglish;
}

void StorehouseLib::Finalize()
{
    delete m_sprBackground;
    delete m_sprCursor;
    delete m_SE;
    delete m_font;
}

void NSStorehouseLib::StorehouseLib::SetInventoryList(const std::vector<StoreItem>& arg)
{
    m_leftList = arg;
}

void NSStorehouseLib::StorehouseLib::SetStorehouseList(const std::vector<StoreItem>& arg)
{
    m_rightList = arg;
}

void NSStorehouseLib::StorehouseLib::MoveFromInventoryToStorehouse(const std::wstring& id, const int subid)
{
    auto it = std::find_if(m_leftList.begin(), m_leftList.end(),
                           [&](const StoreItem& x)
                           {
                               return x.GetId() == id && x.GetSubId() == subid;
                           });

    if (it == m_leftList.end())
    {
        throw std::exception();
    }

    StoreItem item = *it;
    m_leftList.erase(it);
    m_rightList.push_back(item);

    std::sort(m_rightList.begin(), m_rightList.end(),
              [&](const StoreItem& left, const StoreItem& right)
              {
                  if (left.GetId() < right.GetId())
                  {
                      return true;
                  }
                  else if (left.GetId() > right.GetId())
                  {
                      return false;
                  }
                  else
                  {
                      if (left.GetSubId() < right.GetSubId())
                      {
                          return true;
                      }
                      else
                      {
                          return false;
                      }
                  }
              });

    // スクロールしている状態でアイテムを反対側に移動し
    // さらに、そのとき一番下の要素が最後の要素だった場合
    // 下に1つスクロールする
    if (m_leftBegin != 0)
    {
        if (m_leftBegin + PANEL_ROW_MAX - 1 >= (int)m_leftList.size())
        {
            m_leftBegin--;
            m_leftSelect--;
        }
    }
}

void NSStorehouseLib::StorehouseLib::MoveFromStorehouseToInventory(const std::wstring& id, const int subid)
{
    auto it = std::find_if(m_rightList.begin(), m_rightList.end(),
                           [&](const StoreItem& x)
                           {
                               return x.GetId() == id && x.GetSubId() == subid;
                           });

    if (it == m_rightList.end())
    {
        throw std::exception();
    }

    StoreItem item = *it;
    m_rightList.erase(it);
    m_leftList.push_back(item);

    std::sort(m_leftList.begin(), m_leftList.end(),
              [&](const StoreItem& left, const StoreItem& right)
              {
                  if (left.GetId() < right.GetId())
                  {
                      return true;
                  }
                  else if (left.GetId() > right.GetId())
                  {
                      return false;
                  }
                  else
                  {
                      if (left.GetSubId() < right.GetSubId())
                      {
                          return true;
                      }
                      else
                      {
                          return false;
                      }
                  }
              });

    // スクロールしている状態でアイテムを反対側に移動し
    // さらに、そのとき一番下の要素が最後の要素だった場合
    // 下に1つスクロールする
    if (m_rightBegin != 0)
    {
        if (m_rightBegin + PANEL_ROW_MAX - 1 >= (int)m_rightList.size())
        {
            m_rightBegin--;
            m_rightSelect--;
        }
    }
}

std::wstring StorehouseLib::Up()
{
    int work = 0;

    if (m_eFocus == eFocus::LEFT)
    {
        work = m_leftSelect;

        if (m_leftSelect >= 1)
        {
            m_leftSelect--;
            m_SE->PlayMove();
        }
        // カーソルが一番上にあるときに上ボタンを押されたら
        // カーソルはそのままでリストが下に移動する
        if (m_leftCursor != 0)
        {
            m_leftCursor--;
        }
        else if (m_leftCursor == 0)
        {
            if (work != 0)
            {
                m_leftBegin--;
            }
        }
    }
    else if (m_eFocus == eFocus::RIGHT)
    {
        work = m_rightSelect;

        if (m_rightSelect >= 1)
        {
            m_rightSelect--;
            m_SE->PlayMove();
        }
        // カーソルが一番上にあるときに上ボタンを押されたら
        // カーソルはそのままでリストが下に移動する
        if (m_rightCursor != 0)
        {
            m_rightCursor--;
        }
        else if (m_rightCursor == 0)
        {
            if (work != 0)
            {
                m_rightBegin--;
            }
        }
    }
    return L"";
}

std::wstring StorehouseLib::Down()
{
    int work = 0;
    if (m_eFocus == eFocus::LEFT)
    {
        work = m_leftSelect;

        if (m_leftSelect <= (int)m_leftList.size() - 2)
        {
            m_leftSelect++;
            m_SE->PlayMove();

            // カーソルが一番下にあるときに下ボタンを押されたら
            // カーソルはそのままでリストが上に移動する
            if (m_leftCursor != PANEL_ROW_MAX - 1)
            {
                m_leftCursor++;
            }
            else if (m_leftCursor == PANEL_ROW_MAX - 1)
            {
                if (work <= (int)m_leftList.size() - 2)
                {
                    m_leftBegin++;
                }
            }
        }
    }
    else if (m_eFocus == eFocus::RIGHT)
    {
        work = m_rightSelect;

        if (m_rightSelect <= (int)m_rightList.size() - 2)
        {
            m_rightSelect++;
            m_SE->PlayMove();

            // カーソルが一番下にあるときに下ボタンを押されたら
            // カーソルはそのままでリストが上に移動する
            if (m_rightCursor != PANEL_ROW_MAX - 1)
            {
                m_rightCursor++;
            }
            else if (m_rightCursor == PANEL_ROW_MAX - 1)
            {
                if (work <= (int)m_rightList.size() - 2)
                {
                    m_rightBegin++;
                }
            }
        }
    }
    return L"";
}

std::wstring StorehouseLib::Right()
{
    if (!m_rightList.empty())
    {
        if (m_eFocus == eFocus::LEFT)
        {
            m_eFocus = eFocus::RIGHT;
            m_SE->PlayMove();

            if (m_rightCursor == -1)
            {
                m_rightCursor = 0;
            }

            if (m_rightSelect == -1)
            {
                m_rightSelect = 0;
            }
        }
    }
    return std::wstring();
}

std::wstring StorehouseLib::Left()
{
    if (!m_leftList.empty())
    {
        if (m_eFocus == eFocus::RIGHT)
        {
            m_eFocus = eFocus::LEFT;
            m_SE->PlayMove();

            if (m_leftCursor == -1)
            {
                m_leftCursor = 0;
            }

            if (m_leftSelect == -1)
            {
                m_leftSelect = 0;
            }
        }
    }
    return std::wstring();
}

std::wstring StorehouseLib::Into()
{
    std::wstring result;
    if (m_eFocus == eFocus::LEFT)
    {
        if (m_leftSelect >= m_leftList.size())
        {
            m_leftCursor = 0;
            m_leftSelect = 0;
            return result;
        }

        if (!m_leftList.empty())
        {
            result = _T("left:");
            result += m_leftList.at(m_leftSelect).GetName();
            result += _T(":") + m_leftList.at(m_leftSelect).GetId();
            result += _T(":") + std::to_wstring(m_leftList.at(m_leftSelect).GetSubId());
            m_SE->PlayClick();

            if (m_leftList.size() == 1)
            {
                m_eFocus = eFocus::RIGHT;
            }
            else
            {
                //--------------------------------------------------------
                // 最後の要素をクリックしたら、カーソルを１つ上に移動させる。
                // ただし、カーソルが一番下にあった場合はリストを１つ下に下げる
                // カーソルを一つ上に移動させる
                //--------------------------------------------------------

                if (m_leftSelect == m_leftList.size() - 1)
                {
                    if (m_leftCursor != PANEL_ROW_MAX - 1)
                    {
                        m_leftCursor--;
                        m_leftSelect--;
                    }
                    else
                    {
                        if (m_leftBegin >= 1)
                        {
                            m_leftBegin--;
                            m_leftSelect--;
                        }
                        else if (m_leftBegin == 0)
                        {
                            m_leftCursor--;
                            m_leftSelect--;
                        }
                    }
                }
            }
        }
    }
    else if (m_eFocus == eFocus::RIGHT)
    {
        if (m_rightSelect >= m_rightList.size())
        {
            m_rightCursor = 0;
            m_rightSelect = 0;
            return result;
        }

        if (!m_rightList.empty())
        {
            result = _T("right:");
            result += m_rightList.at(m_rightSelect).GetName();
            result += _T(":") + m_rightList.at(m_rightSelect).GetId();
            result += _T(":") + std::to_wstring(m_rightList.at(m_rightSelect).GetSubId());
            m_SE->PlayClick();

            if (m_rightList.size() == 1)
            {
                m_eFocus = eFocus::LEFT;
            }
            else
            {
                //--------------------------------------------------------
                // 最後の要素をクリックしたら、カーソルを１つ上に移動させる。
                // ただし、カーソルが一番下にあった場合はリストを１つ下に下げる
                // カーソルを一つ上に移動させる
                //--------------------------------------------------------

                if (m_rightSelect == m_rightList.size() - 1)
                {
                    if (m_rightCursor != PANEL_ROW_MAX - 1)
                    {
                        m_rightCursor--;
                        m_rightSelect--;
                    }
                    else
                    {
                        if (m_rightBegin >= 1)
                        {
                            m_rightBegin--;
                            m_rightSelect--;
                        }
                        else if (m_rightBegin == 0)
                        {
                            m_rightCursor--;
                            m_rightSelect--;
                        }
                    }
                }
            }
        }
    }

    return result;
}

std::wstring StorehouseLib::Back()
{
    std::wstring result;
    result = _T("EXIT");
    m_SE->PlayBack();

    return result;
}

std::wstring NSStorehouseLib::StorehouseLib::Next()
{
    return Down();
}

std::wstring NSStorehouseLib::StorehouseLib::Previous()
{
    return Up();
}

void StorehouseLib::CursorOn(const int x, const int y)
{
    // ホイール移動して0.5秒以内なら反応しないようにする
    {
        auto current = std::chrono::system_clock::now();
        auto diff = current - m_wheelUseTime;
        if (diff <= std::chrono::milliseconds(500))
        {
            return;
        }
    }

    int previousCursor = m_leftCursor;
    int previousSelect = m_leftSelect;
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
        if (m_leftSelect >= (int)m_leftList.size())
        {
            m_leftCursor = previousCursor;
            m_leftSelect = previousSelect;
        }
        else
        {
            m_SE->PlayMove();
        }
    }

    int previousRightCursor = m_rightCursor;
    int previousRightSelect = m_rightSelect;

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
        if (m_rightSelect >= (int)m_rightList.size())
        {
            m_rightCursor = previousRightCursor;
            m_rightSelect = previousRightSelect;
        }
        else
        {
            m_SE->PlayMove();
        }
    }
}

std::wstring StorehouseLib::Click(const int x, const int y)
{
    std::wstring result;
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

    if (!result.empty())
    {
        m_SE->PlayClick();
    }

    return result;
}

void StorehouseLib::Draw()
{
    // 背景
    m_sprBackground->DrawImage(0, 0);

    // 上部分の左に「インベントリ」、右側に「倉庫」と表示する
    if (!m_bEnglish)
    {
        m_font->DrawText_(_T("インベントリ"), 205, 50, 200);

        m_font->DrawText_(_T("倉庫"), 855, 50, 200);

        m_font->DrawText_(_T("左右で切り替え、上下でアイテムを選択"), 1085, 30, 100);
        m_font->DrawText_(_T("決定ボタンでアイテムを反対側に移動"), 1085, 80, 100);
    }
    else
    {
        m_font->DrawText_(_T("Inventory"), 205, 50, 200);

        m_font->DrawText_(_T("Storage"), 855, 50, 200);

        m_font->DrawText_(_T("L/R: Switch U/D: Select item"), 1085, 30, 100);
        m_font->DrawText_(_T("Confirm: Move to other side"), 1085, 80, 100);
    }

    //--------------------------------------------------------
    // 左の列のインベントリを表示
    //--------------------------------------------------------

    if (!m_bEnglish)
    {
        m_font->DrawText_(_T("アイテム名"), 200, 135, 120);
        m_font->DrawText_(_T("強化"), 200 + 350, 135, 120);
        m_font->DrawText_(_T("耐久"), 200 + 450, 135, 120);
    }
    else
    {
        m_font->DrawText_(_T("Item"), 200, 135, 120);
        m_font->DrawText_(_T("Level"), 200 + 350, 135, 120);
        m_font->DrawText_(_T("Durability"), 200 + 450, 135, 120);
    }

    if ((int)m_leftList.size() >= PANEL_ROW_MAX)
    {
        for (int i = m_leftBegin; i < m_leftBegin + PANEL_ROW_MAX; ++i)
        {
            m_font->DrawText_(m_leftList.at(i).GetName(),
                              LEFT_PANEL_STARTX,
                              LEFT_PANEL_STARTY + ((i - m_leftBegin) * PANEL_HEIGHT));

            if (m_leftList.at(i).GetLevel() != -1)
            {
                m_font->DrawText_(std::to_wstring(m_leftList.at(i).GetLevel()),
                                  LEFT_PANEL_STARTX + 400,
                                  LEFT_PANEL_STARTY + ((i - m_leftBegin) * PANEL_HEIGHT));
            }

            if (m_leftList.at(i).GetDurability() != -1)
            {
                m_font->DrawText_(std::to_wstring(m_leftList.at(i).GetDurability()),
                                  LEFT_PANEL_STARTX + 500,
                                  LEFT_PANEL_STARTY + ((i - m_leftBegin) * PANEL_HEIGHT));
            }
        }
    }
    else
    {
        for (std::size_t i = 0; i < m_leftList.size(); ++i)
        {
            m_font->DrawText_(m_leftList.at(i).GetName(),
                              LEFT_PANEL_STARTX,
                              LEFT_PANEL_STARTY + ((int)i * PANEL_HEIGHT));

            if (m_leftList.at(i).GetLevel() != -1)
            {
                m_font->DrawText_(std::to_wstring(m_leftList.at(i).GetLevel()),
                                  LEFT_PANEL_STARTX + 400,
                                  LEFT_PANEL_STARTY + (((int)i - m_leftBegin) * PANEL_HEIGHT));
            }

            if (m_leftList.at(i).GetDurability() != -1)
            {
                m_font->DrawText_(std::to_wstring(m_leftList.at(i).GetDurability()),
                                  LEFT_PANEL_STARTX + 500,
                                  LEFT_PANEL_STARTY + (((int)i - m_leftBegin) * PANEL_HEIGHT));
            }
        }
    }

    //--------------------------------------------------------
    // 右の列の倉庫を表示
    //--------------------------------------------------------

    if (!m_bEnglish)
    {
        m_font->DrawText_(_T("アイテム名"), 850, 135, 120);
        m_font->DrawText_(_T("強化"), 850 + 350, 135, 120);
        m_font->DrawText_(_T("耐久"), 850 + 450, 135, 120);
    }
    else
    {
        m_font->DrawText_(_T("Item"), 850, 135, 120);
        m_font->DrawText_(_T("Level"), 850 + 350, 135, 120);
        m_font->DrawText_(_T("Durability"), 850 + 450, 135, 120);
    }

    if ((int)m_rightList.size() >= PANEL_ROW_MAX)
    {
        for (int i = m_rightBegin; i < m_rightBegin + PANEL_ROW_MAX; ++i)
        {
            m_font->DrawText_(m_rightList.at(i).GetName(),
                              RIGHT_PANEL_STARTX,
                              RIGHT_PANEL_STARTY + ((i - m_rightBegin) * PANEL_HEIGHT));

            if (m_rightList.at(i).GetLevel() != -1)
            {
                m_font->DrawText_(std::to_wstring(m_rightList.at(i).GetLevel()),
                                  RIGHT_PANEL_STARTX + 400,
                                  RIGHT_PANEL_STARTY + ((i - m_rightBegin) * PANEL_HEIGHT));
            }

            if (m_rightList.at(i).GetDurability() != -1)
            {
                m_font->DrawText_(std::to_wstring(m_rightList.at(i).GetDurability()),
                                  RIGHT_PANEL_STARTX + 500,
                                  RIGHT_PANEL_STARTY + ((i - m_rightBegin) * PANEL_HEIGHT));
            }
        }
    }
    else
    {
        for (std::size_t i = 0; i < m_rightList.size(); ++i)
        {
            m_font->DrawText_(m_rightList.at(i).GetName(),
                              RIGHT_PANEL_STARTX,
                              RIGHT_PANEL_STARTY + ((int)i * PANEL_HEIGHT));

            if (m_rightList.at(i).GetLevel() != -1)
            {
                m_font->DrawText_(std::to_wstring(m_rightList.at(i).GetLevel()),
                                  RIGHT_PANEL_STARTX + 400,
                                  RIGHT_PANEL_STARTY + (((int)i - m_rightBegin) * PANEL_HEIGHT));
            }

            if (m_rightList.at(i).GetDurability() != -1)
            {
                m_font->DrawText_(std::to_wstring(m_rightList.at(i).GetDurability()),
                                  RIGHT_PANEL_STARTX + 500,
                                  RIGHT_PANEL_STARTY + (((int)i - m_rightBegin) * PANEL_HEIGHT));
            }
        }
    }

    // カーソルの表示
    if (m_eFocus == eFocus::LEFT)
    {
        if (!m_leftList.empty())
        {
            m_sprCursor->DrawImage(110, 206 + (m_leftCursor * 60));
        }
    }
    else if (m_eFocus == eFocus::RIGHT)
    {
        if (!m_rightList.empty())
        {
            m_sprCursor->DrawImage(760, 206 + (m_rightCursor * 60));
        }
    }


}

void NSStorehouseLib::StorehouseLib::UseWheel()
{
    m_wheelUseTime = std::chrono::system_clock::now();
}

void NSStorehouseLib::StoreItem::SetId(const std::wstring& arg)
{
    m_id = arg;
}

std::wstring NSStorehouseLib::StoreItem::GetId() const
{
    return m_id;
}

void NSStorehouseLib::StoreItem::SetSubId(const int arg)
{
    m_idSub = arg;
}

int NSStorehouseLib::StoreItem::GetSubId() const
{
    return m_idSub;
}

void NSStorehouseLib::StoreItem::SetName(const std::wstring& arg)
{
    m_name = arg;
}

std::wstring NSStorehouseLib::StoreItem::GetName() const
{
    return m_name;
}

void NSStorehouseLib::StoreItem::SetLevel(const int arg)
{
    m_level = arg;
}

int NSStorehouseLib::StoreItem::GetLevel() const
{
    return m_level;
}

void NSStorehouseLib::StoreItem::SetDurability(const int arg)
{
    m_durability = arg;
}

int NSStorehouseLib::StoreItem::GetDurability() const
{
    return m_durability;
}
