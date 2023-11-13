#include "calculate.h"
#include <algorithm>
#include <iostream>

calculator::calculator(std::string expr)
{
	objs.push_back({ {0.0}, "" });
	buf_expr = expr;
	objs.front().vls.pop_back();

	format(expr);
	std::string operators = "+-*/^";

	int i = 0;
	while (true)
	{
		double x = to_double(expr);
		if (x > 0) 
			objs[i].vls.push_back(x);
		if (expr == "")
			break;

		if (expr[0] == '(')
		{
			objs[i].vls.push_back(DOWN);
			i++;

			if (i == objs.size())
			{
				i--;
				objs.push_back({ {}, "" });
				i++;
			}

			objs[i].vls.push_back(UP);
			if (x > 0) objs[i].vls.push_back(x);

		}
		else if (expr[0] == ')')
			i--;
		else
			objs[i].oprs += expr[0];
		auto er_it = expr.begin();
		er_it++;
		expr.erase(expr.begin(), er_it);
		if (expr == "")
			break;
	}
}

double calculator::calculate()
{
	int i = objs.size() - 1;
	if (objs.empty())
		*this = (calculator)(buf_expr);

	while (true)
	{
		//check for pow
		int pos = objs[i].oprs.rfind('^');
		if (pos != std::string::npos)
		{
			while (pos != std::string::npos)
			{
				int k = 0;
				for (int t = 0; t <= i; t++)
					if (objs[i].vls[t] < 0) k++;
				objs[i].vls[pos+k] = pow(objs[i].vls[pos+k], objs[i].vls[pos + k + 1]);
				objs[i].oprs[pos] = '!';

				auto it = objs[i].vls.end();
				it--;
				while (*it != objs[i].vls[pos+k+1])
					it--;
				auto it1 = it;
				it1++;
				objs[i].vls.erase(it, it1);
				pos = objs[i].oprs.rfind('^');
			}

			std::string new_oprs = "";
			for (char i : objs[i].oprs)
				if (i != '!') new_oprs += i;

			std::swap(objs[i].oprs, new_oprs);
		}

		int j = objs[i].vls.size() - 1;
		double x = objs[i].vls[j], y = (j - 1) >= 0 ? objs[i].vls[j - 1] : INT_MIN;
		objs[i].vls.pop_back();
		j--;

		if (y == INT_MIN)
		{
			if (objs.back().oprs != "" && objs.back().oprs.back() == '-') x *= -1;
			return x;
		}
		if (y == UP)
		{
			if (i - 1 < 0)
			{
				std::cout << "ERROR 0";
				exit(-1);
			}

			if (objs.back().oprs.size() == 1 && objs.back().vls.size() == 1 && objs.back().oprs.back() == '-')
				x *= -1;

			int k;
			for (k = objs[i - 1].vls.size() - 1; k >= 0; k--)
				if (objs[i - 1].vls[k] == DOWN)
				{
					objs[i - 1].vls[k] = x;

					if (x < 0)
					{
						objs[i - 1].vls[k] *= -1;
						int t = k-1;
						for (; t >= 0 && (objs[i - 1].oprs[t] == '*' || objs[i - 1].oprs[t] == '/'); t--);

						auto it = objs[i - 1].oprs.begin();
						for (int f = 0; f < t; f++, it++);
						objs[i - 1].oprs.insert(it, '-');
					}

					objs[i].vls.pop_back();

					if (objs[i].vls.empty())
					{
						objs.pop_back();
						i--;
					}

					break;
				}
			if (k == -1)
			{
				std::cout << "ERROR 1\n";
				exit(-1);
			}
			continue;
		}

		char oper = objs[i].oprs.back();
		objs[i].oprs.pop_back();

		char prev_oper = objs[i].oprs.empty() ? 0 : objs[i].oprs.back();
		if (prev_oper != 0)
		{
			switch (prev_oper)
			{
			case '-':
				if (j == 0 || oper != '*' && oper != '/') {
					y *= -1;
					objs[i].oprs.back() = '+'; 
				}
				break;
			default:
				while (objs[i].vls.back() != -1 && objs[i].oprs != "" && (objs[i].oprs.back() == '*' || objs[i].oprs.back() == '/'))
				{
					objs[i].vls.pop_back();
					y = objs[i].vls.back() = objs[i].oprs.back() == '*' ? objs[i].vls.back() * y : objs[i].vls.back() / y;
					if (!objs[i].oprs.empty()) 
						objs[i].oprs.pop_back();
				}
				char prev_prev_oper = objs[i].oprs.empty() ? 0 : objs[i].oprs.back();
				if (prev_prev_oper == '-' && (j == 0 || oper != '*' && oper != '/' && objs[i].vls[j - 1] != -1)) {
					y *= -1;
					objs[i].oprs.back() = '+';
				}
				break;
			}
		}
		switch (oper)
		{
		case '+':
			y += x;
			break;
		case '-':
			y -= x;
			break;
		default:
			y = oper == '*' ? y * x : y / x;
			break;
		}

		objs[i].vls.back() = y;
	}
}

double calculator::to_double(std::string& expr)
{
	auto expr_it = expr.begin();
	bool f = 0;
	int intg = 0, frct = 0, n = 0; //amount of fractional discharges
	while (expr_it != expr.end() && (*expr_it >= '0' && *expr_it <= '9' || *expr_it == '.'))
	{
		if (*expr_it == '.') {
			f = 1;
			expr_it++;
		}
		if (!f)
		{
			intg *= 10;
			intg += *expr_it - '0';
		}
		else
		{
			frct *= 10;
			frct += *expr_it - '0';
			n++;
		}
		expr_it++;
	}
	expr.erase(expr.begin(), expr_it);
	return expr_it != expr.begin() ? intg + (double)(frct * pow(0.1, n)) : -1;
}

void calculator::format(std::string& expr)
{
	std::vector<int> stack;
	std::string operators = "+-^/*";

	std::string final_expr = "";
	try {
		for (int i = 0; i < expr.size(); i++)
		{
			if (expr[i] == ' ')
				continue;
			final_expr += expr[i];

			if (expr[i] == '(')
			{
				if (expr[i + 1] != '-' && (expr[i + 1] < '0' || expr[i + 1] > '9') && expr[i + 1] != '(')
					throw "After ( unknown symbol";
				stack.push_back(1);
			}
			else if (expr[i] == ')')
			{
				if (stack.empty())
					throw "( doesn't found";
				stack.pop_back();
			}
			else if (expr[i] >= '0' && expr[i] <= '9')
			{
				int _end = final_expr.size();
				if (_end > 0)
				{
					_end -= 2;
					if (_end >= 0 && final_expr[_end] == ')')
						throw "(  doesn't found";
				}
				if (expr[i + 1] == '(')
					final_expr += '*';
			}
			else if (expr[i] == '.')
			{
				if (expr[i + 1] < '0' || expr[i + 1] > '9')
					final_expr += '0';
			}
			else if (operators.find(expr[i]) == std::string::npos)
				throw "Unknown operator";
		}
		if (!stack.empty())
			throw ") doesn't found";
	}
	catch (std::string err_msg)
	{
		std::cout << err_msg << '\n';
		exit(-1);
	}
	catch (...)
	{
		std::cout << "Unknown error\n";
		exit(-1);
	}
	expr = final_expr;
}