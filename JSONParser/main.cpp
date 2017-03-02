/*	
	JSON Parser
	Written by Daniel Cironi
	February 21, 2017

	Description:
	This program accepts a JSON Document as input
	and outputs information about the document.
*/

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class jsonDoc
{
public:
	~jsonDoc() {};
	vector<char> document; //the document to be read
	int size = 0; //size of document;
	char currentItem;

	void readFile(string inputString)
	{
		ifstream inputFile;
		inputFile.open(inputString);
		char tempChar;
		while (inputFile >> noskipws >> tempChar)
		{
			cout << tempChar;
			document.push_back(tempChar);
			size++;
		}
	}

	void parse(int index)
	{
		//check if we are at end of document before doing anything else
		if (index > size)
		{
			cout << "End of Document!\n";
			return;
		}

		//skip white space
		if (document[index] == ' ')
		{
			parse(index + 1);
		}

		if (isdigit(document[index]))
		{
			index = parseNumber(index);
		}
		else
		{
			switch (document[index])
			{
			case '{':
			case '}':
				//parse object
				cout << "Found an Object!\n";
				index = parseObject(index);
				break;
			case '[':
			case ']':
				//parse array
				cout << "Found an Array!\n";
				index = parseArray(index);
				break;
			case '"':
				//parse string
				cout << "Found a String!\n";
				index = parseString(index);
				break;
			case ':':
				//parse pair
				cout << "Found a key value pair!\n";
				parse(index + 1);
				break;
			case 'T':
				//parse true
				cout << "Found True\n";
				parse(index + 1);
				break;
			case 'F':
				//parse false
				cout << "Found False\n";
				parse(index + 1);
				break;
			case 'N':
				//parse null
				cout << "Found Null\n";
				parse(index + 1);
				break;
			case '\n':
				cout << "Found New Line\n";
				parse(index + 1);
				break;
			default:
				break;
			}
		}

		parse(index + 1);
	};

private:
	int getDocumentSize()
	{
		return document.size();
	}

	int parseNumber(int index)
	{
		if (index > size)
		{
			return index;
		}
		if (document[index] == ' ' || document[index] == ',')
		{
			cout << "Found the end of a number\n";
			return index + 1;
		}
		else if(isdigit(document[index]))
		{
			cout << document[index];
			parseNumber(index + 1);
		}
		else
		{
			cout << "Unexpected value when parsing a number\n";
			return index + 1;
		}
	}

	int parseObject(int index)
	{
		if (index > size)
		{
			return index;
		}
		if (document[index] == '}')
		{
			cout << "Found the end of an object!\n";
			return index + 1;
		}
		else
		{
			cout << document[index];
			parse(index + 1);
		}
	}

	int parseArray(int index)
	{
		if (index > size)
		{
			return index;
		}
		if (document[index] == ']')
		{
			cout << "Found the end of an array!\n";
			return index + 1;
		}
		else
		{
			cout << document[index];
			parse(index + 1);
		}
	}

	int parseString(int index)
	{
		if (index > size)
		{
			return index;
		}
		if (document[index] == '"')
		{
			cout << "Found the end of a string!\n";
			return index + 1;
		}
		else
		{
			cout << document[index];
			parse(index + 1);
		}
	}
};

class jsonArray: public jsonDoc
{
public:
private:
	void parseArray(char item)
	{
		//call main parse
	}
};

class jsonObject : public jsonDoc
{
public:
private:
	void parseObject(char item)
	{
		//call main parse
	}
};

class jsonNumber : public jsonDoc
{
	
};

int main()
{
	//create JSON document
	jsonDoc Document;
		
	Document.readFile("JSONExample.txt");

	Document.parse(0);


	cin.get(); //just for testing
	return 0;
}