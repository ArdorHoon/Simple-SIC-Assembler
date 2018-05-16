#pragma once
#include <string>
#include <list>
#include <map>


using namespace std;


typedef struct OperationCodeTable
{
	string Mnemonic;
	int Format;
	unsigned short int MachineCode;
}SIC_OPTAB;


class InstructionStruct
{
public:
	string opcode;
	string operand;
	string label;
	int instruction_format;
	int address;
	static list<SIC_OPTAB> sic_optable;
	static int startAddress;
	static int currentAddress;
	static string sourceName;
	static list<string> sizeInst;
	static map<string, int> symbolTable;
	static list<SIC_OPTAB> tempOptable;

	InstructionStruct(string* words);
	void calAddress();
	void convertOpcode();
	void convertOperand();
	void makeSymbolTable();
	static void ShowTable();

	int hex_opcode;

	static void initSizeInst();
	static void initOptab();


	~InstructionStruct();
};

std::ostream& operator<<(std::ostream &strm, const InstructionStruct &a);
