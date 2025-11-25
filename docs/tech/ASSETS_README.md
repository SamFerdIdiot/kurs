# Road Trip Adventure RPG - Asset List
# Список ассетов для Road Trip Adventure RPG

**Version:** 1.0  
**Date:** 2025-11-12  
**Target Resolution:** 1366x768  
**Frame Rate:** 60 FPS

---

## Table of Contents / Содержание

### English Sections
1. [Overview](#overview)
2. [Asset Categories](#asset-categories)
3. [Sprites & Characters](#sprites--characters)
4. [Environment & Backgrounds](#environment--backgrounds)
5. [UI Elements](#ui-elements)
6. [Animations](#animations)
7. [Fonts & Typography](#fonts--typography)
8. [Audio Assets](#audio-assets)
9. [Technical Specifications](#technical-specifications)

### Russian Sections / Русские разделы
10. [Обзор ассетов](#обзор-ассетов)
11. [Спрайты персонажей](#спрайты-персонажей)
12. [Окружение и фоны](#окружение-и-фоны)
13. [Элементы интерфейса](#элементы-интерфейса)
14. [Анимации](#анимации)
15. [Технические требования](#технические-требования)

---

## Overview

This document provides a comprehensive list of all required assets for the final implementation of Road Trip Adventure RPG based on the analysis of Stages 1-9. Assets are categorized by type and include technical specifications, usage context, and priority levels.

**Priority Levels:**
- 🔴 **Critical** - Required for core gameplay functionality
- 🟡 **High** - Important for complete game experience
- 🟢 **Medium** - Enhances gameplay but not essential
- ⚪ **Low** - Nice to have, future enhancement

---

## Asset Categories

### Summary by Category

| Category | Critical | High | Medium | Low | Total |
|----------|----------|------|--------|-----|-------|
| Sprites & Characters | 15 | 12 | 8 | 5 | 40 |
| Environment & Backgrounds | 10 | 15 | 10 | 8 | 43 |
| UI Elements | 25 | 18 | 12 | 6 | 61 |
| Animations | 8 | 10 | 15 | 10 | 43 |
| Fonts | 2 | 1 | 0 | 0 | 3 |
| Audio | 5 | 15 | 20 | 15 | 55 |
| **TOTAL** | **65** | **71** | **65** | **44** | **245** |

---

## Sprites & Characters

### Player Vehicles (🔴 Critical)

**1. Old Sedan (Седан)**
- **Format:** PNG with transparency
- **Size:** 64x32 pixels (top-down), 128x64 pixels (side view)
- **States Required:**
  - Idle
  - Moving (subtle animation, 2-3 frames)
  - Damaged (visual damage indicators)
- **Color Variants:** Blue, Red, Gray
- **File Count:** 3 variants × 3 states × 2 views = 18 files
- **Usage:** Character Creation Scene, Driving Scene, Side View Scene
- **Notes:** Main player vehicle option with 3 passenger slots

**2. Offroad Van (Внедорожник)**
- **Format:** PNG with transparency
- **Size:** 80x40 pixels (top-down), 160x80 pixels (side view)
- **States Required:**
  - Idle
  - Moving (2-3 frames)
  - Damaged
- **Color Variants:** Green, Brown, Gray
- **File Count:** 3 variants × 3 states × 2 views = 18 files
- **Usage:** Character Creation Scene, Driving Scene, Side View Scene
- **Notes:** Larger vehicle with 2 passenger slots, higher cargo capacity

**3. Sports Coupe (Спортивное купе)**
- **Format:** PNG with transparency
- **Size:** 64x28 pixels (top-down), 128x56 pixels (side view)
- **States Required:**
  - Idle
  - Moving (2-3 frames)
  - Damaged
- **Color Variants:** Red, Black, Yellow
- **File Count:** 3 variants × 3 states × 2 views = 18 files
- **Usage:** Character Creation Scene, Driving Scene, Side View Scene
- **Notes:** Fast vehicle with 3 passenger slots

**Total Vehicle Sprites:** 54 files

### NPC Characters (🟡 High)

**4. NPC Portraits - Mechanic (Механик)**
- **Format:** PNG with transparency
- **Size:** 128x128 pixels (portrait), 32x48 pixels (sprite)
- **Variants:** 3 different characters
- **Expressions:** Neutral, Happy, Concerned (3 per character)
- **File Count:** 3 characters × 3 expressions = 9 portraits, 3 sprites
- **Usage:** Dialogue Scene, NPC Management Screen
- **Notes:** Shows character during conversations and in team roster

**5. NPC Portraits - Trader (Торговец)**
- **Format:** PNG with transparency
- **Size:** 128x128 pixels (portrait), 32x48 pixels (sprite)
- **Variants:** 3 different characters
- **Expressions:** Neutral, Happy, Suspicious
- **File Count:** 9 portraits, 3 sprites
- **Usage:** Dialogue Scene, NPC Management Screen

**6. NPC Portraits - Companion (Компаньон)**
- **Format:** PNG with transparency
- **Size:** 128x128 pixels (portrait), 32x48 pixels (sprite)
- **Variants:** 3 different characters
- **Expressions:** Neutral, Happy, Tired
- **File Count:** 9 portraits, 3 sprites
- **Usage:** Dialogue Scene, NPC Management Screen

**7. NPC Portraits - Medic (Медик)**
- **Format:** PNG with transparency
- **Size:** 128x128 pixels (portrait), 32x48 pixels (sprite)
- **Variants:** 2 different characters
- **Expressions:** Neutral, Concerned, Professional
- **File Count:** 6 portraits, 2 sprites
- **Usage:** Dialogue Scene, NPC Management Screen

**8. NPC Portraits - Navigator (Навигатор)**
- **Format:** PNG with transparency
- **Size:** 128x128 pixels (portrait), 32x48 pixels (sprite)
- **Variants:** 2 different characters
- **Expressions:** Neutral, Focused, Excited
- **File Count:** 6 portraits, 2 sprites
- **Usage:** Dialogue Scene, NPC Management Screen

**Total NPC Portraits:** 39 portraits + 13 sprites = 52 files

### Traffic Vehicles (🟡 High)

**9. Oncoming Traffic (Встречный транспорт)**
- **Format:** PNG with transparency
- **Size:** 128x64 pixels (side view only)
- **Variants:** 
  - Sedan (2 colors)
  - Truck (2 colors)
  - Van (2 colors)
- **File Count:** 6 vehicles
- **Usage:** Side View Scene traffic system
- **Animation:** Single frame, movement handled by code

**10. Passing Traffic (Попутный транспорт)**
- **Format:** PNG with transparency
- **Size:** 128x64 pixels (side view only)
- **Variants:** 
  - Fast car (2 colors)
  - Motorcycle (2 colors)
  - Bus (2 colors)
- **File Count:** 6 vehicles
- **Usage:** Side View Scene traffic system

**Total Traffic Sprites:** 12 files

### Inventory Items (🔴 Critical)

**11. Item Sprites - Food Category**
- **Format:** PNG with transparency
- **Size:** 32x32 pixels base, scaled per grid size
- **Items:**
  - Canned Food (1x1) - 3 variants
  - Burger (1x1)
  - Pizza Slice (1x1)
  - Energy Bar (1x1)
- **File Count:** 6 items
- **Usage:** Inventory System, GridInventorySystem

**12. Item Sprites - Tools Category**
- **Format:** PNG with transparency
- **Size:** 32x32 pixels per cell
- **Items:**
  - Wrench (2x1)
  - Toolbox (3x2)
  - Crowbar (1x3)
  - Screwdriver (1x2)
  - Hammer (1x2)
- **File Count:** 5 items
- **Usage:** Inventory System, GridInventorySystem

**13. Item Sprites - Spare Parts**
- **Format:** PNG with transparency
- **Size:** 32x32 pixels per cell
- **Items:**
  - Spare Tire (2x2)
  - Engine Part (2x2)
  - Fuel Canister (1x2) - 2 variants
  - Battery (2x1)
  - Oil Can (1x1)
- **File Count:** 6 items
- **Usage:** Inventory System

**14. Item Sprites - Medical**
- **Format:** PNG with transparency
- **Size:** 32x32 pixels per cell
- **Items:**
  - First Aid Kit (2x1)
  - Medicine Pills (1x1)
  - Bandages (1x1)
- **File Count:** 3 items
- **Usage:** Inventory System

**15. Item Sprites - Quest Items**
- **Format:** PNG with transparency
- **Size:** Variable (1x1 to 2x2)
- **Items:**
  - Map (1x1)
  - Compass (1x1)
  - Documents (2x1)
  - Package (2x2)
- **File Count:** 4 items
- **Usage:** Inventory System, Quest System

**Total Inventory Item Sprites:** 24 files

---

## Environment & Backgrounds

### World Map Elements (🔴 Critical)

**16. Road Tiles**
- **Format:** PNG tileset
- **Tile Size:** 64x64 pixels
- **Variants:**
  - Straight horizontal road
  - Straight vertical road
  - Corner (4 directions)
  - T-junction (4 directions)
  - Crossroads
  - Highway (wider, 2 lanes each direction)
- **File Count:** 1 tileset with 12 tiles
- **Usage:** World Map Scene, procedural road generation

**17. World Map Background**
- **Format:** PNG
- **Size:** 1366x768 pixels (full screen)
- **Variants:**
  - Desert terrain
  - Forest terrain
  - Mountain terrain
  - Coastal terrain
- **File Count:** 4 backgrounds
- **Usage:** World Map Scene
- **Notes:** Tiling or large single image for map background

### Location Structures (🔴 Critical)

**18. Gas Station (Заправка)**
- **Format:** PNG with transparency
- **Size:** 128x96 pixels (top-down), 256x192 pixels (side view)
- **Variants:** 2 different designs
- **File Count:** 2 × 2 views = 4 files
- **Usage:** Driving Scene, Side View Scene
- **Icon:** 32x32 pixels for HUD/map

**19. Store/Shop (Магазин)**
- **Format:** PNG with transparency
- **Size:** 96x80 pixels (top-down), 192x160 pixels (side view)
- **Variants:** 2 different designs
- **File Count:** 4 files
- **Usage:** Driving Scene, Side View Scene
- **Icon:** 32x32 pixels

**20. Motel (Мотель)**
- **Format:** PNG with transparency
- **Size:** 160x120 pixels (top-down), 320x240 pixels (side view)
- **Variants:** 2 different designs
- **File Count:** 4 files
- **Usage:** Driving Scene, Side View Scene
- **Icon:** 32x32 pixels

**21. Garage/Mechanic (Гараж/Механик)**
- **Format:** PNG with transparency
- **Size:** 120x100 pixels (top-down), 240x200 pixels (side view)
- **Variants:** 2 different designs
- **File Count:** 4 files
- **Usage:** Driving Scene, Side View Scene
- **Icon:** 32x32 pixels

**22. Diner (Закусочная)**
- **Format:** PNG with transparency
- **Size:** 96x80 pixels (top-down), 192x160 pixels (side view)
- **Variants:** 2 different designs
- **File Count:** 4 files
- **Usage:** Driving Scene, Side View Scene
- **Icon:** 32x32 pixels

**23. Rest Stop (Остановка для отдыха)**
- **Format:** PNG with transparency
- **Size:** 80x64 pixels (top-down), 160x128 pixels (side view)
- **Variants:** 2 different designs
- **File Count:** 4 files
- **Usage:** Side View Scene
- **Icon:** 32x32 pixels

**24. Billboard (Рекламный щит)**
- **Format:** PNG with transparency
- **Size:** 64x96 pixels (side view only)
- **Variants:** 4 different advertisements
- **File Count:** 4 files
- **Usage:** Side View Scene (decorative)

**25. Home Location (Дом)**
- **Format:** PNG with transparency
- **Size:** 128x128 pixels (top-down)
- **Variants:** 1 unique design
- **File Count:** 1 file
- **Usage:** Start Zone in Driving Scene
- **Icon:** 32x32 pixels
- **Notes:** Player's starting location

**Total Structure Sprites:** 33 files + 8 icons = 41 files

### Parallax Backgrounds (🟡 High)

**26. Sky Layer (Слой неба)**
- **Format:** PNG
- **Size:** 1366x300 pixels
- **Variants:**
  - Clear day
  - Cloudy
  - Sunset
  - Night
- **File Count:** 4 files
- **Usage:** Side View Scene Layer 1
- **Speed:** 0.1x (slowest parallax)

**27. Mountain/Hills Layer (Горы/Холмы)**
- **Format:** PNG with transparency
- **Size:** 1366x200 pixels
- **Variants:**
  - Desert mountains
  - Forest hills
  - Rocky mountains
- **File Count:** 3 files
- **Usage:** Side View Scene Layer 2
- **Speed:** 0.3x

**28. Tree/Vegetation Layer (Деревья/Растительность)**
- **Format:** PNG with transparency
- **Size:** Seamless tiling, 512x200 pixels
- **Variants:**
  - Pine trees
  - Desert cacti
  - Deciduous trees
  - Bushes
- **File Count:** 4 files
- **Usage:** Side View Scene Layer 3
- **Speed:** 0.5x

**29. Road Layer (Дорога)**
- **Format:** PNG
- **Size:** 1366x150 pixels (repeating)
- **Variants:**
  - Highway (2-lane)
  - Highway (4-lane)
  - Damaged road
- **File Count:** 3 files
- **Usage:** Side View Scene Layer 4
- **Speed:** 1.0x (matches player speed)

**30. Ground Layer (Земля)**
- **Format:** PNG
- **Size:** 1366x200 pixels (repeating)
- **Variants:**
  - Desert
  - Grass
  - Rocky
- **File Count:** 3 files
- **Usage:** Side View Scene Layer 5
- **Speed:** 1.0x

**Total Parallax Backgrounds:** 17 files

### Scene Backgrounds (🟡 High)

**31. Main Menu Background**
- **Format:** PNG
- **Size:** 1366x768 pixels
- **Variants:** 1 atmospheric background
- **File Count:** 1 file
- **Usage:** Main Menu Scene
- **Notes:** Should evoke road trip atmosphere

**32. Character Creation Background**
- **Format:** PNG
- **Size:** 1366x768 pixels
- **Variants:** 1 background showing garage/preparation scene
- **File Count:** 1 file
- **Usage:** Character Creation Scene

**33. Motel Room Interior**
- **Format:** PNG
- **Size:** 1366x768 pixels
- **Variants:** 1 cozy motel room
- **File Count:** 1 file
- **Usage:** Motel Scene (rest, save, inventory)

**34. Town Interior**
- **Format:** PNG
- **Size:** 1366x768 pixels
- **Variants:** 1 small town center
- **File Count:** 1 file
- **Usage:** Town Scene

**Total Scene Backgrounds:** 4 files

---

## UI Elements

### HUD Components (🔴 Critical)

**35. HUD Panel Frame**
- **Format:** PNG with transparency
- **Size:** 1366x120 pixels (full width top bar)
- **Variants:** 1 retro-styled panel
- **File Count:** 1 file
- **Usage:** All gameplay scenes (Driving, Side View)
- **Notes:** Retro LCD-style panel with borders and segments

**36. Resource Bars (Полоски ресурсов)**
- **Format:** PNG with transparency
- **Size:** 200x30 pixels (horizontal bar)
- **Variants:**
  - Energy bar (blue/green gradient)
  - Fuel bar (yellow/orange gradient)
  - Empty bar background
  - Bar frame/border
- **File Count:** 4 files
- **Usage:** HUD Panel display
- **Notes:** Can be filled dynamically or use sprite sheets

**37. Status Icons (Иконки состояния)**
- **Format:** PNG with transparency
- **Size:** 32x32 pixels each
- **Icons:**
  - Energy (⚡)
  - Money ($)
  - Fuel (⛽)
  - Temperature (🌡️)
  - Warning (⚠️)
  - Speed (➤)
  - Status OK (✓)
- **File Count:** 7 icons
- **Usage:** HUD Panel

**38. LCD Display Background**
- **Format:** PNG
- **Size:** 400x60 pixels
- **Variants:** 1 retro LCD screen style
- **File Count:** 1 file
- **Usage:** Center section of HUD for messages
- **Notes:** Dark background with slight greenish tint

**Total HUD Components:** 13 files

### Inventory UI (🔴 Critical)

**39. Inventory Grid Background**
- **Format:** PNG
- **Size:** 600x400 pixels (6x4 cells @ 80px per cell + padding)
- **Variants:** 1 grid background with cell borders
- **File Count:** 1 file
- **Usage:** Inventory Scene, GridInventorySystem
- **Notes:** Clear cell boundaries, dark background

**40. Cell Highlight Overlays**
- **Format:** PNG with transparency
- **Size:** 80x80 pixels (single cell)
- **Variants:**
  - Valid placement (green, 50% opacity)
  - Invalid placement (red, 50% opacity)
  - Selected cell (yellow, 30% opacity)
  - Hovered cell (blue, 20% opacity)
- **File Count:** 4 overlay variants
- **Usage:** GridInventorySystem visual feedback

**41. Item Rarity Borders**
- **Format:** PNG with transparency
- **Size:** Variable (matches item size + 4px border)
- **Variants:**
  - Common (gray)
  - Uncommon (green)
  - Rare (blue)
  - Epic (purple)
  - Legendary (orange)
- **File Count:** 5 border templates
- **Usage:** Inventory items display

**42. Inventory Side Panel**
- **Format:** PNG
- **Size:** 400x600 pixels
- **Variants:** 1 panel for item details
- **File Count:** 1 file
- **Usage:** Right side of inventory showing selected item info
- **Notes:** Retro panel style matching HUD

**43. Weight/Capacity Display**
- **Format:** PNG with transparency
- **Size:** 300x40 pixels
- **Variants:** 1 bar for weight visualization
- **File Count:** 1 file
- **Usage:** Bottom of inventory screen

**Total Inventory UI:** 12 files

### Menu Elements (🔴 Critical)

**44. Menu Panel Background**
- **Format:** PNG with transparency
- **Size:** 800x600 pixels (centered panel)
- **Variants:**
  - Standard menu panel
  - Dialogue panel
  - Confirmation panel
- **File Count:** 3 panel variants
- **Usage:** Main Menu, Pause Menu, Dialogue Scene

**45. Button States**
- **Format:** PNG with transparency
- **Size:** 400x60 pixels
- **Variants:**
  - Normal state
  - Hovered state
  - Pressed state
  - Disabled state
- **File Count:** 4 button states
- **Usage:** All menu scenes

**46. Selection Arrow/Cursor**
- **Format:** PNG with transparency
- **Size:** 32x32 pixels
- **Variants:**
  - Orange arrow (►)
  - Blinking animation (2 frames)
- **File Count:** 2 files
- **Usage:** Menu navigation
- **Notes:** Typewriter effect compatible

**47. Dialogue Choice Box**
- **Format:** PNG with transparency
- **Size:** 700x100 pixels
- **Variants:**
  - Selected choice (highlighted)
  - Normal choice
  - Disabled choice (grayed out)
- **File Count:** 3 variants
- **Usage:** Dialogue Scene

**48. NPC Morale Bar**
- **Format:** PNG with transparency
- **Size:** 200x20 pixels
- **Variants:**
  - High morale (green, >70%)
  - Medium morale (yellow, 30-70%)
  - Low morale (red, <30%)
  - Bar background/frame
- **File Count:** 4 files
- **Usage:** NPC Management, Dialogue Scene

**Total Menu Elements:** 16 files

### World Map UI (🟡 High)

**49. Map Node Icons**
- **Format:** PNG with transparency
- **Size:** 48x48 pixels
- **Variants:**
  - Unvisited node (gray)
  - Current node (yellow glow)
  - Visited node (white)
  - Home node (special icon)
- **File Count:** 4 icons
- **Usage:** World Map Scene

**50. Route Line Graphics**
- **Format:** PNG with transparency
- **Size:** Tileable, 64x16 pixels
- **Variants:**
  - Available route (white)
  - Selected route (yellow)
  - Completed route (gray)
- **File Count:** 3 line variants
- **Usage:** World Map Scene path visualization

**51. Confirmation Window**
- **Format:** PNG with transparency
- **Size:** 500x300 pixels
- **Variants:** 1 popup window
- **File Count:** 1 file
- **Usage:** World Map Scene route confirmation
- **Notes:** Shows distance, fuel cost, estimated time

**Total World Map UI:** 8 files

### Interaction Prompts (🟡 High)

**52. Key Prompt Icons**
- **Format:** PNG with transparency
- **Size:** 48x48 pixels
- **Icons:**
  - [A] key
  - [D] key
  - [E] key
  - [I] key
  - [ESC] key
- **File Count:** 5 icons
- **Usage:** Context prompts in gameplay scenes
- **Notes:** Clear, readable icons showing which key to press

**53. Interaction Indicator**
- **Format:** PNG with transparency
- **Size:** 64x32 pixels
- **Variants:**
  - "Press E to interact" prompt
  - Animated attention indicator (3 frames)
- **File Count:** 4 files (1 base + 3 animation frames)
- **Usage:** Appears near interactable structures

**Total Interaction Prompts:** 9 files

---

## Animations

### Transition Animations (🔴 Critical)

**54. Scene Fade Overlay**
- **Format:** PNG sequence
- **Size:** 1366x768 pixels
- **Frames:** 10 frames (fade in/out)
- **File Count:** 10 files
- **Usage:** Scene transitions between all scenes
- **Notes:** Black overlay with varying opacity (0% to 100%)

**55. Loading Spinner**
- **Format:** PNG with transparency
- **Size:** 64x64 pixels
- **Frames:** 8 frames (rotation animation)
- **File Count:** 8 files
- **Usage:** Loading states, scene transitions
- **Notes:** Retro-styled spinner or dots

**Total Transition Animations:** 18 files

### Vehicle Animations (🟡 High)

**56. Player Vehicle Movement**
- **Format:** PNG sprite sheet
- **Size:** 192x32 pixels (3 frames @ 64x32 each)
- **Frames:** 3 frames per vehicle per view
- **Vehicles:** 3 vehicle types × 2 views
- **File Count:** 6 sprite sheets
- **Usage:** Driving Scene, Side View Scene
- **Notes:** Subtle movement animation (wheel rotation, slight bobbing)

**57. Traffic Vehicle Movement**
- **Format:** PNG sprite sheet
- **Size:** 384x64 pixels (3 frames @ 128x64 each)
- **Frames:** 3 frames per traffic vehicle
- **Vehicles:** 12 traffic vehicle types
- **File Count:** 12 sprite sheets
- **Usage:** Side View Scene traffic system

**Total Vehicle Animations:** 18 sprite sheets

### UI Animations (🟢 Medium)

**58. Button Hover Effect**
- **Format:** PNG sequence
- **Size:** 400x60 pixels
- **Frames:** 4 frames (subtle glow/highlight)
- **File Count:** 4 files
- **Usage:** Menu button interactions

**59. Resource Bar Fill Animation**
- **Format:** PNG sequence
- **Size:** 200x30 pixels
- **Frames:** 5 frames (smooth filling)
- **File Count:** 5 files per resource type × 2 types = 10 files
- **Usage:** HUD resource bars

**60. Inventory Item Pickup Animation**
- **Format:** PNG sequence
- **Size:** 32x32 pixels
- **Frames:** 6 frames (item floating up)
- **File Count:** 6 files
- **Usage:** Inventory system feedback

**61. Dialogue Text Typewriter Effect**
- **Format:** Handled by code, no sprites needed
- **Notes:** Character-by-character text reveal
- **Usage:** Dialogue Scene, Character Creation

**Total UI Animations:** 20 files

### Environmental Animations (🟢 Medium)

**62. Cloud Movement**
- **Format:** PNG sprite sheet with transparency
- **Size:** 2732x300 pixels (2× width for seamless loop)
- **Frames:** Single tileable image
- **File Count:** 4 files (one per sky variant)
- **Usage:** Side View Scene sky layer
- **Notes:** Moves slowly across screen for parallax effect

**63. Tree Sway Animation**
- **Format:** PNG sprite sheet with transparency
- **Size:** Individual tree 64x128 pixels
- **Frames:** 4 frames (subtle swaying)
- **File Count:** 4 sprite sheets (one per tree type)
- **Usage:** Side View Scene vegetation layer
- **Notes:** Gentle side-to-side movement

**64. Billboard Lights Blink**
- **Format:** PNG sequence
- **Size:** 64x96 pixels
- **Frames:** 4 frames (lights on/off pattern)
- **File Count:** 4 files
- **Usage:** Side View Scene billboards
- **Notes:** For night-time variants

**Total Environmental Animations:** 12 files

### Character Animations (🟢 Medium)

**65. NPC Idle Animation**
- **Format:** PNG sprite sheet
- **Size:** 96x48 pixels (3 frames @ 32x48 each)
- **Frames:** 3 frames (breathing, slight movement)
- **File Count:** 5 NPC types × 1 = 5 sprite sheets
- **Usage:** Dialogue Scene, team management

**66. Vehicle Entry/Exit Animation**
- **Format:** PNG sequence
- **Size:** 128x64 pixels
- **Frames:** 6 frames (character getting in/out of vehicle)
- **File Count:** 6 files
- **Usage:** Event scenes, location interactions
- **Notes:** Optional, can be implied rather than shown

**Total Character Animations:** 11 files

---

## Fonts & Typography

### Primary Fonts (🔴 Critical)

**67. Main UI Font - Press Start 2P**
- **Format:** TTF/OTF
- **License:** SIL Open Font License (already included)
- **Usage:** Menu text, buttons, headers
- **Sizes Needed:** 12px, 16px, 24px, 32px
- **Location:** `images/Press_Start_2P/`
- **Status:** ✅ Already present
- **Notes:** Retro pixel-style font for authentic look

**68. HUD Font - Jersey 10**
- **Format:** TTF/OTF
- **License:** SIL Open Font License (already included)
- **Usage:** HUD displays, LCD screens, resource values
- **Sizes Needed:** 14px, 18px, 22px
- **Location:** `images/Jersey_10/`
- **Status:** ✅ Already present
- **Notes:** Monospace for technical displays

**69. Dialogue Font - Press Start 2P (smaller)**
- **Format:** TTF/OTF
- **Usage:** Dialogue text, descriptions, item details
- **Sizes Needed:** 10px, 12px, 14px
- **Status:** ✅ Can reuse Press Start 2P at smaller sizes
- **Notes:** May need increased line spacing for readability

**Total Fonts:** 3 (2 font families, already present)

---

## Audio Assets

### Music Tracks (🟡 High)

**70. Main Menu Theme**
- **Format:** OGG Vorbis or MP3
- **Length:** 2-3 minutes (looping)
- **Style:** Ambient, nostalgic, road trip atmosphere
- **File Count:** 1 track
- **Usage:** Main Menu Scene

**71. Driving Music - Calm**
- **Format:** OGG Vorbis or MP3
- **Length:** 3-4 minutes (looping)
- **Style:** Relaxing, highway cruising
- **File Count:** 2-3 variants
- **Usage:** Driving Scene, Side View Scene (day)

**72. Driving Music - Upbeat**
- **Format:** OGG Vorbis or MP3
- **Length:** 3-4 minutes (looping)
- **Style:** Energetic, adventure feeling
- **File Count:** 2-3 variants
- **Usage:** Side View Scene (fast travel)

**73. Night Driving Music**
- **Format:** OGG Vorbis or MP3
- **Length:** 3-4 minutes (looping)
- **Style:** Ambient, calm, nighttime atmosphere
- **File Count:** 1-2 variants
- **Usage:** Driving scenes at night

**74. Motel/Rest Music**
- **Format:** OGG Vorbis or MP3
- **Length:** 2 minutes (looping)
- **Style:** Calm, peaceful, restful
- **File Count:** 1 track
- **Usage:** Motel Scene, rest stops

**75. Event/Tension Music**
- **Format:** OGG Vorbis or MP3
- **Length:** 1-2 minutes
- **Style:** Suspenseful, decision-making atmosphere
- **File Count:** 2-3 variants
- **Usage:** Event scenes with critical choices

**Total Music Tracks:** 11-14 files

### Sound Effects - UI (🟡 High)

**76. Menu Navigation Sounds**
- **Format:** WAV or OGG
- **Length:** <0.5 seconds each
- **Sounds:**
  - Menu hover (subtle beep)
  - Menu select (confirm beep)
  - Menu back (cancel sound)
  - Menu open/close (swoosh)
- **File Count:** 4 sounds
- **Usage:** All menu scenes

**77. Inventory Sounds**
- **Format:** WAV or OGG
- **Length:** <0.5 seconds each
- **Sounds:**
  - Item pickup (click)
  - Item place (drop/clunk)
  - Item rotate (swish)
  - Invalid placement (error beep)
  - Item stack (multiple clicks)
- **File Count:** 5 sounds
- **Usage:** Inventory Scene, GridInventorySystem

**78. Notification Sounds**
- **Format:** WAV or OGG
- **Length:** <1 second each
- **Sounds:**
  - Warning (urgent beep)
  - Success (positive chime)
  - Error (negative buzz)
  - Message received (notification)
- **File Count:** 4 sounds
- **Usage:** All scenes for feedback

**Total UI Sound Effects:** 13 files

### Sound Effects - Gameplay (🟡 High)

**79. Vehicle Sounds**
- **Format:** WAV or OGG (looping for engine)
- **Length:** Variable
- **Sounds:**
  - Engine idle (looping)
  - Engine driving (looping)
  - Engine acceleration
  - Braking/stopping
  - Horn (short beep)
  - Door open/close
- **File Count:** 6 sounds × 3 vehicle types = 18 sounds
- **Usage:** Driving Scene, Side View Scene

**80. Interaction Sounds**
- **Format:** WAV or OGG
- **Length:** <2 seconds each
- **Sounds:**
  - Gas pump (refueling)
  - Cash register (buying)
  - Wrench/tools (repairing)
  - Door opening (entering building)
  - Footsteps (entering location)
- **File Count:** 5 sounds
- **Usage:** Structure interactions

**81. Ambient Environment Sounds**
- **Format:** WAV or OGG (looping)
- **Length:** 5-10 seconds (looping)
- **Sounds:**
  - Highway ambience (wind, distant traffic)
  - Desert ambience (wind, tumbleweeds)
  - Forest ambience (birds, wind through trees)
  - Night ambience (crickets, quiet wind)
  - Rain ambience (rainfall)
- **File Count:** 5 ambient loops
- **Usage:** Side View Scene, Driving Scene background

**82. Traffic Sounds**
- **Format:** WAV or OGG
- **Length:** <3 seconds each
- **Sounds:**
  - Car passing (whoosh)
  - Truck passing (heavy rumble)
  - Motorcycle passing (high-pitched zoom)
  - Horn from other vehicles
- **File Count:** 4 sounds
- **Usage:** Side View Scene traffic system

**Total Gameplay Sound Effects:** 32 files

### Sound Effects - Events (🟢 Medium)

**83. Event Sounds**
- **Format:** WAV or OGG
- **Length:** Variable
- **Sounds:**
  - Encounter intro (mysterious tone)
  - Positive event (cheerful sound)
  - Negative event (ominous tone)
  - Quest complete (success fanfare)
  - Level up/achievement (reward sound)
- **File Count:** 5 sounds
- **Usage:** Event System, dialogue outcomes

**Total Event Sound Effects:** 5 files

---

## Technical Specifications

### General Requirements

**File Formats:**
- **Sprites/Images:** PNG (32-bit RGBA for transparency)
- **Backgrounds:** PNG or JPG (JPG for solid backgrounds without transparency)
- **Fonts:** TTF or OTF (OpenType preferred for better rendering)
- **Music:** OGG Vorbis (better quality/compression than MP3, open format)
- **Sound Effects:** WAV (uncompressed for short effects) or OGG (compressed)

**Color Depth:**
- All sprites: 32-bit RGBA (8 bits per channel + alpha)
- Backgrounds: 24-bit RGB minimum (32-bit if transparency needed)

**Resolution Guidelines:**
- **UI elements:** Design at target resolution (1366x768)
- **Sprites:** Design at native pixel size, scale will be handled by code
- **Backgrounds:** Exact size or larger (for parallax scrolling)
- **Icons:** 32x32, 48x48, or 64x64 pixels (power of 2 for optimal rendering)

**Transparency:**
- Use alpha channel for smooth edges
- Avoid semi-transparent pixels on edges (causes artifacts)
- Pure transparent (0% opacity) or fully opaque (100%) preferred for pixel art

**Naming Conventions:**
```
category_name_variant_state.extension

Examples:
- vehicle_sedan_blue_idle.png
- npc_mechanic_01_neutral.png
- ui_button_normal.png
- item_wrench_2x1.png
- bg_parallax_sky_day.png
- sfx_menu_select.ogg
- music_driving_calm_01.ogg
```

### Performance Considerations

**Sprite Sheets:**
- Combine multiple frames into single sprite sheet where possible
- Reduces file I/O and improves loading times
- Maximum recommended size: 2048x2048 pixels

**Texture Atlases:**
- Group related small sprites (icons, items) into texture atlases
- Recommended for inventory items (all items in one atlas)
- Reduces draw calls, improves rendering performance

**Audio:**
- Music: OGG Vorbis at 128-192 kbps (stereo)
- Sound Effects: OGG Vorbis at 96-128 kbps (mono for most effects)
- Keep individual sound files under 5MB
- Use looping for ambient/background sounds

**Optimization:**
- Compress PNG files with tools like OptiPNG or TinyPNG
- Remove unnecessary metadata from images
- Use power-of-2 dimensions where possible (256, 512, 1024, 2048)
- Test loading times and memory usage with all assets

---

## Обзор ассетов

Этот документ содержит полный список всех необходимых ассетов для финальной реализации Road Trip Adventure RPG на основе анализа этапов 1-9. Ассеты категоризированы по типам и включают технические спецификации, контекст использования и уровни приоритета.

**Уровни приоритета:**
- 🔴 **Критический** - Требуется для основного функционала игры
- 🟡 **Высокий** - Важно для полного игрового опыта
- 🟢 **Средний** - Улучшает геймплей, но не является обязательным
- ⚪ **Низкий** - Желательно иметь, будущее улучшение

---

## Спрайты персонажей

### Автомобили игрока (🔴 Критический)

Каждый автомобиль требует:
- 2 вида (сверху и сбоку)
- 3 состояния (покой, движение, повреждён)
- 3 цветовых варианта
- Формат PNG с прозрачностью

**Итого автомобилей:** 54 файла (3 типа × 3 цвета × 3 состояния × 2 вида)

### NPC персонажи (🟡 Высокий)

Каждый тип NPC требует:
- Портрет 128x128 пикселей
- Спрайт 32x48 пикселей
- 3 выражения лица (нейтральное, радостное, обеспокоенное)
- Формат PNG с прозрачностью

**Типы NPC:**
- Механик (3 варианта персонажей)
- Торговец (3 варианта)
- Компаньон (3 варианта)
- Медик (2 варианта)
- Навигатор (2 варианта)

**Итого NPC:** 52 файла (39 портретов + 13 спрайтов)

---

## Окружение и фоны

### Элементы карты мира (🔴 Критический)

**Дорожные тайлы:**
- Размер: 64x64 пикселя
- Варианты: прямые, углы, перекрёстки, шоссе
- Формат: PNG тайлсет
- Итого: 1 тайлсет с 12 тайлами

**Фоны карты:**
- Размер: 1366x768 пикселей
- Варианты: пустыня, лес, горы, побережье
- Итого: 4 фона

### Локационные структуры (🔴 Критический)

Каждая структура требует:
- Вид сверху (top-down)
- Вид сбоку (side view)
- Иконку 32x32 для HUD/карты
- 2 варианта дизайна (где применимо)

**Типы структур:**
1. Заправка (Gas Station) - 4 файла
2. Магазин (Store) - 4 файла
3. Мотель (Motel) - 4 файла
4. Гараж (Garage) - 4 файла
5. Закусочная (Diner) - 4 файла
6. Остановка для отдыха (Rest Stop) - 4 файла
7. Рекламный щит (Billboard) - 4 файла
8. Дом (Home) - 1 файл + иконка

**Итого структур:** 41 файл

### Параллакс-фоны (🟡 Высокий)

**Слои для Side View Scene:**
1. Слой неба - 4 варианта (день, облачно, закат, ночь)
2. Слой гор/холмов - 3 варианта (пустынные горы, лесные холмы, скалистые горы)
3. Слой деревьев/растительности - 4 варианта (сосны, кактусы, лиственные, кусты)
4. Слой дороги - 3 варианта (2-полосное шоссе, 4-полосное, повреждённая дорога)
5. Слой земли - 3 варианта (пустыня, трава, скалы)

**Итого параллакс-фонов:** 17 файлов

---

## Элементы интерфейса

### Компоненты HUD (🔴 Критический)

**Основные элементы:**
1. Рамка HUD-панели (1366x120 пикселей)
2. Полоски ресурсов (4 варианта)
3. Иконки состояния (7 иконок)
4. Фон LCD-дисплея

**Итого HUD:** 13 файлов

### UI инвентаря (🔴 Критический)

**Элементы сетки инвентаря:**
1. Фон сетки 6x4 (600x400 пикселей)
2. Оверлеи подсветки ячеек (4 варианта)
3. Рамки редкости предметов (5 вариантов)
4. Боковая панель деталей предмета
5. Дисплей веса/вместимости

**Итого UI инвентаря:** 12 файлов

### Элементы меню (🔴 Критический)

**Компоненты меню:**
1. Фоны панелей меню (3 варианта)
2. Состояния кнопок (4 состояния)
3. Стрелка/курсор выбора (2 кадра)
4. Окна выбора в диалогах (3 варианта)
5. Полоска морали NPC (4 файла)

**Итого элементов меню:** 16 файлов

---

## Анимации

### Анимации переходов (🔴 Критический)

1. **Затемнение сцены:** 10 кадров (переход между сценами)
2. **Спиннер загрузки:** 8 кадров (индикатор загрузки)

**Итого анимаций переходов:** 18 файлов

### Анимации транспорта (🟡 Высокий)

1. **Движение автомобиля игрока:** 6 спрайт-шитов (3 типа × 2 вида)
2. **Движение транспортного потока:** 12 спрайт-шитов

**Итого анимаций транспорта:** 18 спрайт-шитов

### Анимации UI (🟢 Средний)

1. **Эффект наведения на кнопку:** 4 кадра
2. **Анимация заполнения полосок ресурсов:** 10 файлов
3. **Анимация подбора предмета:** 6 кадров

**Итого анимаций UI:** 20 файлов

### Анимации окружения (🟢 Средний)

1. **Движение облаков:** 4 файла
2. **Качание деревьев:** 4 спрайт-шита
3. **Мигание огней билборда:** 4 кадра

**Итого анимаций окружения:** 12 файлов

---

## Технические требования

### Форматы файлов

**Изображения:**
- Спрайты: PNG (32-бит RGBA)
- Фоны: PNG или JPG
- Иконки: PNG (32x32, 48x48, 64x64)

**Шрифты:**
- Формат: TTF или OTF
- Имеющиеся: Press Start 2P, Jersey 10

**Аудио:**
- Музыка: OGG Vorbis (128-192 kbps)
- Звуковые эффекты: WAV или OGG (96-128 kbps)

### Конвенции именования

```
категория_название_вариант_состояние.расширение

Примеры:
- vehicle_sedan_blue_idle.png
- npc_mechanic_01_neutral.png
- ui_button_normal.png
- item_wrench_2x1.png
- bg_parallax_sky_day.png
- sfx_menu_select.ogg
- music_driving_calm_01.ogg
```

### Оптимизация производительности

**Спрайт-шиты:**
- Объединять несколько кадров в один файл
- Максимальный размер: 2048x2048 пикселей

**Атласы текстур:**
- Группировать связанные спрайты (иконки, предметы)
- Рекомендуется для предметов инвентаря

**Аудио:**
- Музыка: OGG Vorbis 128-192 kbps (стерео)
- Эффекты: OGG Vorbis 96-128 kbps (моно)
- Размер отдельных файлов: до 5MB
- Использовать зацикливание для фоновых звуков

---

## Asset Production Pipeline / Процесс создания ассетов

### Priority Implementation Order / Порядок приоритетной реализации

**Phase 1: Core Gameplay (Weeks 1-2)**
1. Player vehicles (all variants, all views) - 54 files
2. Basic structure sprites (gas station, motel, store) - 12 files
3. HUD components - 13 files
4. Basic inventory UI - 12 files
5. Essential fonts (already present)

**Phase 2: Environment & NPCs (Weeks 3-4)**
1. NPC portraits and sprites - 52 files
2. Parallax backgrounds - 17 files
3. Road tiles and map backgrounds - 5 files
4. Remaining structures - 29 files

**Phase 3: Items & Details (Week 5)**
1. All inventory item sprites - 24 files
2. Traffic vehicles - 12 files
3. Item rarity borders and UI polish - 5 files

**Phase 4: Animations & Polish (Weeks 6-7)**
1. Scene transition animations - 18 files
2. Vehicle movement animations - 18 sprite sheets
3. UI animations - 20 files
4. Environmental animations - 12 files

**Phase 5: Audio (Week 8)**
1. Essential UI sounds - 13 files
2. Music tracks - 11-14 files
3. Gameplay sound effects - 32 files
4. Event sounds - 5 files

---

## Asset Checklist / Чек-лист ассетов

### Sprites & Characters (40 items total)
- [ ] 🔴 Old Sedan vehicles (18 files)
- [ ] 🔴 Offroad Van vehicles (18 files)
- [ ] 🔴 Sports Coupe vehicles (18 files)
- [ ] 🟡 NPC Mechanic portraits/sprites (12 files)
- [ ] 🟡 NPC Trader portraits/sprites (12 files)
- [ ] 🟡 NPC Companion portraits/sprites (12 files)
- [ ] 🟡 NPC Medic portraits/sprites (8 files)
- [ ] 🟡 NPC Navigator portraits/sprites (8 files)
- [ ] 🟡 Oncoming traffic vehicles (6 files)
- [ ] 🟡 Passing traffic vehicles (6 files)
- [ ] 🔴 Food item sprites (6 files)
- [ ] 🔴 Tool item sprites (5 files)
- [ ] 🔴 Spare parts sprites (6 files)
- [ ] 🔴 Medical item sprites (3 files)
- [ ] 🔴 Quest item sprites (4 files)

### Environment & Backgrounds (43 items total)
- [ ] 🔴 Road tiles tileset (1 tileset, 12 tiles)
- [ ] 🔴 World map backgrounds (4 files)
- [ ] 🔴 Gas Station sprites (4 files + icon)
- [ ] 🔴 Store sprites (4 files + icon)
- [ ] 🔴 Motel sprites (4 files + icon)
- [ ] 🔴 Garage sprites (4 files + icon)
- [ ] 🔴 Diner sprites (4 files + icon)
- [ ] 🔴 Rest Stop sprites (4 files + icon)
- [ ] 🔴 Billboard sprites (4 files)
- [ ] 🔴 Home location sprite (1 file + icon)
- [ ] 🟡 Sky layer parallax (4 files)
- [ ] 🟡 Mountain/hills parallax (3 files)
- [ ] 🟡 Tree/vegetation parallax (4 files)
- [ ] 🟡 Road layer parallax (3 files)
- [ ] 🟡 Ground layer parallax (3 files)
- [ ] 🟡 Main menu background (1 file)
- [ ] 🟡 Character creation background (1 file)
- [ ] 🟡 Motel room interior (1 file)
- [ ] 🟡 Town interior background (1 file)

### UI Elements (61 items total)
- [ ] 🔴 HUD panel frame (1 file)
- [ ] 🔴 Resource bars (4 files)
- [ ] 🔴 Status icons (7 files)
- [ ] 🔴 LCD display background (1 file)
- [ ] 🔴 Inventory grid background (1 file)
- [ ] 🔴 Cell highlight overlays (4 files)
- [ ] 🔴 Item rarity borders (5 files)
- [ ] 🔴 Inventory side panel (1 file)
- [ ] 🔴 Weight/capacity display (1 file)
- [ ] 🔴 Menu panel backgrounds (3 files)
- [ ] 🔴 Button states (4 files)
- [ ] 🔴 Selection arrow/cursor (2 files)
- [ ] 🔴 Dialogue choice boxes (3 files)
- [ ] 🔴 NPC morale bars (4 files)
- [ ] 🟡 Map node icons (4 files)
- [ ] 🟡 Route line graphics (3 files)
- [ ] 🟡 Confirmation window (1 file)
- [ ] 🟡 Key prompt icons (5 files)
- [ ] 🟡 Interaction indicator (4 files)

### Animations (43 items total)
- [ ] 🔴 Scene fade overlay (10 files)
- [ ] 🔴 Loading spinner (8 files)
- [ ] 🟡 Player vehicle movement (6 sprite sheets)
- [ ] 🟡 Traffic vehicle movement (12 sprite sheets)
- [ ] 🟢 Button hover effect (4 files)
- [ ] 🟢 Resource bar fill animation (10 files)
- [ ] 🟢 Inventory item pickup (6 files)
- [ ] 🟢 Cloud movement (4 files)
- [ ] 🟢 Tree sway animation (4 files)
- [ ] 🟢 Billboard lights blink (4 files)
- [ ] 🟢 NPC idle animation (5 sprite sheets)
- [ ] 🟢 Vehicle entry/exit (6 files)

### Fonts (3 items total)
- [x] ✅ Press Start 2P font (already present)
- [x] ✅ Jersey 10 font (already present)
- [x] ✅ Dialogue font (reuse Press Start 2P)

### Audio (55 items total)
- [ ] 🟡 Main menu theme (1 track)
- [ ] 🟡 Driving music calm (2-3 tracks)
- [ ] 🟡 Driving music upbeat (2-3 tracks)
- [ ] 🟡 Night driving music (1-2 tracks)
- [ ] 🟡 Motel/rest music (1 track)
- [ ] 🟡 Event/tension music (2-3 tracks)
- [ ] 🟡 Menu navigation sounds (4 files)
- [ ] 🟡 Inventory sounds (5 files)
- [ ] 🟡 Notification sounds (4 files)
- [ ] 🟡 Vehicle sounds (18 files)
- [ ] 🟡 Interaction sounds (5 files)
- [ ] 🟡 Ambient environment sounds (5 files)
- [ ] 🟡 Traffic sounds (4 files)
- [ ] 🟢 Event sounds (5 files)

---

## Summary / Итоги

**Total Assets Required:** 245 assets across all categories

**By Priority:**
- 🔴 Critical: 65 assets (core gameplay)
- 🟡 High: 71 assets (complete experience)
- 🟢 Medium: 65 assets (enhancements)
- ⚪ Low: 44 assets (future additions)

**Current Status:**
- ✅ Fonts: 2/3 already present (Press Start 2P, Jersey 10)
- ✅ Basic images: Some placeholder images exist (car_blue.png, car_red.png, background.png, hud.png)
- ⚠️ Remaining: ~240 assets to be created

**Estimated Production Time:**
- Core gameplay assets (Phase 1-2): 4-5 weeks
- Full implementation (Phase 1-5): 8 weeks
- With parallel art production: 6 weeks possible

---

**Document Version:** 1.0  
**Last Updated:** 2025-11-12  
**Status:** Complete asset analysis based on README Stages 1-9

This asset list should be updated as implementation progresses and new requirements are identified.
