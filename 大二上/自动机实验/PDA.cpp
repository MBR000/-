#include <iostream>
#include<vector>
#include<string>
#include<stack>
#include <algorithm>
using namespace std;
class Old2NewRelation {

private:
	string oldState;//��ǰ״̬
	char trans;//��ȡ���ַ�
	string newState;//��״̬
	char top;//ջ���ַ�
	char putInStack;//Ҫѹ��ջ�е��ַ�

public:
	string getOldState() {
		return oldState;
	}
	void setOldState(string oldState) {
		this->oldState = oldState;
	}
	char getTrans() {
		return trans;
	}
	void setTrans(char trans) {
		this->trans = trans;
	}
	string getNewState() {
		return newState;
	}
	void setNewState(string newState) {
		this->newState = newState;
	}
	Old2NewRelation(string oldState, char trans, string newState, char top, char putInStack) {
		this->oldState = oldState;
		this->trans = trans;
		this->newState = newState;
		this->top = top;
		this->putInStack = putInStack;
	}
	string toString() {
		return "state tranfer relation [nowstate��" + oldState + ",  recognition char��" + trans + ", newState��" + newState + ", the operation for stack is ��" + putInStack
			+ "replace the top of stack��" + top + "]";
	}
	char getPutInStack() {
		return putInStack;
	}
	void setPutInStack(char putInStack) {
		this->putInStack = putInStack;
	}
	char getTop() {
		return top;
	}
	void setTop(char top) {
		this->top = top;
	}
};


static vector<Old2NewRelation> initRelation() {
	//�á�#����ʾ���ַ�
	//�ֱ��ʾ ��ǰ״̬-ʶ���ַ�-ת��״̬-ջ��Ԫ��-�滻ջ����Ԫ��
	vector<Old2NewRelation> list;
	list.push_back(Old2NewRelation("q1", '#', "q2", '#', '$'));
	list.push_back(Old2NewRelation("q2", 'a', "q4", '#', '#'));
	list.push_back(Old2NewRelation("q2", 'b', "q2", '#', 'b'));
	list.push_back(Old2NewRelation("q2", 'c', "q3", 'b', '#'));
	list.push_back(Old2NewRelation("q2", 'c', "q6", '#', 'c'));
	list.push_back(Old2NewRelation("q2", '#', "q7", '$', '#'));
	list.push_back(Old2NewRelation("q3", 'a', "q4", '#', '#'));
	list.push_back(Old2NewRelation("q3", 'b', "q2", '#', 'b'));
	list.push_back(Old2NewRelation("q3", 'c', "q3", 'b', '#'));
	list.push_back(Old2NewRelation("q3", 'c', "q6", '#', 'c'));
	list.push_back(Old2NewRelation("q3", '#', "q7", '$', '#'));
	list.push_back(Old2NewRelation("q4", 'a', "q4", '#', '#'));
	list.push_back(Old2NewRelation("q4", 'b', "q5", 'c', '#'));
	list.push_back(Old2NewRelation("q4", 'b', "q2", '#', 'b'));
	list.push_back(Old2NewRelation("q4", 'c', "q3", 'b', '#'));
	list.push_back(Old2NewRelation("q4", 'c', "q6", '#', 'c'));
	list.push_back(Old2NewRelation("q4", '#', "q7", '$', '#'));
	list.push_back(Old2NewRelation("q5", 'a', "q4", '#', '#'));
	list.push_back(Old2NewRelation("q5", 'b', "q5", 'c', '#'));
	list.push_back(Old2NewRelation("q5", 'b', "q2", '#', 'b'));
	list.push_back(Old2NewRelation("q5", 'c', "q6", '#', 'c'));
	list.push_back(Old2NewRelation("q5", '#', "q7", '$', '#'));
	list.push_back(Old2NewRelation("q6", 'a', "q4", '#', '#'));
	list.push_back(Old2NewRelation("q6", 'b', "q5", 'c', '#'));
	list.push_back(Old2NewRelation("q6", 'c', "q6", '#', 'c'));
	list.push_back(Old2NewRelation("q6", '#', "q7", '$', '#'));
	return list;
}
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char** argv) {
	cout << "input string��only include 'abc'��,judging the number of b is same c��accetp if same��refuse if different" << endl;
	//���ַ������ַ�������
	string str;
	getline(cin, str);
	str.insert(0, "#");//���⴦������ʹջ�иտ�ʼʶ�����봮ʱջ��ջ������Ϊ$
	vector<Old2NewRelation> list = initRelation();
	stack<int> stack;
	string strState = "q1";
	string endState = "q7";
	cout << "The original state of PDA�� " << strState << "The accept state is ��" << endState << endl;
	cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
	bool isTrave = false;
	//��ʼ�������ʶ���ַ���bc����Ҫѡ��һ������ѹջ���˴�ѡ���b���в���
	for (int i = 0; i < str.length(); i++) {
		char cNow = str[i];
		if (cNow != '#') {
			cout << "The " << i << "of char be recognized is" << cNow << endl;
		}
		else {
			cout << " making special mark$" << endl;
			cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
		}
		isTrave = false;
		for (int k = 0; k < list.size(); k++) {
			if (isTrave) {//״̬����ת��֮�󣬾Ͳ��ٱ��������Ƕ���һ���ַ����¿�ʼ�ж�
				break;
			}
			if (strState == list[k].getOldState() && (cNow == list[k].getTrans())) {
				//���ջ�ջ�ջ���ַ��͵�ǰʶ����ַ�һ��(stack.empty()ջ�����⴦��Ҳ�鵽��һ�߼���)
				if (stack.empty() || ((char)stack.top()) == '$' || ((char)stack.top()) == cNow) {
					if (list[k].getTop() == '#') {
						strState = list[k].getNewState();//״̬ת�Ƶ���״̬
						cout << list[k].toString() << endl;
						isTrave = true;
						if (list[k].getPutInStack() != '#') {//����ջ�Ƿ�Ϊ�գ�ֻҪ�Ƿǿ��滻�գ�����ջ
							//�ǿ��滻�յ�״̬��Ҳ����Ҫѹ��ջ��һ���ַ�								
							cout << "push into stack" << list[k].getPutInStack() << endl;
							cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
							stack.push(list[k].getPutInStack());
						}
						else {
							cout << "Replace empty space with empty space and do not operate on the stack" << endl;
							cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
						}
					}
				}
				else {//ջ����Ԫ�أ��Һ������ַ���ͬ
					strState = list[k].getNewState();//״̬ת�Ƶ���״̬
					cout << list[k].toString();
					isTrave = true;
					if (list[k].getTop() != '#') {
						cout << "The out stack element is��" << stack.top() << endl;
						cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
						stack.pop();
					}
					else {
						cout << "Replace empty space with empty space and do not operate on the stack" << endl;
						cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
					}

				}
			}
		}
	}
	isTrave = false;
	if (!stack.empty() && ((char)stack.top()) == '$') {
		//����״̬ת�ƺ����õ����մ����״̬
		for (int k = 0; k < list.size(); k++) {
			if (isTrave) {
				break;
			}
			if (list[k].getTop() == '$' && strState == list[k].getOldState() && ('#' == list[k].getTrans())) {
				isTrave = true;
				strState = list[k].getNewState();//״̬ת�Ƶ���״̬
				cout << "The top of stack is ��" << stack.top() << "deal the char��change the stack" << endl;
				cout << list[k].toString() << endl;
				cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
			}
		}
	}
	else {
		cout << "After all strings are recognized, the top character of the stack is:" << stack.top() << "Not a makeing stack flag$" << endl;
		cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
	}
	if (strState == endState) {
		cout << "The characters B and C appear in the string the same number of times, can be correctly accepted by the PDA," << endl << " the complete string recognition in the final state is" << strState << endl;
	}
	else {
		cout << "The characters B and C appear in the string for different times and are rejected by the PDA. after the string is all recognized ,The current state is  " << strState << endl;

	}
	return 0;

}