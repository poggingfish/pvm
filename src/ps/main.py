import ply.lex as lex
import ply.yacc as yacc
import lib.codegen as codegen
from lib.types import *
from itertools import chain
reserved = {
    'fn': 'FUNCTION',
    'print': 'PRINT',
    "if": "IF"
}
tokens = [
    "IDENT",
    "PLUS",
    "INT",
    "LPAREN",
    "RPAREN",
    "STRING",
    "NEWLINE",
    "SEMI",
    "LCURL",
    "RCURL",
    "WHITESPACE",
    "EQUAL"
] + list(reserved.values())
t_ignore_WHITESPACE = r'\s+'
t_STRING = r'("[^"]*")|(\'[^\']*\')'
t_PLUS = r'\+'
t_LPAREN = r'\('
t_RPAREN = r'\)'
t_SEMI = r';'
t_LCURL = '{'
t_RCURL = '}'
t_EQUAL = '=='
def t_newline(t):
    r'\n'
    t.lexer.lineno += 1
    t.lexer.linepos = 0
    pass
def t_IDENT(t):
    r'[\$_a-zA-Z]\w*'

    t.type = reserved.get(t.value, t.type)
    return t
def t_INT(t):
    r'\d+'
    t.value = int(t.value)
    return t
def t_error(t):
    print("Illegal characters!" + t.value)
    t.lexer.skip(1)
lexer = lex.lex()
def p_program(p):
    '''
        program : fn
                    | program fn
    '''
def p_fn(p):
    '''
        fn : FUNCTION identifier LCURL statement_list RCURL
    '''
    if len(p) == 6:
        codegen.NewFunc(p[2])
        codegen.codegenStatementList(p[4])
        codegen.EndFunc(p[2])
def p_if(p):
    '''
        if : IF LPAREN INT EQUAL INT RPAREN LCURL statement_list RCURL 
    '''
    p[0] = IfStatement(p[3],p[5],p[8])
def p_statement_list(p):
    '''
        statement_list : statement
                    | statement_list statement
    '''
    if len(p) == 2:
        p[0] = p[1]
    else:
        p[1].append(p[2])
        p[0] = p[1]
        p[0] = list(chain(p[1]))
def p_statement(p):
    '''
        statement : PRINT_ST
            | fcall
            | if
    '''
    p[0] = [p[1]]
def p_print_statement(p):
    '''
        PRINT_ST : PRINT expr SEMI
    '''
    if type(p[2]) == int:
        p[0] = PrintInteger(p[2])
    if type(p[2]) == str:
        p[0] = PrintStr(p[2])
def p_expr(p):
    '''
        expr : LPAREN STRING RPAREN
            | LPAREN INT RPAREN
            | LPAREN RPAREN
    '''
    p[0] = p[2]
def p_call(p):
    '''
        fcall : identifier expr SEMI
    '''
    p[0] = CallFunction(p[1])
def p_identifier(p):
    '''
        identifier : IDENT
    '''
    p[0] = p[1]
def p_error(p):
    if p is not None:
        print(p)
        print("Uhoh. Error at line {}".format(p.lineno))
        exit(1)
    else:
        print("Unexpected end of input")
        exit(1)
parser = yacc.yacc()
parser.parse(open("code.ps").read())