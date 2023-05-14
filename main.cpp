//Ilie Dumitru
#include<SFML/Graphics.hpp>
#include<chrono>
#include<cstdlib>
#include"MenuSystem.h"
#include"Game.h"
#include"AppException.h"

int main()
{
    int WINDOWHEIGHT=600;
    int WINDOWWIDTH=600;
    sf::RenderWindow window;
    // NOTE: sync with env variable APP_WINDOW from .github/workflows/cmake.yml:30
    window.create(sf::VideoMode({600, 600}), "Shooter", sf::Style::Default);
    window.setVerticalSyncEnabled(false);
    //window.setFramerateLimit(60);

    MenuSystem::init();

    Game game;

    srand(time(nullptr));

    game.getPlayer().setCenter(vec2f((float)WINDOWWIDTH/2.f, (float)WINDOWHEIGHT/2.f));

    while(window.isOpen())
    {
        game.tick(window);

        window.clear();
        try
        {
            game.render(window);
        }
        catch(MissingFontException&)
        {
            window.close();
            break;
        }
        window.display();

        if(MenuSystem::getMenu()==EXITGAME)
        {
            window.close();
            break;
        }
    }

    return 0;
}
