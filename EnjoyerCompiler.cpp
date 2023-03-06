#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <regex>
using namespace std;

bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if (start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

string replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if (from.empty())
        return str;
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); 
    }
    return str;
}

/*
LDS-0
STS-1
MOV-2
ADD-3
ADI-4
SUB-5
CP-6
AND-7
ANDI-8
OR-9
ORI-A
XOR-B
XORI-C
ROL-D
JMP-E
BRBS-F
*/
char asm_decode(string const& inst) {
    if (inst == "LDS") return 0x0;
    if (inst == "STS") return 0x1;
    if (inst == "MOV") return 0x2;
    if (inst == "ADD") return 0x3;
    if (inst == "ADI") return 0x4;
    if (inst == "SUB") return 0x5;
    if (inst == "CP") return 0x6;
    if (inst == "AND") return 0x7;
    if (inst == "ANDI") return 0x8;
    if (inst == "OR") return 0x9;
    if (inst == "ORI") return 0xA;
    if (inst == "XOR") return 0xB;
    if (inst == "XORI") return 0xC;
    if (inst == "ROL") return 0xD;
    if (inst == "JMP") return 0xE;
    if (inst == "BRBS") return 0xF;
}

int asm_regnum(string const& reg) {
    if (reg == "R0") return 0x0;
    if (reg == "R1") return 0x1;
    if (reg == "R2") return 0x2;
    if (reg == "R3") return 0x3;
    if (reg == "R4") return 0x4;
    if (reg == "R5") return 0x5;
    if (reg == "R6") return 0x6;
    if (reg == "R7") return 0x7;
    if (reg == "R8") return 0x8;
    if (reg == "R9") return 0x9;
    if (reg == "R10") return 0xA;
    if (reg == "R11") return 0xB;
    if (reg == "R12") return 0xC;
    if (reg == "R13") return 0xD;
    if (reg == "R14") return 0xE;
    if (reg == "R15") return 0xF;
}



const int MEMORY_SIZE = 4096;
const int MAX_STATEMENT_COUNT = 8192;
int main(int argument_count, char* argument_list[])
{

    char regs[8];
    char ports[6];
    char clock[2];
    char config;
    char status;
    char memory[MEMORY_SIZE];
    int memoryIdx = 0;
    string line;
    memset(&memory[0], 0x00, MEMORY_SIZE);
	string filePath;

    if (argument_count < 2)
    {
         cout << "Enter the desired file path: ";
  cin >> filePath;
    }
    else
    {
        filePath = argument_list[1];
    }

    if (filePath[filePath.size() - 1] == 'c' || filePath[filePath.size() - 1] == 'C') {
        cout << "Beggining the process of compiling C to ASM..." << endl;
        
        ifstream file(filePath);

        if (!file) {
            cerr << "Error: Unable to open file at " << filePath << endl;
            return 1;
        }

        string statements[MAX_STATEMENT_COUNT];
        int statementIdx=0;
        string statementCurrent="";
        bool insideComment=false;
        //Part 1: Separating statements
        while (getline(file, line)) {
            cout << "[DEBUG] CURRENT LINE: "<< line << endl;
            
           

                
                size_t pos = 0;
                size_t bracketPos = 0;
                size_t startPos = 0;
                if (insideComment) {
                if ((pos = line.find("*/")) != std::string::npos) {
                    insideComment = false;
                    line.erase(0, pos + 2);
                }
                else {
                    continue;
                }
                }
               
                if ((startPos = line.find("/*")) != std::string::npos) {
                   
                    cout << "[DEBUG] LINE CONTAINS '/*'" << endl;
                    if (((pos = line.find("*/")) != std::string::npos) && (startPos < pos)) {
                    line.erase(startPos,pos-startPos+2);
                    cout << "[DEBUG] LINE CONTAINS '*/'" << endl;
                    }
                    else {
                        cout << "[DEBUG] COMMENT CONTINUES BEYOND LINE" << endl;
                        line.erase(pos + 1);
                        insideComment = true;
                    }
                   

                }


                if ((pos = line.find("//")) != std::string::npos) {
                    cout << "[DEBUG] LINE CONTAINS '//'" << endl;
                    line.erase(pos);

                }
                if ((pos = line.find("#")) != std::string::npos) {
                    if ((pos = line.find("define ")) != std::string::npos) {
                        statements[statementIdx] = line;
                        statementIdx++;

                    }
                    else {
                    cout << "[DEBUG] LINE CONTAINS '#' (NOT define) THEREFORE SKIPPED" << endl;
                    }
                   
                    continue;

                }

                startPos = 0;
                pos = 0;
                bracketPos = 0;
                bool loop = true;
                while (loop) {
                
                    if ((pos < bracketPos) && (pos < startPos)) {//;
                    statements[statementIdx] = statementCurrent+line.substr(0, pos);
                    statementCurrent = "";
                    line.erase(0, pos + 1);
                    statementIdx++;
                    }
                    else if ((bracketPos < pos) && (bracketPos < startPos)) {//{
                        statements[statementIdx] = statementCurrent + line.substr(0, bracketPos+1);
                        statementCurrent = "";
                        line.erase(0, bracketPos + 1);
                        statementIdx++;
                    }
                    else if ((startPos < pos) && (startPos < bracketPos)) {//}
                        statements[statementIdx] = statementCurrent + line.substr(0, startPos + 1);
                        statementCurrent = "";
                        line.erase(0, startPos + 1);
                        statementIdx++;
                    }
                    pos = line.find(";");
                    bracketPos = line.find("{");
                    startPos = line.find("}");
                    if ((pos == std::string::npos) && (startPos == std::string::npos) && (bracketPos == std::string::npos)) {
                        loop = false;
                    }
                }
                statementCurrent+= line;

             
           
        }
       

        //Part 2: Individual statement identification
        vector<string> checkFor;
        vector<string> replaceWith;
        size_t pos = 0;
        for (int i = 0; i <= statementIdx; i++) {
            if ((pos = statements[i].find("#define")) != std::string::npos) { // #define [NAME] [VALUE]
            
                std::regex rgx("[^ ]+");
                std::sregex_token_iterator iter(statements[i].begin(),
                    statements[i].end(),
                    rgx,
                    -1);
                std::sregex_token_iterator end;
                string parts[2];
                iter++;
                parts[0] = *iter;
                iter++;
                parts[1] = *iter;
               
                checkFor.push_back(parts[0]);
                replaceWith.push_back(parts[1]);
            }
            else {
                for (int j = 0; j < checkFor.size(); j++) {
                    if ((pos = statements[i].find(checkFor[j])) != std::string::npos) {  // replace occurences of checkfor with replacewith
                        statements[i].replace(pos, pos + checkFor[j].size()-1, replaceWith[j]);
                    }

                 
                  
                    std::regex rgx("[^ ]+");

                    for (std::sregex_iterator it = std::sregex_iterator(statements[i].begin(), statements[i].end(), rgx);
                        it != std::sregex_iterator();
                        ++it)
                    {
                        std::smatch m = *it;
                        m.str(); //string , switch asi a rozdeleni
                    }
               
                      
                   

                }



            }

        }
       /*
       cout << "[DEBUG] STATEMENT SEPARATED OUTPUT:" << endl;
       for (int i = 0; i <= statementIdx; i++) {
           cout << statements[i]  << endl;
       }
       */
        replace(filePath, ".c", ".asm");
    }
    
  ifstream file(filePath);

  if (!file) {
    cerr << "Error: Unable to open file at " << filePath << endl;
    return 1;
  }
  cout << "Beggining the process of translating ASM to binary...";
  while (getline(file, line)) {
      //cout << line << endl;
      string inst_parts[4];
      size_t pos = 0;
      int i = 0;
      while ((pos = line.find(" ")) != std::string::npos) {
          
          inst_parts[i] = line.substr(0, pos);
        
          line.erase(0, pos + 1);
          i++;
      }
     while ((pos = line.find(",")) != std::string::npos) {

          inst_parts[i] = line.substr(0, pos);

          line.erase(0, pos + 1);
          i++;
      }
      inst_parts[i] = line;
      char bytes[3];
      int tmp1;
      bytes[0] = asm_decode(inst_parts[0]);
      switch (bytes[0])
      {
      case 0x0: //lds
          bytes[0] = (bytes[0] << 4) | (asm_regnum(inst_parts[1]));
          
          bytes[1] = stoul(inst_parts[2], nullptr, 16) >>8;
          bytes[2] = stoul(inst_parts[2], nullptr, 16);
          memory[memoryIdx] = bytes[0];
          memoryIdx++;
          memory[memoryIdx] = bytes[1];
          memoryIdx++;
          memory[memoryIdx] = bytes[2];
          memoryIdx++;
          break;
      case 0x1://sts
          bytes[0] = (bytes[0] << 4) | (asm_regnum(inst_parts[1]));
          bytes[1] = stoul(inst_parts[2], nullptr, 16) >> 8;
          bytes[2] = stoul(inst_parts[2], nullptr, 16);
          memory[memoryIdx] = bytes[0];
          memoryIdx++;
          memory[memoryIdx] = bytes[1];
          memoryIdx++;
          memory[memoryIdx] = bytes[2];
          memoryIdx++;
          break;
      case 0x2://mov
          bytes[0] = (bytes[0] << 4) | (asm_regnum(inst_parts[1]));
          bytes[1] = asm_regnum(inst_parts[2]);
          memory[memoryIdx] = bytes[0];
          memoryIdx++;
          memory[memoryIdx] = bytes[1];
          memoryIdx++;
    
          break;
      case 0x3://add
          bytes[0] = (bytes[0] << 4) | (asm_regnum(inst_parts[1]));
          bytes[1] = asm_regnum(inst_parts[2])<<4;
          bytes[1] = bytes[1] | asm_regnum(inst_parts[3]);
          memory[memoryIdx] = bytes[0];
          memoryIdx++;
          memory[memoryIdx] = bytes[1];
          memoryIdx++;
          
          break;
      case 0x4://adi
          bytes[0] = (bytes[0] << 4) | (asm_regnum(inst_parts[1]));
          bytes[1] = stoul(inst_parts[2], nullptr, 16);
         
          memory[memoryIdx] = bytes[0];
          memoryIdx++;
          memory[memoryIdx] = bytes[1];
          memoryIdx++;
         
          break;
      case 0x5://sub
          bytes[0] = (bytes[0] << 4) | (asm_regnum(inst_parts[1]));
          bytes[1] = asm_regnum(inst_parts[2]) << 4;
          bytes[1] = bytes[1] | asm_regnum(inst_parts[3]);
          memory[memoryIdx] = bytes[0];
          memoryIdx++;
          memory[memoryIdx] = bytes[1];
          memoryIdx++;
         
          break;
      case 0x6://cp
          bytes[0] = (bytes[0] << 4) | (asm_regnum(inst_parts[1]));
          bytes[1] = asm_regnum(inst_parts[2]) << 4;
          bytes[1] = bytes[1] | asm_regnum(inst_parts[3]);
          memory[memoryIdx] = bytes[0];
          memoryIdx++;
          memory[memoryIdx] = bytes[1];
          memoryIdx++;
         
          break;
      case 0x7://and
          bytes[0] = (bytes[0] << 4) | (asm_regnum(inst_parts[1]));
          bytes[1] = asm_regnum(inst_parts[2]) << 4;
          bytes[1] = bytes[1] | asm_regnum(inst_parts[3]);
          memory[memoryIdx] = bytes[0];
          memoryIdx++;
          memory[memoryIdx] = bytes[1];
          memoryIdx++;
         
          break;
      case 0x8://andi
          bytes[0] = (bytes[0] << 4) | (asm_regnum(inst_parts[1]));
          bytes[1] = stoul(inst_parts[2], nullptr, 16);
          memory[memoryIdx] = bytes[0];
          memoryIdx++;
          memory[memoryIdx] = bytes[1];
          memoryIdx++;
       
          break;
      case 0x9://or
          bytes[0] = (bytes[0] << 4) | (asm_regnum(inst_parts[1]));
          bytes[1] = asm_regnum(inst_parts[2]) << 4;
          bytes[1] = bytes[1] | asm_regnum(inst_parts[3]);
          memory[memoryIdx] = bytes[0];
          memoryIdx++;
          memory[memoryIdx] = bytes[1];
          memoryIdx++;
         
          break;
      case 0xA://ori
          bytes[0] = (bytes[0] << 4) | (asm_regnum(inst_parts[1]));
          bytes[1] = stoul(inst_parts[2], nullptr, 16);
          memory[memoryIdx] = bytes[0];
          memoryIdx++;
          memory[memoryIdx] = bytes[1];
          memoryIdx++;
    
          break;
      case 0xB://xor
          bytes[0] = (bytes[0] << 4) | (asm_regnum(inst_parts[1]));
          bytes[1] = asm_regnum(inst_parts[2]) << 4;
          bytes[1] = bytes[1] | asm_regnum(inst_parts[3]);
          memory[memoryIdx] = bytes[0];
          memoryIdx++;
          memory[memoryIdx] = bytes[1];
          memoryIdx++;
   
          break;
      case 0xC://xori
          bytes[0] = (bytes[0] << 4) | (asm_regnum(inst_parts[1]));
          bytes[1] = stoul(inst_parts[2], nullptr, 16);
          memory[memoryIdx] = bytes[0];
          memoryIdx++;
          memory[memoryIdx] = bytes[1];
          memoryIdx++;
         
          break;
      case 0xD://roll
          bytes[0] = (bytes[0] << 4) | (asm_regnum(inst_parts[1]));
          memory[memoryIdx] = bytes[0];
          memoryIdx++;
       

          break;

      case 0xE://jmp
          bytes[0] = (bytes[0] << 4) | ((stoul(inst_parts[1], nullptr, 16)&0x0FFF) >> 8); //4b JMP
          bytes[1] = stoul(inst_parts[1], nullptr, 16) & 0xFF; //8b JMP
          memory[memoryIdx] = bytes[0];
          memoryIdx++;
          memory[memoryIdx] = bytes[1];
          memoryIdx++;
        
          break;
      case 0xF://brbs
          bytes[0] = (bytes[0] << 4) | (stoi(inst_parts[1]) << 1) | (inst_parts[2] == "S" ? 1 : 0) ;
          bytes[1] = stoul(inst_parts[3], nullptr, 16) >> 8;
          bytes[2] = stoul(inst_parts[3], nullptr, 16);
          memory[memoryIdx] = bytes[0];
          memoryIdx++;
          memory[memoryIdx] = bytes[1];
          memoryIdx++;
          memory[memoryIdx] = bytes[2];
          memoryIdx++;
          break;
        
      default:
          throw uncaught_exception();
          break;
      }

    
     
       
  }

  file.close();
  

  //binary file
  ofstream compiledfile(replaceAll(filePath,".asm",".hex"), ios::out | ios::binary);
  compiledfile.write(memory,MEMORY_SIZE);
  compiledfile.close();
  
  //text based file for simulIDE
  ofstream orderedfile(replaceAll(filePath, ".hex", ".data"), ios::out);
  for (int i = 0; i < MEMORY_SIZE/16; i++) {
      for (int j = 0; j < 16; j++) {
          unsigned int temp = (unsigned char)memory[j + i * 16];
     
          orderedfile << temp;
          if (j != 15) {
              orderedfile << ",    ";
          }
         
      }
      orderedfile << "\n";
  }
  orderedfile.close();
  cout << "Success!";
  return 0;
}

