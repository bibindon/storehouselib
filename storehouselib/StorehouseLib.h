#pragma once
#include <string>
#include <vector>
#include <unordered_map>

// �A�C�e�������E��2��ŕ\������
// �����C���x���g���A�E���q��
// �N���b�N������E�����Ɉړ�����B�m�F�͍s��Ȃ��B
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

    // �X�N���[���\�ł��邱�Ƃ��l������
    // �ォ���ʏ�ŉ��ԖڂɃJ�[�\�������邩�B
    int m_leftCursor = 0;

    // �J�[�\�����I�����Ă���v�f��m_outputList�̉��Ԗڂ̗v�f���B
    int m_leftSelect = 0;

    // ���Ԗڂ̃A�C�e������ԏ�ɕ\������Ă��邩
    // �X�N���[���\�Ȃ̂ň�ԏ�ɕ\�������A�C�e���̓X�N���[������ƕς��B
    int m_leftBegin = 0;

    // �X�N���[���\�ł��邱�Ƃ��l������
    // �ォ���ʏ�ŉ��ԖڂɃJ�[�\�������邩�B
    int m_rightCursor = 0;

    // �J�[�\�����I�����Ă���v�f��m_outputList�̉��Ԗڂ̗v�f���B
    int m_rightSelect = 0;

    // ���Ԗڂ̃A�C�e������ԏ�ɕ\������Ă��邩
    // �X�N���[���\�Ȃ̂ň�ԏ�ɕ\�������A�C�e���̓X�N���[������ƕς��B
    int m_rightBegin = 0;
};
}

