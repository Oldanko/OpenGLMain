#include <iostream>



using namespace std;

typedef unsigned char ubyte;
typedef unsigned int uint;

ubyte elem[][7] = {	{ 0, 0, 0, 0, 0, 0, 0 },
					{ 0, 0, 1, 1, 1, 0, 0 },
					{ 0, 1, 1, 1, 1, 1, 0 }};

ubyte mask[] = { 0, 1, 2, 2, 2, 1, 0 };

class placeholder
{
public:
	int ba;
	static uint num;
	placeholder()
	{
		ba = 1;
		num++;
	}
	~placeholder()
	{
		num--;
	}
};

uint placeholder::num = 0;

class Tiles
{
public:
	placeholder* tiles[7][7];
	Tiles()
	{
		for (int i = 0; i < 7; i++)
			for (int j = 0; j < 7; j++)
			{
				if (elem[mask[i]][j])
					tiles[i][j] = new placeholder();
				else
					tiles[i][j] = nullptr;
			}
	}
	~Tiles()
	{
		for (int i = 0; i < 7; i++)
			for (int j = 0; j < 7; j++)
			{
				if(tiles[i][j])
					delete tiles[i][j];
			}
	}

	void North()
	{
		for (int j = 0; j < 7; j++)
		{
			if (tiles[6][j])
				delete tiles[6][j];

			tiles[6][j] = nullptr;
		}

		for(int i = 6; i > 0; i--)
				for (int j = 0; j < 7; j++)
				{
					tiles[i][j] = tiles[i - 1][j];
					if (!tiles[i][j] && elem[mask[i]][j])
						tiles[i][j] = new placeholder();
				}
			
		for (int j = 0; j < 7; j++)
		{
			if(tiles[0][j])
				tiles[0][j] = nullptr;

		}
	}

	void South()
	{
		for (int j = 0; j < 7; j++)
		{
			if (tiles[0][j])
				delete tiles[0][j];

			tiles[0][j] = nullptr;
		}

		for (int i = 0; i < 6; i++)
			for (int j = 0; j < 7; j++)
			{
				tiles[i][j] = tiles[i + 1][j];
				if (!tiles[i][j] && elem[mask[i]][j])
					tiles[i][j] = new placeholder();
			}

		for (int j = 0; j < 7; j++)
		{
			if (tiles[6][j])
				tiles[6][j] = nullptr;

		}
	}

	void West()
	{
		for (int i = 0; i < 7; i++)
		{
			if (tiles[i][6])
				delete tiles[i][6];

			tiles[i][6] = nullptr;
		}

		for (int i = 0; i < 7; i++)
			for (int j = 6; j > 0; j--)
			{
				tiles[i][j] = tiles[i][j - 1];
				if (!tiles[i][j] && elem[mask[i]][j])
					tiles[i][j] = new placeholder();
			}

		for (int i = 0; i < 7; i++)
		{
			if (tiles[i][0])
				tiles[i][0] = nullptr;

		}
	}

	void East()
	{
		for (int i = 0; i < 7; i++)
		{
			if (tiles[i][0])
				delete tiles[i][0];

			tiles[i][0] = nullptr;
		}

		for (int i = 0; i < 7; i++)
			for (int j = 0; j < 6; j++)
			{
				tiles[i][j] = tiles[i][j+1];
				if (!tiles[i][j] && elem[mask[i]][j])
					tiles[i][j] = new placeholder();
			}

		for (int i = 0; i < 7; i++)
		{
			if (tiles[i][6])
				tiles[i][6] = nullptr;

		}
	}
};


void test()
{
	Tiles t;

	auto printMap = [&t]()
	{
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if (t.tiles[i][j])
					std::cout << t.tiles[i][j]->ba << " ";
				else
					std::cout << 0 << " ";
			}
			std::cout << "\n";
		}
		std::cout << "\n";
	};


	printMap();
	
	t.West();
	printMap();
	t.West();
	printMap();
	
	t.East();
	printMap();
	t.East();
	printMap();
	
}

void main()
{
	test();
	std::cout << placeholder::num << "\n";
}

