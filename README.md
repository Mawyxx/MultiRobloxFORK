# MultiRoblox (CMake Fork)

## О проекте
MultiRoblox — лёгкая WinAPI-утилита, позволяющая запускать несколько экземпляров Roblox одновременно. Этот форк переписан на C++ и собирается через CMake, сохраняя исходный функционал.

## Сборка
1. Установите [Visual Studio Build Tools](https://visualstudio.microsoft.com/downloads/) с компонентами C++ и CMake.
2. Клонируйте репозиторий и выполните команды:
   ```powershell
   cmake -S . -B build -G "Visual Studio 17 2022"
   cmake --build build --config Release
   ```
3. Готовый исполняемый файл появится в `build/Release/MultiRoblox.exe`.

## Использование
1. Запустите `MultiRoblox.exe` **до** запуска Roblox.
2. Оставляйте утилиту открытой, пока нужны несколько сессий Roblox.
3. После закрытия MultiRoblox все дополнительные инстансы закроются, останется один.

