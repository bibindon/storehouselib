﻿#pragma comment( lib, "d3d9.lib" )
#if defined(NDEBUG)
#pragma comment( lib, "d3dx9.lib" )
#else
#pragma comment( lib, "d3dx9d.lib" )
#endif

#pragma comment (lib, "winmm.lib")

#pragma comment( lib, "storehouselib.lib" )

#include "..\storehouselib\StorehouseLib.h"

#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <vector>
#include <sstream>
#include <cassert>

std::vector<std::string> split(const std::string& s, char delim)
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

using namespace NSStorehouseLib;

#define SAFE_RELEASE(p) { if (p) { (p)->Release(); (p) = NULL; } }

class Sprite : public ISprite
{
public:

    Sprite(LPDIRECT3DDEVICE9 dev)
        : m_pD3DDevice(dev)
    {
    }

    void DrawImage(const int x, const int y, const int transparency) override
    {
        D3DXVECTOR3 pos {(float)x, (float)y, 0.f};
        m_D3DSprite->Begin(D3DXSPRITE_ALPHABLEND);
        RECT rect = {
            0,
            0,
            static_cast<LONG>(m_width),
            static_cast<LONG>(m_height) };
        D3DXVECTOR3 center { 0, 0, 0 };
        m_D3DSprite->Draw(
            m_pD3DTexture,
            &rect,
            &center,
            &pos,
            D3DCOLOR_ARGB(transparency, 255, 255, 255));
        m_D3DSprite->End();

    }

    void Load(const std::string& filepath) override
    {
        LPD3DXSPRITE tempSprite { nullptr };
        if (FAILED(D3DXCreateSprite(m_pD3DDevice, &m_D3DSprite)))
        {
            throw std::exception("Failed to create a sprite.");
        }

        if (FAILED(D3DXCreateTextureFromFile(
            m_pD3DDevice,
            filepath.c_str(),
            &m_pD3DTexture)))
        {
            throw std::exception("Failed to create a texture.");
        }

        D3DSURFACE_DESC desc { };
        if (FAILED(m_pD3DTexture->GetLevelDesc(0, &desc)))
        {
            throw std::exception("Failed to create a texture.");
        }
        m_width = desc.Width;
        m_height = desc.Height;
    }

    ~Sprite()
    {
        m_D3DSprite->Release();
        m_pD3DTexture->Release();
    }

private:

    LPDIRECT3DDEVICE9 m_pD3DDevice = NULL;
    LPD3DXSPRITE m_D3DSprite = NULL;
    LPDIRECT3DTEXTURE9 m_pD3DTexture = NULL;
    UINT m_width { 0 };
    UINT m_height { 0 };
};

class Font : public IFont
{
public:

    Font(LPDIRECT3DDEVICE9 pD3DDevice)
        : m_pD3DDevice(pD3DDevice)
    {
    }

    void Init(bool bEnglish)
    {
        HRESULT hr = E_FAIL;
        if (!bEnglish)
        {
            hr = D3DXCreateFont(m_pD3DDevice,
                                24,
                                0,
                                FW_NORMAL,
                                1,
                                false,
                                SHIFTJIS_CHARSET,
                                OUT_TT_ONLY_PRECIS,
                                ANTIALIASED_QUALITY,
                                FF_DONTCARE,
                                "ＭＳ 明朝",
                                &m_pFont);
        }
        else
        {
            hr = D3DXCreateFont(m_pD3DDevice,
                                24,
                                0,
                                FW_NORMAL,
                                1,
                                false,
                                ANSI_CHARSET,
                                OUT_TT_ONLY_PRECIS,
                                ANTIALIASED_QUALITY,
                                FF_DONTCARE,
                                "Times New Roman",
                                &m_pFont);
        }

        assert(hr == S_OK);
    }

    virtual void DrawText_(const std::string& msg, const int x, const int y, const int transparency)
    {
        RECT rect = { x, y, 0, 0 };
        m_pFont->DrawText(NULL, msg.c_str(), -1, &rect, DT_LEFT | DT_NOCLIP,
            D3DCOLOR_ARGB(transparency, 255, 255, 255));
    }

    ~Font()
    {
        m_pFont->Release();
    }

private:

    LPDIRECT3DDEVICE9 m_pD3DDevice = NULL;
    LPD3DXFONT m_pFont = NULL;
};


class SoundEffect : public ISoundEffect
{
    virtual void PlayMove() override
    {
        PlaySound("cursor_move.wav", NULL, SND_FILENAME | SND_ASYNC);
    }
    virtual void PlayClick() override
    {
        PlaySound("cursor_confirm.wav", NULL, SND_FILENAME | SND_ASYNC);
    }
    virtual void PlayBack() override
    {
        PlaySound("cursor_cancel.wav", NULL, SND_FILENAME | SND_ASYNC);
    }
    virtual void Init() override
    {

    }
};

LPDIRECT3D9 g_pD3D = NULL;
LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;
LPD3DXFONT g_pFont = NULL;
LPD3DXMESH pMesh = NULL;
D3DMATERIAL9* pMaterials = NULL;
LPDIRECT3DTEXTURE9* pTextures = NULL;
DWORD dwNumMaterials = 0;
LPD3DXEFFECT pEffect = NULL;
D3DXMATERIAL* d3dxMaterials = NULL;
float f = 0.0f;
bool bShowMenu = true;

StorehouseLib menu;

void TextDraw(LPD3DXFONT pFont, char* text, int X, int Y)
{
    RECT rect = { X,Y,0,0 };
    pFont->DrawText(NULL, text, -1, &rect, DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 0));
}

HRESULT InitD3D(HWND hWnd)
{
    if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
    {
        return E_FAIL;
    }

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    d3dpp.BackBufferCount = 1;
    d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
    d3dpp.MultiSampleQuality = 0;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    d3dpp.hDeviceWindow = hWnd;
    d3dpp.Flags = 0;
    d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

    if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice)))
    {
        if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice)))
        {
            return(E_FAIL);
        }
    }

    HRESULT hr = D3DXCreateFont(
        g_pd3dDevice,
        20,
        0,
        FW_HEAVY,
        1,
        false,
        SHIFTJIS_CHARSET,
        OUT_TT_ONLY_PRECIS,
        ANTIALIASED_QUALITY,
        FF_DONTCARE,
        "ＭＳ ゴシック",
        &g_pFont);
    if FAILED(hr)
    {
        return(E_FAIL);
    }

    LPD3DXBUFFER pD3DXMtrlBuffer = NULL;

    if (FAILED(D3DXLoadMeshFromX("cube.x", D3DXMESH_SYSTEMMEM,
        g_pd3dDevice, NULL, &pD3DXMtrlBuffer, NULL,
        &dwNumMaterials, &pMesh)))
    {
        MessageBox(NULL, "Xファイルの読み込みに失敗しました", NULL, MB_OK);
        return E_FAIL;
    }
    d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
    pMaterials = new D3DMATERIAL9[dwNumMaterials];
    pTextures = new LPDIRECT3DTEXTURE9[dwNumMaterials];

    for (DWORD i = 0; i < dwNumMaterials; i++)
    {
        pMaterials[i] = d3dxMaterials[i].MatD3D;
        pMaterials[i].Ambient = pMaterials[i].Diffuse;
        pTextures[i] = NULL;
        if (d3dxMaterials[i].pTextureFilename != NULL &&
            lstrlen(d3dxMaterials[i].pTextureFilename) > 0)
        {
            if (FAILED(D3DXCreateTextureFromFile(g_pd3dDevice,
                d3dxMaterials[i].pTextureFilename,
                &pTextures[i])))
            {
                MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
            }
        }
    }
    pD3DXMtrlBuffer->Release();

    D3DXCreateEffectFromFile(
        g_pd3dDevice,
        "simple.fx",
        NULL,
        NULL,
        D3DXSHADER_DEBUG,
        NULL,
        &pEffect,
        NULL
    );

    Sprite* sprCursor = new Sprite(g_pd3dDevice);
    sprCursor->Load("cursor.png");

    Sprite* sprBackground = new Sprite(g_pd3dDevice);
    sprBackground->Load("background.png");

    IFont* pFont = new Font(g_pd3dDevice);
    pFont->Init(true);

    ISoundEffect* pSE = new SoundEffect();

    menu.Init(pFont, pSE, sprCursor, sprBackground, true);
    {
        std::vector<StoreItem> vs;

        for (int i = 0; i < 15; ++i)
        {
            StoreItem storeItem;
            storeItem.SetId(i);
            storeItem.SetSubId(1);
            std::string work;
            work = "アイテムＡＡＡ" + std::to_string(i);
            storeItem.SetName(work);
            vs.push_back(storeItem);
        }
        menu.SetInventoryList(vs);
        vs.clear();

        for (int i = 0; i < 20; ++i)
        {
            StoreItem storeItem;
            storeItem.SetId(i+30);
            storeItem.SetSubId(1);
            std::string work;
            work = "アイテムＢＢＢ" + std::to_string(i);
            storeItem.SetName(work);
            vs.push_back(storeItem);
        }
        menu.SetStorehouseList(vs);
    }

    return S_OK;
}

VOID Cleanup()
{
    SAFE_RELEASE(pMesh);
    SAFE_RELEASE(g_pFont);
    SAFE_RELEASE(g_pd3dDevice);
    SAFE_RELEASE(g_pD3D);
}

VOID Render()
{
    if (NULL == g_pd3dDevice)
    {
        return;
    }
    f += 0.010f;

    D3DXMATRIX mat;
    D3DXMATRIX View, Proj;
    D3DXMatrixPerspectiveFovLH(&Proj, D3DXToRadian(45), 1600.0f / 900.0f, 1.0f, 1000.0f);
    D3DXVECTOR3 vec1(3 * sinf(f), 3, -3 * cosf(f));
    D3DXVECTOR3 vec2(0, 0, 0);
    D3DXVECTOR3 vec3(0, 1, 0);
    D3DXMatrixLookAtLH(&View, &vec1, &vec2, &vec3);
    D3DXMatrixIdentity(&mat);
    mat = mat * View * Proj;
    pEffect->SetMatrix("matWorldViewProj", &mat);

    g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
        D3DCOLOR_XRGB(100, 100, 100), 1.0f, 0);

    if (SUCCEEDED(g_pd3dDevice->BeginScene()))
    {
        char msg[128];
        strcpy_s(msg, 128, "Cキーでクラフト画面を表示");
        TextDraw(g_pFont, msg, 0, 0);

        pEffect->SetTechnique("BasicTec");
        UINT numPass;
        pEffect->Begin(&numPass, 0);
        pEffect->BeginPass(0);
        for (DWORD i = 0; i < dwNumMaterials; i++)
        {
            pEffect->SetTexture("texture1", pTextures[i]);
            pMesh->DrawSubset(i);
        }
        if (bShowMenu)
        {
            menu.Draw();
        }
        pEffect->EndPass();
        pEffect->End();
        g_pd3dDevice->EndScene();
    }

    g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        Cleanup();
        PostQuitMessage(0);
        return 0;
    case WM_PAINT:
        Render();
        return 0;
    case WM_SIZE:
        InvalidateRect(hWnd, NULL, true);
        return 0;
    case WM_KEYDOWN:
        switch (wParam)
        {
        case 'C':
            if (bShowMenu == false)
            {
                bShowMenu = true;
            }
            break;
        // メニューを表示している最中にメニューに表示されている内容を変える
        case VK_F2:
        {
            // menu.SetItem(itemInfoList);
        }
        case VK_UP:
            menu.Up();
            break;
        case VK_DOWN:
            menu.Down();
            break;
        case VK_LEFT:
            menu.Left();
            break;
        case VK_RIGHT:
            menu.Right();
            break;
        case VK_RETURN:
        {
            std::string result = menu.Into();
            auto vs = split(result, ':');

            int id_ = std::stoi(vs.at(2));
            int subId_ = std::stoi(vs.at(3));

            if (vs.at(0) == "left")
            {
                menu.MoveFromInventoryToStorehouse(id_, subId_);
            }
            else if (vs.at(0) == "right")
            {
                menu.MoveFromStorehouseToInventory(id_, subId_);
            }

            break;
        }
        case VK_BACK:
        {
            std::string result;
            result = menu.Back();
            if (result == "EXIT")
            {
                bShowMenu = false;
            }
            break;
        }
        case VK_ESCAPE:
            PostQuitMessage(0);
            break;
        }
        break;
    case WM_MOUSEMOVE:
    {
        POINTS mouse_p = MAKEPOINTS(lParam);
        menu.CursorOn(mouse_p.x, mouse_p.y);
        break;
    }
    case WM_LBUTTONDOWN:
    {
        POINTS mouse_p = MAKEPOINTS(lParam);
        menu.Click(mouse_p.x, mouse_p.y);
        break;
    }
    case WM_RBUTTONDOWN:
    {
        std::string result;
        result = menu.Back();
        if (result == "EXIT")
        {
            bShowMenu = false;
        }
        break;
    }
    case WM_MOUSEWHEEL:
    {
        int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
        if (zDelta <= 0)
        {
            menu.Next();
        }
        else
        {
            menu.Previous();
        }
        break;
    }
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

INT WINAPI wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ INT)
{
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
                      GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
                      "Window1", NULL };
    RegisterClassEx(&wc);

    RECT rect;
    SetRect(&rect, 0, 0, 1600, 900);
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
    rect.right = rect.right - rect.left;
    rect.bottom = rect.bottom - rect.top;
    rect.top = 0;
    rect.left = 0;

    HWND hWnd = CreateWindow("Window1", "Hello DirectX9 World !!",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rect.right, rect.bottom,
        NULL, NULL, wc.hInstance, NULL);

    if (SUCCEEDED(InitD3D(hWnd)))
    {
        ShowWindow(hWnd, SW_SHOWDEFAULT);
        UpdateWindow(hWnd);

        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    UnregisterClass("Window1", wc.hInstance);
    return 0;
}
