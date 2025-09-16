from compiler import instruction_classes

def generate_c_enum(instruction_classes, enum_name="OpCode", prefix="OP_"):
    # Ordering classes by opcode
    sorted_instr = sorted(instruction_classes, key=lambda cls: cls.opcode)
    
    lines = []
    lines.append(f"typedef enum {{")
    
    for instr in sorted_instr:
        # Convert name upper case and format it (replaces spaces, if necessary)
        enum_name_val = instr.name.upper().replace(" ", "_")
        # Build line with enum and its value
        lines.append(f"    {prefix}{enum_name_val} = {instr.opcode},")
    
    lines.append(f"}} {enum_name};")
    return "\n".join(lines)

if __name__ == "__main__":
    enum_code = generate_c_enum(instruction_classes)
    print(enum_code)
