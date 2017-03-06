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
#include <string>
#include <algorithm>

using namespace std;

class jsonValue : public string
{
public:
	jsonValue() = default;
	~jsonValue() {};
	string value;
	string type = "default";
};

class jsonObject : public jsonValue
{
public:
	jsonObject() = default;
	~jsonObject() = default;
	vector<jsonValue> items;
	int size = 0;
	string type = "object";
private:

};

class jsonArray : public jsonValue
{
public:
	jsonArray() = default;
	~jsonArray() = default;
	vector<jsonValue> items;
	string type = "array";
	int size = 0;
private:

};

class jsonString : public jsonValue
{
public:
	jsonString() = default;
	~jsonString() = default;
	vector<jsonValue> items;
	string type = "string";
	int size = 0;
private:

};

class jsonNumber : public jsonValue
{

};

class jsonNull : public jsonValue
{
public:
	jsonNull() = default;
	~jsonNull() = default;
	string type = "null";
};



class jsonDoc : public jsonValue
{
public:
	~jsonDoc() {};
	vector<jsonValue> values; //the document to be read, stored as a vector
	int size = 0; //size of JSON document
	int objectCount = 0;
	int arrayCount = 0;
	int numCount = 0;
	int stringCount = 0;
	int index = 0;

	jsonValue * parse()
	{	
		//skip white space
		if (values[index].value == "")
		{
			index++;
			parse();
		}
		else
		{
			if (values[index].value == "{")
				return parseObject();

			else if (values[index].value == "[")
				return parseArray();

			else if (values[index].value == "\"")
				return parseString();

			else
			{
				jsonNull *jNull = new jsonNull();
				return jNull;
			}

			/*switch (*F)
			{
			case ':':
				//parse pair
				//cout << "Found a key value pair!\n";
				//parse(F, L);
				break;
			case 'T':
				//parse true
				//cout << "Found True\n";
				//parse(F, L);
				break;
			case 'F':
				//parse false
				//cout << "Found False\n";
				//parse(F, L);
				break;
			case 'N':
				//parse null
				//cout << "Found Null\n";
				//parse(F, L);
				break;
			case '\n':
				//cout << "Found New Line\n";
				//parse(F, L);
				break;
			default:
				jsonNull * jNull;
				return jNull;
				break;
			}*/
		}
	};

private:
	jsonObject * parseObject()
	{
		if (values[index].value == "{")
		{
			cout << "Found Beginning of an Object!\n";
			++index;
		}

		jsonObject * currentObjectArray = new jsonObject;

		while (index < size) //parseObject until we find the end of an array
		{
			if (values[index].value == "}")
			{
				//end of array
				cout << "Found End of Object!\n";
				++index;
				++objectCount;

				if (index == size)
				{
					cout << "Reached End of Document!";
				}
				return currentObjectArray;
			}
			else
			{
				jsonValue * value = new jsonValue;
				//add each item to the object array
				//this is where I need to parse other things that could be in the object (string, array, etc.)
				//thus I'll just call the main parse function
				//the main parse function will return a value to be added to this object array
				value = parse();
				currentObjectArray->items.push_back(*value); //this will push the array, string, etc. into the object array. Thus making currentObjectArray a composite object
				/*if(value->type == "null")
					cout << "Pushed a null object into the Object Array!\n";
				else if (value->type == "array")
					cout << "Pushed an array into the Object Array!\n";
				else if (value->type == "object")
					cout << "Pushed an object into the Object Array!\n";
				else
					cout << "Something was pushed into the Object Array!\n";*/
				currentObjectArray->size++;
				++index;
				delete value;
			}
		}
		delete currentObjectArray;
	}

	jsonArray * parseArray()
	{
		if (values[index].value == "[")
		{
			++index;
			cout << "Found Beginning of an Array!\n";
		}

		jsonArray * currentArray = new jsonArray();

		while (index < size) //parseArray until we find the end of an array
		{
			if (values[index].value == "]")
			{
				//end of array
				cout << "Found End of Array!\n";
				++arrayCount;
				++index;
				return currentArray;
			}
			else
			{
				//add this item to the array object
				currentArray->items.push_back(values[index]);
				//cout << "Pushed " << document[index].value << " into an array!\n";
				currentArray->size++;
				++index;
			}
		}

		delete currentArray;
	}

	jsonString * parseString()
	{
		//skip the first double quote character
		++index;

		jsonString * currentStringArray = new jsonString();

		while (index < size) //parseString until we find the end of the string
		{
			if (values[index].value == "\"")
			{
				//end of string
				cout << "Found End of String!\n";
				++stringCount;
				++index;
				return currentStringArray;
			}
			else
			{
				//add this item to the string object
				currentStringArray->items.push_back(values[index]);
				currentStringArray->size++;
				++index;
			}
		}

		delete currentStringArray;
	}
};


void readFile(jsonDoc * Document)
{
	ifstream inputFile;
	string inputString = "JSONExample.txt";
	inputFile.open(inputString);
	char tempChar;
	while (inputFile >> noskipws >> tempChar)
	{
		cout << tempChar;
		jsonValue v;
		v.value += tempChar;
		Document->values.push_back(v);
		Document->size++;
	}
	cout << "\n\n\n";
}
int main()
{
	//Read in and create a JSON document structure
	jsonDoc * Document = new jsonDoc;
	readFile(Document);

	//Parse the Document
	Document->parse();

	//Print basic information about the document after parsing
	cout << "\n\n\n" << Document->objectCount << " Objects\n"
		<< Document->arrayCount << " Arrays\n"
		<< Document->stringCount << " Strings\n";

	cin.get(); //just for testing

	return 0;
}