ifptr = 1240
def unlevel(obj):
    while isinstance(obj, list) and len(obj) == 1:
        obj = obj[0]
    if isinstance(obj, list):
        return [unlevel(item) for item in obj]
    else:
        return obj
out = open("out.pvm","w")
line_number = 0
def write(text):
    global line_number
    global out
    line_number += 1
    out.write(text)
write("MOVA 255\n")
write("PUSHA\n")
write("JMP\n")
funcs = {}
fnptr = 0
def PrintInteger(val):
    global out
    write("MOVA {}\n".format(val))
    write("PUSHA\n")
    write("SYSCALL 0\n")
    write("MOVA 10\n")
    write("PUSHA\n")
    write("SYSCALL 1\n")
def NewFunc(Name):
    global fnptr
    if Name == "Main":
        write("lbl 255\n")
        funcs.update({Name:255})
    else:
        write("lbl {}\n".format(fnptr))
        funcs.update({Name:fnptr})
        fnptr+=1
def CallFunc(name):
    global fnptr
    global line_number
    write("MOVA {}\n".format(funcs[name]))
    write("PUSHA\n")
    write("JMP\n")
def EndFunc(name):
    global callstack
    if name != "Main":
        write("RET\n")
def If(IfArr):
    global ifptr
    write("MOVA {}\n".format(ifptr+2))
    write("PUSHA\n")
    write("JMP\n")
    write("lbl {}\n".format(ifptr))
    codegenStatementList(IfArr[2])
    write("RET\n")
    write("lbl {}\n".format(ifptr+1))
    write("RET\n")
    write("lbl {}\n".format(ifptr+2))
    write("MOVA {}\n".format(IfArr[0]))
    write("PUSHA\n")
    write("MOVA {}\n".format(IfArr[1]))
    write("PUSHA\n")
    write("MOVA {}\n".format(ifptr))
    write("PUSHA\n")
    write("MOVA {}\n".format(ifptr+1))
    write("PUSHA\n")
    write("JE\n")
    ifptr+=3
def PrintStr(val):
    for i in val[1:-1]:
        write("MOVA {}\n".format(ord(i)))
        write("PUSHA\n")
        write("SYSCALL 1\n")
    write("MOVA 10\n")
    write("PUSHA\n")
    write("SYSCALL 1\n")
def CodeGen(statement):
    for x in statement:
        if x == "PrintInt":
            PrintInteger(statement[x])
        if x == "Function":
            CallFunc(statement[x])
        if x == "If":
            If(statement[x])
        if x == "PrintStr":
            PrintStr(statement[x])
def codegenStatementList(stlist):
    stlist = unlevel(stlist)
    if type(stlist) == list:
        for i in stlist:
            CodeGen(i)
    else:
        CodeGen(stlist)