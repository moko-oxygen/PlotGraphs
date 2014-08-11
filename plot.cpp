#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <map>
using namespace std;

vector <char> RPN(string formula);
double CalcResult(vector <char> converted);
template <typename T> void VectorPrint(vector<T> converted);

int main()
 {
 	string formula = " (1 + x) * (3 + 7) / 5";

	cout << "input = " << formula  << endl;
	cout << "converted = "; 
	VectorPrint(RPN(formula));
	cout << "calculated = " << CalcResult(RPN(formula)) << endl;

	return 0;
}	

vector <char> RPN(string formula)
{
	map<char, int> op; /* キー (単語) がchar, 値 (番号) がint */
	op.insert( map<int, char>::value_type('(', 1));
	op.insert( map<int, char>::value_type('+', 2));
	op.insert( map<int, char>::value_type('-', 2));
	op.insert( map<int, char>::value_type('*', 4));
	op.insert( map<int, char>::value_type('/', 4));

	stack <char> operand; /* 演算子入れるスタック */
	vector<char> result; /* 数字入れる文字列 */
	
	for (int i = 0; i < (formula.size()); i++)	/* 式から1トークン取り出す */
	{
		if(formula[i] == ' ')
		{
			continue;
		}
		else if(('0' <=  formula[i] && formula[i] <= '9')||(formula[i] == 'x')) /* 数字または変数xのとき */
		{
			result.push_back(formula[i]); /* resultベクタに入れる */
		}
		else if(formula[i] == ')')	/* 右括弧か？ */	
		{
			while(1)
			{
				if(operand.empty())
				{
					break;
				}
				else if(operand.top() == '(')
				{
					operand.pop();
					break;
				}
				else
				{
					result.push_back(operand.top());
					operand.pop();
					continue;
				}
			}
		}
		else if(formula[i] == '(')	/* 左括弧か？*/
		{
			operand.push(formula[i]);
		}
		else
		{
			while(1)
			{
				if(operand.empty())
				{
					operand.push(formula[i]);
					break;
				}
				else if(op[formula[i]] <= op[operand.top()])
				/* スタック最上段の演算子よりトークンの演算子の優先順位が低いか、同じであるとき */
				{
					result.push_back(operand.top());
					operand.pop();			
				}
				else
				{
					operand.push(formula[i]);
					break;
				}
			}
		}
	}

	int size = operand.size(); /* pop したらサイズが変わるので記録 */
	for (int i = 0; i < size; i++) /*  トークンを出し尽くしたらスタックが空になるまでpop バッファへ */
	{
		result.push_back(operand.top());
		operand.pop();
	}
	return result;
}

double CalcResult(vector <char> converted)
{
	stack <double> calc;
	double a;
	double b;

	for(int i = 0; i < (converted.size()); i++)
	{
		if(converted[i] == '+')
		{
			a = calc.top();
			calc.pop();
			b = calc.top();
			calc.pop();
			calc.push(b + a);
		}
		else if(converted[i] == '-')
		{
			a = calc.top();
			calc.pop();
			b = calc.top();
			calc.pop();
			calc.push(b - a);
		}
		else if(converted[i] == '*')
		{
			a = calc.top();
			calc.pop();
			b = calc.top();
			calc.pop();
			calc.push(b * a);
		}
		else if(converted[i] == '/')
		{
			a = calc.top();
			calc.pop();
			if (a == 0) break;
			b = calc.top();
			calc.pop();
			calc.push(b / a);
		}
		else
		{
			calc.push((double)(converted[i] - '0'));	/* char -> int へのキャストはこれしかない（死）*/
		}
	}

	return calc.top();
}
template <typename T> void VectorPrint(vector<T> converted)
{
	for(int i = 0; i < converted.size(); i++)
	{
		cout << converted[i];
	}
	cout << endl;
}
	
