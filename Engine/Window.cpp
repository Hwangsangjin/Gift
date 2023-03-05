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
        // 윈도우가 생성될 때 실행되는 이벤트
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
        // 윈도우의 크기를 변경할 때 실행되는 이벤트
        Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        if (window)
            window->OnSize(window->GetClientSize());
        break;
    }
    case WM_SETFOCUS:
    {
        // 윈도우에 포커스가 있을 때 실행되는 이벤트
        Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        if (window)
            window->OnFocus();
        break;
    }
    case WM_KILLFOCUS:
    {
        // 윈도우에 포커스가 없을 때 실행되는 이벤트
        Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        window->OnKillFocus();
        break;
    }
    case WM_DESTROY:
    {
        // 윈도우가 소멸될 때 실행되는 이벤트
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
        // 윈도우를 닫았을 때 실행되는 이벤트
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
    // 윈도우 클래스 객체 설정
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpszClassName = L"Gift";
    wc.lpfnWndProc = &WndProc;

    // 클래스 등록을 실패한 경우
    auto id = RegisterClassEx(&wc);
    if (!id)
        GiftError("Window not registered successfully.");

    // 윈도우 생성
    RECT rect = { 0, 0, static_cast<LONG>(m_size.m_width), static_cast<LONG>(m_size.m_height) };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

    m_hwnd = CreateWindowEx(NULL, MAKEINTATOM(id), L"Gift", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, NULL, NULL);

    // 윈도우 생성을 실패한 경우
    if (!m_hwnd)
        GiftError("Window not created successfully.");

    auto hwnd = static_cast<HWND>(m_hwnd);
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);

    // 윈도우 창 보이기
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
