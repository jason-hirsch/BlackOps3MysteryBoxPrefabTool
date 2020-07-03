#include <filesystem>
#include <regex>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


int main()
{
	std::string path;
	std::string howManyDupeStr;
	int howManyDupe;

	std::cout << "Destination folder: ";
	getline(std::cin, path);

	std::cout << "Number to duplicate: ";
	getline(std::cin, howManyDupeStr);
	if (std::find_if(howManyDupeStr.begin(), howManyDupeStr.end(), [](char c) {return !std::isdigit(c); }) != howManyDupeStr.end())
	{
		std::cout << "Number to duplicate must be a number";
		int temp;
		std::cin >> temp;
		return 0;
	}
	howManyDupe = stoi(howManyDupeStr);

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

	std::string line;
	while (getline(fin, line))
	{
		format.push_back(line);
		if (std::regex_match(line, zBarrier)) zBarrierIdx = format.size() - 1;
		if (std::regex_match(line, chest)) chestIdx = format.size() - 1;
	}

	fin.close();
	if (zBarrierIdx == -1 || chestIdx == -1)
	{
		std::cout << "Format file is incorrect";
		int temp;
		std::cin >> temp;
		return 0;
	}

	for (int i = 1; i < howManyDupe; i++)
	{
		std::string zBarrierOld = format[zBarrierIdx];
		std::string chestOld = format[chestIdx];
		format[zBarrierIdx].replace(27, 1, std::to_string(i));
		format[chestIdx].replace(27, 1, std::to_string(i));
		std::ofstream fout(path + "/box_" + (i < 10 ? "0" + std::to_string(i) : std::to_string(i)) + ".map");
		if (!fout.is_open()) std::cout << "Box " << i << "already exists, continuing\n";
		else
		{
			for (std::string line : format)  fout << line + "\n";
		}
		fout.close();
		format[zBarrierIdx] = zBarrierOld;
		format[chestIdx] = chestOld;
	}

	return 0;
}

