#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <chrono>

// アイテムを左右の2列で表示する
// 左がインベントリ、右が倉庫
// クリックしたら右か左に移動する。確認は行わない。
namespace NSStorehouseLib
{
class ISprite
{
public:
    virtual void DrawImage(const int x, const int y, const int transparency = 255) = 0;
    virtual void Load(const std::wstring& filepath) = 0;
    virtual ~ISprite() {};
};

class IFont
{
public:
    virtual void DrawText_(const std::wstring& msg,
                           const int x,
                           const int y,
                           const int transparency = 255) = 0;

    virtual void Init(bool bEnglish) = 0;
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
    void SetId(const std::wstring& arg);
    std::wstring GetId() const;

    void SetSubId(const int arg);
    int GetSubId() const;

    void SetName(const std::wstring& arg);
    std::wstring GetName() const;

    void SetLevel(const int arg);
    int GetLevel() const;

    void SetDurability(const int arg);
    int GetDurability() const;

private:

    std::wstring m_id;
    int m_idSub = 0;
    std::wstring m_name;

    int m_level = -1;
    int m_durability = -1;
};

class StorehouseLib
{
public:

    void Init(IFont* font,
              ISoundEffect* SE,
              ISprite* sprCursor,
              ISprite* sprBackground,
              bool bEnglish);

    void Finalize();

    void SetInventoryList(const std::vector<StoreItem>& arg);
    void SetStorehouseList(const std::vector<StoreItem>& arg);

    void MoveFromInventoryToStorehouse(const std::wstring& id, const int subid);
    void MoveFromStorehouseToInventory(const std::wstring& id, const int subid);

    std::wstring Up();
    std::wstring Down();
    std::wstring Right();
    std::wstring Left();
    std::wstring Into();
    std::wstring Back();
    std::wstring Next();
    std::wstring Previous();
    void CursorOn(const int x, const int y);
    std::wstring Click(const int x, const int y);
    void Draw();

    // ホイール使ってから0.5秒以内はカーソルを無効にするため、ホイールを使ったら
    // 教えてもらう必要がある
    void UseWheel();
    
private:

    enum class eFocus
    {
        LEFT,
        RIGHT,
    };

    ISprite* m_sprCursor = nullptr;
    ISprite* m_sprBackground = nullptr;
    IFont* m_font = nullptr;
    ISoundEffect* m_SE = nullptr;
    eFocus m_eFocus = eFocus::LEFT;

    std::vector<StoreItem> m_leftList;
    std::vector<StoreItem> m_rightList;

    const int PANEL_WIDTH = 432;
    const int PANEL_HEIGHT = 60;

    const int LEFT_PANEL_STARTX = 150;
    const int LEFT_PANEL_STARTY = 200;

    const int RIGHT_PANEL_STARTX = 800;
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

    bool m_bEnglish = false;

    std::chrono::system_clock::time_point m_wheelUseTime;
};
}

