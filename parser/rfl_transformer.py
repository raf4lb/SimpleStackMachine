from lark import Transformer, Tree, Token, v_args


class RFLTransformer(Transformer):

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.if_count = 0

    def number(self, n):
        (n,) = n
        return n

    def identifier(self, n):
        (n,) = n
        return n

    def type(self, items):
        return str(items[0])

    def variable_declaration(self, items):
        variable_name, variable_type = items  # still not using variable type
        code = f"VAR {variable_name}\n"
        # print(code)
        return code

    def variable_assignment(self, items):
        # print(items)
        variable_name, value = items
        code = f"POPA ${variable_name}\n"
        if value and isinstance(value, str):
            if value.isdigit():
                instruction = "PSHL"
                operand = value
                code = f"{instruction} {operand}\n{code}"
            elif value.isalnum():
                instruction = "PSH"
                operand = f"${value}"
                code = f"{instruction} {operand}\n{code}"
            else:
                code = f"{value}\n{code}"
        # print(code)
        return code

    def parameter(self, param):
        variable_name, variable_type = param  # still not using variable type
        code = f"VAR {variable_name}\nPOPA ${variable_name}\n"
        # print(code)
        return code

    def parameters(self, params):
        code = ""
        if any(params):
            code = "".join([p for p in params[::-1]])
            # print(code)
        return code

    @staticmethod
    def _operation(items):
        code = ""
        for i in items:
            if isinstance(i, Token):
                if i.type == "CNAME":
                    code += f"PSH ${i}\n"
                    # print(code)
                elif i.type == "NUMBER":
                    code += f"PSHL {i}\n"
                    # print(code)
            else:
                code += i
        return code

    def add(self, items):
        code = self._operation(items)
        code += "ADD\n"
        # print(code)
        return code

    def sub(self, items):
        code = self._operation(items)
        code += "SUB\n"
        # print(code)
        return code

    def mul(self, items):
        code = self._operation(items)
        code += "MUL\n"
        # print(code)
        return code

    def div(self, items):
        code = self._operation(items)
        code += "DIV\n"
        # print(code)
        return code

    def neg(self, n):
        (n,) = n
        code = ""
        if isinstance(n, Token):
            if n.type == "CNAME":
                code += f"PSH ${n}\nNEG\n"
            elif n.type == "NUMBER":
                code += f"PSHL {n}\nNEG\n"
        else:
            code += f"{n}NEG\n"
        # print(code)
        return code

    def return_statement(self, ret):
        code = "".join(str(c) for c in ret)
        return code

    def function_body(self, body):
        code = "".join(str(c) for c in body) + "\nRET\n"
        return code

    def argument(self, arg):
        (arg,) = arg
        if isinstance(arg, Token):
            if arg.type == "CNAME":
                code = f"PSH ${arg}\n"
            elif arg.type == "NUMBER":
                code = f"PSHL {arg}\n"
        else:
            code = arg
        # print(code)
        return code

    def arguments(self, args):
        code = ""
        if any(args):
            code = "".join([p for p in args])
            # print(code)
        return code

    def function_call(self, items):
        function_name, args = items
        call = f"CALL .{function_name}\n"
        code = args + call
        return code

    def function_declaration(self, items):
        function_identifier = items[0]
        function_parameters = items[1]
        function_body = items[2]

        code = f".{function_identifier}\n{function_parameters}\n{function_body}\n"
        # print(code)
        return code

    def statement(self, items):
        code = "".join(str(i) for i in items)
        return code

    def code_block(self, items):
        code = "".join(str(i) for i in items)
        return code

    def if_statement(self, items):
        condition = items[0].children[0]
        if isinstance(condition, str):
            if condition.isdigit():
                block_condition = f"PSHL {condition}"
            elif condition.isalnum():
                block_condition = f"PSH ${condition}"
            else:
                block_condition = condition
        else:
            raise Exception("Erro in condition")
        block_true = items[1]
        if len(items) > 2:  # there is else block
            block_false = items[2].children[0]
            code = f"""{block_condition}
                PSHL 0
                CPG
                PJIF .condition_false_{self.if_count}
                JMP .condition_true_{self.if_count}
            .condition_true_{self.if_count}
                {block_true}
                JMP .out_{self.if_count}
            .condition_false_{self.if_count}
                {block_false}
            .out_{self.if_count}
            """
        else:
            code = f"""{block_condition}
                PSHL 0
                CPG
                PJIF .out_{self.if_count}
                JMP .condition_true_{self.if_count}
            .condition_true_{self.if_count}
                {block_true}
            .out_{self.if_count}
            """
        self.if_count += 1
        return code

    # def condition(self, items):
    #     print(items)

    def start(self, items):
        code = "CALL .main\nHLT\n"
        code = code + "".join(items)
        with open("rfl.asm", "w") as asm:
            asm.writelines(code)
        return items
