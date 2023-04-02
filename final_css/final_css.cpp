#include <iostream>
#include <cstdlib>
#include <cctype>
#include <csignal>

#include "str.h"
#include "attribute.h"
#include "LinkedList.h"
#include "selector.h"
#include "Vector.h"
#include "section.h"
#include "BlockStorage.h"

#define _CRT_SECURE_NO_WARNINGS

using namespace std;

constexpr int max_line_length = 1024;

constexpr char start_commands[] = "????";

constexpr char resume_css[] = "****";

void read_commands(BlockStorage& storage);

void print_list(const LinkedList<section>& main_list)
{
	for (const auto& element : main_list)
	{
		for (const auto& s : element.get_selectors())
		{
			cout << s.get_name() << " ";
		}

		cout << "{ " << endl;

		for (const auto& a : element.get_attributes())
		{
			cout << "\t" << a.get_name() << ": " << a.get_value() << ";" << endl;
		}

		cout << "}" << endl;
	}
}

void print_storage(const BlockStorage& blocks)
{
	const auto data = blocks.get_data();
	for (int i = 0; i < data.length(); i++)
	{
		cout << "BLOCK => " << i + 1 << endl;
		const auto& element = data.get(i);
		print_list(element);
		cout << "============================================" << endl;
	}
} 

bool is_integer(const char* str) {
	if (str == nullptr || !*str)
		return false;

	while (*str) {
		if (!isdigit(*str))
			return false;
		++str;
	}

	return true;
}

void read_css(BlockStorage& storage)
{
	char line[1024];
	while (fgets(line, max_line_length, stdin))
	{
		str str_line(line);
		if (str_line.empty()) continue;
		section current_block;
		str_line.trim();

		if (str_line == start_commands) read_commands(storage);
		if (str_line.length() == 0) continue;

		if (str_line.find('{') != -1 && str_line.find('}') != -1)
		{
			//Single line block
			str first_part = str_line.substr(0, str_line.find('{'));
			str second_part = str_line.substr(str_line.find('{') + 1);

			first_part.trim();
			if (second_part.find('}') != -1) second_part.pop_back();
			second_part.trim();

			Vector<str> line_of_selectors = first_part.split(',');
			Vector<str> line_of_attributes = second_part.split(';');

			for (auto& element : line_of_selectors)
			{
				element.trim();
				//current_block.add_selector(selector(element));
				if (!current_block.has_selector(element) && element.length() > 0)
					current_block.add_selector(selector(element));

			}

			for (auto& element : line_of_attributes)
			{
				const auto colon_pos = element.find(':');
				str key = element.substr(0, colon_pos);
				str val = element.substr(colon_pos + 1);

				key.trim();
				val.trim();

				if (key.empty() || val.empty()) continue;

				if (current_block.has_attribute(key))
				{
					current_block.overwrite_attribute(key, val);
				}
				else
				{
					current_block.add_attribute(attribute(key, val));
				}
			}

			storage.add_new_element(current_block);
			//main_list.push_back(current_block);
		}
		else
		{
			bool multiline = true;
			if (str_line.find('{') != -1) multiline = false;

			Vector<str> initial_selectors = str_line.split(',');
			for (auto& element : initial_selectors)
			{
				element.trim();
				if (element.back() == '{') element.pop_back();
				element.trim();
				if (!current_block.has_selector(element))
					current_block.add_selector(selector(element));
			}

			while (multiline && fgets(line, max_line_length, stdin))
			{
				str new_line(line);
				new_line.trim();

				if (new_line.empty()) continue;

				if (new_line.find('{') != -1)
				{
					multiline = false;
					//End of selectors
					Vector<str> final_selectors = new_line.split(',');
					if (final_selectors.size() != 0)
					{
						for (auto& element : final_selectors)
						{
							element.trim();
							if (element.back() == '{') element.pop_back();
							element.trim();
							if (!current_block.has_selector(element) && element.length() > 0)
								current_block.add_selector(selector(element));
						}
					}
					break;
				}

				Vector<str> inline_selectors = new_line.split(',');
				for (auto& element : inline_selectors)
				{
					element.trim();
					if (!current_block.has_selector(element) &&  element.length() > 0)
						current_block.add_selector(selector(element));
				}
			}

			//Read attributes
			while (fgets(line, max_line_length, stdin))
			{
				str attr_line(line);
				attr_line.trim();

				if (attr_line.front() == '}')
				{
					//End of block
					storage.add_new_element(current_block);
					//main_list.push_back(current_block);
					break;
				}

				if (attr_line.length() == 1) continue;

				const auto colon_pos = attr_line.find(':');

				str key = attr_line.substr(0, colon_pos);
				str val = attr_line.substr(colon_pos + 1);

				if (key.empty() || val.empty()) continue;

				key.trim();
				val.trim();

				if (val.back() == ';') val.pop_back();

				if (current_block.has_attribute(key))
				{
					current_block.overwrite_attribute(key, val);
				}
				else
				{
					current_block.add_attribute(attribute(key, val));
				}
			}
		}
	}
}

void read_commands(BlockStorage& storage)
{
	char line[max_line_length];
	memset(line, '\0', max_line_length);
	while (cin.getline(line, max_line_length))
	{
		str command(line);
		command.trim();

		if (command.length() == 0) continue;

		if (command == resume_css)
		{
			read_css(storage);
		}
		else if (command == "?")
		{
			storage.display_sections_count();
		}
		else
		{
			const auto command_parts = command.split(',');
			for (auto& element : command_parts) element.trim();

			if (is_integer(command_parts[0].c_str()) && command_parts[1] == "S" && command_parts[2] == "?")
			{
				//i,S,?
				const int index = atoi(command_parts[0].c_str());
				storage.display_selectors_for_section(index - 1);
			}
			else if (is_integer(command_parts[0].c_str()) && command_parts[1] == "A" && command_parts[2] == "?")
			{
				//i,A,?
				const int index = atoi(command_parts[0].c_str());
				storage.display_attribute_count_for_section(index - 1);
			}
			else if (is_integer(command_parts[0].c_str()) && command_parts[1] == "S" && is_integer(command_parts[2].c_str()))
			{
				//i,S,j
				const int i = atoi(command_parts[0].c_str());
				const int j = atoi(command_parts[2].c_str());
				storage.selector_for_block(i - 1, j - 1);
			}
			else if (is_integer(command_parts[0].c_str()) && command_parts[1] == "A")
			{
				//i,A,n
				const str& n_name = command_parts[2];
				const int index = atoi(command_parts[0].c_str());
				storage.attribute_name_value(n_name, index - 1);
			}
			else if (command_parts[1] == "A" && command_parts[2] == "?")
			{
				//n,A,?
				const str& n_name = command_parts[0];
				storage.count_attribute_occ(n_name);
			}
			else if (command_parts[1] == "S" && command_parts[2] == "?")
			{
				//z,S,?
				const str& z_name = command_parts[0];
				storage.count_selector_occ(z_name);
			}
			else if (command_parts[1] == "E")
			{
				//z,E,n
				const str& z_name = command_parts[0];
				const str& n_name = command_parts[2];
				storage.z_e_n(n_name, z_name);
			}
			else if (is_integer(command_parts[0].c_str()) && command_parts[1] == "D" && command_parts[2] == "*")
			{
				//i,D,*
				const int index = atoi(command_parts[0].c_str());
				storage.delete_entire_section(index - 1);
			}
			else if (is_integer(command_parts[0].c_str()) && command_parts[1] == "D")
			{
				//i,D,n
				const int index = atoi(command_parts[0].c_str());
				const str& attr = command_parts[2];
				storage.delete_attribute(index - 1, attr);
			}
		}
	}
}

int main()
{
	//LinkedList<section> main_list;
	BlockStorage storage;


	read_css(storage);


	return 0;
}