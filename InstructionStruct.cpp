#include "InstructionSturct.h"
#include <iostream>
#include <string>
#include <map>

int InstructionStruct::currentAddress = 0;
int InstructionStruct::startAddress = 0;
string InstructionStruct::sourceName = "0";
list<string> InstructionStruct::sizeInst = {};
list<SIC_OPTAB> InstructionStruct::sic_optable = {};
map<string, int> InstructionStruct::symbolTable = {};
list<SIC_OPTAB> InstructionStruct::tempOptable = {};


InstructionStruct::InstructionStruct(string* words)
{
	this->label = words[0];
	this->opcode = words[1];
	this->operand = words[2];
}

void InstructionStruct::initOptab() //SIC Instruction Table
{
	InstructionStruct::sic_optable.push_back({ "add", '3', 0x18 });
	InstructionStruct::sic_optable.push_back({ "and", '3', 0x40 });
	InstructionStruct::sic_optable.push_back({ "comp", '3', 0x28 });
	InstructionStruct::sic_optable.push_back({ "div", '3', 0x24 });
	InstructionStruct::sic_optable.push_back({ "j", '3', 0x3C });
	InstructionStruct::sic_optable.push_back({ "jeq", '3', 0x30 });
	InstructionStruct::sic_optable.push_back({ "jgt", '3', 0x34 });
	InstructionStruct::sic_optable.push_back({ "jlt", '3', 0x38 });
	InstructionStruct::sic_optable.push_back({ "jsub", '3', 0x48 });
	InstructionStruct::sic_optable.push_back({ "lda", '3', 0x00 });
	InstructionStruct::sic_optable.push_back({ "ldch", '3', 0x50 });
	InstructionStruct::sic_optable.push_back({ "ldl", '3', 0x08 });
	InstructionStruct::sic_optable.push_back({ "ldx", '3', 0x04 });
	InstructionStruct::sic_optable.push_back({ "mul", '3', 0x20 });
	InstructionStruct::sic_optable.push_back({ "add", '3', 0x18 });
	InstructionStruct::sic_optable.push_back({ "or", '3', 0x44 });
	InstructionStruct::sic_optable.push_back({ "rd", '3', 0xD8 });
	InstructionStruct::sic_optable.push_back({ "rsub", '3', 0x4c });
	InstructionStruct::sic_optable.push_back({ "sta", '3', 0x0C });
	InstructionStruct::sic_optable.push_back({ "stch", '3', 0x54 });
	InstructionStruct::sic_optable.push_back({ "stl", '3', 0x14 });
	InstructionStruct::sic_optable.push_back({ "stsw", '3', 0xE8 });
	InstructionStruct::sic_optable.push_back({ "stx", '3', 0x10 });
	InstructionStruct::sic_optable.push_back({ "sub", '3', 0x1c });
	InstructionStruct::sic_optable.push_back({ "td", '3', 0xE0 });
	InstructionStruct::sic_optable.push_back({ "tix", '3', 0x2C });
	InstructionStruct::sic_optable.push_back({ "wd", '3', 0xDC });
}


void InstructionStruct::calAddress()
{
	if (this->opcode == "start")
	{
		InstructionStruct::sourceName = this->label;
		InstructionStruct::startAddress = stoi(this->operand, 0, 16);
		InstructionStruct::currentAddress = InstructionStruct::startAddress;
	}

	else if (find(begin(sizeInst), end(sizeInst), this->opcode) == end(sizeInst))
	{
		this->address = this->currentAddress;
		this->currentAddress += 0x3;
	}

	else
	{
		if (this->opcode == "resb")
		{
			this->address = this->currentAddress;
			this->currentAddress += 0x1 * stoi(this->operand);
		}
		else if (this->opcode == "resw")
		{
			this->address = this->currentAddress;
			this->currentAddress += 0x3 * stoi(this->operand);
		}
		else if (this->opcode == "word")
		{
			this->address = this->currentAddress;
			this->currentAddress += 0x3;
		}
		else if (this->opcode == "byte")
		{
			this->address = this->currentAddress;
			this->currentAddress += 0x1 * this->operand.size();
		}
	}
}
void InstructionStruct::initSizeInst()
{
	InstructionStruct::sizeInst.push_back("resb");
	InstructionStruct::sizeInst.push_back("resw");
	InstructionStruct::sizeInst.push_back("word");
	InstructionStruct::sizeInst.push_back("byte");
}

void InstructionStruct::convertOpcode()
{
	list<SIC_OPTAB>::iterator itr;

	for (itr = InstructionStruct::sic_optable.begin();
		itr != InstructionStruct::sic_optable.end(); itr++)
	{
		if (this->opcode == itr->Mnemonic)
		{
			this->hex_opcode = itr->MachineCode;
			this->instruction_format = itr->Format;
			InstructionStruct::tempOptable.push_back({ itr->Mnemonic, itr->Format, itr->MachineCode });
		}
	}
}

void InstructionStruct::makeSymbolTable()
{
	if (this->label.compare("") != 0)
	{
		if (!InstructionStruct::symbolTable.count(this->label))
		{
			InstructionStruct::symbolTable.insert
			(pair<string, int>(this->label, this->address));
		}

	}
}



void InstructionStruct::ShowTable()
{
	map<string, int> temp = symbolTable;
	list<SIC_OPTAB> tmp = tempOptable;

	cout << "============Symbol Table============" << endl;
	for (auto const& iter : temp)
		cout << iter.first << "  " << iter.second << endl;
	cout << "====================================" << endl;

	cout << "============Opcode (convert) Table ============" << endl;
	for (auto const& itr : tmp)
		cout << itr.Mnemonic << "  " << itr.MachineCode << endl;
	cout << "===============================================" << endl;
}


std::ostream& operator<<(std::ostream &strm, const InstructionStruct &a)
{
	return strm << "label=" << a.label << endl <<
		"opcode=" << a.opcode << endl <<
		"operand=" << a.operand << endl <<
		"address=" << hex << a.address << endl;
}

InstructionStruct::~InstructionStruct()
{
}
