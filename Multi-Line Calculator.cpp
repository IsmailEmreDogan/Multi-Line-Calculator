//  İsmail Emre Doğan 
//  1316190011
//  Date: 24.1.2021
//  Development Enviroment: Visual Studio 2019

#include <iostream>
#include <filesystem>
#include <fstream>
#include <cctype>
#include <utility>
#include <algorithm>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>




constexpr const char* operators[] = { "*", "/", "+", "-", "<", ">", "==", "<=", ">=", "!", "!=" };
constexpr const char* keywords[] = { "OUT", "IN", "IF", "THEN", "ELSE", "LOOP", "TIMES" };
std::unordered_map<std::string, double> double_variables;
std::unordered_map<std::string, int> integer_variables;

bool is_valid_keyword(std::string_view str)
{
	return std::find(std::begin(keywords), std::end(keywords), str) != std::end(keywords);
}

bool is_valid_operator(std::string_view str)
{
	return std::find(std::begin(operators), std::end(operators), str) != std::end(operators);
}

int status(std::string_view str) //Status - 0 = Variable Name - 1 = Double Constant - 2 = Integer Constant
{
	return std::isalpha(str[0]) ? 0 : str.find('.') != std::string::npos ? 1 : 2;
}

template <typename T>
auto do_binary_operation(T first_operand, T second_operand, std::string_view op) -> decltype(first_operand)
{
	if (op == "*") return first_operand * second_operand;
	if (op == "/")
	{
		if(second_operand == 0)
		{
			std::cerr << "Cannot divide by zero!\n";
			return 0;
		}
		return first_operand / second_operand;
	}
	if (op == "+") return first_operand + second_operand;
	if (op == "-") return first_operand - second_operand;
	if (op == "<") return first_operand < second_operand;
	if (op == ">") return first_operand > second_operand;
	if (op == "==") return first_operand == second_operand;
	if (op == "<=") return first_operand <= second_operand;
	if (op == ">=") return first_operand >= second_operand;
	if (op == "!=") return first_operand != second_operand;
	return 0;
}

template <typename T>
T get_value(const std::string& str)
{
	switch (status(str))
	{
	case 0:
		return double_variables.find(str) != double_variables.end() ? double_variables[str] : integer_variables[str];
	case 1:
		return std::stod(str);
	case 2:
		return std::stoi(str);
	default: ;
	}
	return -1; //Syntax error
}

auto handle_operators(std::vector<std::string>& tokens)
{
	for (const auto& item : operators)
	{
		std::vector<std::string>::iterator find_result;
		while ((find_result = std::find(tokens.begin(), tokens.end(), item)) != tokens.end())
		{
			auto first_operand{ *(find_result - 1) };
			auto second_operand{ *(find_result + 1) };
			const auto first_operand_status{ status(first_operand) };
			const auto second_operand_status{ status(second_operand) };
			if (first_operand_status == 1 || second_operand_status == 1
				|| (first_operand_status == 0 && double_variables.find(first_operand) != double_variables.end())
				|| (second_operand_status == 0 && double_variables.find(second_operand) != double_variables.end()))
			{
				double first_operand_d{};
				double second_operand_d{};
				if (first_operand_status == 0)
				{
					if (double_variables.find(first_operand) != double_variables.end())
					{
						first_operand_d = double_variables[first_operand];
					}
					else
					{
						first_operand_d = integer_variables[first_operand];
					}
				}
				else if (first_operand_status == 1)
				{
					first_operand_d = std::stod(first_operand);
				}

				if (second_operand_status == 0)
				{
					if (double_variables.find(second_operand) != double_variables.end())
					{
						second_operand_d = double_variables[second_operand];
					}
					else
					{
						second_operand_d = integer_variables[second_operand];
					}
				}
				else if (second_operand_status == 1)
				{
					second_operand_d = std::stod(second_operand);
				}
				*(find_result - 1) = std::to_string(do_binary_operation(first_operand_d, second_operand_d, item));
			}
			else
			{
				int first_operand_d{};
				int second_operand_d{};
				if (first_operand_status == 0)
				{
					if (double_variables.find(first_operand) != double_variables.end())
					{
						first_operand_d = static_cast<int>(double_variables[first_operand]);
					}
					else
					{
						first_operand_d = integer_variables[first_operand];
					}
				}
				else if (first_operand_status == 2)
				{
					first_operand_d = std::stoi(first_operand);
				}

				if (second_operand_status == 0)
				{
					if (double_variables.find(second_operand) != double_variables.end())
					{
						second_operand_d = static_cast<int>(double_variables[second_operand]);
					}
					else
					{
						second_operand_d = integer_variables[second_operand];
					}
				}
				else if (second_operand_status == 2)
				{
					second_operand_d = std::stoi(second_operand);
				}
				return std::to_string(do_binary_operation(first_operand_d, second_operand_d, item));
			}
		}
	}
	return std::string{};
}

void handle_tokens(std::vector<std::string>& tokens)
{
	static int count = 0;
	std::vector<std::string>::iterator start;
	while ((start = std::find(tokens.begin(), tokens.end(), "(")) != tokens.end())
	{
		auto end{ std::find(tokens.begin(), tokens.end(), ")") };
		if (end == tokens.end())
		{
			//Syntax Error
		}
		std::vector<std::string> paranthese_tokens{start + 1, end};
		if(!paranthese_tokens.empty())
		{
			//handle_tokens(paranthese_tokens);
			*start = handle_operators(paranthese_tokens);
			tokens.erase(start + 1, end + 1);
		}
	}
	for (const auto& item : operators)
	{
		std::vector<std::string>::iterator find_result;
		while ((find_result = std::find(tokens.begin(), tokens.end(), item)) != tokens.end())
		{
			auto first_operand{ *(find_result - 1) };
			auto second_operand{ *(find_result + 1) };
			const auto first_operand_status{ status(first_operand) };
			const auto second_operand_status{ status(second_operand) };
			if (first_operand_status == 1 || second_operand_status == 1
				|| (first_operand_status == 0 && double_variables.find(first_operand) != double_variables.end())
				|| (second_operand_status == 0 && double_variables.find(second_operand) != double_variables.end()))
			{
				double first_operand_d{};
				double second_operand_d{};
				if (first_operand_status == 0)
				{
					if (double_variables.find(first_operand) != double_variables.end())
					{
						first_operand_d = double_variables[first_operand];
					}
					else
					{
						first_operand_d = integer_variables[first_operand];
					}
				}
				else if (first_operand_status == 1)
				{
					first_operand_d = std::stod(first_operand);
				}

				if (second_operand_status == 0)
				{
					if (double_variables.find(second_operand) != double_variables.end())
					{
						second_operand_d = double_variables[second_operand];
					}
					else
					{
						second_operand_d = integer_variables[second_operand];
					}
				}
				else if (second_operand_status == 1)
				{
					second_operand_d = std::stod(second_operand);
				}
				*(find_result - 1) = std::to_string(do_binary_operation(first_operand_d, second_operand_d, item));
			}
			else
			{
				int first_operand_d{};
				int second_operand_d{};
				if (first_operand_status == 0)
				{
					if (double_variables.find(first_operand) != double_variables.end())
					{
						first_operand_d = static_cast<int>(double_variables[first_operand]);
					}
					else
					{
						first_operand_d = integer_variables[first_operand];
					}
				}
				else if (first_operand_status == 2)
				{
					first_operand_d = std::stoi(first_operand);
				}

				if (second_operand_status == 0)
				{
					if (double_variables.find(second_operand) != double_variables.end())
					{
						second_operand_d = static_cast<int>(double_variables[second_operand]);
					}
					else
					{
						second_operand_d = integer_variables[second_operand];
					}
				}
				else if (second_operand_status == 2)
				{
					second_operand_d = std::stoi(second_operand);
				}
				*(find_result - 1) = std::to_string(do_binary_operation(first_operand_d, second_operand_d, item));
			}
			tokens.erase(find_result, find_result + 2);
		}
	}
	if (!tokens.empty() && is_valid_keyword(tokens[0]))
	{
		if(tokens[0] == "OUT")
		{
			std::ofstream file{"challenge-" + std::to_string(++count)+".out"};
			if(double_variables.find(tokens[1]) != double_variables.end())
			{
				file << double_variables[tokens[1]] << "\n";
			}
			else
			{
				file << integer_variables[tokens[1]] << "\n";
			}
		}
		else if(tokens[0] == "LOOP")
		{
			auto loop_index = std::find(tokens.begin(), tokens.end(), "LOOP");
			auto times_index = std::find(tokens.begin(), tokens.end(), "TIMES");
			auto times_count = std::stoi(*(times_index + 1));
			int count{};
			std::vector<std::string> in_tokens{loop_index + 1, times_index};
			while (count < times_count)
			{
				handle_tokens(in_tokens);
				++count;
			}
		}
		tokens.clear();
	}
	else if (tokens.size() > 2 && tokens[1] == "=" && !is_valid_keyword(tokens[2]))
	{
		if (tokens[2].find('.') != std::string::npos) // double
		{
			double_variables[tokens[0]] = std::stod(tokens[2]);
			integer_variables.erase(tokens[0]);
		}
		else //integer
		{
			integer_variables[tokens[0]] = std::stoi(tokens[2]);
			double_variables.erase(tokens[0]);
		}
		tokens.clear();
	}
	else if(tokens.size() > 2 && tokens[1] == "=" && is_valid_keyword(tokens[2]))
	{
		if(tokens[2] == "IF") //Rule Start
		{
			auto start_index = std::find(tokens.begin(), tokens.end(), "IF"); //expression [start-positive)
			auto positive_index = std::find(tokens.begin(), tokens.end(), "THEN"); //positive [positive-negative)
			auto negative_index = std::find(tokens.begin(), tokens.end(), "ELSE"); //negative [negative-end)
			if(std::stoi(*(start_index + 1)) > 0)
			{
				if (double_variables.find(tokens[1]) != double_variables.end())
				{
					*start_index = std::to_string(double_variables[*(positive_index + 1)]);
				}
				else
				{
					*start_index = std::to_string(integer_variables[*(positive_index + 1)]);
				}
			}
			else
			{
				if (double_variables.find(tokens[1]) != double_variables.end())
				{
					*start_index = std::to_string(double_variables[*(negative_index + 1)]);
				}
				else
				{
					*start_index = std::to_string(integer_variables[*(negative_index + 1)]);
				}
			}
			tokens.erase(start_index + 1, negative_index + 1);
		}
	}
}

int main()
{
    std::string directory_path;
	std::cout << "Enter a directory path that includes \".inp\" extension files : ";
	std::cin >> directory_path;
	std::vector<std::ifstream> files;

	if(!std::filesystem::exists(directory_path) || !std::filesystem::is_directory(directory_path))
	{
		std::cerr << "There is no directory on this path!\n";
		std::exit(1);
	}
	
	for (const auto& item : std::filesystem::directory_iterator{directory_path})
	{
		if(item.is_regular_file() && item.path().extension().compare(".inp") == 0)
		{
			files.emplace_back(item.path());
			if(!files.back().is_open())
			{
				std::cerr << item.path().filename() << " could not be opened for any reason!\n";
			}
		}
	}

	if(files.empty())
	{
		std::cerr << "There is no file to get input!\n";
		std::exit(1);
	}

	for (auto& item : files)
	{
		std::string input{std::istreambuf_iterator<char>{item}, std::istreambuf_iterator<char>{}};
		std::string output;
		std::unique_copy(input.begin(), input.end(), std::back_inserter(output), [](char a, char b)
		{
			return std::isspace(a) && std::isspace(b);
		});
		std::stringstream ss{ output };
		std::string line;
		while(getline(ss, line))
		{
			std::vector<std::string> tokens;
			std::string token;
			std::stringstream line_ss{line};
			while(getline(line_ss, token, ' '))
			{
				while(!token.empty())
				{
					if (std::isalpha(token[0])) //Keyword or variable
					{
						auto name{ std::find_if(token.begin(), token.end(), [](char c)
						{
							return !std::isalnum(c);
						}) };
						tokens.emplace_back(token.begin(), name);
						token.erase(token.begin(), name);
					}

					else if (std::ispunct(token[0])) //Operator
					{
						auto operator_word{ std::find_if(token.begin(), token.end(), [](char c)
						{
							return !std::ispunct(c);
						}) };
						tokens.emplace_back(token.begin(), operator_word);
						token.erase(token.begin(), operator_word);
					}

					else if(std::isalnum(token[0]) && !std::isalpha(token[0]))
					{
						auto constant{ std::find_if(token.begin(), token.end(), [](char c)
						{
							return (c < '0' || c > '9') && c != '.';
						}) };
						tokens.emplace_back(token.begin(), constant);
						token.erase(token.begin(), constant);
					}
				}
			}
			while(!tokens.empty())
			{
				handle_tokens(tokens);
			}
		}
		integer_variables.clear();
		double_variables.clear();
	}
	
}
