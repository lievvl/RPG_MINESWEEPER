#include <gamebase/Gamebase.h>

using namespace gamebase;
using namespace std;

enum Type
{
    None,
    Grass,
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
		randomize();
        map <Color, int> colorToType;
        colorToType[Color(0, 255, 0)] = Grass;
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
					if (randomInt(1, 10) == 1)
					{
						gmap[x][y] = Bomb;
						auto dog = dogs.load("dog.json", pixels(x, y));
						//dog.anim.play("hid");
					}
				}

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
    }

    Vec2 pixels(int x, int y)
    {
        Vec2 v;
        v.x = x * 50 - gmap.w * 50 / 2 + 50 / 2;
        v.y = y * 50 - gmap.h * 50 / 2 + 50 / 2;
        return v;
    }

	// deque

	void volni(IntVec2 v)
	{
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
						num+=5;

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
		if (gmap[pp.x + 1][pp.y] == Grass || gmap[pp.x + 1][pp.y] == OpenGrass)
        if (input.pressed(Right) && shadow.anim.isEmpty(0))
        {
            shadow.anim.run("right");
			pp.x++;
			volni(pp);
        }

		if (gmap[pp.x - 1][pp.y] == Grass || gmap[pp.x - 1][pp.y] == OpenGrass)
        if (input.pressed(Left) && shadow.anim.isEmpty(0))
        {
            shadow.anim.run("left");
            pp.x--;
			volni(pp);
        }

		if (gmap[pp.x][pp.y + 1] == Grass || gmap[pp.x][pp.y + 1] == OpenGrass)
        if (input.pressed(Up) && shadow.anim.isEmpty(0))
        {
            shadow.anim.run("up");
            pp.y++;
			volni(pp);
        }

		if (gmap[pp.x][pp.y - 1] == Grass || gmap[pp.x][pp.y - 1] == OpenGrass)
        if (input.pressed(Down) && shadow.anim.isEmpty(0))
        {
            shadow.anim.run("down");
            pp.y--;
			volni(pp);
        }
    }

    void move()
    {
        field.setView(shadow.pos());
    }

    GameMap gmap;
    FromDesign(GameObj, shadow);
    FromDesign(GameView, field);

    LayerFromDesign(void, grasses);
    LayerFromDesign(void, waters);
	LayerFromDesign(void, dogs);

    IntVec2 pp;
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
