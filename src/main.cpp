#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>

#include <limits>
#include <map>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <algorithm>

#include <SFML/Graphics.hpp>

#define KNIGHT_DEBUG

#include "../lib/simple_ecs.hpp"
#include "../lib/animation.hpp"
#include "../lib/loader.hpp"
#include "../lib/hex.hpp"
#include "../lib/hex_map.hpp"
#include "../lib/game.hpp"
#include "../lib/pathfinding.hpp"
#include "../lib/gui/gui.hpp"

#include "maps.hpp"

struct Animated {
    std::vector<K::Animation> animations;
};

struct AnimatedObject {
    std::unordered_map<std::string, K::Animation> animations;

    std::string active_animation;
    std::string direction;
    K::Hex position;

    void set_position(const K::Hex& hex, const K::Point& hex_size, K::HexMap<Animated>* hex_map) {
        position = hex;

        auto& anim = animations[active_animation];
        const auto sprite_size = anim.get_sprite_size();

        const auto p = hex_map->get_hex_middle(hex);

        anim.set_position(p.x - sprite_size.x / 2, p.y - sprite_size.y / 2);
    }

    void update(const sf::Time &delta, const sf::Vector2i &mouse_pos) {
        animations[active_animation].update(delta);
    }

    void render(sf::RenderWindow &window) {
        animations[active_animation].render(window);
    }
};

int main(int argc, char **argv) {
    K::Game game;

    const int WINDOW_W = 1200;
    const int WINDOW_H = 800;

    game.window = std::make_unique<sf::RenderWindow>(sf::VideoMode(WINDOW_W, WINDOW_H), "knights");

    game.loader = std::make_unique<K::Loader>();

    game.loader
        ->add_texture("grass_01", "../condottiero_assets/grass_01.png")
        ->add_texture("grass_02", "../condottiero_assets/grass_02.png")
        ->add_texture("grass_03", "../condottiero_assets/grass_03.png")
        ->add_texture("grass_04", "../condottiero_assets/grass_04.png")
        ->add_texture("grass_05", "../condottiero_assets/grass_05.png")
        ->add_texture("grass_06", "../condottiero_assets/grass_06.png")
        ->add_texture("grass_07", "../condottiero_assets/grass_07.png")
        ->add_texture("grass_08", "../condottiero_assets/grass_08.png")
        ->add_texture("grass_09", "../condottiero_assets/grass_09.png")
        ->add_texture("grass_10", "../condottiero_assets/grass_10.png")
        ->add_texture("grass_11", "../condottiero_assets/grass_11.png")
        ->add_texture("grass_12", "../condottiero_assets/grass_12.png")
        ->add_texture("grass_13", "../condottiero_assets/grass_13.png")
        ->add_texture("grass_14", "../condottiero_assets/grass_14.png")
        ->add_texture("grass_15", "../condottiero_assets/grass_15.png")
        ->add_texture("grass_16", "../condottiero_assets/grass_16.png")
        ->add_texture("grass_17", "../condottiero_assets/grass_17.png")
        ->add_texture("grass_18", "../condottiero_assets/grass_18.png")

        ->add_texture("dirt_01", "../condottiero_assets/dirt_01.png")
        ->add_texture("dirt_02", "../condottiero_assets/dirt_02.png")
        ->add_texture("dirt_03", "../condottiero_assets/dirt_03.png")
        ->add_texture("dirt_04", "../condottiero_assets/dirt_04.png")
        ->add_texture("dirt_05", "../condottiero_assets/dirt_05.png")
        ->add_texture("dirt_06", "../condottiero_assets/dirt_06.png")
        ->add_texture("dirt_07", "../condottiero_assets/dirt_07.png")
        ->add_texture("dirt_08", "../condottiero_assets/dirt_08.png")
        ->add_texture("dirt_09", "../condottiero_assets/dirt_09.png")
        ->add_texture("dirt_10", "../condottiero_assets/dirt_10.png")
        ->add_texture("dirt_11", "../condottiero_assets/dirt_11.png")
        ->add_texture("dirt_12", "../condottiero_assets/dirt_12.png")
        ->add_texture("dirt_13", "../condottiero_assets/dirt_13.png")
        ->add_texture("dirt_14", "../condottiero_assets/dirt_14.png")
        ->add_texture("dirt_15", "../condottiero_assets/dirt_15.png")
        ->add_texture("dirt_16", "../condottiero_assets/dirt_16.png")
        ->add_texture("dirt_17", "../condottiero_assets/dirt_17.png")
        ->add_texture("dirt_18", "../condottiero_assets/dirt_18.png")

        ->add_texture("stone_01", "../condottiero_assets/stone_01.png")
        ->add_texture("stone_03", "../condottiero_assets/stone_03.png")
        ->add_texture("stone_04", "../condottiero_assets/stone_04.png")
        ->add_texture("stone_05", "../condottiero_assets/stone_05.png")
        ->add_texture("stone_06", "../condottiero_assets/stone_06.png")
        ->add_texture("stone_07", "../condottiero_assets/stone_07.png")
        ->add_texture("stone_08", "../condottiero_assets/stone_08.png")
        ->add_texture("stone_09", "../condottiero_assets/stone_09.png")
        ->add_texture("stone_10", "../condottiero_assets/stone_10.png")
        ->add_texture("stone_11", "../condottiero_assets/stone_11.png")
        ->add_texture("stone_12", "../condottiero_assets/stone_12.png")
        ->add_texture("stone_13", "../condottiero_assets/stone_13.png")
        ->add_texture("stone_14", "../condottiero_assets/stone_14.png")
        ->add_texture("stone_15", "../condottiero_assets/stone_15.png")
        ->add_texture("stone_16", "../condottiero_assets/stone_16.png")
        ->add_texture("stone_17", "../condottiero_assets/stone_17.png")
        ->add_texture("stone_18", "../condottiero_assets/stone_18.png")
        ->add_texture("stone_19", "../condottiero_assets/stone_19.png")

        ->add_texture("archery", "../condottiero_assets/medieval_archery.png")
        ->add_texture("archway", "../condottiero_assets/medieval_archway.png")
        ->add_texture("blacksmith", "../condottiero_assets/medieval_blacksmith.png")
        ->add_texture("cabin", "../condottiero_assets/medieval_cabin.png")
        ->add_texture("church", "../condottiero_assets/medieval_church.png")
        ->add_texture("farm", "../condottiero_assets/medieval_farm.png")
        ->add_texture("house", "../condottiero_assets/medieval_house.png")
        ->add_texture("lumber", "../condottiero_assets/medieval_lumber.png")
        ->add_texture("mine", "../condottiero_assets/medieval_mine.png")
        ->add_texture("large_castle", "../condottiero_assets/medieval_largeCastle.png")
        ->add_texture("open_castle", "../condottiero_assets/medieval_openCastle.png")
        ->add_texture("small_castle", "../condottiero_assets/medieval_smallCastle.png")
        ->add_texture("tower", "../condottiero_assets/medieval_tower.png")
        ->add_texture("ruins", "../condottiero_assets/medieval_ruins.png")
        ->add_texture("windmill", "../condottiero_assets/medieval_windmill.png")

        ->add_font("font_1", "../condottiero_assets/DejaVuSans.ttf")
        ->add_font("font_caviar", "../condottiero_assets/CaviarDreams.ttf")
        ->add_font("font_pens_regular", "../condottiero_assets/penshurst_regular.ttf")
        ->add_font("font_pens_bold", "../condottiero_assets/penshurst_bold.ttf")
        ;
    game.loader->load();

    K::Layout layout(K::Hex::ORIENTATION_POINTY, K::Point(46, 46), K::Point(0, 0));

    const K::Point hex_size(80, 93);

    const auto p1 = K::Hex::from_point(layout, hex_size);

    const K::Point origin(0, 0);
    const K::Point size(WINDOW_W, WINDOW_H);

    sf::View main_view(sf::FloatRect(origin.x, origin.y, size.x, size.y));
    sf::View minimap_view(sf::FloatRect(origin.x, origin.y, size.x, size.y));
    sf::View gui_view(sf::FloatRect(origin.x, origin.y, size.x, size.y));

    game.set_main_view(std::move(main_view));
    game.set_minimap_view(std::move(minimap_view));
    game.set_gui_view(std::move(gui_view));

    K::HexCamera hex_camera(origin, size, hex_size, game.get_main_view());
    hex_camera.set_speed(5);
    hex_camera.set_window_pct(0.18);

    auto hex_map = std::make_unique<K::HexMap<Animated>>(layout, hex_camera, hex_size, game.get_minimap_view());

    game.get_minimap_view()->zoom(3);
    game.get_minimap_view()->setViewport(sf::FloatRect(0.75f, 0.f, 0.25f, 0.25f));

    #ifdef KNIGHT_DEBUG
    hex_map->set_font(game.get_loader().get_font("font_1"));
    hex_map->set_debug_text_size(8);
    #endif

    hex_map->set_hexes(game.get_loader(), get_map_1());
    hex_map->set_view(game.get_window());

    sf::Time per_frame = sf::seconds(1.0f / 60.0f);
    sf::Clock clock;
    sf::Time last_update;

    ecs::System system;

    while (game.get_window().isOpen()) {
        sf::Event event;

        sf::Time delta = clock.getElapsedTime() - last_update;

        sf::Vector2i pos;
        pos = sf::Mouse::getPosition(game.get_window());
        auto world_pos = game.get_window().mapPixelToCoords(pos);

        while (game.get_window().pollEvent(event)) {
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Escape) {
                        game.get_window().close();
                    }
                }
                if (event.type == sf::Event::Closed)
                    game.get_window().close();
            }

        if (delta > per_frame) {
            const auto view = ecs::VectorView::create<AnimatedObject>(system);

            hex_map->update(delta, pos);

            for (ecs::Entity *e : view.entities()) {
                auto &ao = e->get<AnimatedObject>();
                ao->update(delta, pos);
            }

            game.get_window().clear();
            game.get_window().setView(*game.get_main_view());
            hex_map->render(*game.window);

            for (ecs::Entity *e : view.entities()) {
                auto &ao = e->get<AnimatedObject>();
                ao->render(game.get_window());
            }

            game.get_window().setView(*game.get_minimap_view());
            hex_map->draw_tiles(game.get_window());

            game.get_window().setView(*game.get_gui_view());
            game.get_window().display();

            last_update = clock.getElapsedTime();
        }
    }

    return 0;
}
