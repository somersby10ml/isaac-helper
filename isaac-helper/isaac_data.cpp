
#define NOMINMAX
#include <rapidcsv.h>

#include "common.h"
#include "isaac_data.h"
#include "util.h"


// helper
std::string GAME::imagePath;
ItemObject** GAME::pItemArray = nullptr;
DWORD GAME::MapItemCount = 0;
uint32_t* GAME::pPlayerData = nullptr;

std::unordered_map<int, card> GAME::cardMap;
std::unordered_map<int, pill> GAME::pillMap;
std::unordered_map<int, item> GAME::itemMap;
std::unordered_map<int, trinkets> GAME::trinketsMap;



DWORD GetPillItem(DWORD n) {
	CApp* App = CApp::getInstance();
	LPDWORD addr = (LPDWORD)( App->dwBaseAddress + 0x4C6C98 );
	DWORD val;
	if (addr && *addr) {
		val = *addr + 0x77BC + 0xA18;

		addr = (LPDWORD)val;
		return addr[n];
	}
	return 0;
}



void LoadData(std::string lang_path) {
	CApp* App = CApp::getInstance();

	std::string csv_path;
	{
		csv_path = path_join(lang_path, "card.csv");
		if (file_exist(csv_path)) {
			try {
				rapidcsv::Document doc(csv_path.c_str());
				std::vector<int> col1 = doc.GetColumn<int>("ID");
				std::vector<std::string> col2 = doc.GetColumn<std::string>("Name");
				std::vector<std::string> col3 = doc.GetColumn<std::string>("Description");
				for (size_t i = 0; i < col1.size(); i++) {
					GAME::cardMap.insert({ col1[i], {col2[i], col3[i]} });
				}
			}
			catch (...) {
				App->Log.Write("[WARN] card.csv loading error (%s)", csv_path.c_str());
			}
		}
		else {
			App->Log.Write("[WARN] csv_path is not found %s", csv_path.c_str());

		}
	}

	{
		if (file_exist(csv_path)) {
			try {
				csv_path = path_join(lang_path, "pill.csv");
				rapidcsv::Document doc(csv_path.c_str());
				std::vector<int> col1 = doc.GetColumn<int>("ID");
				std::vector<std::string> col2 = doc.GetColumn<std::string>("Name");
				std::vector<std::string> col3 = doc.GetColumn<std::string>("Description");
				for (size_t i = 0; i < col1.size(); i++) {
					GAME::pillMap.insert({ col1[i], {col2[i], col3[i]} });
				}
			}
			catch (...) {
				App->Log.Write("[WARN] pill.csv loading error (%s)", csv_path.c_str());
			}
		}
		else {
			App->Log.Write("[WARN] csv_path is not found %s", csv_path.c_str());

		}
	}

	{
		if (file_exist(csv_path)) {
			try {
				csv_path = path_join(lang_path, "trinkets.csv");
				rapidcsv::Document doc(csv_path.c_str());
				std::vector<int> col1 = doc.GetColumn<int>("ID");
				std::vector<std::string> col2 = doc.GetColumn<std::string>("Name");
				std::vector<std::string> col3 = doc.GetColumn<std::string>("Description");
				for (size_t i = 0; i < col1.size(); i++) {
					GAME::trinketsMap.insert({ col1[i], {col2[i], col3[i]} });
				}
			}
			catch (...) {
				App->Log.Write("[WARN] trinkets.csv loading error (%s)", csv_path.c_str());
			}
		}
		else {
			App->Log.Write("[WARN] csv_path is not found %s", csv_path.c_str());

		}

	}


	{
		if (file_exist(csv_path)) {
			try {
				csv_path = path_join(lang_path, "collectibles.csv");
				rapidcsv::Document doc(csv_path.c_str());
				std::vector<int> col1 = doc.GetColumn<int>("ID");
				std::vector<int> col2 = doc.GetColumn<int>("active");
				std::vector<std::string> col3 = doc.GetColumn<std::string>("Name");
				std::vector<std::string> col4 = doc.GetColumn<std::string>("Description");
				for (size_t i = 0; i < col1.size(); i++) {
					GAME::itemMap.insert({ col1[i], {col2[i] == 1, col3[i], col4[i]} });
				}
			}
			catch (...) {
				App->Log.Write("[WARN] collectibles.csv loading error (%s)", csv_path.c_str());
			}
		}
		else {
			App->Log.Write("[WARN] csv_path is not found %s", csv_path.c_str());

		}
	}
}