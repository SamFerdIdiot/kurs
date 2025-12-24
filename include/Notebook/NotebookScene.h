#ifndef NOTEBOOK_SCENE_H
#define NOTEBOOK_SCENE_H

#include "Scene.h"
#include "PlayerState.h"
#include "FontLoader.h"
#include "Notebook/NotebookEntry.h"
#include "EventManager.h"
#include "ResourceEventSystem.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <string>
#include <vector>
#include <map>


















class NotebookScene : public Scene {
public:





    explicit NotebookScene(PlayerState* playerState,
                          const std::string& initialEntryId = "tutorial_start");

    ~NotebookScene() override = default;



    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

    SceneType getNextScene() const override;
    bool isFinished() const override;







    void showEntry(const std::string& entryId);




    void skipTextAnimation();

private:

    PlayerState* m_playerState;
    bool m_isFinished;
    SceneType m_nextScene;


    std::string m_currentEntryId;
    NotebookEntry m_currentEntry;
    std::string m_fullText;
    std::string m_revealedText;


    float m_textRevealTimer;
    float m_charactersPerSecond;
    bool m_textFullyRevealed;
    bool m_canSkipText;


    std::vector<NotebookChoice> m_choices;
    int m_selectedChoiceIndex;
    float m_choiceTimer;


    EventManager m_eventManager;
    ResourceEventSystem m_resourceEventSystem;
    float m_eventTriggerChance;
    std::string m_pendingEventId;
    std::string m_pendingNextEntryId;


    std::map<std::string, NotebookEntry> m_entries;


    sf::Font m_fontPresent;
    sf::Font m_fontPast;
    sf::Font m_fontThought;
    bool m_fontLoaded;











    void updateTextReveal(float deltaTime);





    void renderText(sf::RenderWindow& window);





    void renderChoices(sf::RenderWindow& window);





    void renderHUD(sf::RenderWindow& window);





    void handleChoice(int choiceIndex);




    void moveSelectionUp();




    void moveSelectionDown();




    const sf::Font& getCurrentFont() const;




    sf::Color getCurrentTextColor() const;




    unsigned int getCurrentFontSize() const;




    std::string wrapText(const std::string& text, const sf::Font& font, unsigned int fontSize, float maxWidth) const;







    bool tryTriggerRandomEvent();






    NotebookEntry convertEventToEntry(const GameEvent& event);





    void showEventAsEntry(const std::string& eventId);






    void loadDay0_KnockOnDoor();




    void loadDay0_Volga();




    void loadDay0_FirstTask();


    void loadDay0_BranchA_MapAndTower();
    void loadDay0_BranchA_Geologist();
    void loadDay0_BranchA_3();
    void loadDay0_BranchA_4();
    void loadDay0_BranchA_5();


    void loadDay0_BranchB_Engine();
    void loadDay0_BranchB_Blacksmith();
    void loadDay0_BranchB_3();
    void loadDay0_BranchB_4();
    void loadDay0_BranchB_5();
    void loadDay0_BranchB_Driver();


    void loadDay0_BranchC_Truck();
    void loadDay0_BranchC_Teahouse();
    void loadDay0_BranchC_3();
    void loadDay0_BranchC_4();
    void loadDay0_BranchC_5();
    void loadDay0_BranchC_Package();


    void loadDay0_BranchD_Route();
    void loadDay0_BranchD_Boys();
    void loadDay0_BranchD_3();
    void loadDay0_BranchD_4();
    void loadDay0_BranchD_5();
    void loadDay0_BranchD_Choice();





    void loadDay0_Provocation();




    void loadDay0_GarageFinale();




    void loadDay0_TransitionPresent();


    void loadDay1_BorderCrossing();
    void loadDay1_CustomsCheck();



    void loadTestThoughtSystem();
    void loadDemoTestResources();
    void loadDemoTestResources2();
    void loadDemoTestPrinciples();
    void loadDemoTestPrinciples2();
    void loadDemoTestPrinciples3();
    void loadDemoTestConditional();
    void loadDemoTestConditional2();
    void loadDemoTestConditional3();
    void loadDemoTestConditional4();
    void loadDemoFullStart();
    void loadDemoFull1();
    void loadDemoFull2();
    void loadDemoFull3();
    void loadDemoFinale();






    void loadDemo_Start();




    void loadDemo_Road();




    void loadDemo_Choice();




    void loadDemo_Highway();




    void loadDemo_Backroad();




    void loadDemo_Finale();




    void loadDemo_RestStop();




    void loadDemo_MechanicEncounter();




    void loadDemo_LowFuel();




    void loadDemo_FinalChoice();




    void loadDemo_ForestPath();




    void loadDemo_CityPath();




    void loadDemo_End();
};

#endif
