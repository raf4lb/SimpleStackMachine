import sys

INSTRUCTIONS = {
    "HLT": 0,
    "PSHL": 1,
    "PSH": 2,
    "POP": 3,
    "POPA": 4,
    "TOP": 5,
    "DLY": 6,
    "JMP": 7,
    "PJIF": 8,
    "CPE": 9,
    "CPL": 10,
    "CPG": 11,
    "CPLE": 12,
    "CPGE": 13,
    "ADD": 14,
    "SUB": 15,
    "MUL": 16,
    "DIV": 17,
    "AND": 18,
    "OR": 19,
    "XOR": 20,
    "NOT": 21,
    "LSH": 22,
    "RSH": 23,
    "CALL": 24,
    "RET": 25,
    "NEG": 26,
}

LONG_INSTRUCTIONS = {
    "PSHL",
    "PSH",
    "POPA",
    "DLY",
    "JMP",
    "PJIF",
    "CALL",
}

PORTS = {
    "ddrb": "0",
    "portb": "1",
    "pinb": "2",
}


def strip(lines: list[str]) -> None:
    for i, line in enumerate(lines):
        lines[i] = line.strip()


def remove_comments(lines: list[str]) -> list[str]:
    for line, content in enumerate(lines):
        index = content.find(";")
        if index > -1:
            lines[line] = content[:index]
    return lines


def remove_blank_lines(lines: list[str]) -> list[str]:
    return [line for line in lines if line]


def remove_jumps(lines: list[str]):
    return [line for line in lines if not line.startswith(";")]


def build_jumps(lines: list[str]) -> list[str]:
    new_lines = []
    addresses = {}
    address = 0
    for content in lines:
        if content.startswith("."):
            addresses[content[1:]] = address - len(addresses)
        else:
            new_lines.append(content)
        if len(content.split(" ")) == 2:
            address += 3
        else:
            address += 1

    new_lines = remove_jumps(new_lines)
    for line, content in enumerate(new_lines):
        if "." in content:
            instruction, label = content.split(" ")
            new_lines[line] = f"{instruction} {addresses[label[1:]]}"
    return new_lines


def remove_vars(lines: list[str]) -> list[str]:
    return [line for line in lines if not line.startswith("VAR")]


def build_vars(lines: list[str], total_port_banks: int):
    addresses = PORTS.copy()
    for line in lines:
        if line.startswith("VAR"):
            var_name = line.split(" ")[1]
            addresses[var_name] = 2 * len(addresses)

    new_lines = remove_vars(lines)
    for line, content in enumerate(new_lines):
        if "$" in content:
            instruction, var_name = content.split(" ")
            new_lines[line] = f"{instruction} {addresses[var_name[1:]]}"
    return new_lines


def map_ports(lines: list[str]) -> None:
    for line, content in enumerate(lines):
        contents = content.split(" ")
        if len(contents) == 2:
            if contents[1] in PORTS:
                lines[line] = content.replace(contents[1], PORTS[contents[1]])


def pprint(lines):
    for line, content in enumerate(lines):
        print(f"{line}\t", content)


def compile_rfl(filename: str, total_port_banks=0, debug=False) -> list[int]:
    program = []
    with open(filename) as p:
        lines = p.read().splitlines()
        lines = remove_comments(lines)
        strip(lines)
        lines = remove_blank_lines(lines)
        lines = build_vars(lines, total_port_banks)
        lines = build_jumps(lines)
        map_ports(lines)
        # print(lines)
        if debug:
            dline = 0
        for line in lines:
            instruction = line.split(" ")
            if debug:
                print(f"{dline}\t", instruction)
                if len(instruction) == 2:
                    dline += 3
                else:
                    dline += 1
            opcode = bin(INSTRUCTIONS[instruction[0]])[2:]
            program.append(int(opcode, 2))
            operand = bin(int(instruction[1]))[2:] if len(instruction) == 2 else None
            if operand:
                operand = operand.zfill(16)
                h_operand = operand[:8]
                l_operand = operand[8:]
                program.append(int(h_operand, 2))
                program.append(int(l_operand, 2))
    return program


def get_instruction(code):
    for inst in INSTRUCTIONS:
        if code == INSTRUCTIONS[inst]:
            return inst


def disassembly(code):
    disassembled = []
    i = 0
    while i < len(code):
        inst = get_instruction(code[i])
        try:
            if inst in LONG_INSTRUCTIONS:
                param = code[i + 1] << 8 | code[i + 2]
                i += 3
                disassembled.append(f"{inst} {param}")
            else:
                disassembled.append(inst)
                i += 1
        except:
            print(f"Error in line {i}: {code[i]}({inst})")
            break
    return disassembled


if __name__ == "__main__":
    total_port_banks = int(sys.argv[2])
    program = compile_rfl(sys.argv[1], total_port_banks, False)
    # print(disassembly(program))
    program_len = len(program)
    output = "{" + ",".join([str(inst) for inst in program]) + "}"
    print(output, program_len)
