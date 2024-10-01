#ifndef GENERATION_INCLUDE_INSTRUCTION_INFO_HPP
#define GENERATION_INCLUDE_INSTRUCTION_INFO_HPP

#include <array>
#include <string>

namespace IRGen {

// TODO: use yaml file
enum class InstructionType { I8, I16, I32, I64, U8, U16, U32, U64, VOID };

static std::array<std::string, static_cast<size_t>(InstructionType::VOID) + 1> typeToNameConverter = {
    "I8", "I16", "I32", "I64", "U8", "U16", "U32", "U64", "VOID"};

enum class InstructionOpcode {
    ADDI,
    MULI,
    MOVI,
    CMP,
    JA,   // Conditional
    JMP,  // Non-conditional
    RET,
    CAST,
    PHI  // should be last in enum
};

static std::array<std::string, static_cast<size_t>(InstructionOpcode::PHI) + 1> opcodeToNameConverter = {
    "ADDI", "MULI", "MOVI", "CMP",
    "JA",   // Conditional
    "JMP",  // Non-conditional
    "RET",  "CAST", "PHI"};

}  // namespace IRGen
#endif  // GENERATION_INCLUDE_INSTRUCTION_INFO_HPP
