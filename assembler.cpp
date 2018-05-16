#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <map>
#include "InstructionSturct.h"

using namespace std;
static string* parseCode(string line);
static void makeCodeList(string* words);

list<InstructionStruct> codes; //One line approach
vector<string> file_out; //vector for write file

int main()
{
	
	stringstream ss;
	int idx = 0; //in order to verify,index
	int count = 0; 
	int vindex = 0; //RaddAddress vector index
	int fline = 0; //The total number of lines in the file
	int startAddress; 
	int SaddAddress = 0; 
	int FaddAddress = 0; 
	int sizeofassem = 0; //size of object code(total)

	vector<int> RaddAddress; //Size of object code in one line
	string newfilename = "test.txt";
	string fileName;
	cin >> fileName;
	ifstream is(fileName);
	ofstream writefile(newfilename);


	if (is.is_open())
	{
		while (!is.eof())
		{
			string line;
			getline(is, line); 
			if (line == "")                          
				continue;

			fline++;
			string* temp = parseCode(line);
			makeCodeList(temp);
			cout << line << endl;

		} 
		//=======read file=======

		InstructionStruct::initSizeInst();
		InstructionStruct::initOptab();

		list<InstructionStruct>::iterator iter;
		for (iter = codes.begin(); iter != codes.end(); iter++)
		{

			iter->calAddress();
			iter->convertOpcode();
			iter->makeSymbolTable();
		
			if (idx == 1)
				startAddress = iter->address; //save start Address
		
			if (count == 1)
			{
				SaddAddress = iter->address; 

			}
			if (count == 10)
			{
				FaddAddress = iter->address;
				FaddAddress = FaddAddress - SaddAddress;
				RaddAddress.push_back(FaddAddress); 
				FaddAddress = 0; 
				SaddAddress = 0;
				count = 0; //count initialization

			}

			if (idx == (fline - 2))
			{
				if (FaddAddress == 0)
				{
					FaddAddress = iter->address;
					FaddAddress = FaddAddress - SaddAddress;
					RaddAddress.push_back(FaddAddress);
					FaddAddress = 0; SaddAddress = 0;

				}

			}

			idx++;
			count++;

		}
		InstructionStruct::ShowTable();
	}
	else //not open file 
	{
		cout << "file does not exist" << endl;
	}

	idx = 0;//initialization
	count = 0;

	for (int i = 0; i < RaddAddress.size(); i++)
		sizeofassem += RaddAddress[i];

	sizeofassem += startAddress;

	//=========================================ONE PASS===========================================

	for (auto temp : codes) {

		if (temp.opcode == "start")
		{
			ss << "H" << temp.sourceName << " " << "00" << std::hex << startAddress << "00" << std::hex << sizeofassem << "\n";
			file_out.push_back(ss.str());
			ss.str(""); //stringstream initialization
		}

		if (temp.opcode != "start" && temp.opcode != "end")
		{
			if (count == 1)
			{
				ss << "T" << "00" << std::hex << temp.address << std::hex << RaddAddress[vindex];
				vindex++;

			}

			for (auto temp2 : temp.sic_optable)
			{
				if (temp.opcode == temp2.Mnemonic)
				{
					if (temp2.MachineCode / 16 < 1)
						ss << 0;

					ss << std::hex << temp2.MachineCode;
				}
			}

			if (temp.operand == "")
			{
				ss << "0000";
			}
			else if (temp.opcode == "byte" || temp.opcode == "resb"
				|| temp.opcode == "resw" || temp.opcode == "word") {
				continue;
			}
			else
				ss << std::hex << temp.symbolTable.find(temp.operand)->second;


		}
		if (count == 10) {
			ss << '\n';
			file_out.push_back(ss.str());
			ss.str("");
			count = 0;
		}

		count++;
		if (temp.opcode == "end")
		{
			ss << "E" << "00" << std::hex << startAddress << "\n";
			file_out.push_back(ss.str());
		}
		idx++;

	}

	//=============================================Two Pass=========================================================
	if (writefile.is_open())
	{
		for (int i = 0; i < file_out.size(); i++)
		{
			writefile << file_out[i];

		}

	}
	writefile.close();
}

static string* parseCode(string line) //parsing line
{
	string* words = new string[3];                        // lable, opcode, operend save list
	int intTemp;

	if (line.find(" ") == 0)                           // don't have label
	{
		words[0] = "";
		line = line.substr(9, line.size() - 9);            
		if (line.find(" ") != string::npos)                  
		{
			words[1] = line.substr(0, line.find(" "));         
			line = line.substr(8, line.size());
			words[2] = line.substr(0, line.find(" "));         
		}
		else                                                  
		{
			words[1] = line.substr(0, line.find(" "));        
			words[2] = "";
		}

		return words;
	}

	else
	{
		words[0] = line.substr(0, line.find(" "));
		line = line.substr(9, line.size() - 9);
		if (line.find(" ") != string::npos)
		{
			words[1] = line.substr(0, line.find(" "));
			line = line.substr(8, line.size());

			while (line[line.size() - 1] == ' ') // modified end of sentence blank
			{
				line.erase(line.size() - 1);
			}

			if (line.find("c'") != string::npos)
			{
				line.erase(line.size() - 1);
				line = line.substr(2, line.size() - 2);
			}

			if (line.find("x'") != string::npos)
			{
				line.erase(line.size() - 1);
				line = line.substr(2, line.size() - 2);
				line = to_string(stoi(line, 0, 16));
			}

			words[2] = line;
		}
		else
		{
			words[1] = line.substr(0, line.find(" ")); 
			words[2] = "";
		}
	}
	return words;
}

void makeCodeList(string* words)
{
	InstructionStruct newLine(words);
	codes.push_back(newLine);
}