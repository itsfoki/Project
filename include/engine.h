#pragma once
#include <SFML/Graphics.hpp>
#include "sapper.h"

class Engine
{
public:
	// Обработка нажатий клавиш мыши
	enum class Mouse
	{
		LEFT_CLICK,						// левая кнопка мыши
		RIGHT_CLICK,					// правая кнопка мыши
		MAX_MOUSE_VALUES				// максимальное значение
	};

protected:
	sf::Texture m_texture_background;   // текстура заднего фона сапера
	sf::Sprite m_sprite_backround;		// спрайт заднего фона сапера

private:
	sf::RenderWindow main_window;		// главное окно
	Sapper m_sapper;					// экземпляр сапера

	// Обработка событий
	void event_processing();
	// Логика игры
	void sapper_game(Mouse mouse_click);
	// Отрисовка окна
	void draw_window();

public:
	Engine();

	// Запуск программы
	void start_game();
};