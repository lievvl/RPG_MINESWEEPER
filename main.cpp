#include <gamebase/Gamebase.h>
#include <fstream>

using namespace gamebase;
using namespace std;

enum Type
{ //. - ÁÌ‡Í ÍÓÌˆ‡
    None,  //Õ≈“ Œ¡Œ«Õ¿◊≈Õ»ﬂ
    Grass, //g
	Grass1,//Õ≈“ Œ¡Œ«Õ¿◊≈Õ»ﬂ
	Grass2,//Õ≈“ Œ¡Œ«Õ¿◊≈Õ»ﬂ
	Grass3,//Õ≈“ Œ¡Œ«Õ¿◊≈Õ»ﬂ
	Grass4,//Õ≈“ Œ¡Œ«Õ¿◊≈Õ»ﬂ
	Grass5,//Õ≈“ Œ¡Œ«Õ¿◊≈Õ»ﬂ
	Mountain, //m
	OpenGrass, //÷»‘–¿, Õ¿◊¿ÀŒ »  ŒÕ≈÷ "," »À» e ƒÀﬂ œ”—“Œ… Õ¿ƒœ»—»
    Water, //w
	Bomb,  //z, y ÓÚÍ˚Ú˚È
	Bomb2, //x, u
	Bomb3, //c, i
	Bomb4, //v, o
	Bomb5, //b, p
	Wall,  //w
    Hero   //h
};

class MyApp : public App
{
	void load()
	{
		connect(NewGame, startgame1);
		connect(Exit, return, 0);
		connect(SaveGame, savegame, "save.txt");
		connect(LoadGame, loadgame1);


		connect(timerstartgame, startgame2);
		connect(timertostats, tostats2);
		connect(timerfromstatstogame, fromstatstogame2);
		connect(timerloadgame, loadgame2, "save.txt");
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

	void loadgame1()
	{
		blackscreen.anim.play("transit1");
		timerloadgame.repeat(0.3);
	}

	void loadgame2(string filename)
	{

		timerfromstatstogame.stop();
		timertostats.stop();
		timerstartgame.stop();
		timervictory.stop();
		timerdefeat.stop();
		timerafterdefeat.stop();
		timerloadgame.stop();

		ifstream file(filename);
		int w, h;
		string rab;

		getline(file, rab);
		w = stoi(rab);

		getline(file, rab);
		h = stoi(rab);

		getline(file, rab);
		hp = stoi(rab);

		getline(file, rab);
		prof = stoi(rab);

		getline(file, rab);
		lvl = stoi(rab);

		getline(file, rab);
		exp = stoi(rab);

		getline(file, rab);
		nextexp = stoi(rab);

		getline(file, rab);
		bombnum = stoi(rab);

		gmap = createMap(w, h);
		for (int y = gmap.h - 1; y >= 0; y--)
		{
			getline(file, rab);
			for (int x = 0; x < gmap.w; x++)
			{
				
				if (rab[x] == 'h')
				{
					gmap[x][y] = OpenGrass;
					shadow.setPos(pixels(x, y));
					pp.x = x;
					pp.y = y;
					auto obj = grasses.load("grass.json", pixels(x, y));
					obj.anim.play("hid");
					auto l = obj.child<Label>("label");
					l.hide();
					
				}

				if (rab[x] == 's')
				{
					gmap[x][y] = OpenGrass;
					auto obj = grasses.load("grass.json", pixels(x, y));
					obj.anim.play("hid");
					auto l = obj.child<Label>("label");
					l.show();
				}

				if (rab[x] == 'j')
				{
					gmap[x][y] = OpenGrass;
					auto obj = grasses.load("grass.json", pixels(x, y));
					obj.anim.play("hid");
					auto l = obj.child<Label>("label");
					l.hide();
				}

				if (rab[x] == 'g')
				{
					grasses.load("grass.json", pixels(x, y));
					gmap[x][y] = Grass;
				}

				if (rab[x] == 'm')
				{
					mountains.load("mountain.json", pixels(x, y));
					gmap[x][y] = Mountain;
				}

				if (rab[x] == 'w')
				{
					waters.load("water.json", pixels(x, y));
					gmap[x][y] = Water;
				}

				if (rab[x] == 'z')
				{
					gmap[x][y] = Bomb;
					auto obj = grasses.load("grass.json", pixels(x, y));
					auto dog = bombs.load("dog.json", pixels(x, y));
					dog.hide();
					continue;
				}

				if (rab[x] == 'y')
				{
					gmap[x][y] = Bomb;
					auto obj = grasses.load("grass.json", pixels(x, y));
					auto dog = bombs.load("dog.json", pixels(x, y));
					dog.show();
					continue;
				}

				if (rab[x] == 'x')
				{
					gmap[x][y] = Bomb2;
					auto obj = grasses.load("grass.json", pixels(x, y));
					auto bomb = bombs.load("ghost.json", pixels(x, y));
					bomb.hide();
					continue;
				}

				if (rab[x] == 'u')
				{
					gmap[x][y] = Bomb2;
					auto obj = grasses.load("grass.json", pixels(x, y));
					auto bomb = bombs.load("ghost.json", pixels(x, y));
					bomb.show();
					continue;
				}

			}
		}

		

		file.close();

		game.select(1);
		prov();
		blackscreen.anim.play("transit2");
	}

	void prov()
	{
		for (int y = gmap.h - 1; y >= 0; y--)
		{
			for (int x = 0; x < gmap.w; x++)
			{
				if (gmap[x][y] == OpenGrass)
				{
					Color mas[8] = {
						Color(0,0,255), Color(0,255,0), Color(255,0,0), Color(148,0,211),
						Color(86,3,25), Color(17,96,98), Color(0,0,0), Color(128,128,128),
					};
					int num = 0;

					for (int sx = -1; sx < 2; sx++)
						for (int sy = -1; sy < 2; sy++)
						{
							if (gmap.get(x + sx, y + sy) == Bomb)
								num++;

							if (gmap.get(x + sx, y + sy) == Bomb2)
								num += 2;

							if (gmap.get(x + sx, y + sy) == Bomb3)
								num += 3;

							if (gmap.get(x + sx, y + sy) == Bomb4)
								num += 4;

							if (gmap.get(x + sx, y + sy) == Bomb5)
								num += 5;
						}

					auto obj = grasses.find(pixels(x, y)).back();
					auto l = obj.child<Label>("label");

					if (num != 0)
					{
						l.show();
						if (num == 40)
							num--;
						l.setColor(mas[num / 5]);
						l << num;
					}

				}
			}
		}
	}

	void savegame(string filename)
	{
		ofstream file(filename);
		file << gmap.w << endl;
		file << gmap.h << endl;
		file << hp << endl;
		file << prof << endl;
		file << lvl << endl;
		file << exp << endl;
		file << nextexp << endl;
		file << bombnum << endl;

		for (int y = gmap.h - 1; y>=0; y--)
		{
			for (int x = 0; x < gmap.w ; x++)
			{
				if (pp.x == x && pp.y == y)
				{
					file << "h";
					continue;
				}

				if (gmap[x][y] == Grass)
				{
					file << "g";
					continue;
				}

				if (gmap[x][y] == Mountain)
				{
					file << "m";
					continue;
				}

				if (gmap[x][y] == Water)
				{
					file << "w";
					continue;
				}

				if (gmap[x][y] == OpenGrass)
				{
					auto obj = grasses.find(pixels(x, y)).back();
					auto l = obj.child<Label>("label");
					if (!l.isVisible())
					{
						file << "j";
						continue;
					}
					else
					{
						file << "s";
						continue;
					}
				}

				if (gmap[x][y] == Bomb)
				{
					auto obj = bombs.find(pixels(x, y)).back();
					if (obj.isVisible())
						file << "y";
					else
						file << "z";
					continue;
				}

				if (gmap[x][y] == Bomb2)
				{
					auto obj = bombs.find(pixels(x, y)).back();
					if (obj.isVisible())
						file << "u";
					else
						file << "x";
					continue;
				}

				if (gmap[x][y] == Bomb3)
				{
					auto obj = bombs.find(pixels(x, y)).back();
					if (obj.isVisible())
						file << "i";
					else
						file << "c";
					continue;
				}

				if (gmap[x][y] == Bomb4)
				{
					auto obj = bombs.find(pixels(x, y)).back();
					if (obj.isVisible())
						file << "o";
					else
						file << "v";
					continue;
				}

				if (gmap[x][y] == Bomb5)
				{
					auto obj = bombs.find(pixels(x, y)).back();
					if (obj.isVisible())
						file << "p";
					else
						file << "b";
					continue;
				}
			}
			file << endl;
		}
		file.close();
		cout << "Done" << endl;
	}

	void restart()
	{
		timerfromstatstogame.stop();
		timertostats.stop();
		timerstartgame.stop();
		timervictory.stop();
		timerdefeat.stop();
		timerafterdefeat.stop();
		timerloadgame.stop();



		randomize();
		map <Color, int> colorToType;
		colorToType[Color(0, 255, 0)] = Grass;
		colorToType[Color(0, 200, 0)] = Grass1;
		colorToType[Color(0, 160, 0)] = Grass2;
		colorToType[Color(0, 120, 0)] = Grass3;
		colorToType[Color(0, 80, 0)] = Grass4;
		colorToType[Color(0, 40, 0)] = Grass5;
		colorToType[Color(0, 0, 255)] = Water;
		colorToType[Color(185, 122, 87)] = Mountain;
		colorToType[Color(128, 128, 128)] = Hero;
		colorToType[Color(20, 20, 20)] = Bomb2;
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
						dog.hide();
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
						dog.hide();
						continue;
					}

					if (randomInt(1, 100) <= 20)
					{
						gmap[x][y] = Bomb2;
						auto ghost = bombs.load("ghost.json", pixels(x, y));
						ghost.hide();
						continue;
					}
					gmap[x][y] = Grass;
				}





				//“≈ —“”– »

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

				if (gmap.get(x, y) == Mountain)
					mountains.load("mountain.json", pixels(x, y));
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

	void afterdefeat(string direct)
	{
		timerafterdefeat.stop();
		shadow.anim.play("from" + direct, 5);
		shadow.anim.play("afterdefeat", 5);
	}

	void defeat(IntVec2 v, int bomblvl, string direct)
	{
		timerdefeat.stop();
		shadow.anim.resume();
		hp -= dmgbomb[bomblvl - 1];

		if (direct == "left")
			pp.x++;

		if (direct == "right")
			pp.x--;

		if (direct == "up")
			pp.y--;

		if (direct == "down")
			pp.y++;

		if (hp <= 0)
		{
			ded();
		}
		else
		{
			connect(timerafterdefeat, afterdefeat, direct);
			timerafterdefeat.repeat(0.5);
		}
	}

	void ded()
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
				bomb.show();

				connect(timervictory, victory, v, bomblvl);
				timervictory.repeat(1);
				return;
			}
			else
			{
				shadow.anim.pause(0);
				auto bomb = bombs.find(pixels(v.x, v.y)).back();
				bomb.show();

				connect(timerdefeat, defeat, v, bomblvl, direct);
				timerdefeat.repeat(1);
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
			l.hide();
			if (num != 0)
			{
				l.show();
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
		if (game.selected() == 1 && timervictory.isPaused() && timerdefeat.isPaused() && hp > 0)
		{
			if (gmap[pp.x + 1][pp.y] == Grass || gmap[pp.x + 1][pp.y] == OpenGrass || (gmap[pp.x + 1][pp.y] >= Bomb && gmap[pp.x + 1][pp.y] <= Bomb5))
				if (input.pressed(Right) && shadow.anim.isEmpty(0))
				{
					shadow.anim.play("rotateright", 5);
					shadow.anim.run("right");
					pp.x++;
					volni(pp, "right");
				}
			if (pp.x != 0)
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

			if (pp.y != 0)
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
		if (lvl < 5)
			nextexp = lvlmas[lvl - 1] - exp;
		else
			nextexp = 0;


		if (exp >= lvlmas[0] && lvl == 1)
			lvl++;
		if (exp >= lvlmas[1] && lvl == 2)
			lvl++;
		if (exp >= lvlmas[2] && lvl == 3)
			lvl++;
		if (exp >= lvlmas[3] && lvl == 4)
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
	FromDesign(Button, SaveGame);
	FromDesign(Button, ToMenuFromStats);

	FromDesign(Label, profl);
	FromDesign(Label, hpl);
	FromDesign(Label, lvll);
	FromDesign(Label, expl);
	FromDesign(Label, nextexpl);

    LayerFromDesign(void, grasses);
    LayerFromDesign(void, waters);
	LayerFromDesign(void, mountains);
	LayerFromDesign(void, bombs);

    IntVec2 pp;
	Timer timertostats;
	Timer timerstartgame;
	Timer timerfromstatstogame;
	Timer timervictory;
	Timer timerdefeat;
	Timer timerafterdefeat;
	Timer timerloadgame;

	int flagstartgame = 0;
	int bombnum = 0;

	int prof = 0;
	int lvl = 1;
	int exp = 0;
	int nextexp = 0;
	int hp = 100;

	int dmgbomb[5] = {
		10, 20, 35, 60, 100
	};

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
