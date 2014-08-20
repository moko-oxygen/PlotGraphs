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
 	string formula;
 	cout << "演算を終えるときは「end」と入力" << endl;
	while(getline(cin, formula))
 	{
		if(formula == "end") break;
	 	VectorPrint(RPN(formula));
	 	string filename = "test.csv";
	 	PlotPoint(1.0, 10.0, 1.0, formula, filename);
	}
	return 0;
}	

vector <char> RPN(string formula)
{
	map<char, int> op; /* キー (単語) がchar, 値 (わりあて番号) がint */
	op.insert(map<char, int>::value_type('(', 1));
	op.insert(map<char, int>::value_type('+', 2));
	op.insert(map<char, int>::value_type('-', 2));
	op.insert(map<char, int>::value_type('*', 3));
	op.insert(map<char, int>::value_type('/', 3));
	op.insert(map<char, int>::value_type('^', 4)); /* 累乗 */
	op.insert(map<char, int>::value_type('_', 5)); /* 単項マイナス演算子 */

 
	stack <char> operand; /* 演算子入れるスタック */
	vector<char> result; /* 数字入れる文字列 */

	bool mode = true; /* 値モードと演算子モードの切り替え用　初期値は true (値) */
	
	//cout << "map読むところまではきた？" ;

	for (int i = 0; i < (formula.size()); i++)	/* 式から1トークン取り出す */
	{
		if(formula[i] == ' ')
		{
			continue;
		}
		else if(('0' <=  formula[i] && formula[i] <= '9')||(formula[i] == 'x')) /* 数字または変数xのとき */
		{
			result.push_back(formula[i]); /* resultベクタに入れる */
			mode = false; /* 演算子モードへ */
		}
		else if(formula[i] == ')')	/* 右括弧か？ */	
		{
			mode = false;
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
			mode = true;
			operand.push(formula[i]);
		}
		else
		{
			//cout << "演算子と判定" << endl;
			while(1)
			{
				if((mode == true)&&(formula[i] == '-'))
				{
					//cout << "単項演算子? " << endl;
					operand.push('_');
					break;
				}
				else if(operand.empty())
				{
					operand.push(formula[i]);
					break;
				}
				else if(op[formula[i]] <= op[operand.top()]) /* スタック最上段の演算子よりトークンの演算子の優先順位が低いか、同じであるとき */
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
			mode = true;
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
	bool minus = false;

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
			if (a == 0)
				{
					cout << "おお勇者よ、0で割るとは情けない" << endl;
					break;
				}
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
		else if(converted[i] == '_')	/* 単項マイナス演算子ちゃん！ */
		{
			a = calc.top();
			calc.pop();
			calc.push(-a); /* 符号反転だドン */
		}
		else
		{
			if('0' <=  converted[i] && converted[i] <= '9')
			{
				calc.push((double)(converted[i] - '0'));				/* char -> int へのキャストはこれしかない（死）*/
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

	VectorPrint(OutputValueAry);
	
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
		cout << IsThrown[i] << ", " ;
	}
	cout << endl;
}
