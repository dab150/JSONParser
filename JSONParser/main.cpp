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

using namespace std;

class jsonValue : public string
{
public:
	jsonValue() = default;
	~jsonValue() {};
	string value;
	string type = "default";
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
	vector<jsonValue> document; //the document to be read, stored as a vector
	int size = 0; //size of JSON document
	int objectCount = 0;
	int arrayCount = 0;
	int index = 0;

	void readFile(string inputString)
	{
		ifstream inputFile;
		inputFile.open(inputString);
		char tempChar;
		while (inputFile >> noskipws >> tempChar)
		{
			cout << tempChar;
			jsonValue v;
			v.value += tempChar;
			document.push_back(v);
			size++;
		}
		cout << "\n\n\n";
	}

	jsonValue * parse()
	{	
		//skip white space
		if (document[index].value == "")
		{
			index++;
			parse();
		}
		else
		{
			if (document[index].value == "{")
			{
				return parseObject();
			}
			else if (document[index].value == "[")
			{
				return parseArray();
			}
			else
			{
				jsonNull *jNull = new jsonNull();
				return jNull;
			}
			/*switch (*F)
			{
			case '{':
			//case '}':
				//parse object
				//cout << "Found an Object!\n";
				//return parseObject(*F);
				break;
			case '[':
			//case ']':
				//parse array
				//jsonArray * jArray;
				return parseArray(F, L);
				break;
			case '"':
				//parse string
				//cout << "Found a String!\n";
				//parseString(*F);
				break;
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
		if (document[index].value == "{")
		{
			cout << "Found Beginning of an Object!\n";
			++index;
		}

		jsonObject * currentObjectArray = new jsonObject;

		while (index < size) //parseObject until we find the end of an array
		{
			if (document[index].value == "}")
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
		if (document[index].value == "[")
		{
			++index;
			cout << "Found Beginning of an Array!\n";
		}

		jsonArray * currentArray = new jsonArray();

		while (index < size) //parseArray until we find the end of an array
		{
			if (document[index].value == "]")
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
				currentArray->items.push_back(document[index]);
				//cout << "Pushed " << document[index].value << " into an array!\n";
				currentArray->size++;
				++index;
			}
		}

		delete &currentArray;
	}
};



int main()
{
	//create JSON document
	jsonDoc Document;
		
	Document.readFile("JSONExample.txt");


	Document.parse();

	cout << "\n\n\nThere were " << Document.objectCount << " objects and " << Document.arrayCount << " arrays\n";

	cin.get(); //just for testing
	return 0;
}