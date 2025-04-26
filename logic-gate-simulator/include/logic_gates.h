#ifndef LOGIC_GATES_H
#define LOGIC_GATES_H

#include <stdbool.h>

// Logic gate types
#define AND_GATE 0
#define OR_GATE 1
#define NOT_GATE 2
#define NAND_GATE 3
#define NOR_GATE 4
#define XOR_GATE 5
#define XNOR_GATE 6

// Function prototypes
bool compute_logic_output(int gate_type, bool input_a, bool input_b);
const char* get_gate_name(int gate_type);
int get_gate_count(void);

#endif // LOGIC_GATES_H