#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <math.h>
#include <vector>
#include <utility>
#include "project.h"

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


/*  
 * Create an SLR(1) Parsing Table, which is based on LR(0) with enhancement of 
 * only reducing based on the FOLLOW set of the LHS rule 
    *  The tables has S (shift) and R (reduce) actions, 
    *  Constant number indicating the rule number that is to be reduced to
    *  The table will have E (error) action for all other blank cells (to be updated later)

*/

//create the table of size 84x54 based on the above SLR(1) parsing table
std::string table[84][54] = {
    {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "S2", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "1", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "ACC", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "S5", "S6", "S7", "S8", "", "S9", "", "", "", "", "", "", "S10", "", "", "", "", "", "", "3", "", "4", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "S11", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R45", "R45", "R45", "R45", "R45", "", "R45", "S12", "S13", "S14", "S15", "S16", "S17", "S18", "S19", "S20", "S21", "R45", "R45", "R45", "S5", "S6", "S7", "S8", "", "S9", "", "", "", "", "", "", "S10", "", "", "22", "", "", "", "", "", "23", "", "", "", "", "", "", "", "", "", "24", ""},
    {"", "R38", "R38", "R38", "R38", "R38", "", "R38", "R38", "R38", "R38", "R38", "R38", "R38", "R38", "R38", "R38", "R38", "R38", "R38", "R38", "R38", "R38", "R38", "R38", "", "R38", "", "", "", "", "", "", "R38", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R39", "R39", "R39", "R39", "R39", "", "R39", "R39", "R39", "R39", "R39", "R39", "R39", "R39", "R39", "R39", "R39", "R39", "R39", "R39", "R39", "R39", "R39", "R39", "", "R39", "", "", "", "", "", "", "R39", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R40", "R40", "R40", "R40", "R40", "", "R40", "R40", "R40", "R40", "R40", "R40", "R40", "R40", "R40", "R40", "R40", "R40", "R40", "R40", "R40", "R40", "R40", "R40", "", "R40", "", "", "", "", "", "", "R40", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R41", "R41", "R41", "R41", "R41", "", "R41", "R41", "R41", "R41", "R41", "R41", "R41", "R41", "R41", "R41", "R41", "R41", "R41", "R41", "R41", "R41", "R41", "R41", "", "R41", "", "", "", "", "", "", "R41", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R43", "R43", "R43", "R43", "R43", "", "R43", "R43", "R43", "R43", "R43", "R43", "R43", "R43", "R43", "R43", "R43", "R43", "R43", "R43", "R43", "R43", "R43", "R43", "", "R43", "", "", "", "", "", "", "R43", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R42", "R42", "R42", "R42", "R42", "", "R42", "R42", "R42", "R42", "R42", "R42", "R42", "R42", "R42", "R42", "R42", "R42", "R42", "R42", "R42", "R42", "R42", "R42", "", "R42", "", "", "", "", "", "", "R42", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "S25", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
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
    {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "S5", "S6", "S7", "S8", "", "S9", "", "", "", "", "", "", "S10", "", "", "", "", "", "28", "29", "", "4", "", "", "", "", "", "", "", "30", "", "", ""},
    {"", "R4", "R4", "R4", "R4", "R4", "", "R4", "", "", "", "", "", "", "", "", "", "", "R4", "R4", "R4", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R3", "R3", "R3", "R3", "R3", "", "R3", "", "", "", "", "", "", "", "", "", "", "R3", "R3", "R3", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "S31", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "S32", "", "", "", "", "", "", "", "", "", "", "", "", "", "R7", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "S33", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "S5", "S6", "S7", "S8", "", "S9", "", "", "", "S39", "", "", "S10", "", "34", "", "", "", "", "35", "", "4", "36", "", "", "", "", "37", "", "", "38", "", ""},
    {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "S5", "S6", "S7", "S8", "", "S9", "", "", "", "", "", "", "S10", "", "", "", "", "", "", "29", "", "4", "", "", "", "", "", "", "", "40", "", "", ""},
    {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "S42", "", "", "", "", "", "", "", "", "", "", "", "", "", "41", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "R12", "R12", "R12", "R12", "R12", "R12", "", "", "", "R12", "", "", "R12", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "S43", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "S5", "S6", "S7", "S8", "R9", "S9", "", "", "", "S39", "", "", "S10", "", "34", "", "", "", "", "35", "", "4", "36", "", "", "", "", "44", "", "", "38", "", ""},
    {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "S45", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "R11", "R11", "R11", "R11", "R11", "R11", "", "", "", "R11", "", "", "R11", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"S46", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "R6", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "S47", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "R8", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"S48", "", "", "S49", "", "S50", "", "", "S12", "S13", "S14", "S15", "S16", "S17", "S18", "S19", "S20", "S21", "", "", "", "S5", "S6", "S7", "S8", "", "S9", "", "", "", "", "", "", "S10", "", "", "51", "52", "53", "", "54", "", "4", "", "55", "", "", "56", "", "57", "", "", "", ""},
    {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "R10", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "S58", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "S5", "S6", "S7", "S8", "", "S9", "", "S61", "", "", "", "", "S10", "", "", "", "", "", "", "59", "", "4", "", "", "60", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "R5", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"S48", "", "", "S49", "", "S50", "", "", "S12", "S13", "S14", "S15", "S16", "S17", "S18", "S19", "S20", "S21", "", "", "", "S5", "S6", "S7", "S8", "", "S9", "", "", "", "", "", "", "S10", "", "", "51", "62", "53", "", "54", "", "4", "", "55", "", "", "56", "", "57", "", "", "", ""},
    {"", "", "", "", "", "", "", "", "R26", "R26", "R26", "R26", "R26", "R26", "R26", "R26", "R26", "R26", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", "R27", "R27", "R27", "R27", "R27", "R27", "R27", "R27", "R27", "R27", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R49", "R49", "R49", "", "R49", "", "R49", "S12", "S13", "S14", "S15", "S16", "S17", "S18", "S19", "S20", "S21", "", "R49", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "63", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "64"},
    {"", "", "", "S65", "", "S66", "", "", "", "", "", "", "", "", "", "", "", "", "", "S67", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R21", "R21", "R21", "", "R21", "", "R21", "", "", "", "", "", "", "", "", "", "", "", "R21", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R22", "R22", "R22", "", "R22", "", "R22", "", "", "", "", "", "", "", "", "", "", "", "R22", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R23", "R23", "R23", "", "R23", "", "R23", "", "", "", "", "", "", "", "", "", "", "", "R23", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", "S12", "S13", "S14", "S15", "S16", "S17", "S18", "S19", "S20", "S21", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "68", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R18", "S69", "R18", "", "R18", "", "S70", "", "", "", "", "", "", "", "", "", "", "", "R18", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "R1", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "S71", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "S5", "S6", "S7", "S8", "", "S9", "", "", "", "", "", "", "S10", "", "", "", "", "", "", "72", "", "4", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "S73", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "S74", "", "S65", "", "S66", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R52", "R52", "R52", "", "R52", "", "R52", "S12", "S13", "S14", "S15", "S16", "S17", "S18", "S19", "S20", "S21", "", "R52", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "63", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "75"},
    {"", "R50", "R50", "R50", "", "R50", "", "R50", "", "", "", "", "", "", "", "", "", "", "", "R50", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"S48", "", "", "S49", "", "S50", "", "", "S12", "S13", "S14", "S15", "S16", "S17", "S18", "S19", "S20", "S21", "", "", "", "S5", "S6", "S7", "S8", "", "S9", "", "", "", "", "", "", "S10", "", "", "51", "", "53", "", "54", "", "4", "", "55", "", "", "56", "", "76", "", "", "", ""},
    {"S48", "", "", "S49", "", "S50", "", "", "S12", "S13", "S14", "S15", "S16", "S17", "S18", "S19", "S20", "S21", "", "", "", "S5", "S6", "S7", "S8", "", "S9", "", "", "", "", "", "", "S10", "", "", "51", "", "53", "", "54", "", "4", "", "55", "", "", "56", "", "77", "", "", "", ""},
    {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "R15", "R15", "R15", "R15", "R15", "R15", "", "", "", "R15", "", "", "R15", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R51", "R51", "R51", "", "R51", "", "R51", "S12", "S13", "S14", "S15", "S16", "S17", "S18", "S19", "S20", "S21", "", "R51", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "63", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "78"},
    {"S48", "", "", "S49", "", "S50", "", "", "S12", "S13", "S14", "S15", "S16", "S17", "S18", "S19", "S20", "S21", "", "", "", "S5", "S6", "S7", "S8", "", "S9", "", "", "", "", "", "", "S10", "", "", "51", "", "79", "", "54", "", "4", "", "55", "", "", "56", "", "", "", "", "", ""},
    {"S48", "", "", "S49", "", "S50", "", "", "S12", "S13", "S14", "S15", "S16", "S17", "S18", "S19", "S20", "S21", "", "", "", "S5", "S6", "S7", "S8", "", "S9", "", "", "", "", "", "", "S10", "", "", "51", "", "80", "", "54", "", "4", "", "55", "", "", "56", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "S81", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "S82", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "R14", "R14", "R14", "R14", "", "R14", "", "", "", "", "", "", "R14", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R24", "R24", "R24", "", "R24", "", "R24", "", "", "", "", "", "", "", "", "", "", "", "R24", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R44", "R44", "R44", "", "R44", "", "R44", "", "", "", "", "", "", "", "", "", "", "", "R44", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R16", "S69", "R16", "", "R16", "", "S70", "", "", "", "", "", "", "", "", "", "", "", "R16", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R17", "S69", "R17", "", "R17", "", "S70", "", "", "", "", "", "", "", "", "", "", "", "R17", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R25", "R25", "R25", "", "R25", "", "R25", "", "", "", "", "", "", "", "", "", "", "", "R25", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R19", "R19", "R19", "", "R19", "", "R19", "", "", "", "", "", "", "", "", "", "", "", "R19", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "R20", "R20", "R20", "", "R20", "", "R20", "", "", "", "", "", "", "", "", "", "", "", "R20", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "R48", "R48", "R48", "R48", "R48", "R48", "", "", "", "R48", "", "", "R48", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "S83", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "R13", "R13", "R13", "R13", "R13", "R13", "", "", "", "R13", "", "", "R13", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""} 
};

/*
    * Grammars have been abstracted in order to ease the validating process.
*/
//rule numbers start from 1, so put dummy values at index 0
std::string rules[53] = {
    "", "P->pI;vHtSe.", "I->LX", "X->LX", "X->DX", "H->V:K;", "V->I,V", "V->I", "K->i", 
    "S->M", "S->MS", "M->W", "M->A", "W->q(OI);", "O->o,", "A->I=E;", "E->E+T", "E->E-T", 
    "E->T", "T->T*F", "T->T/F", "T->F", "F->I", "F->N", "F->(E)", "N->RDY", "R->+", "R->-", 
    "D->0", "D->1", "D->2", "D->3", "D->4", "D->5", "D->6", "D->7", "D->8", "D->9", "L->a", 
    "L->b", "L->c", "L->d", "L->w", "L->f", "Y->DY", "I->L", "X->L", "X->D", "W->q(I);", 
    "N->D", "N->DY", "N->RD", "Y -> D"
};
//length of the right hand side of the rules
int lengthOfRHS[53] = {
    0, 9, 2, 2, 2, 4, 3, 1, 1, 1, 2, 1, 1, 6, 2, 4, 3, 3, 1, 3, 3, 1, 1, 1, 3, 3, 
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 5, 1, 2, 2, 1
};

std::string reducedState[53] = {
    "", "P", "I", "X", "X", "H", "V", "V", "K", "S", "S", "M", "M", "W", "O", "A", "E", 
    "E", "E", "T", "T", "T", "F", "F", "F", "N", "R", "R", "D", "D", "D", "D", "D", "D", 
    "D", "D", "D", "D", "L", "L", "L", "L", "L", "L", "Y", "I", "X", "X", "W", "N", "N", "N", "Y"
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
        outputFile << "#" << ruleNumber << " " << rules[ruleNumber] << "\n";
        int popSize = lengthOfRHS[ruleNumber];
        //print out the pop size 
        outputFile << "Pop: " << "2*|" << rules[ruleNumber].substr(3) << "| = 2*"<< popSize 
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
        std::string gotoFactor = reducedState[ruleNumber];
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
    else if (gotoState == "ACC") { //do nothing,accept is handled in main function
    } //else if state is a reject state
    else if (gotoState == "") { //do nothing, reject is handled in main function
    }
    else {
        std::cout << "Error: not a valid gotoState" << std::endl;
    }
   return gotoState;
}

//function deduce the file into vector of strings and integers
//string is the abstract form of a token as described below 
//and int is the line number of error detection later on
std::vector<std::pair<std::string, int> > deduceFileToAbstractString(std::string fileName) {
    std::vector<std::pair<std::string, int> > fileContents;
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
    std::ifstream file(fileName);
    if (file.is_open()) {
        std::string line;
        int lineNumber = 1;
        while (std::getline(file, line)) {
            std::string word;
            std::stringstream ss(line);
            while (ss >> word) {
                
                if (word == "program") {
                    fileContents.push_back(std::make_pair("p", lineNumber));
                }
                else if (word == "var") {
                    fileContents.push_back(std::make_pair("v", lineNumber));
                }
                else if (word == "begin") {
                    fileContents.push_back(std::make_pair("t", lineNumber));
                }
                else if (word == "end.") {
                    fileContents.push_back(std::make_pair("e.", lineNumber));
                }
                else if (word == "integer") {
                    fileContents.push_back(std::make_pair("i", lineNumber));
                }
                else if (word == "write") {
                    fileContents.push_back(std::make_pair("q", lineNumber));
                }
                else if (word[0] == '"') {
                    fileContents.push_back(std::make_pair("o", lineNumber));
                }
                else if (word[0] == 'a' || word[0] == 'b' || word[0] == 'c' || word[0] == 'd' || word[0] == 'w' || word[0] == 'f' 
                        ||word[0] == '+' || word[0] == '-' || word[0] == '*' || word[0] == '/' || word[0] == '=' || word[0] == '(' 
                        || word[0] == ')' || word[0] == ',' || word[0] == ';' || word[0] == ':') {
                    fileContents.push_back(std::make_pair(word, lineNumber));
                }
                else if (word[0] >= '0' && word[0] <= '9') {
                    fileContents.push_back(std::make_pair(word, lineNumber));
                }    
            }
            lineNumber++;
        }
        fileContents.push_back(std::make_pair("$", -1));
        file.close();
    }
    else {
        std::cout << "Error: cannot open file " << fileName << std::endl;
    }

    return fileContents;
}

void validateFile(std::string filename) {
    std::vector<std::pair<std::string, int> > fileContents = deduceFileToAbstractString(filename);

    //For each pair in the vector, acesss the first element of pair append to a string call abstractProgram
    std::string abstractProgram = "";
    for (int i = 0; i < fileContents.size(); i++) {
        abstractProgram += fileContents[i].first;
    }
    
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
        bool isAccepted = false;
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
                isAccepted = true;
                done = true;
            }
            else if (handledState == "") {
                done = true;
            }
            outputFile << "_____________________________________\n";
        } 

        isAccepted ? outputFile << ">>>>> Accepted <<<<< \n"
                    : outputFile << ">>>>> Rejected <<<<< \n";
        outputFile << "_____________________________________\n";
        outputFile << "_____________________________________\n";
        outputFile << "DONE !!!\n";

        outputFile.close();

        isAccepted ? std::cout << ">>>>> Accepted <<<<< \n"
                    : std::cout << ">>>>> Rejected <<<<< \n";
    }
    catch (std::string error) {
        std::cout << "Error: " << error << std::endl;
    }
    
}
