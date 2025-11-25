# Font Loading Fix - Решение проблемы с загрузкой шрифтов

## Проблема / Problem

Текст не отображался в игре - только черный экран с зеленой рамкой.
Text was not appearing in the game - only black screen with green border.

## Причина / Root Cause

Исполняемый файл запускался из директории `build/`, но шрифты находились в корне проекта:
- Пути к шрифтам в коде: `images/Press_Start_2P/PressStart2P-Regular.ttf`
- Рабочая директория: `build/`
- Фактическое расположение: `../images/Press_Start_2P/PressStart2P-Regular.ttf`

The executable was running from `build/` directory, but fonts were in project root:
- Font paths in code: `images/Press_Start_2P/PressStart2P-Regular.ttf`
- Working directory: `build/`
- Actual location: `../images/Press_Start_2P/PressStart2P-Regular.ttf`

## Решение / Solution

### 1. Автоматическое копирование шрифтов при сборке

В `CMakeLists.txt` добавлены команды для копирования шрифтов в директорию сборки:

```cmake
# Copy fonts
add_custom_command(TARGET kkurs POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets/fonts
        ${CMAKE_BINARY_DIR}/assets/fonts
    COMMENT "Copying fonts to build directory"
)

# Copy font images (Press_Start_2P, Jersey_10)
add_custom_command(TARGET kkurs POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/images/Press_Start_2P
        ${CMAKE_BINARY_DIR}/images/Press_Start_2P
    COMMENT "Copying Press_Start_2P font to build directory"
)

add_custom_command(TARGET kkurs POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/images/Jersey_10
        ${CMAKE_BINARY_DIR}/images/Jersey_10
    COMMENT "Copying Jersey_10 font to build directory"
)
```

### 2. Структура после сборки / Build Directory Structure

После выполнения `./scripts/build.sh`:
```
build/
├── kkurs (executable)
├── assets/
│   └── fonts/
│       └── font.ttf
└── images/
    ├── Press_Start_2P/
    │   └── PressStart2P-Regular.ttf
    └── Jersey_10/
        └── Jersey10-Regular.ttf
```

## Использование / Usage

### Сборка проекта / Build Project
```bash
./scripts/build.sh
```

Шрифты автоматически копируются в `build/` директорию.
Fonts are automatically copied to `build/` directory.

### Запуск игры / Run Game
```bash
./scripts/run.sh
```

Скрипт запускает исполняемый файл из директории `build/`, где уже есть все необходимые шрифты.
Script runs the executable from `build/` directory where all fonts are available.

### Очистка / Clean
```bash
./scripts/clean.sh
```

Удаляет всю директорию `build/` включая скопированные шрифты. При следующей сборке шрифты будут скопированы заново.
Removes entire `build/` directory including copied fonts. Fonts will be re-copied on next build.

## Используемые шрифты / Used Fonts

1. **Press Start 2P** (`images/Press_Start_2P/PressStart2P-Regular.ttf`)
   - Используется в: MainMenuScene, RestScene, CharacterCreationScene
   - Стиль: Ретро-терминал, пиксельный шрифт
   - Used in: MainMenuScene, RestScene, CharacterCreationScene
   - Style: Retro terminal, pixel font

2. **Jersey 10** (`images/Jersey_10/Jersey10-Regular.ttf`)
   - Резервный шрифт
   - Backup font

3. **DejaVu Sans** (системный шрифт / system font)
   - Используется в некоторых сценах как fallback
   - Used in some scenes as fallback
   - Путь: `/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf`

## Файлы с загрузкой шрифтов / Files Loading Fonts

- `src/MainMenuScene.cpp` - Press Start 2P
- `src/CharacterCreationScene.cpp` - Press Start 2P
- `src/CharacterCreationMenu.cpp` - Press Start 2P
- `src/RestScene.cpp` - Press Start 2P
- `src/AbilityTreeScene.cpp` - DejaVu Sans Mono (system)
- `src/LocationScene.cpp` - DejaVu Sans (system)
- `src/CinematicEventScene.cpp` - DejaVu Sans (system)
- `src/TravelCinematicScene.cpp` - DejaVu Sans (system)
- `src/UI/HUDPanel.cpp` - DejaVu Sans (system)

## Проверка / Verification

Чтобы проверить, что шрифты скопированы:
To verify fonts are copied:

```bash
cd build
ls -la assets/fonts/
ls -la images/Press_Start_2P/
ls -la images/Jersey_10/
```

## Важные замечания / Important Notes

1. **Шрифты копируются автоматически** при каждой сборке
   **Fonts are copied automatically** with each build

2. **Не нужно вручную копировать шрифты** - CMake делает это сам
   **No need to manually copy fonts** - CMake handles this

3. **Скрипт запуска работает из корневой папки** проекта
   **Run script works from project root** directory

4. **При очистке билда шрифты удаляются** и копируются заново при следующей сборке
   **When cleaning build, fonts are removed** and re-copied on next build

## Тестирование / Testing

После сборки запустите игру и проверьте:
After building, run the game and check:

1. ✓ Главное меню отображает текст "69 CROSSROADS"
2. ✓ Пункты меню видны ("NEW GAME", "CONTINUE", "EXIT")
3. ✓ В меню создания персонажа виден текст
4. ✓ Все UI элементы отображают текст правильно

1. ✓ Main menu shows "69 CROSSROADS" text
2. ✓ Menu items are visible ("NEW GAME", "CONTINUE", "EXIT")
3. ✓ Character creation menu shows text
4. ✓ All UI elements display text correctly

## Дальнейшие улучшения / Future Improvements

Если нужно добавить новый шрифт:
To add a new font:

1. Положите файл шрифта в `assets/fonts/` или `images/`
2. Добавьте команду копирования в `CMakeLists.txt`
3. Используйте относительный путь в коде C++

1. Place font file in `assets/fonts/` or `images/`
2. Add copy command to `CMakeLists.txt`
3. Use relative path in C++ code
