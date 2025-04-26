#include "logic_gates.h"

const char *gate_names[] = {"AND", "OR", "NOT", "NAND", "NOR", "XOR", "XNOR"};

bool compute_logic_output(int gate_type, bool input_a, bool input_b) {
    switch(gate_type) {
        case AND_GATE:
            return input_a && input_b;
        case OR_GATE:
            return input_a || input_b;
        case NOT_GATE:
            return !input_a;
        case NAND_GATE:
            return !(input_a && input_b);
        case NOR_GATE:
            return !(input_a || input_b);
        case XOR_GATE:
            return input_a != input_b;
        case XNOR_GATE:
            return input_a == input_b;
        default:
            return false;
    }
}

const char* get_gate_name(int gate_type) {
    if (gate_type >= 0 && gate_type < sizeof(gate_names) / sizeof(gate_names[0])) {
        return gate_names[gate_type];
    }
    return "Unknown";
}

int get_gate_count(void) {
    return sizeof(gate_names) / sizeof(gate_names[0]);
}