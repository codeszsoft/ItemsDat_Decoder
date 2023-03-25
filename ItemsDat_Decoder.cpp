#include <cstdint>
#include <cstring>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>

#include <filesystem>

struct item
{
	std::uint32_t _M_Id = 0;
	
	std::uint8_t _M_Editable_Type = 0;
	std::uint8_t _M_Item_Category = 0;
	std::uint8_t _M_Action_Type = 0;
	std::uint8_t _M_Hit_Sound_Type = 0;
	
	std::string _M_Name = "";
	
	std::string _M_Texture_Path = "";
	std::uint32_t _M_Texture_Hash = 0;
	
	std::uint8_t _M_Item_Kind = 0;
	
	std::uint32_t _M_Flags1 = 0;
	std::uint32_t _M_Ingredient = 0;
	
	std::uint8_t _M_Texture_X = 0;
	std::uint8_t _M_Texture_Y = 0;
	std::uint8_t _M_Spread_Type = 0;
	std::uint8_t _M_Is_Stripey_Wallpaper = 0;
	std::uint8_t _M_Collision_Type = 0;
	
	std::uint8_t _M_Break_Hits = 0;
	
	std::uint32_t _M_Reset_Time = 0;
	std::uint32_t _M_Grow_Time = 0;
	std::uint32_t _M_Clothing_Type = 0;
	std::uint32_t _M_Rarity = 0;
	
	std::uint8_t _M_Max_Amount = 0;
	
	std::string _M_Extra_File = "";
	std::uint32_t _M_Extra_File_Hash = 0;
	
	std::uint32_t _M_Audio_Volume = 0;
	
	std::string _M_Pet_Name = "";
	std::string _M_Pet_Prefix = "";
	std::string _M_Pet_Suffix = "";
	std::string _M_Pet_Ability = "";
	
	std::uint8_t _M_Seed_Base = 0;
	std::uint8_t _M_Seed_Overlay = 0;
	std::uint8_t _M_Tree_Base = 0;
	std::uint8_t _M_Tree_Leaves = 0;
	
	std::uint32_t _M_Seed_Color = 0;
	std::uint32_t _M_Seed_Overlay_Color = 0;
	
	std::uint16_t _M_Flags2 = 0;
	std::uint16_t _M_Rayman = 0;
	
	std::string _M_Extra_Options = "";
	std::string _M_Texture2_Path = "";
	std::string _M_Extra_Options2 = "";
	std::string _M_Punch_Options = "";
	
	std::uint8_t _M_Unk_Data1[80] = { 0 };
	std::uint8_t _M_Unk_Data2[25] = { 0 };
	
	std::uint32_t _M_Flags3 = 0;
	std::uint32_t _M_Flags4 = 0;
	std::uint32_t _M_Flags5 = 0;
	
	std::uint8_t _M_Bodypart[9] = { 0 };
	
	std::string _M_Texture3_Path = "";
	std::uint32_t _M_Texture3_Hash = 0;
};

static std::vector<item> items;

auto read_items(std::uint8_t* _Data) noexcept -> void
{
	std::uintmax_t pos = 0;
	
	auto read_int = [&_Data, &pos](void* dst, std::size_t size) -> void
	{
		std::memcpy(dst, _Data + pos, size);
		pos += size;
	};
	
	auto read_str = [&_Data, &pos](std::string& ref) -> void
	{
		std::uint16_t len = *reinterpret_cast<std::uint16_t*>(_Data + pos);
		
		ref = std::string(reinterpret_cast<char*>(_Data + pos + 2), len);
		pos += 2 + len;
	};
	
	auto read_str_chiped = [&_Data, &pos](std::string& ref, std::uint32_t id) -> void
	{
		std::uint16_t len = *reinterpret_cast<std::uint16_t*>(_Data + pos);
		
		std::string str(reinterpret_cast<char*>(_Data + pos + 2), len);
		pos += 2 + len;
		
		constexpr std::string_view key = "PBG892FXX982ABC*";
		
		ref.resize(str.size());
		
		for (int i = 0; i < str.size(); i++)
			ref[i] = str[i] ^ key[(i + id) % key.size()];
	};
	
	std::uint16_t version = *reinterpret_cast<std::uint16_t*>(_Data);
	std::uint32_t itemsize = *reinterpret_cast<std::uint32_t*>(_Data + 2);
	pos += 6;
	
	if (version < 11 || version > 15)
	{
		std::cout << "your items.dat version is not supported" << std::endl;
		return;
	}
	
	items.reserve(itemsize);
	
	for (int i = 0; i < itemsize; i++)
	{
		item it;
		
		read_int(&it._M_Id, 4);
		
		if (i != it._M_Id)
		{
			std::cout << "invalid items data" << std::endl;
			return;
		}
		
		read_int(&it._M_Editable_Type, 1);
		read_int(&it._M_Item_Category, 1);
		read_int(&it._M_Action_Type, 1);
		read_int(&it._M_Hit_Sound_Type, 1);
		
		read_str_chiped(it._M_Name, it._M_Id);
		
		read_str(it._M_Texture_Path);
		read_int(&it._M_Texture_Hash, 4);
		
		read_int(&it._M_Item_Kind, 1);
		read_int(&it._M_Flags1, 4);
		
		read_int(&it._M_Texture_X, 1);
		read_int(&it._M_Texture_Y, 1);
		
		read_int(&it._M_Spread_Type, 1);
		read_int(&it._M_Is_Stripey_Wallpaper, 1);
		read_int(&it._M_Collision_Type, 1);
		
		std::uint8_t tmp_break_hits = 0;
		read_int(&tmp_break_hits, 1);
		it._M_Break_Hits = tmp_break_hits / 6;
		
		read_int(&it._M_Reset_Time, 4);
		read_int(&it._M_Clothing_Type, 1);
		read_int(&it._M_Rarity, 2);
		read_int(&it._M_Max_Amount, 1);
		
		read_str(it._M_Extra_File);
		read_int(&it._M_Extra_File_Hash, 4);
		
		read_int(&it._M_Audio_Volume, 4);
		
		read_str(it._M_Pet_Name);
		read_str(it._M_Pet_Prefix);
		read_str(it._M_Pet_Suffix);
		read_str(it._M_Pet_Ability);
		
		read_int(&it._M_Seed_Base, 1);
		read_int(&it._M_Seed_Overlay, 1);
		read_int(&it._M_Tree_Base, 1);
		read_int(&it._M_Tree_Leaves, 1);
		
		read_int(&it._M_Seed_Color, 4);
		read_int(&it._M_Seed_Overlay_Color, 4);
		read_int(&it._M_Ingredient, 4);
		read_int(&it._M_Grow_Time, 4);
		
		read_int(&it._M_Flags2, 2);
		read_int(&it._M_Rayman, 2);
		
		read_str(it._M_Extra_Options);
		read_str(it._M_Texture2_Path);
		read_str(it._M_Extra_Options2);
		
		read_int(it._M_Unk_Data1, 80);
		
		if (version >= 11)
			read_str(it._M_Punch_Options);
			
		if (version >= 12)
		{
			read_int(&it._M_Flags3, 4);
			read_int(it._M_Bodypart, 9);
		}
		
		if (version >= 13)
			read_int(&it._M_Flags4, 4);
			
		if (version >= 14)
			read_int(&it._M_Flags5, 4);
			
		if (version >= 15)
		{
			read_int(it._M_Unk_Data2, 25);
			read_str(it._M_Texture3_Path);
		}
		
		items.push_back(std::move(it));
	}
	
	// TODO: Write all data from item vector to a file
	//       Or do whatever you wan't
}

#define _tmain main
auto _tmain(int argc, char const *argv[]) -> int
{
	if (argc < 2)
	{
		std::cout << "Usage: " << argv[0] << " <PATH>" << std::endl;
		return -1;
	}
	
	std::string_view path = argv[1];
	
	if (!std::filesystem::exists(path))
	{
		std::cout << path << " doesn't seems to be a valid path" << std::endl;
		return -1;
	}

	auto timer_start = std::chrono::high_resolution_clock::now();
	
	std::uintmax_t size = std::filesystem::file_size(path);
	std::uint8_t* data = static_cast<std::uint8_t*>(std::malloc(size));
	
	if (!data)
	{
		std::cout << "allocating memory buffer failed" << std::endl;
		return -1;
	}
	
	std::ifstream fp(path.data(), std::ios::binary);
	
	if (!fp.good())
	{
		std::cout << "reading file failed" << std::endl;
		return -1;
	}
	
	fp.read(reinterpret_cast<char*>(data), size);
	
	fp.close();
	
	read_items(data);
	
	std::free(data);
	
	auto timer_stop = std::chrono::high_resolution_clock::now() - timer_start;
	
	std::cout << "items.dat decoding finished in " << std::chrono::duration_cast<std::chrono::microseconds>(timer_stop).count() << "μs" << std::endl;
	
	return 0;
}
