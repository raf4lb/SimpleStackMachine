from abc import ABC, abstractmethod
import os
import sys
import inspect
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

    def encode(self, operands: list[str]):
        return [self.opcode]


class U16OperandInstruction(Instruction):
    name = "U16OperandInstruction"
    opcode = -1
    size = 3

    def encode(self, operands: list[str]):
        encoded = [self.opcode]
        encoded.extend(string_to_uint8_list(operands[0], 2))
        return encoded


class TwoU16OperandInstruction(Instruction):
    name = "TwoU16OperandInstruction"
    opcode = -1
    size = 5

    def encode(self, operands: list[str]):
        encoded = [self.opcode]
        for operand in operands:
            encoded.extend(string_to_uint8_list(operand, 2))
        return encoded


class I16OperandInstruction(Instruction):
    name = "I16OperandInstruction"
    opcode = -1
    size = 3

    def encode(self, operands: list[str]):
        encoded = [self.opcode]
        encoded.extend(string_to_int8_list(operands[0], 2))
        return encoded


class F32OperandInstruction(Instruction):
    name = "F32OperandInstruction"
    opcode = -1
    size = 5

    def encode(self, operands: list[str]):
        encoded = [self.opcode]
        encoded.extend(string_to_float_ieee754(operands[0]))
        return encoded


class HaltInstruction(NoOperandInstruction):
    name = "HALT"
    opcode = 0


class DelayInstruction(U16OperandInstruction):
    name = "DELAY"
    opcode = 1


class JumpInstruction(U16OperandInstruction):
    name = "JUMP"
    opcode = 2


class PopJumpIfFalseInstruction(U16OperandInstruction):
    name = "POP_JUMP_IF_FALSE"
    opcode = 3


class CompareEqualInstruction(NoOperandInstruction):
    name = "COMPARE_EQ"
    opcode = 4


class CompareLessInstruction(NoOperandInstruction):
    name = "COMPARE_LT"
    opcode = 5


class CompareGreaterInstruction(NoOperandInstruction):
    name = "COMPARE_GT"
    opcode = 6


class CompareLessEqualInstruction(NoOperandInstruction):
    name = "COMPARE_LE"
    opcode = 7


class CompareGreaterEqualInstruction(NoOperandInstruction):
    name = "COMPARE_GE"
    opcode = 8


class CallInstruction(U16OperandInstruction):
    name = "CALL"
    opcode = 9


class ReturnInstruction(NoOperandInstruction):
    name = "RETURN"
    opcode = 10


class SysCallInstruction(U16OperandInstruction):
    name = "SYSCALL"
    opcode = 11


class AsyncCallInstruction(U16OperandInstruction):
    name = "ASYNC_CALL"
    opcode = 12


class AsyncReturnInstruction(NoOperandInstruction):
    name = "ASYNC_RETURN"
    opcode = 13


class PushLiteralU16Instruction(U16OperandInstruction):
    name = "PUSHL_U16"
    opcode = 14


class PopU16Instruction(NoOperandInstruction):
    name = "POP_U16"
    opcode = 15


class TopU16Instruction(NoOperandInstruction):
    name = "TOP_U16"
    opcode = 16


class AddU16Instruction(NoOperandInstruction):
    name = "ADD_U16"
    opcode = 17


class SubtractInstructionInstruction(NoOperandInstruction):
    name = "SUB_U16"
    opcode = 18


class MultiplyU16Instruction(NoOperandInstruction):
    name = "MUL_U16"
    opcode = 19


class DivideU16Instruction(NoOperandInstruction):
    name = "DIV_U16"
    opcode = 20


class PopAddressU16Instruction(U16OperandInstruction):
    name = "POPA_U16"
    opcode = 21


class PushU16Instruction(U16OperandInstruction):
    name = "PUSH_U16"
    opcode = 22


class AllocateLocalStackInstruction(U16OperandInstruction):
    name = "ALLOC_LOCAL"
    opcode = 23


class PushLocalStackInstruction(NoOperandInstruction):
    name = "PUSH_LOCAL"
    opcode = 24


class PopLocalStackInstruction(NoOperandInstruction):
    name = "POP_LOCAL"
    opcode = 25


class PushLiteralI16Instruction(I16OperandInstruction):
    name = "PUSHL_I16"
    opcode = 26


class PopI16Instruction(NoOperandInstruction):
    name = "POP_I16"
    opcode = 27


class TopI16Instruction(NoOperandInstruction):
    name = "TOP_I16"
    opcode = 28


class AddI16Instruction(NoOperandInstruction):
    name = "ADD_I16"
    opcode = 29


class SubtractI16Instruction(NoOperandInstruction):
    name = "SUB_I16"
    opcode = 30


class MultiplyI16Instruction(NoOperandInstruction):
    name = "MUL_I16"
    opcode = 31


class DivideI16Instruction(NoOperandInstruction):
    name = "DIV_I16"
    opcode = 32


class ReadI16Instruction(I16OperandInstruction):
    name = "READ_I16"
    opcode = 33


class WriteI16Instruction(I16OperandInstruction):
    name = "WRITE_I16"
    opcode = 34


class AllocateI16Instruction(U16OperandInstruction):
    name = "ALLOC_I16"
    opcode = 35


class FreeI16Instruction(I16OperandInstruction):
    name = "FREE_I16"
    opcode = 36


class PushLiteralF32Instruction(F32OperandInstruction):
    name = "PUSHL_F32"
    opcode = 37


class PopF32Instruction(NoOperandInstruction):
    name = "POP_F32"
    opcode = 38


class TopF32Instruction(NoOperandInstruction):
    name = "TOP_F32"
    opcode = 39


class AddF32Instruction(NoOperandInstruction):
    name = "ADD_F32"
    opcode = 40


class SubtractF32Instruction(NoOperandInstruction):
    name = "SUB_F32"
    opcode = 41


class MultiplyF32Instruction(NoOperandInstruction):
    name = "MUL_F32"
    opcode = 42


class DivideF32Instruction(NoOperandInstruction):
    name = "DIV_F32"
    opcode = 43


class PushMillisInstruction(NoOperandInstruction):
    name = "PUSH_MILLIS"
    opcode = 44


class LeftShiftU16Instruction(NoOperandInstruction):
    name = "LSHIFT_U16"
    opcode = 45


class RightShiftU16Instruction(NoOperandInstruction):
    name = "RSHIFT_U16"
    opcode = 46


class OrU16Instruction(NoOperandInstruction):
    name = "OR_U16"
    opcode = 47


class XorU16Instruction(NoOperandInstruction):
    name = "XOR_U16"
    opcode = 48


class AndU16Instruction(NoOperandInstruction):
    name = "AND_U16"
    opcode = 49


class NotU16Instruction(NoOperandInstruction):
    name = "NOT_U16"
    opcode = 50


class PushLocalReferenceInstruction(NoOperandInstruction):
    name = "PUSH_LOCAL_REF"
    opcode = 51


class PushConstantReferenceInstruction(NoOperandInstruction):
    name = "PUSH_CONST_REF"
    opcode = 52


class PushConstantInstruction(NoOperandInstruction):
    name = "PUSH_CONST"
    opcode = 53


instruction_classes = [
    cls_obj
    for cls_name, cls_obj in inspect.getmembers(sys.modules[__name__])
    if inspect.isclass(cls_obj)
    and cls_name.lower().endswith("instruction")
    and len(cls_name) > 11
    and "operand" not in cls_name.lower()
]

INSTRUCTIONS = {cls.name: cls() for cls in instruction_classes}

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


def build_jumps(lines: list[str]) -> list[str]:
    new_lines = []
    addresses = {}
    address = 0
    for content in lines:
        if content.startswith(".") or content.startswith("fn ."):
            if content.startswith("."):
                addresses[content[1:]] = address
            else:
                addresses[content[4:]] = address
        else:
            try:
                instruction, label, *_ = content.split(" ")
            except ValueError:
                instruction = content
            size = INSTRUCTIONS[instruction].size
            address += size
            new_lines.append(content)

    for line, content in enumerate(new_lines):
        if " ." in content:
            instruction, label = content.split(" ")
            new_lines[line] = f"{instruction} {addresses[label[1:]]}"
    return new_lines


def remove_static_data_lines(lines: list[str]) -> list[str]:
    return [line for line in lines if not line.startswith("CONST")]


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


def build_const_data(lines: list[str]):
    addresses = PORTS.copy()
    data = []

    def allocate_str(value):
        data.extend(int(c) for c in value.split(","))  # store ascii value

    def allocate_U16(value):
        data.extend(string_to_uint8_list(value, 2))

    for line in lines:
        if line.startswith("CONST"):
            const_name, const_type, const_value = line.split(" ")[1:]
            addresses[const_name] = len(data) + len(PORTS)  # free index
            if const_type == "STRING":
                allocate_str(const_value)
            elif const_type == "U16":
                allocate_U16(const_value)

    new_lines = remove_static_data_lines(lines)
    for line, content in enumerate(new_lines):
        if "$" in content:
            instruction, const_name, *_ = content.split(" ")
            if const_name[1:] in addresses:
                new_lines[line] = f"{instruction} {addresses[const_name[1:]]}"
    return new_lines, data


def build_local_variables(lines: list[str]):
    addresses = {}
    default_offset = 2  # it's 2 because we're storing the task id at the beginning of the stack frame
    new_lines = []
    for line, content in enumerate(lines):
        if content.startswith("fn ."):
            offset = default_offset
            var_prefix = content[4:] + "_"

        if content.startswith("ALLOC_LOCAL"):
            var_size = lines[line-1].split(" ")[1]
            var_name = content.split(" ")[1]
            var_name = var_prefix + var_name[1:]
            if var_name in addresses:
                raise Exception(
                    f"Variable {var_name} already allocated at {addresses[var_name]}"
                )
            addresses[var_name] = offset
            offset += int(var_size)
        new_lines.append(content)

    for line, content in enumerate(new_lines):
        if content.startswith("fn ."):
            offset = default_offset
            var_prefix = content[4:] + "_"
        if "$" in content and "_LOCAL" in content:
            instruction, var_name = content.split(" ")
            var_name = var_prefix + var_name[1:]
            if var_name in addresses:
                new_lines[line] = f"{instruction} {addresses[var_name]}"
            else:
                print(addresses)
                raise Exception(f"{var_name} not found in addressess")
        elif "$" in content:
            instruction, var_name = content.split(" ")
            var_name = var_prefix + var_name[1:]
            if var_name in addresses:
                new_lines[line] = f"{instruction} {addresses[var_name]}"
            else:
                print(addresses)
                raise Exception(f"{var_name} not found in addressess")
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


def encode_line(i: int, line: str) -> list[int]:
    inst_name, *operand = line.split(" ")
    # print(f"decoding {inst_name}")
    instruction = INSTRUCTIONS[inst_name]
    if not instruction:
        print(f"Error at line {i}: unknow instruction {inst_name}")
    try:
        return instruction.encode(operand)
    except:
        print("Error at:")
        print(i, instruction)


def copy_code_from(source_file: str) -> list[str]:
    with open(source_file, "r") as p:
        return p.readlines()


def process_includes(
    lines: list[str], base_dir: str, included_files: set[str] = None
) -> list[str]:
    if included_files is None:
        included_files = set()

    processed_lines = []

    for line in lines:
        if line.startswith("#include"):
            _, source_file = line.split(" ")
            source_file = source_file.strip()
            full_path = f"{base_dir}/{source_file}"

            if full_path not in included_files:
                included_files.add(full_path)
                included_lines = copy_code_from(full_path)
                # Recursively process the included file
                processed_lines.extend(
                    process_includes(included_lines, base_dir, included_files)
                )
        else:
            processed_lines.append(line)

    return processed_lines


def include_main_call(lines: list[str]) -> list[str]:
    main_lines = ["CALL .main\n", "HALT"]
    main_lines.extend(lines)
    return main_lines


def print_lines(lines: list[str]) -> None:
    max_lines = len(lines)
    leadings = len(str(max_lines))
    for i, line in enumerate(lines):
        print(f"{str(i).zfill(leadings)}  {line}")


def print_code(code: list[str]) -> None:
    address = 0
    while address < len(code):
        if instruction := get_instruction(code[address]):
            operands = code[address+1:address+instruction.size]
            print(address, instruction.name, *operands)
            address += instruction.size
        else:
            print(f"invalid opcode: {code[address]}")
            break


def compile_rfl(filename: str, debug: bool = True) -> list[int]:
    program = []
    with open(filename, "r") as p:
        base_dir = os.path.dirname(p.name)
        lines = p.readlines()
        lines = process_includes(lines, base_dir)
        lines = include_main_call(lines)
        lines = remove_comments(lines)
        strip(lines)
        lines = remove_blank_lines(lines)
        build_utf8_strings(lines)
        lines, data = build_const_data(lines)
        lines = build_local_variables(lines)
        lines = build_jumps(lines)
        map_ports(lines)
        for i, line in enumerate(lines):
            program.extend(encode_line(i, line))
    if debug:
        print_code(program)
    data_address = len(program)
    program.extend(data)
    return program, len(program), data_address


def get_instruction(opcode):
    for inst in INSTRUCTIONS:
        if opcode == INSTRUCTIONS[inst].opcode:
            return INSTRUCTIONS[inst]


if __name__ == "__main__":
    try:
        filename = sys.argv[1]
    except IndexError:
        raise IndexError("Missing arg filename")
    try:
        debug = bool(sys.argv[2])
    except IndexError:
        debug = False

    program, program_size, data_address = compile_rfl(filename, debug)
    output = "{" + ",".join([str(inst) for inst in program]) + "}"
    print(output, program_size, data_address)
