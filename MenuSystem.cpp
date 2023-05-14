//Ilie Dumitru
#include"MenuSystem.h"

MENU MenuSystem::menu=MAINMENU;
std::vector<Button*> MenuSystem::buttons[MENUCNT]={};

const int CIRCLESPLITFACTOR=5;
const int XSPLITFACTOR=8;
const int PAUSESPLITFACTOR=5;

void pauseGame() {MenuSystem::setMenu(PAUSEMENU);}
void resumeGame() {MenuSystem::setMenu(GAMEPLAY);}
void goToMainMenu() {MenuSystem::setMenu(MAINMENU);}
void exitGame() {MenuSystem::setMenu(EXITGAME);}

void drawPauseButton(Button& btn, bool hover, sf::RenderWindow& window)
{
    sf::RectangleShape rect;
    vec2i topLeft=btn.getTopLeft();
    vec2i dim=btn.getBottomRight()-topLeft;

    rect.setPosition(sf::Vector2f((float)topLeft.get_x(), (float)topLeft.get_y()));
    rect.setSize(sf::Vector2f((float)dim.get_x(), (float)dim.get_y()));
    if(hover)
        rect.setFillColor(sf::Color(191, 0, 0));
    else
        rect.setFillColor(sf::Color(255, 0, 0));

    window.draw(rect);

    dim/=CIRCLESPLITFACTOR;

    rect.setPosition(sf::Vector2f((float)(topLeft+dim).get_x(), (float)(topLeft+dim).get_y()));
    rect.setSize(sf::Vector2f((float)dim.get_x(), 3*(float)dim.get_y()));
    if(hover)
        rect.setFillColor(sf::Color(191, 191, 191));
    else
        rect.setFillColor(sf::Color(255, 255, 255));

    window.draw(rect);

    rect.setPosition(sf::Vector2f((float)(topLeft+3*dim).get_x(), (float)(topLeft+dim).get_y()));
    window.draw(rect);
}
void drawResumeButton(Button& btn, bool hover, sf::RenderWindow& window)
{
    sf::RectangleShape rect;
    vec2i topLeft=btn.getTopLeft();
    vec2i dim=btn.getBottomRight()-topLeft;

    rect.setPosition(sf::Vector2f((float)topLeft.get_x(), (float)topLeft.get_y()));
    rect.setSize(sf::Vector2f((float)dim.get_x(), (float)dim.get_y()));
    if(hover)
        rect.setFillColor(sf::Color(0, 191, 0));
    else
        rect.setFillColor(sf::Color(0, 255, 0));

    window.draw(rect);

    dim/=CIRCLESPLITFACTOR;
    sf::CircleShape triangle((float)dim.get_x()*2, 3);

    triangle.setRotation(90);
    triangle.setPosition(sf::Vector2f((float)topLeft.get_x()+(float)(dim*4).get_x(), (float)(topLeft+dim/2).get_y()));
    if(hover)
        triangle.setFillColor(sf::Color(191, 191, 191));
    else
        triangle.setFillColor(sf::Color(255, 255, 255));

    window.draw(triangle);
}
void drawXButton(Button& btn, bool hover, sf::RenderWindow& window)
{
    sf::RectangleShape rect;
    sf::ConvexShape line;
    vec2i topLeft=btn.getTopLeft();
    vec2i dim=btn.getBottomRight()-topLeft, aux;

    rect.setPosition(sf::Vector2f((float)topLeft.get_x(), (float)topLeft.get_y()));
    rect.setSize(sf::Vector2f((float)dim.get_x(), (float)dim.get_y()));
    if(hover)
        rect.setFillColor(sf::Color(191, 0, 0));
    else
        rect.setFillColor(sf::Color(255, 0, 0));

    window.draw(rect);

    aux=dim/XSPLITFACTOR;

    line.setPosition(sf::Vector2f((float)topLeft.get_x(), (float)topLeft.get_y()));
    line.setPointCount(6);
    if(hover)
        line.setFillColor(sf::Color(191, 191, 191));
    else
        line.setFillColor(sf::Color(255, 255, 255));

    line.setPoint(0, sf::Vector2f(0, 0));
    line.setPoint(1, sf::Vector2f(0, (float)aux.get_y()));
    line.setPoint(2, sf::Vector2f((float)dim.get_x()-(float)aux.get_x(), (float)dim.get_y()));
    line.setPoint(3, sf::Vector2f((float)dim.get_x(), (float)dim.get_y()));
    line.setPoint(4, sf::Vector2f((float)dim.get_x(), (float)dim.get_y()-(float)aux.get_y()));
    line.setPoint(5, sf::Vector2f((float)aux.get_x(), 0));

    window.draw(line);

    line.setPoint(0, sf::Vector2f((float)dim.get_x(), 0));
    line.setPoint(1, sf::Vector2f((float)dim.get_x(), (float)aux.get_y()));
    line.setPoint(2, sf::Vector2f((float)aux.get_x(), (float)dim.get_y()));
    line.setPoint(3, sf::Vector2f(0, (float)dim.get_y()));
    line.setPoint(4, sf::Vector2f(0, (float)dim.get_y()-(float)aux.get_y()));
    line.setPoint(5, sf::Vector2f((float)dim.get_x()-(float)aux.get_x(), 0));

    window.draw(line);
}
void drawReplayButton(Button& btn, bool hover, sf::RenderWindow& window)
{
    sf::RectangleShape rect;
    vec2i topLeft=btn.getTopLeft();
    vec2i dim=btn.getBottomRight()-topLeft;

    rect.setPosition(sf::Vector2f((float)topLeft.get_x(), (float)topLeft.get_y()));
    rect.setSize(sf::Vector2f((float)dim.get_x(), (float)dim.get_y()));
    if(hover)
        rect.setFillColor(sf::Color(0, 0, 191));
    else
        rect.setFillColor(sf::Color(0, 0, 255));

    window.draw(rect);

    dim/=PAUSESPLITFACTOR;
    sf::CircleShape triangle((float)dim.get_x()*2, 3);

    triangle.setRotation(90);
    triangle.setPosition(sf::Vector2f((float)topLeft.get_x()+(float)(dim*4).get_x(), (float)(topLeft+dim/2).get_y()));
    if(hover)
        triangle.setFillColor(sf::Color(191, 191, 191));
    else
        triangle.setFillColor(sf::Color(255, 255, 255));

    window.draw(triangle);
}

Button MenuSystem::gp_pauseButton(vec2i(0, 0), vec2i(50, 50), pauseGame, drawPauseButton);
Button MenuSystem::ps_resumeButton(vec2i(200, 200), vec2i(250, 250), resumeGame, drawResumeButton);
Button MenuSystem::ps_mainMenuButton(vec2i(350, 200), vec2i(400, 250), goToMainMenu, drawXButton);
Button MenuSystem::mm_playGameButton(vec2i(268, 268), vec2i(332, 332), resumeGame, drawResumeButton);
Button MenuSystem::mm_exitGameButton(vec2i(550, 0), vec2i(600, 50), exitGame, drawXButton);
Button MenuSystem::go_replayButton(vec2i(200, 250), vec2i(250, 300), resumeGame, drawReplayButton);
Button MenuSystem::go_mainMenuButton(vec2i(350, 250), vec2i(400, 300), goToMainMenu, drawXButton);

void MenuSystem::init()
{
    buttons[MAINMENU].push_back(&mm_playGameButton);
    buttons[MAINMENU].push_back(&mm_exitGameButton);

    buttons[GAMEPLAY].push_back(&gp_pauseButton);

    buttons[PAUSEMENU].push_back(&ps_resumeButton);
    buttons[PAUSEMENU].push_back(&ps_mainMenuButton);

    buttons[GAMEOVER].push_back(&go_replayButton);
    buttons[GAMEOVER].push_back(&go_mainMenuButton);
}

void MenuSystem::setMenu(MENU newMenu) {MenuSystem::menu=newMenu;}

void MenuSystem::render(sf::RenderWindow& window)
{
    int i;

    for(i=0;i<(int)buttons[menu].size();++i)
        buttons[menu][i]->draw(window);
}

void MenuSystem::click(vec2i mousePos)
{
    int i;

    for(i=0;i<(int)buttons[menu].size();++i)
        buttons[menu][i]->click(mousePos);
}

MENU MenuSystem::getMenu() {return menu;}