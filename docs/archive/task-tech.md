# Task-Tech / Технические задачи

**One-sentence description:** Consolidated technical design specifications, implementation patterns, and detailed feature requirements that bridge user stories (tasks.md) with technical documentation (tech.md).

**Краткое описание:** Консолидированные технические спецификации дизайна, паттерны реализации и детальные требования к функциям, которые связывают пользовательские истории (tasks.md) с технической документацией (tech.md).

---

## Purpose / Назначение

This document serves as the **technical specification hub** that translates high-level tasks into concrete implementation requirements, architectural decisions, and coding patterns. It answers the question: **"How should we build this?"**

**Этот документ служит центром технических спецификаций**, который переводит высокоуровневые задачи в конкретные требования к реализации, архитектурные решения и паттерны кодирования. Он отвечает на вопрос: **"Как мы должны это построить?"**

### How This Will Be Used / Как это будет использоваться

- **Before Coding**: Reference technical specifications before starting implementation
- **Design Reviews**: Validate architectural decisions against project principles
- **Implementation Guide**: Step-by-step technical breakdown for complex features
- **Pattern Library**: Reusable design patterns and code structures
- **Integration Planning**: Understanding dependencies and component interactions
- **Technical Debt Tracking**: Document design compromises and future refactoring needs

**Использование в разработке:** справка перед кодированием, валидация архитектурных решений, пошаговое техническое описание сложных функций, библиотека паттернов, планирование интеграции, отслеживание технического долга.

---

## Document Structure / Структура документа

### 📋 Feature Specifications / Спецификации функций
<!-- Technical requirements for each feature from tasks.md -->

#### Template for Feature Specs:
- **Feature Name**: [Link to tasks.md]
- **Technical Approach**: Architecture and design pattern
- **Dependencies**: Required systems and components
- **Implementation Steps**: Ordered checklist
- **Testing Strategy**: How to verify it works
- **Integration Points**: How it connects with existing code

### 🏗️ Architecture Decisions / Архитектурные решения
<!-- Key architectural choices and their rationale -->

### 🔧 Implementation Patterns / Паттерны реализации
<!-- Reusable code patterns and best practices -->

### 🧩 Component Interfaces / Интерфейсы компонентов
<!-- API specifications and component contracts -->

### ⚡ Performance Considerations / Соображения производительности
<!-- Performance requirements and optimization strategies -->

### 🔐 Security Guidelines / Рекомендации по безопасности
<!-- Security patterns and vulnerability mitigation -->

### 🚨 Known Technical Debt / Известный технический долг
<!-- Design compromises requiring future refactoring -->

---

## Cross-References / Перекрёстные ссылки

### Core Project Documentation / Основная проектная документация
- **[docs/8_CORE_PRINCIPLES.md](./docs/8_CORE_PRINCIPLES.md)** - 8 обязательных принципов проекта / 8 mandatory project principles guiding technical decisions
- **[docs/tasks/MASTER_IMPLEMENTATION_PLAN.md](./docs/tasks/MASTER_IMPLEMENTATION_PLAN.md)** - Мастер-план этапного внедрения / Master implementation plan with feature documentation requirements

### Monolith Documentation Files / Файлы монолитной документации
- **[tasks.md](./tasks.md)** - User stories and requirements that this document provides technical specs for
- **[tech.md](./tech.md)** - Technical documentation for implemented systems described here
- **[progress.md](./progress.md)** - Implementation status of features specified here
- **[README.md](./README.md)** - High-level architecture overview and system design

### Additional Resources / Дополнительные ресурсы
- **[docs/TESTING_GUIDE.md](./docs/TESTING_GUIDE.md)** - Testing approaches for verification
- **[docs/PROMPT_TEMPLATE.md](./docs/PROMPT_TEMPLATE.md)** - Templates for creating technical specifications

### Subdirectory References / Ссылки на подкаталоги
- **[docs/task-tech/](./docs/task-tech/)** - Detailed stage-specific technical specifications
  - `STAGE2_SIDEVIEW_GAMEPLAY.md` - SideView scene technical details
  - `STAGE5_ROAD_EVENT_MANAGER.md` - Event system specifications
  - `TEMPLATE.md` - Template for creating new task-tech documents

---

## History & Change Log / История и журнал изменений

| Date | PR | Author | Changes |
|------|-----|---------|---------|
| 2025-11-14 | docs/initial-monoliths | GitHub Copilot | Enhanced cross-references to 8_CORE_PRINCIPLES.md and MASTER_IMPLEMENTATION_PLAN.md |
| 2025-11-14 | Initial | GitHub Copilot | Created foundational technical specification document with structure and purpose |

### Guidelines for Updates / Рекомендации по обновлению

- Create new sections for each major feature or system being designed
- Update specifications when architectural decisions change
- Link to specific task items in `tasks.md` for traceability
- Document **why** decisions were made, not just **what** was decided
- Include code examples and diagrams for complex patterns
- Cross-reference with `tech.md` once features are implemented
- Track technical debt and refactoring needs explicitly

---

## Notes / Примечания

**Documentation Flow / Поток документации:**

```
tasks.md           →  task-tech.md      →  Implementation  →  tech.md         →  progress.md
(What to build)       (How to build)       (Coding)           (How it works)      (What's done)
```

**This document is the "How" bridge between "What" and implementation.**  
**Этот документ — мост "Как" между "Что" и реализацией.**

### When to Update This Document / Когда обновлять этот документ

- ✅ **Before starting implementation** of a new feature
- ✅ When making **significant architectural decisions**
- ✅ When discovering **design patterns** worth reusing
- ✅ When encountering **technical challenges** needing documentation
- ❌ Not for logging completed work (use `progress.md`)
- ❌ Not for user-facing feature lists (use `README.md`)

### Relationship to Code / Отношение к коду

- **Specifications here** should be detailed enough to guide implementation
- **Code in src/** is the actual implementation of these specifications
- **tech.md** documents how the implemented code actually works
- This document may **precede code** (design phase) or **follow code** (documentation of existing patterns)
