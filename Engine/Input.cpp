#include "pch.h"
#include "Input.h"
#include "Vector2.h"

Input::Input()
{
}

Input::~Input()
{
}

void Input::Update()
{
    POINT current_mouse_position = {};
    ::GetCursorPos(&current_mouse_position);

    if (current_mouse_position.x != m_old_mouse_position.m_x || current_mouse_position.y != m_old_mouse_position.m_y)
    {
        m_delta_mouse_position = Vector2(static_cast<float>(current_mouse_position.x) - static_cast<float>(m_old_mouse_position.m_x), static_cast<float>(current_mouse_position.y) - static_cast<float>(m_old_mouse_position.m_y));
    }
    else
    {
        m_delta_mouse_position = Vector2(0.0f, 0.0f);
    }

    if (!m_cursor_locked)
        m_old_mouse_position = Vector2(static_cast<float>(current_mouse_position.x), static_cast<float>(current_mouse_position.y));
    else
    {
        ::SetCursorPos(static_cast<int>(m_lock_area_center.m_x), static_cast<int>(m_lock_area_center.m_y));
        m_old_mouse_position = m_lock_area_center;
    }

    for (UINT i = 0; i < 256; i++)
    {
        // 비동기 키 입력
        m_key_states[i] = ::GetAsyncKeyState(i);

        // 키가 눌린 경우
        if (m_key_states[i] & 0x8001)
        {
            m_final_key_states[i] = 0;
        }
        else
        {
            // 키의 현재 상태가 이전 상태와 같지 않은 경우
            if (m_key_states[i] != m_old_key_states[i])
            {
                m_final_key_states[i] = 1;
            }
            else
            {
                m_final_key_states[i] = 2;
            }
        }
    }

    // 현재 키 상태를 이전 키 상태 버퍼에 저장
    ::memcpy(m_old_key_states, m_key_states, sizeof(short) * 256);
}

void Input::LockCursor(bool lock)
{
    m_cursor_locked = lock;

    if (lock)
        while (::ShowCursor(false) >= 0);
    else
        while (::ShowCursor(true) <= 1);
}

void Input::SetLockArea(const Rect& area)
{
    m_lock_area = area;
    m_lock_area_center = Vector2(std::floor(area.m_left + static_cast<float>(area.m_width) / 2.0f), std::floor(area.m_top + static_cast<float>(area.m_height) / 2.0f));
}

bool Input::IsKeyUp(const Key& key)
{
    return (m_final_key_states[GetKeyCode(key)] == 1);
}

bool Input::IsKeyDown(const Key& key)
{
    return (m_final_key_states[GetKeyCode(key)] == 0);
}

Vector2 Input::GetDeltaMousePosition()
{
    return m_delta_mouse_position;
}

short Input::GetKeyCode(const Key& key)
{
    short result = 0;

    if (key >= Key::A && key <= Key::Z)
        result = 'A' + ((short)key - (short)Key::A);
    else if (key >= Key::_0 && key <= Key::_9)
        result = '0' + ((short)key - (short)Key::_0);
    else if (key == Key::Shift)
        result = VK_SHIFT;
    else if (key == Key::Escape)
        result = VK_ESCAPE;
    else if (key == Key::Space)
        result = VK_SPACE;
    else if (key == Key::Enter)
        result = VK_RETURN;
    else if (key == Key::LeftMouseButton)
        result = VK_LBUTTON;
    else if (key == Key::MiddleMouseButton)
        result = VK_MBUTTON;
    else if (key == Key::RightMouseButton)
        result = VK_RBUTTON;

    return result;
}
