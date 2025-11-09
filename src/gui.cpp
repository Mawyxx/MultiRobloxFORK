#include "gui.hpp"

#include <string>
#include <string_view>
#include <utility>

namespace
{
    class UniqueHandle
    {
    public:
        explicit UniqueHandle(HANDLE handle = nullptr) noexcept
            : m_handle(handle)
        {
        }

        ~UniqueHandle()
        {
            reset();
        }

        UniqueHandle(const UniqueHandle&) = delete;
        UniqueHandle& operator=(const UniqueHandle&) = delete;

        UniqueHandle(UniqueHandle&& other) noexcept
            : m_handle(std::exchange(other.m_handle, nullptr))
        {
        }

        UniqueHandle& operator=(UniqueHandle&& other) noexcept
        {
            if (this != &other)
            {
                reset(std::exchange(other.m_handle, nullptr));
            }
            return *this;
        }

        [[nodiscard]] HANDLE get() const noexcept
        {
            return m_handle;
        }

        [[nodiscard]] bool valid() const noexcept
        {
            return m_handle != nullptr;
        }

        void reset(HANDLE handle = nullptr) noexcept
        {
            if (m_handle)
            {
                CloseHandle(m_handle);
            }
            m_handle = handle;
        }

        explicit operator bool() const noexcept
        {
            return valid();
        }

    private:
        HANDLE m_handle;
    };
}

namespace
{
    constexpr wchar_t kWindowClassName[] = L"MULTIROBLOX_CLASS";
    constexpr wchar_t kWindowTitle[] = L"MultiRoblox";
    constexpr wchar_t kTitleCaption[] = L"MultiRoblox 0.0.3";
    constexpr wchar_t kInstructions[] =
        L"Thanks for using MultiRoblox!\r\n"
        L"\r\n"
        L"In case if you're wondering, you must open\r\n"
        L"MultiRoblox FIRST before opening multiple ROBLOX\r\n"
        L"sessions. Once you close MultiRoblox, all but one\r\n"
        L"ROBLOX instance will close.\r\n"
        L"\r\n"
        L"Discord: https://discord.gg/g2Jgw8Z6Rb\r\n"
        L"WeAreDevs: https://forum.wearedevs.net/t/29695\r\n"
        L"GitHub: https://github.com/Dashbloxx/MultiRoblox\r\n";

    constexpr std::wstring_view kMutexName = L"ROBLOX_singletonEvent";

    HWND g_mainWindow = nullptr;
    HWND g_titleLabel = nullptr;
    HWND g_textBox = nullptr;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProcW(hwnd, uMsg, wParam, lParam);
    }
}

namespace
{
    bool RegisterWindowClass(HINSTANCE instance)
    {
        WNDCLASSW wc{};
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = instance;
        wc.lpszClassName = kWindowClassName;

        return RegisterClassW(&wc) != 0;
    }

    void CreateControls(HINSTANCE instance)
    {
        g_titleLabel = CreateWindowExW(
            0, L"STATIC", kTitleCaption,
            WS_VISIBLE | WS_CHILD,
            10, 10, 365, 20,
            g_mainWindow, nullptr, instance, nullptr);

        g_textBox = CreateWindowExW(
            WS_EX_CLIENTEDGE,
            L"EDIT", kInstructions,
            WS_VISIBLE | WS_CHILD | ES_LEFT | ES_AUTOVSCROLL | ES_MULTILINE | ES_READONLY | WS_VSCROLL,
            10, 40, 365, 365,
            g_mainWindow, nullptr, instance, nullptr);
    }
}

int RunGui(HINSTANCE instance, int showCommand)
{
    UniqueHandle mutexHandle(CreateMutexW(nullptr, TRUE, kMutexName.data()));
    if (!mutexHandle)
    {
        MessageBoxW(nullptr, L"Не удалось создать или открыть мьютекс ROBLOX.", kWindowTitle, MB_ICONERROR | MB_OK);
        return static_cast<int>(GetLastError());
    }

    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        MessageBoxW(nullptr, L"MultiRoblox уже запущен в другой сессии.", kWindowTitle, MB_ICONINFORMATION | MB_OK);
        return static_cast<int>(ERROR_ALREADY_EXISTS);
    }

    if (!RegisterWindowClass(instance))
    {
        MessageBoxW(nullptr, L"Регистрация оконного класса завершилась ошибкой.", kWindowTitle, MB_ICONERROR | MB_OK);
        return static_cast<int>(GetLastError());
    }

    g_mainWindow = CreateWindowExW(
        0,
        kWindowClassName,
        kWindowTitle,
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 455,
        nullptr, nullptr, instance, nullptr);

    if (!g_mainWindow)
    {
        MessageBoxW(nullptr, L"Создание главного окна завершилось ошибкой.", kWindowTitle, MB_ICONERROR | MB_OK);
        return static_cast<int>(GetLastError());
    }

    CreateControls(instance);
    ShowWindow(g_mainWindow, showCommand);
    UpdateWindow(g_mainWindow);

    MSG message{};
    while (GetMessageW(&message, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }

    return static_cast<int>(message.wParam);
}

