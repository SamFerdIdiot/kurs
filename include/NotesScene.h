#ifndef NOTES_SCENE_H
#define NOTES_SCENE_H

#include "Scene.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <optional>

// NotesScene - Simple text notepad for player notes
// Features:
// - Text input for writing notes
// - Multiple lines support
// - Save/load notes automatically
// - Used for navigation planning with static world map
//
// Players use this to:
// - Record city connections they discover
// - Plan routes manually
// - Track quest information
// - Write reminders
class NotesScene : public Scene {
public:
    NotesScene();
    ~NotesScene() override = default;

    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

    SceneType getNextScene() const override;
    bool isFinished() const override;

private:
    // Load notes from save file
    void loadNotes();

    // Save notes to save file
    void saveNotes();

    // Handle text input
    void handleTextInput(std::uint32_t unicode);
    void handleBackspace();
    void handleNewline();
    void handleDelete();

    // Rendering
    void renderHeader(sf::RenderWindow& window);
    void renderNotepad(sf::RenderWindow& window);
    void renderFooter(sf::RenderWindow& window);

    // Notes content
    std::string m_notesContent;
    std::vector<std::string> m_lines;  // Split content into lines for rendering

    // Cursor
    size_t m_cursorPosition;  // Position in m_notesContent
    bool m_cursorVisible;
    float m_cursorBlinkTimer;
    const float CURSOR_BLINK_INTERVAL = 0.5f;

    // Font
    sf::Font m_font;
    bool m_fontLoaded;

    // Scene state
    bool m_finished;
    SceneType m_nextScene;

    // Visual elements
    sf::RectangleShape m_background;
    sf::RectangleShape m_notepadBox;

    // Layout constants
    static constexpr float NOTEPAD_X = 40.0f;
    static constexpr float NOTEPAD_Y = 120.0f;
    static constexpr float NOTEPAD_WIDTH = 1220.0f;  // 1300 - 80
    static constexpr float NOTEPAD_HEIGHT = 560.0f;
    static constexpr float LINE_HEIGHT = 25.0f;
    static constexpr float TEXT_PADDING = 20.0f;
    static constexpr unsigned int MAX_CHARS = 2000;  // Character limit
};

#endif // NOTES_SCENE_H
