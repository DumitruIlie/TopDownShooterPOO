#ifndef MENUSYSTEM_H
#define MENUSYSTEM_H

#include<SFML/Graphics.hpp>
#include"Button.h"
#include"vector"

enum MENU{EXITGAME, MAINMENU, GAMEPLAY, PAUSEMENU, GAMEOVER, MENUCNT};

class MenuSystem
{
private:
    static MENU menu;
    //Acest vector are dimensiune constanta, fiecare meniu are un numar variabil de butoane deci fiecare vector din vector are dimensiune variabila
    static std::vector<Button*> buttons[MENUCNT];

    static Button gp_pauseButton;
    static Button ps_resumeButton;
    static Button mm_playGameButton;
    static Button mm_exitGameButton;
    static Button ps_mainMenuButton;
    static Button go_replayButton;
    static Button go_mainMenuButton;

public:
    MenuSystem() = delete;

    static void init();
    static void setMenu(MENU newMenu);

    static void render(sf::RenderWindow& window);

    static void click(vec2i mousePos);

    static MENU getMenu();
};

#endif//MENUSYSTEM_H