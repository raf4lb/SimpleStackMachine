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
    "SYSCALL": 26,
}

LONG_INSTRUCTIONS = {
    "PSHL",
    "PSH",
    "POPA",
    "DLY",
    "JMP",
    "PJIF",
    "CALL",
    "SYSCALL",
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


def to_utf8(string):
    utf8_bytes = string.encode("utf-8")
    utf8_array = [byte for byte in utf8_bytes]
    return ",".join(str(c) for c in utf8_array) + ",0"


def build_utf8_strings(lines: list[str]):
    for i, content in enumerate(lines):
        content = content.replace("\\n", "\n")
        if '"' in content:
            string = str()
            found = False
            for c in content:
                if c == '"':
                    if found:
                        found = False
                        break
                    else:
                        found = True
                        continue
                if found:
                    string += c
            ascii_list = to_utf8(string)
            lines[i] = content.replace(f'"{string}"', ascii_list)


def build_vars(lines: list[str]):
    addresses = PORTS.copy()
    data = []

    def allocate_str(value):
        data.extend(int(c) for c in value.split(","))  # store ascii value

    for line in lines:
        if line.startswith("VAR"):
            var_name = line.split(" ")[1]
            addresses[var_name] = 2 * len(addresses)

        elif line.startswith("DATA"):
            var_name, var_type, var_value = line.split(" ")[1:]
            addresses[var_name] = len(data)  # free index
            if var_type == "STRING":
                allocate_str(var_value)

    new_lines = remove_vars(lines)
    for line, content in enumerate(new_lines):
        if "$" in content:
            instruction, var_name = content.split(" ")
            new_lines[line] = f"{instruction} {addresses[var_name[1:]]}"
    return new_lines, data


def map_ports(lines: list[str]) -> None:
    for line, content in enumerate(lines):
        contents = content.split(" ")
        if len(contents) == 2:
            if contents[1] in PORTS:
                lines[line] = content.replace(contents[1], PORTS[contents[1]])


def pprint(lines):
    for line, content in enumerate(lines):
        print(f"{line}\t", content)


def compile_rfl(filename: str, debug=False) -> list[int]:
    program = []
    with open(filename, "r") as p:
        lines = p.readlines()
        lines = remove_comments(lines)
        strip(lines)
        lines = remove_blank_lines(lines)
        build_utf8_strings(lines)
        lines, data = build_vars(lines)
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
    data_address = len(program)
    program.extend(data)
    return program, len(program), data_address


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
    program, program_size, data_address = compile_rfl(sys.argv[1], False)
    # print(disassembly(program))
    output = "{" + ",".join([str(inst) for inst in program]) + "}"
    print(output, program_size, data_address)
