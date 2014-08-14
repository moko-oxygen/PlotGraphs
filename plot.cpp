#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <math.h>
using namespace std;

vector <char> RPN(string formula);	/* 入力された式を逆ポーランド記法に変換する */		
template <typename T> void VectorPrint(vector<T> IsThrown);	/* 投げられたやつの中身を見るんじゃ */
void PlotPoint(double init, double end, double interval, string formula, string filename);	/* 指定の区間、指定の刻みで式に値を入力し、得られた値をvectorで保持する */
double CalcResult(vector <char> converted, double value);	/* 変換後の式で計算を行う */

int main()
 {
 	string formula = "x ^ 5";
 	cout << formula << endl;
 	VectorPrint(RPN(formula));
 	string filename = "test.csv";
 	PlotPoint(0.1, 1.0, 0.01, formula, filename);
	return 0;
}	

vector <char> RPN(string formula)
{
	map<char, int> op; /* キー (単語) がchar, 値 (わりあて番号) がint */
	op.insert(map<int, char>::value_type('(', 1));
	op.insert(map<int, char>::value_type('+', 2));
	op.insert(map<int, char>::value_type('-', 2));
	op.insert(map<int, char>::value_type('*', 3));
	op.insert(map<int, char>::value_type('/', 3));
	op.insert(map<int, char>::value_type('^', 4)); //べき乗	

	stack <char> operand; /* 演算子入れるスタック */
	vector<char> result; /* 数字入れる文字列 */

	for (int i = 0; i <(formula.size()); i++)
	{

	}
	
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

double CalcResult(vector <char> converted, double value)
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
		else if(converted[i] == '^')
		{
			a = calc.top();
			calc.pop();
			b = calc.top();
			calc.pop();
			calc.push(pow(b, a));
		}
		else
		{
			if('0' <=  converted[i] && converted[i] <= '9')
			{
				calc.push((double)(converted[i] - '0'));	/* char -> int へのキャストはこれしかない（死）*/
			}
			else if(converted[i] == 'x')
			{
				calc.push(value);	/* 渡された値を代入 */
			}
		}
	}
	return calc.top();
}

void PlotPoint(double init, double end, double interval, string formula, string filename)
{
	/* 入出力を保持するベクタを持っておく */
	vector <double> InputValueAry;
	vector <double> OutputValueAry;
	vector <char> converted = RPN(formula);	/* 変換後の式をRPN()に作らせて持っておく */
	
	double InputValue = init;	/* 入力値を初期化 */
	
	while(InputValue <= end)
	{
		InputValueAry.push_back(InputValue);
		OutputValueAry.push_back(CalcResult(converted, InputValue));

		InputValue += interval;
	}

	ofstream ofs( filename ); 

	//書き込み
	ofs << endl;
	ofs << "x axis" << "," << "y axis" << endl;
	
	/* どうしていいかわからんので入力値の点の数だけ見ている */
	for(int i = 0; i < InputValueAry.size(); i++)
	{
		ofs << InputValueAry[i] << "," << OutputValueAry[i] << endl;
	}
}

template <typename T> void VectorPrint(vector<T> IsThrown)
{
	for(int i = 0; i < IsThrown.size(); i++)
	{
		cout << IsThrown[i];
	}
	cout << endl;
}











