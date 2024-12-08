#pragma once
#include <string>
#include <vector>
#include <unordered_map>

// アイテムを左右の2列で表示する
// 左がインベントリ、右が倉庫
// クリックしたら右か左に移動する。確認は行わない。
namespace NSStorehouseLib
{
class ISprite
{
public:
    virtual void DrawImage(const int x, const int y, const int transparency = 255) = 0;
    virtual void Load(const std::string& filepath) = 0;
    virtual ~ISprite() {};
};

class IFont
{
public:
    virtual void DrawText_(const std::string& msg, const int x, const int y) = 0;
    virtual void Init() = 0;
    virtual ~IFont() {};
};

class ISoundEffect
{
public:
    virtual void PlayMove() = 0;
    virtual void PlayClick() = 0;
    virtual void PlayBack() = 0;
    virtual void Init() = 0;
    virtual ~ISoundEffect() {};
};

class StoreItem
{
public:
    void SetId(const int arg);
    int GetId() const;

    void SetSubId(const int arg);
    int GetSubId() const;

    void SetName(const std::string& arg);
    std::string GetName() const;

private:

    int m_id;
    int m_idSub;
    std::string m_name;
};

class StorehouseLib
{
public:

    void Init(IFont* font,
              ISoundEffect* SE,
              ISprite* sprCursor,
              ISprite* sprBackground,
              ISprite* sprPanel,
              ISprite* sprPanelTop);

    void SetInventoryList(const std::vector<StoreItem>& arg);
    void SetStorehouseList(const std::vector<StoreItem>& arg);

    void MoveFromInventoryToStorehouse(const int id, const int subid);
    void MoveFromStorehouseToInventory(const int id, const int subid);

    std::string Up();
    std::string Down();
    std::string Right();
    std::string Left();
    std::string Into();
    std::string Back();
    std::string Next();
    std::string Previous();
    void CursorOn(const int x, const int y);
    std::string Click(const int x, const int y);
    void Draw();
    
private:

    enum class eFocus
    {
        LEFT,
        RIGHT,
    };

    ISprite* m_sprCursor;
    ISprite* m_sprBackground;
    ISprite* m_sprPanel;
    ISprite* m_sprPanelTop;
    IFont* m_font;
    ISoundEffect* m_SE;
    eFocus m_eFocus = eFocus::LEFT;

    std::vector<StoreItem> m_leftList;
    std::vector<StoreItem> m_rightList;

    const int PANEL_PADDINGX = 50;
    const int PANEL_PADDINGY = 13;

    const int PANEL_WIDTH = 432;
    const int PANEL_HEIGHT = 60;

    const int LEFT_PANEL_STARTX = 100;
    const int LEFT_PANEL_STARTY = 200;

    const int RIGHT_PANEL_STARTX = 700;
    const int RIGHT_PANEL_STARTY = 200;

    const int PANEL_ROW_MAX = 10;

    // スクロール可能であることを考慮する
    // 上から画面上で何番目にカーソルがあるか。
    int m_leftCursor = 0;

    // カーソルが選択している要素がm_outputListの何番目の要素か。
    int m_leftSelect = 0;

    // 何番目のアイテムが一番上に表示されているか
    // スクロール可能なので一番上に表示されるアイテムはスクロールすると変わる。
    int m_leftBegin = 0;

    // スクロール可能であることを考慮する
    // 上から画面上で何番目にカーソルがあるか。
    int m_rightCursor = 0;

    // カーソルが選択している要素がm_outputListの何番目の要素か。
    int m_rightSelect = 0;

    // 何番目のアイテムが一番上に表示されているか
    // スクロール可能なので一番上に表示されるアイテムはスクロールすると変わる。
    int m_rightBegin = 0;
};
}

