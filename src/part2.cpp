#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <math.h>
#include <vector>
#include <utility>
#include "project.h"
#define GREEN "\033[32m"
#define RESET "\033[0m"
#define CYAN "\033[36m"


//-------------------------------------------------------------------------------------------------------------
// Given the grammar of the language, trace the given program and alert any error that occurs.
// <prog> -> program   <identifier>; var  <dec-list>  begin  <stat-list> end.
// <identifier> -> <letter>{<letter>|<digit>}   note: this grammar is in EBNF
// <dec-list> -><dec> : <type> ;
// <dec> -> <identifier>,<dec>| < identifier >
// <type> -> integer 
// <stat-list> -> <stat> | <stat> <stat-list>
// <stat> -> <write> |  <assign>
// <write> -> write ( <str> < identifier > );
// <str> -> ”value=” , | λ  
// <assign> -> < identifier > = <expr>;
// <expr> -> <expr> + <term>  | <expr> - < <term> |  < term>
// <term> -> <term> * <factor> | <term> / <factor>| <factor> 
// <factor> -> < identifier > | <number> | ( <expr> )
// <number> -><sign><digit>{<digit> }  note: this grammar is in EBNF
// <sign> -> + | - | λ
// <digit> -> 0|1|2|…|9
// <letter> -> a|b|c|d|w|f
//--------------------------------------------------------------------------------------------------------------

static std::string abstractProgram = "";

/* 
 * Create an SLR(1) Parsing Table, which is based on LR(0) with enhancement of 
 * only reducing based on the FOLLOW set of the LHS non-terminal
    *  The tables has S (shift) and R (reduce) actions, 
    *  Constant number indicating the rule number that is to be reduced to, also the next row state
    *  The table will have E (error) action for all other blank cells (to be updated later)

*/

//create the table of size 84x54 based on the SLR(1) parsing table for the grammar
std::string table[84][54] = {
    {"", "E1", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "S2", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "1", "", "", "", "", "", "", ""},
    {"", "E1", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "ACC", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "E1", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "S5", "S6", "S7", "S8", "", "S9", "", "", "", "", "", "", "S10", "", "", "", "", "", "", "3", "", "4", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "E1", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "S11", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R45", "R45", "R45", "R45", "R45", "", "R45", "S12", "S13", "S14", "S15", "S16", "S17", "S18", "S19", "S20", "S21", "R45", "R45", "R45", "S5", "S6", "S7", "S8", "", "S9", "", "", "", "", "", "", "S10", "", "", "22", "", "", "", "", "", "23", "", "", "", "", "", "", "", "", "", "24", ""},
    {"", "R38", "R38", "R38", "R38", "R38", "", "R38", "R38", "R38", "R38", "R38", "R38", "R38", "R38", "R38", "R38", "R38", "R38", "R38", "R38", "R38", "R38", "R38", "R38", "", "R38", "", "", "", "", "", "", "R38", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R39", "R39", "R39", "R39", "R39", "", "R39", "R39", "R39", "R39", "R39", "R39", "R39", "R39", "R39", "R39", "R39", "R39", "R39", "R39", "R39", "R39", "R39", "R39", "", "R39", "", "", "", "", "", "", "R39", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R40", "R40", "R40", "R40", "R40", "", "R40", "R40", "R40", "R40", "R40", "R40", "R40", "R40", "R40", "R40", "R40", "R40", "R40", "R40", "R40", "R40", "R40", "R40", "", "R40", "", "", "", "", "", "", "R40", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R41", "R41", "R41", "R41", "R41", "", "R41", "R41", "R41", "R41", "R41", "R41", "R41", "R41", "R41", "R41", "R41", "R41", "R41", "R41", "R41", "R41", "R41", "R41", "", "R41", "", "", "", "", "", "", "R41", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R43", "R43", "R43", "R43", "R43", "", "R43", "R43", "R43", "R43", "R43", "R43", "R43", "R43", "R43", "R43", "R43", "R43", "R43", "R43", "R43", "R43", "R43", "R43", "", "R43", "", "", "", "", "", "", "R43", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R42", "R42", "R42", "R42", "R42", "", "R42", "R42", "R42", "R42", "R42", "R42", "R42", "R42", "R42", "R42", "R42", "R42", "R42", "R42", "R42", "R42", "R42", "R42", "", "R42", "", "", "", "", "", "", "R42", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "E1", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "S25", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R28", "R28", "R28", "R28", "R28", "", "R28", "R28", "R28", "R28", "R28", "R28", "R28", "R28", "R28", "R28", "R28", "R28", "R28", "R28", "R28", "R28", "R28", "R28", "", "R28", "", "", "", "", "", "", "R28", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R29", "R29", "R29", "R29", "R29", "", "R29", "R29", "R29", "R29", "R29", "R29", "R29", "R29", "R29", "R29", "R29", "R29", "R29", "R29", "R29", "R29", "R29", "R29", "", "R29", "", "", "", "", "", "", "R29", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R30", "R30", "R30", "R30", "R30", "", "R30", "R30", "R30", "R30", "R30", "R30", "R30", "R30", "R30", "R30", "R30", "R30", "R30", "R30", "R30", "R30", "R30", "R30", "", "R30", "", "", "", "", "", "", "R30", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R31", "R31", "R31", "R31", "R31", "", "R31", "R31", "R31", "R31", "R31", "R31", "R31", "R31", "R31", "R31", "R31", "R31", "R31", "R31", "R31", "R31", "R31", "R31", "", "R31", "", "", "", "", "", "", "R31", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R32", "R32", "R32", "R32", "R32", "", "R32", "R32", "R32", "R32", "R32", "R32", "R32", "R32", "R32", "R32", "R32", "R32", "R32", "R32", "R32", "R32", "R32", "R32", "", "R32", "", "", "", "", "", "", "R32", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R33", "R33", "R33", "R33", "R33", "", "R33", "R33", "R33", "R33", "R33", "R33", "R33", "R33", "R33", "R33", "R33", "R33", "R33", "R33", "R33", "R33", "R33", "R33", "", "R33", "", "", "", "", "", "", "R33", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R34", "R34", "R34", "R34", "R34", "", "R34", "R34", "R34", "R34", "R34", "R34", "R34", "R34", "R34", "R34", "R34", "R34", "R34", "R34", "R34", "R34", "R34", "R34", "", "R34", "", "", "", "", "", "", "R34", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R35", "R35", "R35", "R35", "R35", "", "R35", "R35", "R35", "R35", "R35", "R35", "R35", "R35", "R35", "R35", "R35", "R35", "R35", "R35", "R35", "R35", "R35", "R35", "", "R35", "", "", "", "", "", "", "R35", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R36", "R36", "R36", "R36", "R36", "", "R36", "R36", "R36", "R36", "R36", "R36", "R36", "R36", "R36", "R36", "R36", "R36", "R36", "R36", "R36", "R36", "R36", "R36", "", "R36", "", "", "", "", "", "", "R36", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R37", "R37", "R37", "R37", "R37", "", "R37", "R37", "R37", "R37", "R37", "R37", "R37", "R37", "R37", "R37", "R37", "R37", "R37", "R37", "R37", "R37", "R37", "R37", "", "R37", "", "", "", "", "", "", "R37", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R47", "R47", "R47", "R47", "R47", "", "R47", "S12", "S13", "S14", "S15", "S16", "S17", "S18", "S19", "S20", "S21", "R47", "R47", "R47", "S5", "S6", "S7", "S8", "", "S9", "", "", "", "", "", "", "S10", "", "", "22", "", "", "", "", "", "23", "", "", "", "", "", "", "", "", "", "26", ""},
    {"", "R46", "R46", "R46", "R46", "R46", "", "R46", "S12", "S13", "S14", "S15", "S16", "S17", "S18", "S19", "S20", "S21", "R46", "R46", "R46", "S5", "S6", "S7", "S8", "", "S9", "", "", "", "", "", "", "S10", "", "", "22", "", "", "", "", "", "23", "", "", "", "", "", "", "", "", "", "27", ""},
    {"", "R2", "R2", "R2", "R2", "R2", "", "R2", "", "", "", "", "", "", "", "", "", "", "R2", "R2", "R2", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "E1", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "S5", "S6", "S7", "S8", "", "S9", "", "", "", "", "", "", "S10", "", "", "", "", "", "28", "29", "", "4", "", "", "", "", "", "", "", "30", "", "", ""},
    {"", "R4", "R4", "R4", "R4", "R4", "", "R4", "", "", "", "", "", "", "", "", "", "", "R4", "R4", "R4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R3", "R3", "R3", "R3", "R3", "", "R3", "", "", "", "", "", "", "", "", "", "", "R3", "R3", "R3", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "E1", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "S31", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "E1", "", "", "S32", "", "", "", "", "", "", "", "", "", "", "", "", "", "R7", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "E1", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "S33", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "E1", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "S5", "S6", "S7", "S8", "", "S9", "", "", "", "S39", "", "", "S10", "", "34", "", "", "", "", "35", "", "4", "36", "", "", "", "", "37", "", "", "38", "", ""},
    {"", "E1", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "S5", "S6", "S7", "S8", "", "S9", "", "", "", "", "", "", "S10", "", "", "", "", "", "", "29", "", "4", "", "", "", "", "", "", "", "40", "", "", ""},
    {"", "E1", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "S42", "", "", "", "", "", "", "", "", "", "", "", "", "", "41", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "E1", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "R12", "R12", "R12", "R12", "R12", "R12", "", "", "", "R12", "", "", "R12", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "E1", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "S43", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "E1", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "S5", "S6", "S7", "S8", "R9", "S9", "", "", "", "S39", "", "", "S10", "", "34", "", "", "", "", "35", "", "4", "36", "", "", "", "", "44", "", "", "38", "", ""},
    {"", "E1", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "S45", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "E1", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "R11", "R11", "R11", "R11", "R11", "R11", "", "", "", "R11", "", "", "R11", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"S46", "E1", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "E1", "", "", "", "", "E1", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "E1", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "R6", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "E1", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "S47", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "E1", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "R8", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"S48", "E1", "", "S49", "", "S50", "", "", "S12", "S13", "S14", "S15", "S16", "S17", "S18", "S19", "S20", "S21", "", "", "", "S5", "S6", "S7", "S8", "", "S9", "", "", "", "", "", "", "S10", "", "", "51", "52", "53", "", "54", "", "4", "", "55", "", "", "56", "", "57", "", "", "", ""},
    {"", "E1", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "R10", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "E1", "", "", "", "", "S58", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "E1", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "S5", "S6", "S7", "S8", "", "S9", "", "S61", "", "", "", "", "S10", "", "", "", "", "", "", "59", "", "4", "", "", "60", "", "", "", "", "", "", "", ""},
    {"", "E1", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "R5", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"S48", "E1", "", "S49", "", "S50", "", "", "S12", "S13", "S14", "S15", "S16", "S17", "S18", "S19", "S20", "S21", "", "", "", "S5", "S6", "S7", "S8", "", "S9", "", "", "", "", "", "", "S10", "", "", "51", "62", "53", "", "54", "", "4", "", "55", "", "", "56", "", "57", "", "", "", ""},
    {"", "E1", "", "", "", "", "", "", "R26", "R26", "R26", "R26", "R26", "R26", "R26", "R26", "R26", "R26", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "E1", "", "", "", "", "", "", "R27", "R27", "R27", "R27", "R27", "R27", "R27", "R27", "R27", "R27", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R49", "R49", "R49", "", "R49", "", "R49", "S12", "S13", "S14", "S15", "S16", "S17", "S18", "S19", "S20", "S21", "", "R49", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "63", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "64"},
    {"", "E1", "", "S65", "", "S66", "", "", "", "", "", "", "", "", "", "", "", "", "", "S67", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R21", "R21", "R21", "", "R21", "", "R21", "", "", "", "", "", "", "", "", "", "", "", "R21", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R22", "R22", "R22", "", "R22", "", "R22", "", "", "", "", "", "", "", "", "", "", "", "R22", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R23", "R23", "R23", "", "R23", "", "R23", "", "", "", "", "", "", "", "", "", "", "", "R23", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "E1", "", "", "", "", "", "", "S12", "S13", "S14", "S15", "S16", "S17", "S18", "S19", "S20", "S21", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "68", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R18", "S69", "R18", "", "R18", "", "S70", "", "", "", "", "", "", "", "", "", "", "", "R18", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "E1", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "R1", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "S71", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "E1", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "S5", "S6", "S7", "S8", "", "S9", "", "", "", "", "", "", "S10", "", "", "", "", "", "", "72", "", "4", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "E1", "", "", "S73", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "S74", "", "S65", "", "S66", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R52", "R52", "R52", "", "R52", "", "R52", "S12", "S13", "S14", "S15", "S16", "S17", "S18", "S19", "S20", "S21", "", "R52", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "63", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "75"},
    {"", "R50", "R50", "R50", "", "R50", "", "R50", "", "", "", "", "", "", "", "", "", "", "", "R50", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"S48", "E1", "", "S49", "", "S50", "", "", "S12", "S13", "S14", "S15", "S16", "S17", "S18", "S19", "S20", "S21", "", "", "", "S5", "S6", "S7", "S8", "", "S9", "", "", "", "", "", "", "S10", "", "", "51", "", "53", "", "54", "", "4", "", "55", "", "", "56", "", "76", "", "", "", ""},
    {"S48", "E1", "", "S49", "", "S50", "", "", "S12", "S13", "S14", "S15", "S16", "S17", "S18", "S19", "S20", "S21", "", "", "", "S5", "S6", "S7", "S8", "", "S9", "", "", "", "", "", "", "S10", "", "", "51", "", "53", "", "54", "", "4", "", "55", "", "", "56", "", "77", "", "", "", ""},
    {"", "E1", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "R15", "R15", "R15", "R15", "R15", "R15", "", "", "", "R15", "", "", "R15", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R51", "R51", "R51", "", "R51", "", "R51", "S12", "S13", "S14", "S15", "S16", "S17", "S18", "S19", "S20", "S21", "", "R51", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "63", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "78"},
    {"S48", "E1", "", "S49", "", "S50", "", "", "S12", "S13", "S14", "S15", "S16", "S17", "S18", "S19", "S20", "S21", "", "", "", "S5", "S6", "S7", "S8", "", "S9", "", "", "", "", "", "", "S10", "", "", "51", "", "79", "", "54", "", "4", "", "55", "", "", "56", "", "", "", "", "", ""},
    {"S48", "E1", "", "S49", "", "S50", "", "", "S12", "S13", "S14", "S15", "S16", "S17", "S18", "S19", "S20", "S21", "", "", "", "S5", "S6", "S7", "S8", "", "S9", "", "", "", "", "", "", "S10", "", "", "51", "", "80", "", "54", "", "4", "", "55", "", "", "56", "", "", "", "", "", ""},
    {"", "E1", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "S81", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "S82", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "E1", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "R14", "R14", "R14", "R14", "", "R14", "", "", "", "", "", "", "R14", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R24", "R24", "R24", "", "R24", "", "R24", "", "", "", "", "", "", "", "", "", "", "", "R24", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R44", "R44", "R44", "", "R44", "", "R44", "", "", "", "", "", "", "", "", "", "", "", "R44", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R16", "S69", "R16", "", "R16", "", "S70", "", "", "", "", "", "", "", "", "", "", "", "R16", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R17", "S69", "R17", "", "R17", "", "S70", "", "", "", "", "", "", "", "", "", "", "", "R17", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R25", "R25", "R25", "", "R25", "", "R25", "", "", "", "", "", "", "", "", "", "", "", "R25", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R19", "R19", "R19", "", "R19", "", "R19", "", "", "", "", "", "", "", "", "", "", "", "R19", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R20", "R20", "R20", "", "R20", "", "R20", "", "", "", "", "", "", "", "", "", "", "", "R20", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "E1", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "R48", "R48", "R48", "R48", "R48", "R48", "", "", "", "R48", "", "", "R48", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "E1", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "S83", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "E1", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "R13", "R13", "R13", "R13", "R13", "R13", "", "", "", "R13", "", "", "R13", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""}
};

/*
    * Grammars have been abstracted in order to ease the validating process.
*/
//rule numbers start from 1, so put dummy values at index 0
std::string rules[53][2] = {
    {"",""}, {"P","pI;vHtSe."}, {"I","LX"}, {"X","LX"}, {"X","DX"}, {"H","V:K;"}, 
    {"V","I,V"}, {"V","I"}, {"K","i"}, {"S","M"}, {"S","MS"}, {"M","W"}, {"M","A"}, 
    {"W","q(OI);"}, {"O","o,"}, {"A","I=E;"}, {"E","E+T"}, {"E","E-T"}, {"E","T"}, 
    {"T","T*F"}, {"T","T/F"}, {"T","F"}, {"F","I"}, {"F","N"}, {"F","(E)"}, {"N","RDY"}, 
    {"R","+"}, {"R","-"}, {"D","0"}, {"D","1"}, {"D","2"}, {"D","3"}, {"D","4"}, {"D","5"}, 
    {"D","6"}, {"D","7"}, {"D","8"}, {"D","9"}, {"L","a"}, {"L","b"}, {"L","c"}, {"L","d"}, 
    {"L","w"}, {"L","f"}, {"Y","DY"}, {"I","L"}, {"X","L"}, {"X","D"}, {"W","q(I);"}, 
    {"N","D"}, {"N","DY"}, {"N","RD"}, {"Y","D"}
};
//length of the right hand side of the rules
int lengthOfRHS[53] = {
    0, 9, 2, 2, 2, 4, 3, 1, 1, 1, 2, 1, 1, 6, 2, 4, 3, 3, 1, 3, 3, 1, 1, 1, 3, 3, 
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 5, 1, 2, 2, 1
};


std::string retrieveState(std::string currentState, std::string gotoFactor) {
    int row = 0;
    int col = 0;;

    //convert string currentState to int and store in row
    row = std::stoi(currentState);
    //find the column from the gotoFactor
    std::string columnLabels = "()*+,-./0123456789:;=abcdefiopqtvw$ADEFHIKLMNOPRSTVWXY";
    col = columnLabels.find(gotoFactor);
    //nonTerminalIndex is the row, and terminalIndex is the column
    return table[row][col];
}
 
std::string handleState(std::string word, std::ofstream& outputFile, int wordIndex, Stack *stack) {
    //get the next state from the table
    std::string topElement = stack->pop();
    //print out the top element of the stack
    outputFile << "Pop: " << topElement << "\n";

    std::string currChar = word.substr(wordIndex, 1);
    //print out current read character in word
    outputFile << "Read: " << currChar << "\n";

    std::string gotoState = retrieveState(topElement, currChar);
    //print out goto state 
    outputFile << "Goto [" << topElement << "," << currChar << "] = " << gotoState << "\n";

    //if the state starts with S then it is a shift state
    if (gotoState[0] == 'S') {
        //Push the topElement to the stack, push currChar to the stack, push gotoState[1] to the stack
        stack->push(topElement);
        stack->push(currChar);
        stack->push(gotoState.substr(1));
        //print out all the pushed operations
        outputFile << "Push: " << topElement << ", " << currChar << ", " 
        << gotoState.substr(1) << "\n";
        //print out the stack
        stack->printStack(outputFile);
        
    } //else if state is a reduce state
    else if (gotoState[0] == 'R') {
        stack->push(topElement);
        //print out the push operation
        outputFile << "Push: " << topElement << "\n";

        //pop the stack twice as the number of characters in the ith right hand side rule 
        int ruleNumber = std::stoi(gotoState.substr(1)); 
        //print out the rule based on the ruleNumber
        outputFile << "#" << ruleNumber << " " << rules[ruleNumber][0] << "->" << rules[ruleNumber][1]<< "\n";
        int popSize = lengthOfRHS[ruleNumber];
        //print out the pop size 
        outputFile << "Pop: " << "2*|" << rules[ruleNumber][1] << "| = 2*"<< popSize 
        << "= " << popSize*2 << "\n";

        for (int k = 0; k < 2 * popSize; k++) {
            stack->pop();
        }
        //print out the stack
        stack->printStack(outputFile);
        //pop topElement from the stack and store it in the topElement
        topElement = stack->pop();
        //print out the pop operation
        outputFile << "Pop: " << topElement << "\n";
        //get the gotoFactor from the reduced state corresponding to the number returned by gotoState[1]
        //which is guaranteed to be a non-terminal letter
        std::string gotoFactor = rules[ruleNumber][0];
        //retrieve the next state from the table
        gotoState = retrieveState(topElement, gotoFactor);
        //print out goto state 
        outputFile << "Goto [" << topElement << "," << gotoFactor << "] = " << gotoState << "\n";
        //push the topElement to the stack, push gotoFactor, and nextState to the stack
        stack->push(topElement);
        stack->push(gotoFactor);
        stack->push(gotoState);
        //print out all the pushed operations
        outputFile << "Push: " << topElement << ", " << gotoFactor << ", " << gotoState << "\n";
        //print out the stack
        stack->printStack(outputFile);
    } //else if state is an accept state
    else if (gotoState == "ACC") { //do nothing,accept is handled in handleState function
    } //else if state is a reject state
    else if (gotoState == "") { //do nothing, reject is handled in handleState function
    }
    else if (gotoState[0] == 'E'){ 
        //currently only handle the case of missing left parenthesis, where it needs to be pushed back
        stack->push(topElement);
    }

   return gotoState;
}

std::pair<int,int> mappingToOriginalFile(std::string word, std::string originalFile, int prevLine) {
    std::ifstream originalFileStream(originalFile);
    if (originalFileStream.is_open()) {
        std::string mappedLine;
        while(std::getline(originalFileStream, mappedLine)) {
            for(int i = 1; i < prevLine; i++) {
                std::getline(originalFileStream, mappedLine);
            }
            int foundPos = mappedLine.find(word);
            prevLine = foundPos != std::string::npos ?  prevLine : ++prevLine;
            return std::make_pair(prevLine, foundPos);
        }
    }
    else {
        std::cout << "Unable to open file in mapping to original file" << std::endl;
    }
    originalFileStream.close();
    std::cout << "Not found token in file!! Corrupted!";
    return std::make_pair(-1,-1);
}

//function deduce the file into vector of strings and integers
//string is the abstract form of a token as described below 
//and int is the line number of error detection later on
std::vector<std::pair<std::string, std::pair<int, int> > > deduceFileToAbstractString(std::string originalFile, std::string outputFile) {
    std::vector<std::pair<std::string, std::pair<int, int> > > fileContents;
    /*
        * USE THIS RULE to abstract file into string format for parsing
        1. keywords, 
            program = p; var = v; begin = t; end = e; integer = i; write = q
        2. identifier, which starts with a, b, c, d, w, f, 
            operator: + - * / = ( ) and separator: , ; . :
            integer: start with a digit from 0 to 9 
           all are simply added to the string
        3. string literal starting with " is replaced by o
        4. append a $ at the end of the string
    */
    std::ifstream file(outputFile);
    if (file.is_open()) {
        std::string line;
        int lineNumber = 1;
        std::pair<int, int> position;
        while (std::getline(file, line)) {
            std::string word;
            std::stringstream ss(line);
            while (ss >> word) {
                position = mappingToOriginalFile(word, originalFile, lineNumber);
                lineNumber = position.first;
                if (word == "program") {
                    fileContents.push_back(std::make_pair("p", position));
                }
                else if (word == "var") {
                    fileContents.push_back(std::make_pair("v", position));
                }
                else if (word == "begin") {
                    fileContents.push_back(std::make_pair("t", position));
                }
                else if (word == "end.") {
                    fileContents.push_back(std::make_pair("e.", position));
                }
                else if (word == "integer") {
                    fileContents.push_back(std::make_pair("i", position));
                }
                else if (word == "write") {
                    fileContents.push_back(std::make_pair("q", position));
                }
                else if (word[0] == '"') {
                    fileContents.push_back(std::make_pair("o", position));
                }
                else if (word[0] == 'a' || word[0] == 'b' || word[0] == 'c' || word[0] == 'd' || word[0] == 'w' || word[0] == 'f' 
                        ||word[0] == '+' || word[0] == '-' || word[0] == '*' || word[0] == '/' || word[0] == '=' || word[0] == '(' 
                        || word[0] == ')' || word[0] == ',' || word[0] == ';' || word[0] == ':') {
                    fileContents.push_back(std::make_pair(word, position));
                }
                else if (word[0] >= '0' && word[0] <= '9') {
                    fileContents.push_back(std::make_pair(word, position));
                }
            }
        }
        fileContents.push_back(std::make_pair("$", std::make_pair(-1,-1)));
        file.close();
    }
    else {
        std::cout << "Error: cannot open file " << outputFile << std::endl;
    }

    return fileContents;
}

std::string retrieveAbstractFile() {
    return abstractProgram;
}


bool validateFile(std::string originalFile, std::string outputFile) {
    
    std::vector<std::pair<std::string, std::pair<int, int> > > fileContents = deduceFileToAbstractString(originalFile, outputFile);

    //For each pair in the vector, acesss the first element of pair append to a string call abstractProgram
    std::vector<std::pair<int, int> > tokenPos;
    for (int i = 0; i < fileContents.size(); i++) {
        //Each character in the abstractProgram belongs to a specific token
        //we can assign each character to a line number, for easier access later on
        for (int j = 0; j < fileContents[i].first.length(); j++) {
            tokenPos.push_back(fileContents[i].second);
        }

        abstractProgram += fileContents[i].first;
    }
    
    bool passedRoutine_0 = routine_0(abstractProgram);

	//create new stack
    Stack *stack = new Stack();
    
    //clear up the stack 
    stack->clearStack();
    //print the tracing word
    std::cout << "Abstract program: " << abstractProgram << std::endl;

    //output to a file for review, but all errors will be printed out to the console
    try {
        std::string tracingOutputFile = "Trace_Out.txt";
        std::ofstream outputFile(tracingOutputFile);
        if (!outputFile.is_open()) {
            throw "Can not open file " + tracingOutputFile;
        }
        outputFile << "Start writing to file Trace_Out.txt\n";
        //initial set up for the stack
        //push state 0 into the stack
        stack->push("0");
        outputFile << "Push 0 \n";
        outputFile << "_____________________________________\n";

        bool done = false;
       
        int currIndex = 0;
        
        // ! Before accepting a program make sure no error routine is ever called
        bool isAccepted = false;
        bool calledRoutine_1 = false;

        while (!done) {
            
            //TODO: Errors will be handled here
            std::string handledState = handleState(abstractProgram, outputFile, currIndex, stack);
            if (handledState[0] == 'S') {
                currIndex++;
            }
            else if (handledState[0] == 'R') {
                continue;
            }
            else if (handledState == "ACC") {
                done = true;
                if(passedRoutine_0 && !calledRoutine_1) {
                    isAccepted = true;
                }
            }
            else if (handledState == "") {
                done = true;
            }
            else if (handledState[0] == 'E') {
                if (handledState.substr(1) == "1") {
                    //find the line number based on the current position
                    std::pair<int, int> position = tokenPos[currIndex];
                    exceptionRoutine_1(&currIndex, position);
                    calledRoutine_1 = true;
                }
            }
            outputFile << "_____________________________________\n";
        } 

        isAccepted ? outputFile << ">>>>> Accepted <<<<< \n"
                    : outputFile << ">>>>> Rejected <<<<< \n";
        outputFile << "========================================\n";
        outputFile << "DONE !!!\n";

        outputFile.close();

        if(isAccepted) {
            std::cout << ">>>>> Accepted <<<<< \n";
            std::cout << "========================================\n";
            return true;
        }
        else {
            std::cout << ">>>>> Rejected <<<<< \n";
            std::cout << "========================================\n";
            return false;
        }
        
    }
    catch (std::string error) {
        std::cout << "Error: " << error << std::endl;
    }

    return false;
    
}

/*
   ! ALL ERROR RECOVERY ROUTINES are defined here
*/

/*
    * Routine 0: handle checking invalid variables assignment before declaration
    * Parse through declaration list and statementList to check if all variables have already been declared
*/
bool routine_0 (std::string program) {
    std::string declaredList;
    int declaredListStart = (int)program.find("v") + 1;
    int declaredListEnd = (int)program.find(":");

    declaredList = program.substr(declaredListStart, declaredListEnd - declaredListStart);
    //std::cout << "Declared list: " << declaredList << std::endl;
    std::vector<std::string> declaredVariables;

    std::stringstream ss(declaredList);
    while (ss.good()) {
        std::string substr;
        std::getline(ss, substr, ',');
        //std::cout << "Declared variable: " << substr << std::endl;
        declaredVariables.push_back(substr);
    }

    std::string statementList;
    int statementListStart = (int)program.find("t");
    int statementListEnd = (int)program.find("e") + 1;
    std::vector<std::string> assignedVariables;

    statementList = program.substr(statementListStart, statementListEnd - statementListStart);
    //std::cout << "Statement list: " << statementList << std::endl;
    std::vector<std::string> usedVariables;
    //iterate through statementList and extract all variable assignment which are the word before the = sign and after ;
    
    int i = 0;
    std::string identifier;
    bool isAssignment = false;
    while(i < statementList.length()) {
        if((statementList[i] == ';'|| statementList[i] == 't') && statementList[i+1]!= 'q' && statementList[i+1] != 'e') {
            isAssignment = true;
            i++;
        }
        while(isAssignment) {
            if(statementList[i] == '=') {
                isAssignment = false;
                break;
            }
            identifier += statementList[i];
            i++;
        }
        if (identifier != "") {
            //std::cout << "Used variable: " << identifier << std::endl;
            usedVariables.push_back(identifier);
            identifier = "";
        }
        i++;
    }

    //for each element in declaredVariables check if it is in usedVariables, if not issue a warning message
    for (int i = 0; i < declaredVariables.size(); i++) {
        if (std::find(usedVariables.begin(), usedVariables.end(), declaredVariables[i]) != usedVariables.end()) {
            continue;
        }
        else {
            //notify the identifier has not been used in the statement list
            std::cout << CYAN << "Warning: variable " << declaredVariables[i] << " has not been used. Consider removing it from the declaration list" <<RESET << std::endl;
        }
    }


    //for each element in usedVariables check if it is in declaredVariables, if not return false and issue an error
    for (int i = 0; i < usedVariables.size(); i++) {
        if (std::find(declaredVariables.begin(), declaredVariables.end(), usedVariables[i]) != declaredVariables.end()) {
            continue;
        }
        else {
            //notify the identifier has not been declared in the declaration list
            std::cout << GREEN << "Error: variable " << usedVariables[i] << " has not been declared" <<RESET << std::endl;
            return false;
        }
    }

    return true;
}


/*
    * Routine 1: handle missing left parenthesis, or right unbalanced parenthesis
    * based on the fact that right parenthesis is seen where it is not expected
    * -> Solution: skipping the token and continue parsing, mark the program as having error              
*/
void exceptionRoutine_1(int* currentIndex, std::pair<int, int> errPos) {
    std::cout << GREEN << "Error: " << "Missing ( at line:col " << errPos.first << ":" << errPos.second << RESET << std::endl;
    //unbalanced parenthesis, found right parenthesis where left parenthesis is missing
    //just skip the current character and continue
    //Increasing the value at address currentIndex, not increase the value of address ///// *currentIndex++ is wrong
    (*currentIndex)++;
}


