from abc import ABC, abstractmethod
import sys
import struct


class Instruction(ABC):
    @property
    @abstractmethod
    def name(self): ...

    @property
    @abstractmethod
    def opcode(self): ...

    @property
    @abstractmethod
    def size(self): ...

    @abstractmethod
    def encode(self, operand): ...

    def get_size(self):
        return self.size


def string_to_uint8_list(string_int, list_size):
    int_val = int(string_int)
    num_bytes = list_size * 8
    # Masking to ensure only the required number of bits are retained
    int_val &= (1 << num_bytes) - 1
    # Generate the list of unsigned 8-bit integers
    uint8_list = [(int_val >> (i * 8)) & 0xFF for i in range(list_size)]
    # uint8_list.reverse()
    return uint8_list


def string_to_int8_list(string_int, list_size):
    int_val = int(string_int)
    num_bytes = list_size * 8
    # Masking to ensure only the required number of bits are retained
    int_val &= (1 << num_bytes) - 1
    # Create the list of signed 8-bit integers
    int8_list = []
    for i in range(list_size):
        byte_val = (int_val >> (i * 8)) & 0xFF
        # Convert to signed 8-bit integer if necessary
        if byte_val & 0x80:  # If the most significant bit is set
            byte_val -= 0x100
        int8_list.append(byte_val)
    # int8_list.reverse()
    return int8_list


def string_to_float_ieee754(float_str):
    # Convert string to float
    float_val = float(float_str)
    # Pack the float value into a 4-byte string according to IEEE754 Single precision
    ieee754_bytes = struct.pack("f", float_val)
    # Convert the bytes to a hexadecimal string
    ieee754_int = [int(byte) for byte in ieee754_bytes]
    # ieee754_int.reverse()
    return ieee754_int


class NoOperandInstruction(Instruction):
    name = "NoOperandInstruction"
    opcode = -1
    size = 1

    def encode(self, operand):
        return [self.opcode]


class HaltInstruction(NoOperandInstruction):
    name = "HALT"
    opcode = 0


class OperandU16Instruction(Instruction):
    name = "OperandU16Instruction"
    opcode = -1
    size = 3

    def encode(self, operand):
        encoded = [self.opcode]
        encoded.extend(string_to_uint8_list(operand, 2))
        return encoded


class OperandI16Instruction(Instruction):
    name = "OperandI16Instruction"
    opcode = -1
    size = 3

    def encode(self, operand):
        encoded = [self.opcode]
        encoded.extend(string_to_int8_list(operand, 2))
        return encoded


class OperandF32Instruction(Instruction):
    name = "OperandF32Instruction"
    opcode = -1
    size = 5

    def encode(self, operand):
        encoded = [self.opcode]
        encoded.extend(string_to_float_ieee754(operand))
        return encoded


class PushLiteralU16Instruction(OperandU16Instruction):
    name = "PUSHL_U16"
    opcode = 28


class PopU16Instruction(NoOperandInstruction):
    name = "POP_U16"
    opcode = 29


class TopU16Instruction(NoOperandInstruction):
    name = "TOP_U16"
    opcode = 30


class AddU16Instruction(NoOperandInstruction):
    name = "ADD_U16"
    opcode = 31


class PushLiteralI16Instruction(OperandI16Instruction):
    name = "PUSHL_I16"
    opcode = 32


class PopI16Instruction(NoOperandInstruction):
    name = "POP_I16"
    opcode = 33


class TopI16Instruction(NoOperandInstruction):
    name = "TOP_I16"
    opcode = 34


class AddI16Instruction(NoOperandInstruction):
    name = "ADD_I16"
    opcode = 35


class SubtractI16Instruction(NoOperandInstruction):
    name = "SUB_I16"
    opcode = 36


class MultiplyI16Instruction(NoOperandInstruction):
    name = "MUL_I16"
    opcode = 37


class DivideI16Instruction(NoOperandInstruction):
    name = "DIV_I16"
    opcode = 38


class ReadI16Instruction(OperandI16Instruction):
    name = "READ_I16"
    opcode = 46


class WriteI16Instruction(OperandI16Instruction):
    name = "WRITE_I16"
    opcode = 47


class Allocate16Instruction(OperandU16Instruction):
    name = "ALLOC_I16"
    opcode = 48


class FreeI16Instruction(OperandI16Instruction):
    name = "FREE_I16"
    opcode = 49


class PushLiteralF32Instruction(OperandF32Instruction):
    name = "PUSHL_F32"
    opcode = 39


class PopF32Instruction(NoOperandInstruction):
    name = "POP_F32"
    opcode = 40


class TopF32Instruction(NoOperandInstruction):
    name = "TOP_F32"
    opcode = 41


class AddF32Instruction(NoOperandInstruction):
    name = "ADD_F32"
    opcode = 42


class SubtractF32Instruction(NoOperandInstruction):
    name = "SUB_F32"
    opcode = 43


class MultiplyF32Instruction(NoOperandInstruction):
    name = "MUL_F32"
    opcode = 44


class DivideF32Instruction(NoOperandInstruction):
    name = "DIV_F32"
    opcode = 45


class DelayInstruction(OperandU16Instruction):
    name = "DELAY"
    opcode = 6


class JumpInstruction(OperandU16Instruction):
    name = "JUMP"
    opcode = 7


class CallInstruction(OperandU16Instruction):
    name = "CALL"
    opcode = 24


class ReturnInstruction(NoOperandInstruction):
    name = "RETURN"
    opcode = 25


class SysCallInstruction(OperandU16Instruction):
    name = "SYSCALL"
    opcode = 26


INSTRUCTIONS = {
    "HALT": HaltInstruction(),
    "PUSHL_U16": PushLiteralU16Instruction(),
    "POP_U16": PopU16Instruction(),
    "TOP_U16": TopU16Instruction(),
    "ADD_U16": AddU16Instruction(),
    "PUSHL_I16": PushLiteralI16Instruction(),
    "POP_I16": PopI16Instruction(),
    "TOP_I16": TopI16Instruction(),
    "ADD_I16": AddI16Instruction(),
    "SUB_I16": SubtractI16Instruction(),
    "MUL_I16": MultiplyI16Instruction(),
    "DIV_I16": DivideI16Instruction(),
    "LOAD_I16": ReadI16Instruction(),
    "STORE_I16": WriteI16Instruction(),
    "ALLOC_I16": Allocate16Instruction(),
    "FREE_I16": FreeI16Instruction(),
    "PUSHL_F32": PushLiteralF32Instruction(),
    "POP_F32": PopF32Instruction(),
    "TOP_F32": TopF32Instruction(),
    "ADD_F32": AddF32Instruction(),
    "SUB_F32": SubtractF32Instruction(),
    "MUL_F32": MultiplyF32Instruction(),
    "DIV_F32": DivideF32Instruction(),
    "DELAY": DelayInstruction(),
    "JUMP": JumpInstruction(),
    "CALL": CallInstruction(),
    "RETURN": ReturnInstruction(),
    "SYSCALL": SysCallInstruction(),
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
        if " ." in content:
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
            # pass

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


def encode_line(line: str) -> list[int]:
    try:
        inst_name, operand = line.split(" ")
    except ValueError:
        inst_name = line
        operand = None
    # print(f"decoding {inst_name}")
    instruction = INSTRUCTIONS[inst_name]
    return instruction.encode(operand)


def compile_rfl(filename: str, debug: bool = True) -> list[int]:
    program = []
    with open(filename, "r") as p:
        lines = p.readlines()
        lines = remove_comments(lines)
        strip(lines)
        lines = remove_blank_lines(lines)
        build_utf8_strings(lines)
        # lines, data = build_vars(lines)
        lines = build_jumps(lines)
        map_ports(lines)
        # print(lines)
        for line in lines:
            program.extend(encode_line(line))
    data_address = len(program)
    # program.extend(data)
    return program, len(program), data_address


def get_instruction(opcode):
    for inst in INSTRUCTIONS:
        if opcode == inst.opcode:
            return inst.name


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
