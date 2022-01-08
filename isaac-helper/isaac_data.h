#pragma once
#include <stdint.h>
#include <Windows.h>
#include <unordered_map>
class card {
public:
	std::string title;
	std::string description;
};

class pill {
public:
	std::string title;
	std::string description;
};

class item {
public:
	bool isActiveItem;
	std::string title;
	std::string description;
};


class trinkets {
public:
	std::string title;
	std::string description;
};


DWORD GetPillItem(DWORD n);

/*
	type2: 5
		type3
			- 70 pill
			- 100 collectibles
			- 300 card
			- 350 trinkets

*/
typedef struct tagItemObject {
	/*0x00*/ unsigned int unknown_p_01;
	/*0x04*/ unsigned int unknown_1[7];
	/*0x20*/ unsigned int type1;
	/*0x24*/ unsigned int type2;
	/*0x28*/ unsigned int type3;
	/*0x2C*/ unsigned int itemID;
	/*0x30*/ unsigned int unknown_2[2];
	/*0x38*/ char* szgfx;
} ItemObject;

enum PlayerType : uint32_t {
	MaxHeart = 0x1b14,
	Heart = 0x1b18,
	SoulHeart = 0x1b20,
	DarkHeart = 0x1b24,

	Coin = 0x1B3C,
	Boom = 0x1B38,
	Key = 0x1B30,

	itemCount = 0x1CF8,
	isCard = 0x1E34,
	cardNumber = 0x1E38,
};


void LoadData(std::string lang_path);

namespace GAME {
	extern std::string imagePath;
	extern ItemObject** pItemArray;
	extern DWORD MapItemCount;
	extern uint32_t* pPlayerData;
	extern std::unordered_map<int, card> cardMap;
	extern std::unordered_map<int, pill> pillMap;
	extern std::unordered_map<int, item> itemMap;
	extern std::unordered_map<int, trinkets> trinketsMap;
}
