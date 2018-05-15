#include <SFML/Graphics.hpp>
#include <time.h>

const int M = 20;
const int N = 10;

int field[M][N] = { 0 };

struct Point
{
	int x=0, y=0;
} a[4], b[4];

int figures[7][4] =
{
	{1, 3, 5, 7}, // I
	{2, 4, 5, 7}, // Z
	{3, 5, 4, 6}, // S
	{3, 5, 4, 7}, // T
	{2, 3, 5, 7}, // L
	{3, 5, 7, 6}, // J
	{2, 3, 4, 5} // O
};

bool check()
{
	for (int i = 0; i < 4; i++)
		if (a[i].x < 0 || a[i].x >= N || a[i].y >= M)
			return 0;
		else if (field[a[i].y][a[i].x])
			return 0;
	return 1;
}


int main()
{
	srand(time(0));

	sf::RenderWindow window(sf::VideoMode(180, 360), "Tetris");

	sf::Texture tex;
	tex.loadFromFile("images/tiles.png");

	sf::Sprite box(tex);
	box.setTextureRect(sf::IntRect(0, 0, 18, 18));

	sf::Text message;
	message.setString("Koniec gry!");

	int dx = 0;
	bool rotateFlag = false;
	int colorNumber = 1;
	float timer = 0, delay = 0.3;

	sf::Clock clock;

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		//------------EVENTS------------//
		sf::Event e;
		while (window.pollEvent(e)) //wykonanie wszystkich zdarzeñ z klawiatury ze stosu
		{
			if (e.type == sf::Event::Closed)
				window.close();
			if (e.type == sf::Event::KeyPressed)
				if (e.key.code == sf::Keyboard::Up)
					rotateFlag = true;
				else if (e.key.code == sf::Keyboard::Left)
					dx = -1;
				else if (e.key.code == sf::Keyboard::Right)
					dx = 1;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			delay = 0.03;
		//--------MOVE-------------------//
		for (int i = 0; i < 4; i++)
		{
			b[i] = a[i];
			a[i].x += dx;
		}
		if (!check())
			for (int i = 0; i < 4; i++)
				a[i] = b[i];

		//---------ROTATE-----------------//
		if (rotateFlag)
		{
			Point p = a[1]; //pivot point of rotation
			for (int i = 0; i < 4; i++)
			{
				int xx = a[i].y - p.y;	
				int yy = a[i].x - p.x;
				a[i].x = p.x - xx;
				a[i].y = p.y + yy;
			}
			if (!check())
				for (int i = 0; i < 4; i++)
					a[i] = b[i];
		}

		//------------ROUND---------------//
		if (timer > delay)
		{
			for (int i = 0; i < 4; i++)
			{
				b[i] = a[i];
				a[i].y += 1;
			}
			if (!check())
			{
				for (int i = 0; i < 4; i++)
					field[b[i].y][b[i].x] = colorNumber;

				colorNumber = 1 + rand() % 7;
				int n = rand() % 7;
				for (int i = 0; i < 4; i++)
				{
					a[i].x = figures[n][i] % 2;
					a[i].y = figures[n][i] / 2;
				}
			}

			timer = 0;
		}
		
		//-------CHECK LINES------------//
		int k = M - 1;
		for (int i = k; i > 0; i--)
		{
			int count = 0;
			for (int j = 0; j < N; j++)
			{
				if (field[i][j])
					count++;
				field[k][j] = field[i][j];
				if (field[1][j] != 0)
					window.close();
			}
			if (count < N)
				k--;
		}

		dx = 0;
		rotateFlag = false;
		delay = 0.3;



		//------------DRAW---------------//
		window.clear(sf::Color::Black);
		for(int i=0; i<M; i++)
			for (int j = 0; j < N; j++)
			{
				if (field[i][j] == 0)
					continue;
				box.setTextureRect(sf::IntRect(field[i][j] * 18, 0, 18, 18));
				box.setPosition(j * 18, i * 18);
				window.draw(box);
			}

		for (int i = 0; i < 4; i++)
		{
			box.setTextureRect(sf::IntRect(colorNumber * 18, 0, 18, 18));
			box.setPosition(a[i].x*18, a[i].y*18);
			window.draw(box);
		}
		window.display();
	}

	return 0;
}