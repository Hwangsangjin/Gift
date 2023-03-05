#include "pch.h"
#include "Window.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
 /*   if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
        return true;*/

    switch (msg)
    {
    case WM_CREATE:
    {
        // �����찡 ������ �� ����Ǵ� �̺�Ʈ
        //Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

        //// Setup Dear ImGui context
        //IMGUI_CHECKVERSION();
        //ImGui::CreateContext();
        //ImGuiIO& io = ImGui::GetIO();

        //// Setup Dear ImGui style
        //ImGui::StyleColorsDark();

        //// Setup Platform/Renderer backends
        //ImGui_ImplWin32_Init(hwnd);
        //ImGui_ImplDX11_Init(Engine::GetInstance()->GetGraphics()->GetD3DDevice(), Engine::GetInstance()->GetGraphics()->GetImmediateContext());
        break;
    }
    case WM_SIZE:
    {
        // �������� ũ�⸦ ������ �� ����Ǵ� �̺�Ʈ
        Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        if (window)
            window->OnSize(window->GetClientSize());
        break;
    }
    case WM_SETFOCUS:
    {
        // �����쿡 ��Ŀ���� ���� �� ����Ǵ� �̺�Ʈ
        Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        if (window)
            window->OnFocus();
        break;
    }
    case WM_KILLFOCUS:
    {
        // �����쿡 ��Ŀ���� ���� �� ����Ǵ� �̺�Ʈ
        Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        window->OnKillFocus();
        break;
    }
    case WM_DESTROY:
    {
        // �����찡 �Ҹ�� �� ����Ǵ� �̺�Ʈ
        Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        window->OnDestroy();

        //// ImGui Cleanup
        //ImGui_ImplDX11_Shutdown();
        //ImGui_ImplWin32_Shutdown();
        //ImGui::DestroyContext();
        break;
    }
    case WM_CLOSE:
    {
        // �����츦 �ݾ��� �� ����Ǵ� �̺�Ʈ
        PostQuitMessage(0);
        break;
    }
    default:
        return ::DefWindowProc(hwnd, msg, wparam, lparam);
    }

    return NULL;
}

Window::Window()
{
    // ������ Ŭ���� ��ü ����
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpszClassName = L"Gift";
    wc.lpfnWndProc = &WndProc;

    // Ŭ���� ����� ������ ���
    auto id = RegisterClassEx(&wc);
    if (!id)
        GiftError("Window not registered successfully.");

    // ������ ����
    RECT rect = { 0, 0, static_cast<LONG>(m_size.m_width), static_cast<LONG>(m_size.m_height) };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

    m_hwnd = CreateWindowEx(NULL, MAKEINTATOM(id), L"Gift", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, NULL, NULL);

    // ������ ������ ������ ���
    if (!m_hwnd)
        GiftError("Window not created successfully.");

    auto hwnd = static_cast<HWND>(m_hwnd);
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);

    // ������ â ���̱�
    ::ShowWindow(hwnd, SW_SHOW);
    ::UpdateWindow(hwnd);
}

Window::~Window()
{
    DestroyWindow(static_cast<HWND>(m_hwnd));
}

Rect Window::GetClientSize() const
{
    RECT rect = {};
    auto hwnd = static_cast<HWND>(m_hwnd);
    ::GetClientRect(hwnd, &rect);
    ::ClientToScreen(hwnd, (LPPOINT)&rect.left);
    ::ClientToScreen(hwnd, (LPPOINT)&rect.right);

    return { static_cast<UINT>(rect.left), static_cast<UINT>(rect.top), static_cast<UINT>(rect.right - rect.left), static_cast<UINT>(rect.bottom - rect.top) };
}

Rect Window::GetScreenSize() const
{
    RECT rect = {};
    rect.right = GetSystemMetrics(SM_CXSCREEN);
    rect.bottom = GetSystemMetrics(SM_CYSCREEN);

    return { 0, 0, static_cast<UINT>(rect.right - rect.left), static_cast<UINT>(rect.bottom - rect.top) };
}

void Window::OnCreate()
{
}

void Window::OnUpdate()
{
}

void Window::OnSize(const Rect& size)
{
}

void Window::OnFocus()
{
}

void Window::OnKillFocus()
{
}

void Window::OnDestroy()
{
}
