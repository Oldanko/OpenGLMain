#include <iostream>

#include <thread>
#include <mutex>

using namespace std;

typedef unsigned char ubyte;
typedef unsigned short ushort;
typedef unsigned int uint;

ubyte elem[3][6] = {{ 0, 0, 0, 0, 0, 0 },
					{ 0, 1, 1, 1, 0, 0 },
					{ 1, 2, 2, 2, 1, 0 }};

ubyte mask[6] = { 1, 2, 2, 2, 1, 0 };

class grassPatch
{
	static const int q = 1024;
	static const int size = 16;
	float grass[q];
	int x;
	int y;
	bool update_needed;
	bool updated;

	mutex update_mtx;

public:

	grassPatch()
	{
		x = INT_MIN; y = INT_MIN;
		update_needed = true;
		updated = false;
	}
	grassPatch(int _x, int _y)
	{
		x = INT_MIN; y = INT_MIN;
		init(_x, _y);
	}
	void init(int _x, int _y)
	{
		if (x == _x && y == _y)
			return;

		x = _x;
		y = _y;

		update_mtx.lock();
		update_needed = true;
		updated = false;
		update_mtx.unlock();
	}
	void update()
	{
		update_mtx.lock();
		if (update_needed)
		{
			update_needed = false;
			update_mtx.unlock();
		}
		else
		{
			update_mtx.unlock();
			return;
		}

		for (int i = 0; i < q; i++)
		{
			if (i % 32 == 31)
			{
				update_mtx.lock();
				if (update_needed)
				{
					update_mtx.unlock();
					return;
				}
				update_mtx.unlock();
			}
			grass[i] = rand();
		}

		update_mtx.lock();
		updated = true;
		update_mtx.unlock();
	}
};



class Tiles
{
	static const ubyte tilesNum = 6;
	int x;
	int y;


public:
	grassPatch * tiles[tilesNum][tilesNum];
	Tiles()
	{
		x = 0; 
		y = 0;

		int _x = x - tilesNum / 2 + 1;
		int _y = y - tilesNum / 2 + 1;

		for (int i = 0; i < tilesNum; i++)
			for (int j = 0; j < tilesNum; j++)
				tiles[i][j] = new grassPatch(_x + i, _y + j);
	}
	~Tiles()
	{
		for (int i = 0; i < tilesNum; i++)
			for (int j = 0; j < tilesNum; j++)
			{
				if(tiles[i][j])
					delete tiles[i][j];
			}
	}

	void North()
	{
		y++;

		int _x = x - tilesNum / 2 + 1;
		int _y = y - tilesNum / 2 + 1;
		
		grassPatch * buff[tilesNum];

		for (int i = 0; i < tilesNum; i++)
			buff[i] = tiles[i][0];

		for(int i = 0; i < tilesNum; i++)
			for (int j = 0; j < tilesNum-1; j++)
			{
				tiles[i][j] = tiles[i][j+1];
			}
		
		for (int i = 0; i < tilesNum; i++)
			tiles[i][tilesNum - 1] = buff[i];


		for (int i = 0; i < tilesNum; i++)
			for (int j = 0; j < tilesNum; j++)
				if (elem[mask[i]][j])
					tiles[i][j]->init(_x + i, _y + j);
	}

	void South()
	{
		y--;

		int _x = x - tilesNum / 2 + 1;
		int _y = y - tilesNum / 2 + 1;

		grassPatch * buff[tilesNum];

		for (int i = 0; i < tilesNum; i++)
			buff[i] = tiles[i][tilesNum - 1];

		for (int i = 0; i < tilesNum; i++)
			for (int j = tilesNum - 1; j > 0; j--)
			{
				tiles[i][j] = tiles[i][j - 1];
			}

		for (int i = 0; i < tilesNum; i++)
			tiles[i][0] = buff[i];


		for (int i = 0; i < tilesNum; i++)
			for (int j = 0; j < tilesNum; j++)
				if (elem[mask[i]][j])
					tiles[i][j]->init(_x + i, _y + j);
	}
	
	void East()
	{
		x++;

		int _x = x - tilesNum / 2 + 1;
		int _y = y - tilesNum / 2 + 1;

		grassPatch * buff[tilesNum];

		for (int i = 0; i < tilesNum; i++)
			buff[i] = tiles[0][i];

		for (int i = 0; i < tilesNum-1; i++)
			for (int j = 0; j < tilesNum; j++)
			{
				tiles[i][j] = tiles[i+1][j];
			}

		for (int i = 0; i < tilesNum; i++)
			tiles[tilesNum - 1][i] = buff[i];


		for (int i = 0; i < tilesNum; i++)
			for (int j = 0; j < tilesNum; j++)
				if (elem[mask[i]][j])
					tiles[i][j]->init(_x + i, _y + j);
	}

	void West()
	{
		x--;

		int _x = x - tilesNum / 2 + 1;
		int _y = y - tilesNum / 2 + 1;

		grassPatch * buff[tilesNum];
		
		for (int i = 0; i < tilesNum; i++)
			buff[i] = tiles[tilesNum-1][i];
		
		for (int i = tilesNum - 1; i > 0 ; i--)
			for (int j = 0; j < tilesNum; j++)
			{
				tiles[i][j] = tiles[i - 1][j];
			}
		
		for (int i = 0; i < tilesNum; i++)
			tiles[0][i] = buff[i];


		for (int i = 0; i < tilesNum; i++)
			for (int j = 0; j < tilesNum; j++)
				if (elem[mask[i]][j])
					tiles[i][j]->init(_x + i, _y + j);
	}

	void update()
	{
		int it = 0;

		for (int i = 0; i < tilesNum; i++)
		{
			for (auto tile : tiles[i])
			{
				tile->update();
			}
		}
	}
};


void test()
{
	Tiles t;

	for (int i = 0; i < 100000; i++)
	{

	}

	t.update();
}

void main()
{
	test();
}

