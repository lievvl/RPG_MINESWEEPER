#include <gamebase/Gamebase.h>

using namespace gamebase;
using namespace std;

enum Type
{
    None,
    Grass,
	Grass1,
	Grass2,
	Grass3,
	Grass4,
	Grass5,
	OpenGrass,
    Water,
	Bomb,
	Bomb2,
	Bomb3,
	Bomb4,
	Bomb5,
	Wall,
    Hero
};

class MyApp : public App
{
    void load()
    {
		connect(NewGame, startgame1);
		connect(Exit, return, 0);


		connect(timerstartgame, startgame2);
		connect(timertostats, tostats2);
		connect(timerfromstatstogame, fromstatstogame2);
    }

	void startgame1()
	{
		blackscreen.anim.play("transit1");
		timerstartgame.repeat(0.3);
	}

	void startgame2()
	{
		timerstartgame.stop();
		game.select(1);
		restart();
	}

	void tostats1()
	{
		blackscreen.anim.play("transit1");
		timertostats.repeat(0.3);
	}

	void tostats2()
	{
		timertostats.stop();
		hpl << hp;
		lvll << lvl;
		expl << exp;
		nextexpl << nextexp;
		game.select(2);
		blackscreen.anim.play("transit2");
	}

	void fromstatstogame1()
	{
		blackscreen.anim.play("transit1");
		timerfromstatstogame.repeat(0.3);
	}

	void fromstatstogame2()
	{
		timerfromstatstogame.stop();
		game.select(1);
		blackscreen.anim.play("transit2");
	}

	void restart()
	{
		timerfromstatstogame.stop();
		timertostats.stop();
		timerstartgame.stop();
		timervictory.stop();



		randomize();
		map <Color, int> colorToType;
		colorToType[Color(0, 255, 0)] = Grass;
		colorToType[Color(0, 200, 0)] = Grass1;
		colorToType[Color(0, 160, 0)] = Grass2;
		colorToType[Color(0, 120, 0)] = Grass3;
		colorToType[Color(0, 80, 0)] = Grass4;
		colorToType[Color(0, 40, 0)] = Grass5;
		colorToType[Color(0, 0, 255)] = Water;
		colorToType[Color(128, 128, 128)] = Hero;
		colorToType[Color(0, 0, 0)] = Wall;

		gmap = loadMap("map.png", colorToType);
		for (int x = 0; x < gmap.w; x++)
		{
			for (int y = 0; y < gmap.h; y++)
			{
				if (gmap.get(x, y) == Grass)
				{
					grasses.load("grass.json", pixels(x, y));
					gmap[x][y] = Grass;
				}





				if (gmap.get(x, y) == Grass1) //11111111111111111111111111111111111111111111111
				{
					grasses.load("grass.json", pixels(x, y));
					if (randomInt(1, 100) <= 20)
					{
						gmap[x][y] = Bomb;
						auto dog = bombs.load("dog.json", pixels(x, y));
						dog.anim.play("hid");
						continue;
					}
					gmap[x][y] = Grass;
				}





				if (gmap.get(x, y) == Grass2) //222222222222222222222222222222222222222222222
				{
					grasses.load("grass.json", pixels(x, y));
					if (randomInt(1, 100) <= 15)
					{
						gmap[x][y] = Bomb;
						auto dog = bombs.load("dog.json", pixels(x, y));
						dog.anim.play("hid");
						continue;
					}

					if (randomInt(1, 100) <= 20)
					{
						gmap[x][y] = Bomb2;
						auto ghost = bombs.load("ghost.json", pixels(x, y));
						ghost.anim.play("hid");
						continue;
					}
					gmap[x][y] = Grass;
				}





				//ÒÅÊÑÒÓÐÊÈ

				if (gmap.get(x, y) == Water)
					waters.load("Water.json", pixels(x, y));

				if (gmap.get(x, y) == Hero)
				{
					shadow.setPos(pixels(x, y));
					pp.x = x;
					pp.y = y;
					gmap[x][y] = Grass;
					grasses.load("grass.json", pixels(x, y));

				}
			}
		}

		flagstartgame = 5;
	}

    Vec2 pixels(int x, int y)
    {
        Vec2 v;
        v.x = x * 50 - gmap.w * 50 / 2 + 50 / 2;
        v.y = y * 50 - gmap.h * 50 / 2 + 50 / 2;
        return v;
    }

	// deque

	void victory(IntVec2 v, int bomblvl)
	{

		Color mas[8] = {
			Color(0,0,255), Color(0,255,0), Color(255,0,0), Color(148,0,211),
			Color(86,3,25), Color(17,96,98), Color(0,0,0), Color(128,128,128),
		};
		int num = 0;
		timervictory.stop();
		shadow.anim.resume();
		gmap[v] = Grass;

		for (int x = -1; x < 2; x++)
		{
			for (int y = -1; y < 2; y++)
			{
				if (gmap.get(v.x + x, v.y + y) == OpenGrass)
				{
					auto obj = grasses.find(pixels(v.x + x, v.y + y)).back();
					auto l = obj.child<Label>("label");
					if (stoi(l.text(), 0, 10) - bomblvl == 0)
						l.hide();
					else
					{
						l.setColor(mas[(stoi(l.text(), 0, 10) - bomblvl) / 5]);
						l << stoi(l.text(), 0, 10) - bomblvl;
					}
				}

				if (gmap.get(v.x + x, v.y + y) == Bomb)
					num++;

				if (gmap.get(v.x + x, v.y + y) == Bomb2)
					num += 2;

				if (gmap.get(v.x + x, v.y + y) == Bomb3)
					num += 3;

				if (gmap.get(v.x + x, v.y + y) == Bomb4)
					num += 4;

				if (gmap.get(v.x + x, v.y + y) == Bomb5)
					num += 5;
			}
		}

		exp += bomblvl * 20;
		auto bomb = bombs.find(pixels(v.x, v.y)).back();
		bomb.kill();
		auto obj = grasses.find(pixels(v.x, v.y)).back();
		obj.anim.play("hid");
		auto l = obj.child<Label>("label");
		if (num != 0)
		{
			if (num == 40)
				num--;
			l.setColor(mas[num / 5]);
			l << num;
		};

		gmap[v] = OpenGrass;


	}

	void defeat(IntVec2 v, int bomblvl)
	{

	}

	void volni(IntVec2 v, string direct)
	{
		if (gmap[v] >= Bomb && gmap[v] <= Bomb5)
		{
			int bomblvl = gmap[v] - Bomb + 1;
			if (lvl >= bomblvl)
			{
				shadow.anim.pause(0);
				auto bomb = bombs.find(pixels(v.x, v.y)).back();
				bomb.anim.play("show", 2);

				connect(timervictory, victory, v, bomblvl);
				timervictory.repeat(1);
				return;
			}
			else
			{
				shadow.anim.pause();
				auto bomb = bombs.find(pixels(v.x, v.y)).back();
				bomb.anim.play("show", 2);

				connect(timerdefeat, defeat, v, bomblvl);
				timervictory.repeat(1);
				return;
			}
		}

		deque<IntVec2> queue;
		queue.push_back(v);

		Color mas[8] = {
			Color(0,0,255), Color(0,255,0), Color(255,0,0), Color(148,0,211),
			Color(86,3,25), Color(17,96,98), Color(0,0,0), Color(128,128,128),
		};

		for (;;)
		{
			int num = 0;
			if (queue.empty())
				break;
			v = queue.back();
			queue.pop_back();

			if (gmap.get(v) != Grass)
				continue;

			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
				{
					if (gmap.get(v.x + x, v.y + y) == Bomb)
						num++;

					if (gmap.get(v.x + x, v.y + y) == Bomb2)
						num+=2;
					
					if (gmap.get(v.x + x, v.y + y) == Bomb3)
						num+=3;

					if (gmap.get(v.x + x, v.y + y) == Bomb4)
						num+=4;

					if (gmap.get(v.x + x, v.y + y) == Bomb5)
						num+=5;
				}
			}
			
			if (gmap.get(v) == Grass && num == 0)
			{
				queue.push_front(v + IntVec2(1, 1));
				queue.push_front(v + IntVec2(1, -1));
				queue.push_front(v + IntVec2(-1, 1));
				queue.push_front(v + IntVec2(-1, -1));

				queue.push_front(v + IntVec2(1, 0));
				queue.push_front(v + IntVec2(-1, 0));
				queue.push_front(v + IntVec2(0, 1));
				queue.push_front(v + IntVec2(0, -1));
			}
			auto obj = grasses.find(pixels(v.x, v.y)).back();
			obj.anim.play("hid");
			auto l = obj.child<Label>("label");
			if (num != 0)
			{
				if (num == 40)
					num--;
				l.setColor(mas[num / 5]);
				l << num;
			}

			gmap[v] = OpenGrass;
		}
	}


    void process(Input input)
    {
        using namespace gamebase::InputKey;
		if (game.selected() == 1 && timervictory.isPaused() && hp > 0)
		{
			if (gmap[pp.x + 1][pp.y] == Grass || gmap[pp.x + 1][pp.y] == OpenGrass || (gmap[pp.x + 1][pp.y] >= Bomb && gmap[pp.x + 1][pp.y] <= Bomb5))
				if (input.pressed(Right) && shadow.anim.isEmpty(0))
				{
					shadow.anim.play("rotateright", 5);
					shadow.anim.run("right");
					pp.x++;
					volni(pp, "right");
				}

			if (gmap[pp.x - 1][pp.y] == Grass || gmap[pp.x - 1][pp.y] == OpenGrass || (gmap[pp.x - 1][pp.y] >= Bomb && gmap[pp.x - 1][pp.y] <= Bomb5))
				if (input.pressed(Left) && shadow.anim.isEmpty(0))
				{
					shadow.anim.play("rotateleft", 5);
					shadow.anim.run("left");
					pp.x--;
					volni(pp, "left");
				}

			if (gmap[pp.x][pp.y + 1] == Grass || gmap[pp.x][pp.y + 1] == OpenGrass || (gmap[pp.x][pp.y + 1] >= Bomb && gmap[pp.x][pp.y + 1] <= Bomb5))
				if (input.pressed(Up) && shadow.anim.isEmpty(0))
				{
					shadow.anim.play("rotateup", 5);
					shadow.anim.run("up");
					pp.y++;
					volni(pp, "up");
				}

			if (gmap[pp.x][pp.y - 1] == Grass || gmap[pp.x][pp.y - 1] == OpenGrass || (gmap[pp.x][pp.y - 1] >= Bomb && gmap[pp.x][pp.y - 1] <= Bomb5))
				if (input.pressed(Down) && shadow.anim.isEmpty(0))
				{
					shadow.anim.play("rotatedown", 5);
					shadow.anim.run("down");
					pp.y--;
					volni(pp, "down");
				}
		}



		if (input.pressed(Tab))
		{
			if (game.selected() == 1)
			{
				tostats1();
			}

			if (game.selected() == 2)
			{
				fromstatstogame1();
			}
		}
    }

    void move()
    {
		if (game.selected() == 1)
		{
			field.setView(shadow.pos());
		}

		nextexp = lvlmas[lvl - 1] - exp;


		if (exp >= lvlmas[0] && lvl == 1)
			lvl++;
		if (exp >= lvlmas[1] && lvl == 2)
			lvl++;


		if (flagstartgame >= 0)
			flagstartgame--;


		if (flagstartgame == 1)
		{
			blackscreen.anim.play("transit2", 3);
		}


    }

    GameMap gmap;
    FromDesign(GameObj, shadow);
    FromDesign(GameView, field);
	FromDesign(GameObj, blackscreen);
	FromDesign(Selector, game);
	FromDesign(Button, NewGame);
	FromDesign(Button, LoadGame);
	FromDesign(Button, Help);
	FromDesign(Button, Exit);

	FromDesign(Label, profl);
	FromDesign(Label, hpl);
	FromDesign(Label, lvll);
	FromDesign(Label, expl);
	FromDesign(Label, nextexpl);

    LayerFromDesign(void, grasses);
    LayerFromDesign(void, waters);
	LayerFromDesign(void, bombs);

    IntVec2 pp;
	Timer timertostats;
	Timer timerstartgame;
	Timer timerfromstatstogame;
	Timer timervictory;
	Timer timerdefeat;
	int flagstartgame = 0;
	int bombnum = 0;

	int prof = 0;
	int lvl = 1;
	int exp = 0;
	int nextexp = 0;
	int hp = 100;



	int lvlmas[4] = {
		200, 1200, 3000, 5000
	};

};

int main(int argc, char** argv)
{
    MyApp app;
    app.setConfig("RPG_MINESWEEPERConfig.json");
    app.setDesign("Design.json");
    if (!app.init(&argc, argv))
        return 1;
    app.run();
    return 0;
}
