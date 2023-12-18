#pragma once

#include <array>
#include <random>

#include <SFML/Graphics.hpp>

class Sapper
{
protected:
    sf::Texture m_texture_tiles;                                // общая текстура ячеек
    std::array<sf::Sprite, 12> m_sprite_tiles;                  // спрайты игровых ячеек

private:
    std::array<std::array<int32_t, 20>, 20> m_logic_field;        // игровая логика поля
    std::array<std::array<int32_t, 20>, 20> m_fill_field;         // сгенерированное поле

public:
    Sapper();
    // Замена ячейки новым переданным значением спрайта после клика мыши
    void replace_fill_sprite(int32_t x_mouse, int32_t y_mouse, int32_t value);
    // Проверка, что ячейка не выходит за пределы игрового поля
    bool check_cell(int32_t &cell_1, int32_t &cell_2) const;

    // Возвращение числа внешней ячейки
    std::array<std::array<int32_t, 20>, 20> get_fill_field() const;
    // Возвращение числа внутренней ячейки
    std::array<std::array<int32_t, 20>, 20> get_logic_field() const;
    // Возвращение спрайта ячейки
    std::array<sf::Sprite, 12> get_sprite_tiles() const;
};