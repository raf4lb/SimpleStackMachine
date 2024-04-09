import lark
import sys


rfl_grammar = """
    start: code_block
    
    code_block: statement+
    
    statement: if_statement
        | loop_statement
        | function_call 
        | variable_declaration 
        | variable_assignment 
        | variable_declaration_with_assignment 
        | function_declaration 
        | class_declaration 
        | class_instantiation
    
    class_declaration: "class" identifier "{" class_body "}"    
    class_body: (method_declaration 
        | attribute_declaration 
        | constructor_declaration)*
    constructor_declaration: "." "constructor" "(" parameters ")" "{" constructor_body "}"
    constructor_body: (variable_declaration_with_assignment 
        | variable_declaration 
        | variable_assignment 
        | attribute_assignment
        | expression)*
    attribute_identifier: "." identifier
    attribute_declaration:  attribute_identifier "<" type ">"
    attribute_assignment: attribute_identifier "=" expression
    method_declaration: "." identifier "(" parameters? ")" "{" method_body "}" return_type
    method_body: (function_call 
        | variable_declaration_with_assignment 
        | variable_declaration 
        | variable_assignment 
        | attribute_assignment 
        | return_statement)*

    class_instantiation: class_name identifier "(" arguments ")"
    class_name: identifier
    arguments: [argument ("," argument)*]
    argument: expression

    function_declaration: identifier "(" parameters ")" "{" function_body "}" return_type
    parameters: [parameter ("," parameter)*]
    parameter: identifier "<" type ">"
    function_body: (code_block |return_statement?)*
    function_call: identifier "(" arguments ")"

    variable_declaration_with_assignment: variable_declaration "=" expression
    variable_declaration: identifier "<" type ">"
    variable_assignment: identifier "=" expression

    if_statement: "?" "(" condition ")" "{" code_block "}" else_block?
    else_block: "!" "{" code_block "}"

    loop_statement: "loop" "(" loop_control ")" "{" code_block "}"

    loop_control: identifier "=" expression ":" expression ":" expression

    condition: expression

    ?expression: expression "+" multiply -> add
        | expression "-" multiply -> sub
        | multiply
    ?multiply: multiply "*" atom -> mul
        | multiply "/" atom -> div
        | atom
    ?atom: "-" atom -> neg 
        | function_call 
        | attribute_identifier 
        | identifier 
        | number 
        | string 
        | "(" expression ")"
    
    return_statement: "ret" expression

    return_type: type

    type: "void"  -> void
        | "int"   -> int
        | "float" -> float
        | "string"  -> string

    identifier: CNAME
    string : ESCAPED_STRING
    number: NUMBER

    %import common.CNAME
    %import common.NUMBER
    %import common.ESCAPED_STRING
    %import common.WS
    %ignore WS
"""

code_class = """
class Person{
    .name <string>
    .age <int>
    .constructor(name <string>, age <int>){
        .name = name
        .age = age
    }
    .get_name(){
        ret .name
    } string

    .set_name(name <string>){
        .name = name
    } void

    .increase_age(n <int>){
        .age = .age + 1
    } void

}
test(x <int>, y <float>){
    z <int>
    z = 10
    ret x + y + z
} int
age <int> = 10
name <string> = "Rafael"
Person rafael(name, age)
"""

from rfl_transformer import RFLTransformer


if __name__ == "__main__":

    with open(sys.argv[1], "r") as code_file:
        parser = lark.Lark(rfl_grammar, start="start")
        code = code_file.read()
        tree = parser.parse(code)
        print(tree.pretty())
        RFLTransformer().transform(tree)
