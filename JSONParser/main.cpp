/*	
	JSON Parser
	Written by Daniel Cironi
	March 6, 2017

	Description:
	This program accepts a .json file as input
	and outputs information about the document.
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>

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
public:
	jsonNumber() = default;
	~jsonNumber() = default;
	vector<jsonValue> items;
	string type = "number";
	int size = 0;
private:

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
	int keyValueCount = 0;
	int falseCount = 0;
	int trueCount = 0;
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

			else if (values[index].value == ":")
			{
				++index;
				++keyValueCount;
				return parse();
			}

			else if (values[index].value == "T")
			{
				++index;
				++trueCount;
				return parse();
			}

			else if (values[index].value == "F")
			{
				++index;
				++falseCount;
				return parse();
			}

			else if (isdigit((values[index].value[0])))
			{
				return parseNumber();
			}
			else
			{
				jsonNull *jNull = new jsonNull();
				return jNull;
			}
		}
	};

private:
	jsonObject * parseObject()
	{
		if (values[index].value == "{")
		{
			++index;
		}

		jsonObject * currentObjectArray = new jsonObject;

		while (index < size) //parseObject until we find the end of an array
		{
			if (values[index].value == "}")
			{
				//end of array
				++index;
				++objectCount;
				return currentObjectArray;
			}
			else
			{
				jsonValue * value = new jsonValue;
				//add each item to the object array
				//this is where I need to parse other things that could be in the object (string, array, etc.)
				//therefore I just call the main parse function
				//the main parse function will return a value to be added to this object array
				value = parse();
				currentObjectArray->items.push_back(*value);
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
		}

		jsonArray * currentArray = new jsonArray();

		while (index < size) //parseArray until we find the end of an array
		{
			if (values[index].value == "]")
			{
				//end of array
				++arrayCount;
				return currentArray;
			}
			else
			{
				//add this item to the array object
				//we call parse here again in order to parse what is in the array
				//for example, this catches an instance of a string in an array
				jsonValue * value = new jsonValue;
				value = parse();
				currentArray->items.push_back(*value);
				currentArray->size++;
				++index;
				delete value;
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
				++stringCount;
				return currentStringArray;
			}
			else
			{
				//add this item to the string object
				//we don't call parse() here because we do not care what is in the string
				//if we called parse(), every "T" or "F" would be interpreted as true or false, which wouldn't be good.
				currentStringArray->items.push_back(values[index]);
				currentStringArray->size++;
				++index;
			}
		}

		delete currentStringArray;
	}

	jsonNumber * parseNumber()
	{
		jsonNumber * currentNumberArray = new jsonNumber();

		while (index < size) //parseString until we find the end of the string
		{
			if (values[index].value == "" || values[index].value == ",")
			{
				//end of number
				++numCount;
				return currentNumberArray;
			}
			else
			{
				currentNumberArray->items.push_back(values[index]);
				currentNumberArray->size++;
				++index;
			}
		}

		delete currentNumberArray;
	}
};


void readFile(jsonDoc * Document, string inputString)
{
	ifstream inputFile;
	inputFile.open(inputString);
	if (!inputFile.is_open())
		cout << "Could not open file\n";
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

int main(int argc, char *argv[])
{
	//read in the file location from the command line
	//arg1 = JSON Document
	string inputString(argv[1]);

	//Read in and create a JSON document structure
	jsonDoc * Document = new jsonDoc;
	readFile(Document, inputString);

	//Parse the Document
	Document->parse();

	//Print basic information about the document after parsing
	cout << "\n\n\n" << Document->objectCount << " Objects\n"
		<< Document->arrayCount << " Arrays\n"
		<< Document->stringCount << " Strings\n"
		<< Document->keyValueCount << " Key/Values\n"
		<< Document->trueCount << " \"True\" Values\n"
		<< Document->falseCount << " \"False\" Values\n"
		<< Document->numCount << " Numbers\n"
		<< "----------------------------------\n"
		<< Document->arrayCount + Document->stringCount + Document->keyValueCount
		+ Document->trueCount + Document->falseCount + Document->numCount << " Total Values in the Document\n";

	return 0;
}