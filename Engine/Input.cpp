#include "pch.h"
#include "Input.h"
#include "Window.h"

Input* Input::GetInstance()
{
    static Input instance;
    return &instance;
}

void Input::Update()
{
    POINT current_mouse_position = {};
    ::GetCursorPos(&current_mouse_position);

    if (m_first_time)
    {
        m_old_mouse_position = Point(current_mouse_position.x, current_mouse_position.y);
        m_first_time = false;
    }

    if (current_mouse_position.x != m_old_mouse_position.m_x || current_mouse_position.y != m_old_mouse_position.m_y)
    {
        // Mouse move event
        std::unordered_set<Window*>::iterator iter = m_set_listeners.begin();

        while (iter != m_set_listeners.end())
        {
            (*iter)->OnMouseMove(Point(current_mouse_position.x, current_mouse_position.y));
            ++iter;
        }
    }
    m_old_mouse_position = Point(current_mouse_position.x, current_mouse_position.y);

    for (unsigned int i = 0; i < 256; i++)
    {
        // �񵿱� Ű �Է�
        m_key_states[i] = ::GetAsyncKeyState(i);

        // Ű�� ���� ���
        if (m_key_states[i] & 0x8001)
        {
            std::unordered_set<Window*>::iterator iter = m_set_listeners.begin();

            while (iter != m_set_listeners.end())
            {
                if (i == VK_LBUTTON)
                {
                    if (m_key_states[i] != m_old_key_states[i])
                    {
                        (*iter)->OnLeftButtonDown(Point(current_mouse_position.x, current_mouse_position.y));
                    }
                }
                else if (i == VK_RBUTTON)
                {
                    if (m_key_states[i] != m_old_key_states[i])
                    {
                        (*iter)->OnRightButtonDown(Point(current_mouse_position.x, current_mouse_position.y));
                    }
                }
                else
                {
                    (*iter)->OnKeyDown(i);
                }
                ++iter;
            }
        }
        else
        {
            // Ű�� ���� ���°� ���� ���¿� ���� ���� ���
            if (m_key_states[i] != m_old_key_states[i])
            {
                std::unordered_set<Window*>::iterator iter = m_set_listeners.begin();

                while (iter != m_set_listeners.end())
                {
                    if (i == VK_LBUTTON)
                    {
                        (*iter)->OnLeftButtonUp(Point(current_mouse_position.x, current_mouse_position.y));
                    }
                    else if (i == VK_RBUTTON)
                    {
                        (*iter)->OnRightButtonUp(Point(current_mouse_position.x, current_mouse_position.y));
                    }
                    else
                    {
                        (*iter)->OnKeyUp(i);
                    }
                    ++iter;
                }
            }
        }
    }

    // ���� Ű ���¸� ���� Ű ���� ���ۿ� ����
    ::memcpy(m_old_key_states, m_key_states, sizeof(UCHAR) * 256);
}

void Input::AddListener(Window* listener)
{
    m_set_listeners.insert(listener);
}

void Input::RemoveListener(Window* listener)
{
    m_set_listeners.erase(listener);
}

void Input::SetCursorPosition(const Point& point)
{
    ::SetCursorPos(point.m_x, point.m_y);
}

void Input::ShowCursor(bool show)
{
    ::ShowCursor(show);
}
