# План Очистки Репозитория / Repository Cleanup Plan

**Дата / Date:** 2025-11-12  
**Цель / Goal:** Подготовить репозиторий к разработке с нуля, оставив только техническое задание

---

## Принципы Очистки / Cleanup Principles

### Что Сохраняется / What Stays

**4 Основных Файла Взаимодействия:**
1. ✅ `docs/progress.md` - Отслеживание прогресса разработки
2. ✅ `docs/task-tech.md` - Технические спецификации задач
3. ✅ `docs/tasks.md` - Описание задач и требований
4. ✅ `docs/tech.md` - Архитектура и технические решения

**Служебные Файлы:**
5. ✅ `docs/PROMPT_TEMPLATE.md` - Шаблон для постановки задач (НОВЫЙ)
6. ✅ `docs/README.md` - Индекс документации
7. ✅ `README.md` (корень) - Главный README проекта

**Необходимая Техническая Документация:**
- `docs/tech/` - Подробные технические спецификации (сохранить как reference)
- `docs/task-tech/` - Детальные спецификации задач (сохранить как reference)
- `docs/tasks/` - Подробные описания задач (сохранить как reference)

### Что Архивируется / What Gets Archived

**Исторические Файлы Прогресса:**
- `docs/progress/ARCHITECTURE_INTEGRATION_VERIFICATION.md` → АРХИВ
- `docs/progress/ARCHITECTURE_VERIFICATION.md` → АРХИВ
- `docs/progress/IMPLEMENTATION_COMPLETE.md` → АРХИВ
- `docs/progress/IMPLEMENTATION_SUMMARY.md` → АРХИВ
- `docs/progress/IMPLEMENTATION_SUMMARY_FINAL.md` → АРХИВ
- `docs/progress/IMPLEMENTATION_SUMMARY_RU.md` → АРХИВ
- `docs/progress/IMPLEMENTATION_VERIFICATION.md` → АРХИВ
- `docs/progress/INTEGRATION_COMPLETE_SUMMARY.md` → АРХИВ
- `docs/progress/MERGE_SUMMARY.md` → АРХИВ
- `docs/progress/STAGE4_COMPLETION_SUMMARY.md` → АРХИВ
- `docs/progress/STAGE7_VERIFICATION_REPORT.md` → АРХИВ
- `docs/progress/VERIFICATION_REPORT.md` → АРХИВ
- `docs/progress/CLEANUP_REPORT.md` → АРХИВ
- `docs/progress/MERGE_PROGRESS.md` → АРХИВ
- `docs/progress/2025-11-12_DOCUMENTATION_RESTRUCTURING.md` → АРХИВ

**Служебные Файлы Агентов:**
- `docs/progress/AGENT_SESSION.md` → АРХИВ
- `docs/progress/SESSION_STATE.md` → АРХИВ
- `docs/progress/DEVELOPMENT_LOG.md` → АРХИВ
- `docs/progress/TEMPLATE.md` → АРХИВ

**Устаревшая Документация:**
- `docs/CONSOLIDATED_DOCS_README.md` → УДАЛИТЬ (устарел)
- `docs/MONOLITHIC_DOCS_VERIFICATION.md` → АРХИВ

### Что Удаляется Полностью / What Gets Deleted

**Дубликаты и Временные Файлы:**
- Файлы с суффиксами `_old`, `_backup`, `_temp`
- Пустые или неиспользуемые файлы
- Автогенерированные скриншоты документации

---

## Структура После Очистки / Structure After Cleanup

```
docs/
├── README.md                    # Индекс документации
├── PROMPT_TEMPLATE.md           # Шаблон для постановки задач
│
├── progress.md                  # ОСНОВНОЙ: Прогресс разработки
├── task-tech.md                 # ОСНОВНОЙ: Технические спецификации
├── tasks.md                     # ОСНОВНОЙ: Задачи и требования
├── tech.md                      # ОСНОВНОЙ: Архитектура и технологии
│
├── progress/                    # Папка прогресса (только архив)
│   └── archived_*.md           # Архивные файлы прогресса
│
├── tech/                        # Детальные технические спецификации
│   ├── system-architecture.md
│   ├── GRID_INVENTORY_SYSTEM.md
│   ├── HUD_IMPLEMENTATION.md
│   ├── UI_INTEGRATION_GUIDE.md
│   └── ...                     # Другие технические документы
│
├── task-tech/                   # Детальные спецификации задач
│   ├── STAGE5_ROAD_EVENT_MANAGER.md
│   ├── ROAD_EVENT_MANAGER_INTEGRATION.md
│   └── ...
│
└── tasks/                       # Детальные описания задач
    ├── MASTER_IMPLEMENTATION_PLAN.md
    ├── ROADSCENE_STAGE4_README.md
    └── ...
```

---

## План Выполнения / Execution Plan

### Шаг 1: Создание Архивной Папки
```bash
mkdir -p docs/progress/archived
```

### Шаг 2: Архивирование Исторических Файлов
```bash
mv docs/progress/ARCHITECTURE_INTEGRATION_VERIFICATION.md docs/progress/archived/
mv docs/progress/ARCHITECTURE_VERIFICATION.md docs/progress/archived/
mv docs/progress/IMPLEMENTATION_*.md docs/progress/archived/
mv docs/progress/INTEGRATION_COMPLETE_SUMMARY.md docs/progress/archived/
mv docs/progress/MERGE_*.md docs/progress/archived/
mv docs/progress/STAGE*_*.md docs/progress/archived/
mv docs/progress/VERIFICATION_REPORT.md docs/progress/archived/
mv docs/progress/CLEANUP_REPORT.md docs/progress/archived/
mv docs/progress/AGENT_SESSION.md docs/progress/archived/
mv docs/progress/SESSION_STATE.md docs/progress/archived/
mv docs/progress/DEVELOPMENT_LOG.md docs/progress/archived/
mv docs/progress/2025-11-12_DOCUMENTATION_RESTRUCTURING.md docs/progress/archived/
```

### Шаг 3: Удаление Устаревших Файлов
```bash
rm docs/CONSOLIDATED_DOCS_README.md
```

### Шаг 4: Архивирование Устаревшей Верификации
```bash
mv docs/MONOLITHIC_DOCS_VERIFICATION.md docs/progress/archived/
```

### Шаг 5: Обновление README.md в docs
Обновить индекс документации с указанием на 4 основных файла + промт-шаблон.

---

## Результат / Result

**До очистки:**
- ~62 markdown файла
- Множество дубликатов и верификационных отчетов
- Сложно найти актуальную информацию

**После очистки:**
- 4 основных файла для работы
- 1 файл с шаблонами промтов
- Чистая структура reference документации
- Архив исторических файлов для справки

**Преимущества:**
1. ✅ Легко найти актуальную документацию
2. ✅ Понятная структура для новых разработчиков
3. ✅ История сохранена в архиве
4. ✅ Нет дубликатов
5. ✅ Фокус на 4 основных файлах + промт-шаблоне

---

## Следующие Шаги / Next Steps

1. **Выполнить очистку** по плану выше
2. **Обновить docs/README.md** с новой структурой
3. **Обновить главный README.md** со ссылками на основные файлы
4. **Проверить** что все ссылки работают
5. **Закоммитить изменения** с описанием очистки

---

**Статус:** Готов к выполнению  
**Влияние:** Упрощение навигации, сохранение истории
