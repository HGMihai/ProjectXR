#pragma once

struct dupe_settings
{
	bool is_active;
	bool use_disconnect;
	bool use_clean;
	bool use_ban_character;
	bool use_ban_account;
	int check_delay;
};

class CDupeScanner
{
public:
	void load_settings(char* path, char* section);
	void scan();
	static bool check_inventory(int aIndex, int item_serial, int item_pos);
	static bool check_warehouse(int aIndex, int item_serial, int item_pos);
	// ----
	int get_check_delay(){ return this->settings.check_delay; }
private:
	dupe_settings settings;
}; extern CDupeScanner gDupeScanner;