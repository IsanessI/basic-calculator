#pragma once

#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

class calculator
{
private:
	enum move
	{
		UP = -1,
		DOWN = -2,
	};
	struct obj
	{
		std::vector<double> vls;
		std::string oprs;
	};

	std::vector<obj> objs;
	std::string buf_expr;

	double to_double(std::string&);
	void format(std::string& expr);

public:
	//constructors
	calculator()
	{
		objs.push_back({ {0.0}, "" });
		objs.front().vls.pop_back();
		buf_expr = "";
	}
	calculator(std::string expr);
	
	calculator(const calculator& calc)
	{
		objs.resize(calc.objs.size());
		buf_expr = calc.buf_expr;

		for (int i = 0; i < objs.size(); i++)
		{
			objs[i].vls = calc.objs[i].vls;
			objs[i].oprs = calc.objs[i].oprs;
		}
	}
	calculator& operator=(const calculator& calc)
	{
		objs.resize(calc.objs.size());
		buf_expr = calc.buf_expr;

		for (int i = 0; i < objs.size(); i++)
		{
			objs[i].vls = calc.objs[i].vls;
			objs[i].oprs = calc.objs[i].oprs;
		}

		return *this;
	}
	calculator(calculator&& calc) noexcept
	{
		std::swap(this->objs, calc.objs);
	}
	calculator& operator=(calculator&& calc) noexcept
	{
		std::swap(this->objs, calc.objs);

		return *this;
	}
	calculator& operator=(const std::string& expr)
	{
		*this = (calculator)(expr);

		return *this;
	}

	~calculator()
	{
		objs.clear();
	}
	double calculate();
};
