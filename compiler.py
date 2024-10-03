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
    "ADDF": 27,
    "PUSHL_U16": 28,
    "POP_U16": 29,
    "TOP_U16": 30,
}

LONG_INSTRUCTIONS = {
    "PSHL",
    "PSH",
    "POPA",
    "DLY",
    "JMP",
    "PJIF",
    "CALL",
    "TOP",
    "POP",
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
        content_splitted = content.split(" ")
        if content_splitted[0] in ["TOP", "POP"]:
            address += 2
        elif content_splitted[0] == "PSHL":
            dtype = int(content_splitted[1])
            if dtype in [1, 5]:  # 2 bytes
                address += 4
            elif dtype in [2, 6, 8]:  # 4 bytes
                address += 6
        elif len(content_splitted) == 2:
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
    return [
        line
        for line in lines
        if not line.startswith("VAR") and not line.startswith("DATA")
    ]


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


def compile_rfl(filename: str, debug: bool = True) -> list[int]:
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
                if instruction[0] in ["TOP", "POP"]:
                    dline += 2
                elif len(instruction) == 2:
                    dline += 3
                elif len(instruction) == 3:
                    dline += 4
                else:
                    dline += 1
            opcode = INSTRUCTIONS[instruction[0]]
            program.append(opcode)
            if len(instruction) > 1:
                if opcode == INSTRUCTIONS["PSHL"]:
                    data_type = int(instruction[1])
                    program.append(data_type)
                    value = bin(int(instruction[2]))[2:]
                    operand = []
                    if data_type in [1, 5]:  # 16 bit size
                        value = value.zfill(16)
                        operand = [
                            value[0:8],
                            value[8:16],
                        ]

                    elif data_type in [2, 6, 8]:  # 32 bit size
                        value = value.zfill(32)
                        operand = [
                            value[0:8],
                            value[8:16],
                            value[16:24],
                            value[24:32],
                        ]
                    else:
                        raise Exception(f"Unknown data type: {data_type}")

                    for o in operand:
                        program.append(int(o, 2))

                elif opcode in [INSTRUCTIONS["TOP"], INSTRUCTIONS["POP"]]:
                    data_type = instruction[1]
                    program.append(int(data_type))
                else:
                    operand = bin(int(instruction[1]))[2:]
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
            if inst == "PSHL":
                dtype = code[i + 1]
                if dtype in [1, 5]:  # 2 bytes
                    value = code[i + 2] << 8 | code[i + 3]
                    disassembled.append(f"{i}\t {inst} {dtype} {value}")
                    i += 4
                elif dtype in [2, 6, 8]:  # 4 bytes
                    value = (
                        code[i + 2] << 24
                        | code[i + 3] << 16
                        | code[i + 4] << 8
                        | code[i + 5]
                    )
                    disassembled.append(f"{i}\t {inst} {dtype} {value}")
                    i += 6
            elif inst in ["TOP", "POP"]:
                dtype = code[i + 1]
                disassembled.append(f"{i}\t {inst} {dtype}")
                i += 2
            elif inst in LONG_INSTRUCTIONS:
                param = code[i + 1] << 8 | code[i + 2]
                disassembled.append(f"{i}\t {inst} {param}")
                i += 3
            else:
                disassembled.append(f"{i}\t {inst}")
                i += 1
        except:
            print(f"Error in line {i}: {code[i]}({inst})")
            break
    return disassembled


if __name__ == "__main__":
    program, program_size, data_address = compile_rfl(sys.argv[1], False)
    # for line in disassembly(program):
    #     print(line)
    output = "{" + ",".join([str(inst) for inst in program]) + "}"
    print(output, program_size, data_address)
