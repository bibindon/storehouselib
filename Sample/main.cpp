#pragma comment( lib, "d3d9.lib")
#if defined(NDEBUG)
#pragma comment( lib, "d3dx9.lib")
#else
#pragma comment( lib, "d3dx9d.lib")
#endif

#pragma comment (lib, "winmm.lib")

#pragma comment( lib, "storehouselib.lib")

#include "..\storehouselib\StorehouseLib.h"

#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <vector>
#include <sstream>
#include <cassert>
#include <tchar.h>
#include <type_traits>

// -------------------------------------------------
// 共通デリータ（テンプレート呼び出し演算子で何型でも Release）
// -------------------------------------------------
struct ComReleaser
{
    template <class U>
    void operator()(U* p) const noexcept
    {
        if (p != nullptr)
        {
            p->Release();
        }
    }
};

// -------------------------------------------------
// 「ポインタならはがす」ユーティリティ型
// -------------------------------------------------
template <class T>
using InterfaceOf = std::remove_pointer_t<std::remove_cv_t<T>>;

// -------------------------------------------------
//   - T が ID3DXFont でも LPD3DXFONT(ID3DXFont*) でも
//     InterfaceOf<T> は ID3DXFont になる
// -------------------------------------------------
template <class T>
using Ptr = std::unique_ptr<InterfaceOf<T>, ComReleaser>;

// ポインタのポインタを取得
template<class P>
auto PtrAddr(P& p) -> typename P::element_type**       // ← ココがポイント
{
    p.reset();                                         // 既存を Release
    return reinterpret_cast<typename P::element_type**>(&p);
}

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

using namespace NSStorehouseLib;

class Sprite : public ISprite
{
public:

    Sprite(const Ptr<LPDIRECT3DDEVICE9>& dev)
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

        m_D3DSprite->Draw(m_pD3DTexture.get(),
                          &rect,
                          &center,
                          &pos,
                          D3DCOLOR_ARGB(transparency, 255, 255, 255));

        m_D3DSprite->End();
    }

    void Load(const std::wstring& filepath) override
    {
        if (FAILED(D3DXCreateSprite(m_pD3DDevice.get(), PtrAddr(m_D3DSprite))))
        {
            throw std::exception("Failed to create a sprite.");
        }

        if (FAILED(D3DXCreateTextureFromFile(m_pD3DDevice.get(),
                                             filepath.c_str(),
                                             PtrAddr(m_pD3DTexture))))
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
    }

private:

    const Ptr<LPDIRECT3DDEVICE9>& m_pD3DDevice;
    Ptr<LPD3DXSPRITE> m_D3DSprite;
    Ptr<LPDIRECT3DTEXTURE9> m_pD3DTexture;
    UINT m_width { 0 };
    UINT m_height { 0 };
};

class Font : public IFont
{
public:

    Font(const Ptr<LPDIRECT3DDEVICE9>& pD3DDevice)
        : m_pD3DDevice(pD3DDevice)
    {
    }

    void Init(bool bEnglish)
    {
        HRESULT hr = E_FAIL;
        if (!bEnglish)
        {
            hr = D3DXCreateFont(m_pD3DDevice.get(),
                                24,
                                0,
                                FW_NORMAL,
                                1,
                                false,
                                SHIFTJIS_CHARSET,
                                OUT_TT_ONLY_PRECIS,
                                ANTIALIASED_QUALITY,
                                FF_DONTCARE,
                                _T("ＭＳ 明朝"),
                                PtrAddr(m_pFont));
        }
        else
        {
            hr = D3DXCreateFont(m_pD3DDevice.get(),
                                24,
                                0,
                                FW_NORMAL,
                                1,
                                false,
                                ANSI_CHARSET,
                                OUT_TT_ONLY_PRECIS,
                                ANTIALIASED_QUALITY,
                                FF_DONTCARE,
                                _T("Times New Roman"),
                                PtrAddr(m_pFont));
        }

        assert(hr == S_OK);
    }

    virtual void DrawText_(const std::wstring& msg, const int x, const int y, const int transparency)
    {
        RECT rect = { x, y, 0, 0 };
        m_pFont->DrawText(NULL,
                          msg.c_str(),
                          -1,
                          &rect,
                          DT_LEFT | DT_NOCLIP,
                          D3DCOLOR_ARGB(transparency, 255, 255, 255));
    }

    ~Font()
    {
    }

private:

    const Ptr<LPDIRECT3DDEVICE9>& m_pD3DDevice;
    Ptr<LPD3DXFONT> m_pFont;
};


class SoundEffect : public ISoundEffect
{
    virtual void PlayMove() override
    {
        PlaySound(_T("cursor_move.wav"), NULL, SND_FILENAME | SND_ASYNC);
    }
    virtual void PlayClick() override
    {
        PlaySound(_T("cursor_confirm.wav"), NULL, SND_FILENAME | SND_ASYNC);
    }
    virtual void PlayBack() override
    {
        PlaySound(_T("cursor_cancel.wav"), NULL, SND_FILENAME | SND_ASYNC);
    }
    virtual void Init() override
    {

    }
};

Ptr<LPDIRECT3D9> g_pD3D;
Ptr<LPDIRECT3DDEVICE9> g_pd3dDevice;
Ptr<LPD3DXFONT> g_pFont;
Ptr<LPD3DXMESH> pMesh;
std::vector<Ptr<LPDIRECT3DTEXTURE9>> pTextures;
DWORD dwNumMaterials = 0;
Ptr<LPD3DXEFFECT> pEffect;
D3DXMATERIAL* d3dxMaterials;
float f = 0.0f;
bool bShowMenu = true;

StorehouseLib menu;

static void TextDraw(LPD3DXFONT pFont, wchar_t* text, int X, int Y)
{
    RECT rect = { X,Y,0,0 };
    pFont->DrawText(NULL, text, -1, &rect, DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 0));
}

static HRESULT InitD3D(HWND hWnd)
{
    g_pD3D.reset( Direct3DCreate9(D3D_SDK_VERSION));
    if (NULL == g_pD3D)
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

    if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                    D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                    &d3dpp, PtrAddr(g_pd3dDevice))))
    {
        if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                        &d3dpp, PtrAddr(g_pd3dDevice))))
        {
            return(E_FAIL);
        }
    }

    HRESULT hr = D3DXCreateFont(g_pd3dDevice.get(),
                                20,
                                0,
                                FW_HEAVY,
                                1,
                                false,
                                SHIFTJIS_CHARSET,
                                OUT_TT_ONLY_PRECIS,
                                ANTIALIASED_QUALITY,
                                FF_DONTCARE,
                                _T("ＭＳ ゴシック"),
                                PtrAddr(g_pFont));
    if FAILED(hr)
    {
        return(E_FAIL);
    }

    LPD3DXBUFFER pD3DXMtrlBuffer = NULL;

    if (FAILED(D3DXLoadMeshFromX(_T("cube.x"),
                                 D3DXMESH_SYSTEMMEM,
                                 g_pd3dDevice.get(),
                                 NULL,
                                 &pD3DXMtrlBuffer,
                                 NULL,
                                 &dwNumMaterials,
                                 PtrAddr(pMesh))))
    {
        MessageBox(NULL, _T("Xファイルの読み込みに失敗しました"), NULL, MB_OK);
        return E_FAIL;
    }
    d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();

    std::vector<D3DMATERIAL9> pMaterials(dwNumMaterials);
    pTextures.resize(dwNumMaterials);

    for (DWORD i = 0; i < dwNumMaterials; i++)
    {
        pMaterials[i] = d3dxMaterials[i].MatD3D;
        pMaterials[i].Ambient = pMaterials[i].Diffuse;
        pTextures[i] = NULL;

        int len = MultiByteToWideChar(CP_UTF8, 0, d3dxMaterials[i].pTextureFilename, -1, nullptr, 0);
        std::wstring result(len - 1, 0);
        MultiByteToWideChar(CP_UTF8, 0, d3dxMaterials[i].pTextureFilename, -1, &result[0], len);

        if (!result.empty())
        {
            if (FAILED(D3DXCreateTextureFromFile(g_pd3dDevice.get(),
                                                 result.c_str(),
                                                 PtrAddr(pTextures[i]))))
            {
                MessageBox(NULL, _T("テクスチャの読み込みに失敗しました"), NULL, MB_OK);
            }
        }
    }
    pD3DXMtrlBuffer->Release();

    D3DXCreateEffectFromFile(g_pd3dDevice.get(),
                             _T("simple.fx"),
                             NULL,
                             NULL, 
                             D3DXSHADER_DEBUG,
                             NULL,
                             PtrAddr(pEffect),
                             NULL
    );

    Sprite* sprCursor = new Sprite(g_pd3dDevice);
    sprCursor->Load(_T("cursor.png"));

    Sprite* sprBackground = new Sprite(g_pd3dDevice);
    sprBackground->Load(_T("background.png"));

    IFont* pFont = new Font(g_pd3dDevice);
    pFont->Init(true);

    ISoundEffect* pSE = new SoundEffect();

    menu.Init(pFont, pSE, sprCursor, sprBackground, true);
    {
        std::vector<StoreItem> vs;

        for (int i = 0; i < 15; ++i)
        {
            StoreItem storeItem;
            std::wstring id = L"itemAAA" + std::to_wstring(i);
            storeItem.SetId(id);
            storeItem.SetSubId(1);
            std::wstring work;
            work = _T("アイテムＡＡＡ") + std::to_wstring(i);
            storeItem.SetName(work);
            vs.push_back(storeItem);
        }
        menu.SetInventoryList(vs);
        vs.clear();

        for (int i = 0; i < 25; ++i)
        {
            StoreItem storeItem;
            std::wstring id = L"itemBBB" + std::to_wstring(i);
            storeItem.SetId(id);
            storeItem.SetSubId(1);
            std::wstring work;
            work = _T("アイテムＢＢＢ") + std::to_wstring(i);
            storeItem.SetName(work);
            vs.push_back(storeItem);
        }
        menu.SetStorehouseList(vs);
    }

    return S_OK;
}

static VOID Cleanup()
{
    menu.Finalize();
}

static VOID Render()
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
        wchar_t msg[128];
        wcscpy_s(msg, 128, _T("Cキーで倉庫画面を表示"));
        TextDraw(g_pFont.get(), msg, 0, 0);

        pEffect->SetTechnique("BasicTec");
        UINT numPass;
        pEffect->Begin(&numPass, 0);
        pEffect->BeginPass(0);
        for (DWORD i = 0; i < dwNumMaterials; i++)
        {
            pEffect->SetTexture("texture1", pTextures[i].get());
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

static LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
            if (bShowMenu)
            {
                bShowMenu = false;
            }
            else
            {
                bShowMenu = true;
            }

            break;
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
            std::wstring result = menu.Into();
            auto vs = split(result, ':');

            auto& id_ = vs.at(2);
            int subId_ = std::stoi(vs.at(3));

            if (vs.at(0) == _T("left"))
            {
                menu.MoveFromInventoryToStorehouse(id_, subId_);
            }
            else if (vs.at(0) == _T("right"))
            {
                menu.MoveFromStorehouseToInventory(id_, subId_);
            }

            break;
        }
        case VK_BACK:
        {
            std::wstring result;
            result = menu.Back();
            if (result == _T("EXIT"))
            {
                bShowMenu = false;
            }
            break;
        }
        case VK_ESCAPE:
            Cleanup();
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
        auto result = menu.Click(mouse_p.x, mouse_p.y);
        break;
    }
    case WM_RBUTTONDOWN:
    {
        std::wstring result;
        result = menu.Back();
        if (result == _T("EXIT"))
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
        menu.UseWheel();
        break;
    }
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

extern "C" INT WINAPI wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ INT);

INT WINAPI wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ INT)
{
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
                      GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
                      _T("Window1"), NULL };
    RegisterClassEx(&wc);

    RECT rect;
    SetRect(&rect, 0, 0, 1600, 900);
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
    rect.right = rect.right - rect.left;
    rect.bottom = rect.bottom - rect.top;
    rect.top = 0;
    rect.left = 0;

    HWND hWnd = CreateWindow(_T("Window1"),
                             _T("Hello DirectX9 World !!"),
                             WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT,
                             CW_USEDEFAULT,
                             rect.right,
                             rect.bottom,
                             NULL,
                             NULL,
                             wc.hInstance,
                             NULL);

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

    UnregisterClass(_T("Window1"), wc.hInstance);
    return 0;
}
