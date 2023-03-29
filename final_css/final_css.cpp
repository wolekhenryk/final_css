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

#define _CRT_SECURE_NO_WARNINGS

using namespace std;

constexpr int max_line_length = 64;

void read_css(LinkedList<section>& main_list);

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

void count_selectors_for_section(const LinkedList<section>& main_list, const int index)
{
	if (index > main_list.length()) return;
	cout << index << ",S,? == " << main_list.get(index - 1).get_selectors().length() << endl;
}

void count_attributes_for_section(const LinkedList<section>& main_list, const int index)
{
	if (index > main_list.length()) return;
	cout << index << ",A,? == " << main_list.get(index - 1).get_attributes().length() << endl;
}

void selector_block(const LinkedList<section>& main_list, const int i, const int j)
{
	if (i > main_list.length()) return;
	const auto& selectors_for_block = main_list.get(i - 1).get_selectors();
	if (j > selectors_for_block.length()) return;

	cout << i << ",S," << j << " == " << selectors_for_block.get(j - 1).get_name() << endl;
}

void attribute_name_value(const LinkedList<section>& main_list, const str& n_name, const int index)
{
	if (index > main_list.length()) return;
	const auto& attributes = main_list.get(index - 1).get_attributes();

	for (const auto& element : attributes)
	{
		if (element.get_name() == n_name)
		{
			cout << index << ",A," << n_name << " == " << element.get_value() << endl;
			break;
		}
	}
}

void count_attribute_occ(const LinkedList<section>& main_list, const str& n_name)
{
	int count = 0;
	for (const auto& element : main_list)
	{
		for (const auto& attr : element.get_attributes())
		{
			if (attr.get_name() == n_name)
			{
				count++;
				break;
			}
		}
	}
	cout << n_name << ",A,? == " << count << endl;
}

void count_selector_occ(const LinkedList<section>& main_list, const str& z_name)
{
	int count = 0;
	for (const auto& element : main_list)
	{
		for (const auto& sel : element.get_selectors())
		{
			if (sel.get_name() == z_name)
			{
				count++;
				break;
			}
		}
	}
	cout << z_name << ",S,? == " << count << endl;
}

void z_e_n(const LinkedList<section>& main_list, const str& n_name, const str& z_name)
{
	for (auto section_iter = main_list.rbegin(); section_iter != main_list.rend(); ++section_iter)
	{
		const auto& curr_section = *section_iter;
		for (const auto& sel : curr_section.get_selectors())
		{
			if (sel.get_name() == z_name)
			{
				const auto& attributes_for_selector = curr_section.get_attributes();
				for (const auto& attr : attributes_for_selector)
				{
					if (attr.get_name() == n_name)
					{
						cout << z_name << ",E," << n_name << " == " << attr.get_value() << endl;
						return;
					}
				}
			}
		}
	}
}

void delete_section(LinkedList<section>& main_list, const int index)
{
	if (index > main_list.length() || index < 1) return;
	main_list.erase(index - 1);
	cout << index << ",D,* == deleted" << endl;
}

void delete_attribute(LinkedList<section>& main_list, const int index, const str& attr)
{
	if (index > main_list.length() || index <= 0) return;
	auto& block = main_list.get(index - 1);
	auto& attrs = block.get_attributes();

	for (int i = 0; i < attrs.length(); i++)
	{
		if (attrs.get(i).get_name() == attr) {
			attrs.erase(i);
			cout << index << ",D," << attr << " == deleted" << endl;
		}
	}

	if (attrs.empty()) main_list.erase(index - 1);
}

void read_commands(LinkedList<section>& main_list)
{
	char line[max_line_length];
	while (cin.getline(line, max_line_length))
	{
		str command(line);
		command.trim();

		if (command.length() == 0) continue;

		if (command == "****")
		{
			//read_css(main_list);
		}
		else if (command == "?")
		{
			cout << "? == " << main_list.length() << endl;
		}
		else
		{
			const auto command_parts = command.split(',');
			for (auto& element : command_parts) element.trim();

			if (is_integer(command_parts[0].c_str()) && command_parts[1] == "S" && command_parts[2] == "?")
			{
				//i,S,?
				const int index = atoi(command_parts[0].c_str());
				count_selectors_for_section(main_list, index);
			}
			else if (is_integer(command_parts[0].c_str()) && command_parts[1] == "A" && command_parts[2] == "?")
			{
				//i,A,?
				const int index = atoi(command_parts[0].c_str());
				count_attributes_for_section(main_list, index);
			}
			else if (is_integer(command_parts[0].c_str()) && command_parts[1] == "S" && is_integer(command_parts[2].c_str()))
			{
				//i,S,j
				const int i = atoi(command_parts[0].c_str());
				const int j = atoi(command_parts[2].c_str());
				selector_block(main_list, i, j);
			}
			else if (is_integer(command_parts[0].c_str()) && command_parts[1] == "A")
			{
				//i,A,n
				const str& n_name = command_parts[2];
				const int index = atoi(command_parts[0].c_str());
				attribute_name_value(main_list, n_name, index);
			}
			else if (command_parts[1] == "A" && command_parts[2] == "?")
			{
				//n,A,?
				const str& n_name = command_parts[0];
				count_attribute_occ(main_list, n_name);
			}
			else if (command_parts[1] == "S" && command_parts[2] == "?")
			{
				//z,S,?
				const str& z_name = command_parts[0];
				count_selector_occ(main_list, z_name);
			}
			else if (command_parts[1] == "E")
			{
				//z,E,n
				const str& z_name = command_parts[0];
				const str& n_name = command_parts[2];
				z_e_n(main_list, n_name, z_name);
			}
			else if (is_integer(command_parts[0].c_str()) && command_parts[1] == "D" && command_parts[2] == "*")
			{
				//i,D,*
				const int index = atoi(command_parts[0].c_str());
				delete_section(main_list, index);
			}
			else if (is_integer(command_parts[0].c_str()) && command_parts[1] == "D")
			{
				//i,D,n
				const int index = atoi(command_parts[0].c_str());
				const str& attr = command_parts[2];
				delete_attribute(main_list, index, attr);
			}
		}
	}
}

void read_css(LinkedList<section>& main_list)
{
	str str_line;
	while (str_line.readline())
	{
		if (str_line.empty()) continue;
		section current_block;
		str_line.trim();

		if (str_line == "????") return;
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
				if (!current_block.has_selector(element))
					current_block.add_selector(selector(element));

			}

			for (auto& element : line_of_attributes)
			{
				const auto colon_pos = element.find(':');
				str key = element.substr(0, colon_pos);
				str val = element.substr(colon_pos + 1);

				key.trim();
				val.trim();

				if (current_block.has_attribute(key))
				{
					current_block.overwrite_attribute(key, val);
				}
				else
				{
					current_block.add_attribute(attribute(key, val));
				}
			}

			main_list.push_back(current_block);
		}
		else
		{
			Vector<str> selector_strings = str_line.split(',');
			for (auto& selector_string : selector_strings)
			{
				selector_string.trim();
				if (selector_string.back() == '{') selector_string.pop_back();

				selector_string.trim();
				if (!current_block.has_selector(selector_string))
					current_block.add_selector(selector(selector_string));
			}

			//Read attributes
			str attr_line;
			while (attr_line.readline())
			{
				attr_line.trim();

				if (attr_line.front() == '}')
				{
					//End of block
					main_list.push_back(current_block);
					break;
				}

				if (attr_line.length() == 1) continue;

				const auto colon_pos = attr_line.find(':');

				str key = attr_line.substr(0, colon_pos);
				str val = attr_line.substr(colon_pos + 1);

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

int main()
{
	LinkedList<section> main_list;

	read_css(main_list);

	//read_commands(main_list);

	print_list(main_list);

	return 0;
}