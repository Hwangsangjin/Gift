#include "pch.h"
#include "Window.h"
#include "Engine.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
        return true;

    switch (msg)
    {
    case WM_CREATE:
    {
        // �����찡 ������ �� ����Ǵ� �̺�Ʈ
        Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        // Setup Platform/Renderer backends
        ImGui_ImplWin32_Init(hwnd);
        ImGui_ImplDX11_Init(Engine::GetInstance()->GetGraphics()->GetD3DDevice(), Engine::GetInstance()->GetGraphics()->GetImmediateContext());
        break;
    }
    case WM_SIZE:
    {
        // �������� ũ�⸦ ������ �� ����Ǵ� �̺�Ʈ
        Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        if (window)
            window->OnSize();
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

        // ImGui Cleanup
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();

        ::PostQuitMessage(0);
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
    WNDCLASSEX wc;
    wc.cbClsExtra = NULL;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.cbWndExtra = NULL;
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wc.hInstance = NULL;
    wc.lpszClassName = L"Gift";
    wc.lpszMenuName = L"";
    wc.style = NULL;
    wc.lpfnWndProc = &WndProc;

    // Ŭ���� ����� ������ ���
    if (!::RegisterClassEx(&wc))
        throw std::exception("Window class not registered successfully");

    // ������ ����
    m_hwnd = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"Gift", L"Gift", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768, NULL, NULL, NULL, NULL);

    // ������ ������ ������ ���
    if (!m_hwnd)
        throw std::exception("Window not created successfully");

    // ������ â ���̱�
    ::ShowWindow(m_hwnd, SW_SHOW);
    ::UpdateWindow(m_hwnd);

    // ������ ���� �÷��� ����
    m_is_running = true;
}

Window::~Window()
{
}

bool Window::IsRunning()
{
    if (m_is_running)
        Broadcast();

    return m_is_running;
}

bool Window::Broadcast()
{
    if (!m_is_initialize)
    {
        SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)this);
        OnCreate();
        m_is_initialize = true;
    }

    OnUpdate();

    MSG msg = {};
    while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    ::Sleep(1);

    return true;
}

RECT Window::GetClientRect() const
{
    RECT rect;
    ::GetClientRect(m_hwnd, &rect);
    ::ClientToScreen(m_hwnd, (LPPOINT)&rect.left);
    ::ClientToScreen(m_hwnd, (LPPOINT)&rect.right);

    return rect;
}

RECT Window::GetScreenSize() const
{
    RECT rect;
    rect.right = GetSystemMetrics(SM_CXSCREEN);
    rect.bottom = GetSystemMetrics(SM_CYSCREEN);

    return rect;
}

void Window::OnCreate()
{
}

void Window::OnUpdate()
{
}

void Window::OnSize()
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
    m_is_running = false;
}

void Window::OnKeyUp(int key)
{
}

void Window::OnKeyDown(int key)
{
}

void Window::OnMouseMove(const Point& point)
{
}

void Window::OnLeftButtonUp(const Point& point)
{
}

void Window::OnLeftButtonDown(const Point& point)
{
}

void Window::OnRightButtonUp(const Point& point)
{
}

void Window::OnRightButtonDown(const Point& point)
{
}
