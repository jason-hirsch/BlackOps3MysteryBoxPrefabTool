#include <filesystem>
#include <regex>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


int main()
{
	std::string path;
	std::string addOrRemove;

	std::cout << "Destination folder: ";
	getline(std::cin, path);

	std::cout << "Add or remove: ";
	getline(std::cin, addOrRemove);
	std::transform(addOrRemove.begin(), addOrRemove.end(), addOrRemove.begin(), [](char c) {return std::tolower(c); });


	std::vector<int> boxes;
	if (addOrRemove == "add" || addOrRemove == "remove")
	{
		std::string boxList;
		std::cout << "Type list of boxes to " << addOrRemove << "(ex 1,3,11): ";
		std::cin >> boxList;

		std::string curr = "";
		for (char& c : boxList + ",")
		{
			if (c == ' ') continue;
			if (!curr.empty() && c == ',')
			{
				if (std::find_if(curr.begin(), curr.end(), [](char c) {return !std::isdigit(c); }) != curr.end())
				{
					std::cout << "Error expected list of numbers(ex 1,3,11)";
					int temp;
					std::cin >> temp;
					return 0;
				}
				else
				{
					int currNum = stoi(curr);
					if (currNum > 99)
					{
						std::cout << "Only box numbers 0-99 are supported, skipping box " << currNum;
					}
					boxes.push_back(currNum);
					curr = "";
				}
			}
			else curr += c;
		}
	}
	else
	{
		std::cout << "Error expected \"add\" or \"remove\"";
		int temp;
		std::cin >> temp;
		return 0;
	}


	if (addOrRemove == "remove")
	{
		for (int& box : boxes)
		{
			bool ret = std::filesystem::remove(path + "/box_" + (box < 10 ? "0" + std::to_string(box) : std::to_string(box)) + ".map");
			if (!ret) std::cout << "Failed to delete box " << box << ", continuing\n";
		}
	}
	else
	{
		std::vector<std::string> format;
		int zBarrierIdx = -1;
		int chestIdx = -1;
		std::regex zBarrier("\"script_noteworthy\" \"chest_\\d*_zbarrier\"");
		std::regex chest("\"script_noteworthy\" \"chest_\\d*\"");

		std::ifstream fin("box_format.map");
		if (!fin.is_open())
		{
			std::cout << "Need format file in this executable's directory. Format file is named box_format.map and is the same as box_00.map";
			int temp;
			std::cin >> temp;
			return 0;
		}
		else
		{
			std::string line;
			while (getline(fin, line))
			{
				format.push_back(line);
				if (std::regex_match(line, zBarrier)) zBarrierIdx = format.size() - 1;
				if (std::regex_match(line, chest)) chestIdx = format.size() - 1;
			}
			fin.close();
		}
		if (zBarrierIdx == -1 || chestIdx == -1)
		{
			std::cout << "Format file is incorrect";
			int temp;
			std::cin >> temp;
			return 0;
		}
		else
		{
			for (int& box : boxes)
			{
				format[zBarrierIdx].replace(27, 1, std::to_string(box));
				format[chestIdx].replace(27, 1, std::to_string(box));
				std::ofstream fout(path + "/box_" + (box < 10 ? "0" + std::to_string(box) : std::to_string(box)) + ".map");
				if (!fout.is_open()) std::cout << "Box " << box << "already exists, continuing\n";
				else
				{
					for (std::string line : format)  fout << line + "\n";
				}
				fout.close();
			}
		}
	}


	return 0;
}

