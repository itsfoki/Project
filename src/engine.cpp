#include "../include/engine.h"

Engine::Engine()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    main_window.create(sf::VideoMode(640, 640), "Sapper", sf::Style::Default, settings);

    m_texture_background.loadFromFile("resources//img//background.jpg");
    m_sprite_backround.setTexture(m_texture_background);
}

void Engine::start_game()
{
    while (main_window.isOpen()) 
    {
        event_processing();
        draw_window();
    }
}

void Engine::event_processing()
{
    sf::Event e;
    while (main_window.pollEvent(e))
    {
        // Закрытие окна после нажатия на крестик
        if (e.type == sf::Event::Closed) main_window.close();

        // Обработка нажатий кнопок мыши
        if (e.type == sf::Event::MouseButtonPressed) 
        {
            // Открываем или закрываем флажок при нажатии правой кнопкой мыши
            if (e.key.code == sf::Mouse::Right) sapper_game(Mouse::RIGHT_CLICK);
            // Открываем закрытую ячейку левой кнопкой мыши
            if (e.key.code == sf::Mouse::Left)  sapper_game(Mouse::LEFT_CLICK);
        }
    }
}

void Engine::sapper_game(Mouse mouse_click)
{
    // Получаем координаты курсора мышки относительно главного окна приложения
    sf::Vector2i pos = sf::Mouse::getPosition(main_window);
    int32_t x_mouse = pos.x / 32;
    int32_t y_mouse = pos.y / 32;

    std::array<std::array<int32_t, 20>, 20> fill = m_sapper.get_fill_field();   // значение ячейки
    std::array<std::array<int32_t, 20>, 20> logic = m_sapper.get_logic_field(); // внутренняя часть

    switch (mouse_click)
    {
        case Mouse::RIGHT_CLICK:
        {
            // Если флаг не стоит, ставим на выбранную позицию
            if (fill.at(x_mouse).at(y_mouse) == 10) {
                m_sapper.replace_fill_sprite(x_mouse, y_mouse, 11);
            }
            // Если флаг уже стоит, то после нажании правой кнопки мыши он убирается
            else if (fill.at(x_mouse).at(y_mouse) == 11) {
                m_sapper.replace_fill_sprite(x_mouse, y_mouse, 10);
            } break;
        }

        case Mouse::LEFT_CLICK:
        {
            // Заменяем спрайт внешней ячейки на спрайт внутренней ячейки (открываем ее)
            m_sapper.replace_fill_sprite(x_mouse, y_mouse, logic.at(x_mouse).at(y_mouse));

            // Если мы нашли мину, открываем ее и заканчиваем игру
            if (logic.at(x_mouse).at(y_mouse) == 9) {
                for (int32_t x{}; x < logic.size(); ++x)
                    for (int32_t y{}; y < logic.size(); ++y)
                        m_sapper.replace_fill_sprite(x, y, logic.at(x).at(y));
            }

            // Если первая ячейка закрытая и пустая, начинаем открывать все близлежащие пустые и прилегающие числа
            if (logic.at(x_mouse).at(y_mouse) == 0 && fill.at(x_mouse).at(y_mouse) == 10) 
            {
                // Проверяем только открытую клетку
                std::array<std::array<bool, 20>, 20> check_open{false};
                // Самая первая пустая ячейка будет открытой
                check_open.at(x_mouse).at(y_mouse) = true;

                // Задаем границы проверяемой области - все игровое поле
                for (int32_t square_cells{0}; square_cells < fill.size(); ++square_cells)

                    // Перебираем все ячейки на игровом поле до тех пор, пока не откроем все прилегающие рядом пустые
                    for (int32_t x {x_mouse - square_cells}; x <= x_mouse + square_cells; ++x)
                        for (int32_t y {y_mouse - square_cells}; y <= y_mouse + square_cells; ++y)

                            // Проверка на выход за границы уже ближайших ячеек (также они должны быть пустыми и открытыми)
                            if (m_sapper.check_cell(x, y) && logic.at(x).at(y) == 0 && check_open.at(x).at(y)) {
                                // Открываем вокруг выбранной ячейки квадрат 3x3
                                for (int32_t i {x - 1}; i <= x + 1; ++i)
                                    for (int32_t j {y - 1}; j <= y + 1; ++j)
                                        // Проверка, что ячейка не выходит за границы игрового поля
                                        if (m_sapper.check_cell(i, j)) {
                                            // Не проверяем те ячейки, что уже открыты
                                            if (check_open.at(i).at(j) == true) continue;
                                            check_open.at(i).at(j) = true;
                                            // Открываем все ячейки вокруг пустой клетки
                                            m_sapper.replace_fill_sprite(i, j, logic.at(i).at(j));
                                        }
                            }
            } break;
        }
        default: break;
    }
}

void Engine::draw_window()
{
    std::array<sf::Sprite, 12> sprite = m_sapper.get_sprite_tiles();            // спрайт ячейки
    std::array<std::array<int32_t, 20>, 20> fill = m_sapper.get_fill_field();   // значение ячейки

    // Отрисовка заднего фона
    main_window.clear(sf::Color::White);
    main_window.draw(m_sprite_backround);

    // Полная отрисовка игрового поля
    for (int32_t x{}; x < fill.size(); ++x) {
        for (int32_t y{}; y < fill.size(); ++y) {
            // Устанавливаем координаты для каждой ячейки
            sprite.at(fill.at(x).at(y)).setPosition(x * 32, y * 32);
            // Для каждой ячейки отрисовывем свой спрайт
            main_window.draw(sprite.at(fill.at(x).at(y)));
        }
    }

    // Полное отображение поля
    main_window.display();
}