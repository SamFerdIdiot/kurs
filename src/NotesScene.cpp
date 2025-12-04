#include "NotesScene.h"
#include "UIConstants.h"
#include "FontLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>

NotesScene::NotesScene()
    : m_cursorPosition(0),
      m_cursorVisible(true),
      m_cursorBlinkTimer(0.0f),
      m_finished(false),
      m_nextScene(SceneType::NODE),
      m_fontLoaded(false) {

    // Load font
    if (auto fontOpt = FontLoader::load()) {
        m_font = std::move(*fontOpt);
        m_fontLoaded = true;
    } else {
        std::cerr << "NotesScene: Failed to load font" << std::endl;
    }

    // Setup background
    m_background.setSize(sf::Vector2f(UI::SCREEN_WIDTH, UI::SCREEN_HEIGHT));
    m_background.setFillColor(UI::Color::BACKGROUND_DARK);

    // Setup notepad box
    m_notepadBox.setSize(sf::Vector2f(NOTEPAD_WIDTH, NOTEPAD_HEIGHT));
    m_notepadBox.setPosition(sf::Vector2f(NOTEPAD_X, NOTEPAD_Y));
    m_notepadBox.setFillColor(sf::Color(30, 30, 40));
    m_notepadBox.setOutlineColor(UI::Color::BORDER_COLOR);
    m_notepadBox.setOutlineThickness(2.0f);

    // Load existing notes
    loadNotes();
}

void NotesScene::loadNotes() {
    // Try to load notes from file
    std::ifstream file("saves/player_notes.txt");
    if (file.is_open()) {
        std::stringstream buffer;
        buffer << file.rdbuf();
        m_notesContent = buffer.str();
        file.close();
        std::cout << "Loaded notes: " << m_notesContent.length() << " characters" << std::endl;
    } else {
        // Initialize with default template
        m_notesContent = "TRAVEL NOTES\n\n"
                        "City Connections:\n"
                        "---\n\n"
                        "Route Plan:\n"
                        "---\n\n"
                        "Quests & Tasks:\n"
                        "---\n\n";
    }

    // Set cursor to end
    m_cursorPosition = m_notesContent.length();
}

void NotesScene::saveNotes() {
    std::ofstream file("saves/player_notes.txt");
    if (file.is_open()) {
        file << m_notesContent;
        file.close();
        std::cout << "Saved notes: " << m_notesContent.length() << " characters" << std::endl;
    } else {
        std::cerr << "Failed to save notes!" << std::endl;
    }
}

void NotesScene::handleInput(const sf::Event& event) {
    // Text input
    if (const auto* textEntered = event.getIf<sf::Event::TextEntered>()) {
        handleTextInput(textEntered->unicode);
        return;
    }

    // Key presses
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        switch (keyPressed->code) {
            case sf::Keyboard::Key::Escape:
                saveNotes();
                m_finished = true;
                m_nextScene = SceneType::NODE;
                break;

            case sf::Keyboard::Key::Backspace:
                handleBackspace();
                break;

            case sf::Keyboard::Key::Delete:
                handleDelete();
                break;

            case sf::Keyboard::Key::Enter:
                handleNewline();
                break;

            case sf::Keyboard::Key::Left:
                if (m_cursorPosition > 0) {
                    m_cursorPosition--;
                }
                break;

            case sf::Keyboard::Key::Right:
                if (m_cursorPosition < m_notesContent.length()) {
                    m_cursorPosition++;
                }
                break;

            case sf::Keyboard::Key::Home:
                m_cursorPosition = 0;
                break;

            case sf::Keyboard::Key::End:
                m_cursorPosition = m_notesContent.length();
                break;

            default:
                break;
        }
    }
}

void NotesScene::handleTextInput(std::uint32_t unicode) {
    // Ignore control characters (except newline which is handled separately)
    if (unicode < 32 || unicode == 127) {
        return;
    }

    // Check character limit
    if (m_notesContent.length() >= MAX_CHARS) {
        return;
    }

    // Insert character at cursor position
    char character = static_cast<char>(unicode);
    m_notesContent.insert(m_cursorPosition, 1, character);
    m_cursorPosition++;
}

void NotesScene::handleBackspace() {
    if (m_cursorPosition > 0) {
        m_notesContent.erase(m_cursorPosition - 1, 1);
        m_cursorPosition--;
    }
}

void NotesScene::handleNewline() {
    if (m_notesContent.length() < MAX_CHARS) {
        m_notesContent.insert(m_cursorPosition, 1, '\n');
        m_cursorPosition++;
    }
}

void NotesScene::handleDelete() {
    if (m_cursorPosition < m_notesContent.length()) {
        m_notesContent.erase(m_cursorPosition, 1);
    }
}

void NotesScene::update(float deltaTime) {
    // Update cursor blink
    m_cursorBlinkTimer += deltaTime;
    if (m_cursorBlinkTimer >= CURSOR_BLINK_INTERVAL) {
        m_cursorVisible = !m_cursorVisible;
        m_cursorBlinkTimer = 0.0f;
    }
}

void NotesScene::render(sf::RenderWindow& window) {
    // Draw background
    window.draw(m_background);

    if (!m_fontLoaded) return;

    // Render components
    renderHeader(window);
    renderNotepad(window);
    renderFooter(window);
}

void NotesScene::renderHeader(sf::RenderWindow& window) {
    float yPos = 40.0f;

    // Title
    sf::Text titleText(m_font, "PERSONAL NOTES", UI::FONT_SIZE_HUGE);
    titleText.setFillColor(UI::Color::TEXT_PRIMARY);
    titleText.setPosition(sf::Vector2f(NOTEPAD_X, yPos));
    window.draw(titleText);
}

void NotesScene::renderNotepad(sf::RenderWindow& window) {
    // Draw notepad box
    window.draw(m_notepadBox);

    // Split content into lines for rendering
    m_lines.clear();
    std::stringstream ss(m_notesContent);
    std::string line;
    while (std::getline(ss, line)) {
        m_lines.push_back(line);
    }

    // Render text lines
    float yPos = NOTEPAD_Y + TEXT_PADDING;
    size_t charCount = 0;

    for (size_t i = 0; i < m_lines.size(); ++i) {
        const std::string& line = m_lines[i];

        // Draw line text
        sf::Text lineText(m_font, line, UI::FONT_SIZE_SMALL);
        lineText.setFillColor(UI::Color::TEXT_PRIMARY);
        lineText.setPosition(sf::Vector2f(NOTEPAD_X + TEXT_PADDING, yPos));
        window.draw(lineText);

        // Draw cursor if it's on this line
        if (m_cursorVisible) {
            size_t lineStart = charCount;
            size_t lineEnd = charCount + line.length();

            if (m_cursorPosition >= lineStart && m_cursorPosition <= lineEnd) {
                // Calculate cursor X position
                size_t cursorOffsetInLine = m_cursorPosition - lineStart;
                std::string textBeforeCursor = line.substr(0, cursorOffsetInLine);

                sf::Text measureText(m_font, textBeforeCursor, UI::FONT_SIZE_SMALL);
                float cursorX = NOTEPAD_X + TEXT_PADDING + measureText.getLocalBounds().size.x;

                // Draw cursor
                sf::RectangleShape cursor;
                cursor.setSize(sf::Vector2f(2.0f, LINE_HEIGHT));
                cursor.setPosition(sf::Vector2f(cursorX, yPos));
                cursor.setFillColor(UI::Color::ACCENT_YELLOW);
                window.draw(cursor);
            }
        }

        charCount += line.length() + 1;  // +1 for newline
        yPos += LINE_HEIGHT;

        // Stop if we've filled the notepad
        if (yPos > NOTEPAD_Y + NOTEPAD_HEIGHT - TEXT_PADDING) {
            break;
        }
    }
}

void NotesScene::renderFooter(sf::RenderWindow& window) {
    float yPos = NOTEPAD_Y + NOTEPAD_HEIGHT + 20.0f;

    // Character count
    std::string charCountText = "Characters: " + std::to_string(m_notesContent.length()) + " / " + std::to_string(MAX_CHARS);
    sf::Text charCountTextObj(m_font, charCountText, UI::FONT_SIZE_SMALL);
    charCountTextObj.setFillColor(UI::Color::TEXT_SECONDARY);
    charCountTextObj.setPosition(sf::Vector2f(NOTEPAD_X, yPos));
    window.draw(charCountTextObj);

    yPos += 30.0f;

    // Controls
    sf::Text controlsText(m_font,
        "[Type to edit]  [Arrows] Move cursor  [Esc] Save and exit",
        UI::FONT_SIZE_SMALL);
    controlsText.setFillColor(UI::Color::TEXT_SECONDARY);
    controlsText.setPosition(sf::Vector2f(NOTEPAD_X, yPos));
    window.draw(controlsText);
}

SceneType NotesScene::getNextScene() const {
    return m_nextScene;
}

bool NotesScene::isFinished() const {
    return m_finished;
}
