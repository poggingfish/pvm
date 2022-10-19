out = open("out.pvm","w")
funcs = {}
fnptr = 0
def PrintInteger(val):
    global out
    out.write("MOVA {}\n".format(val))
    out.write("PUSHA\n")
    out.write("SYSCALL 0\n")
    out.write("MOVA 10\n")
    out.write("PUSHA\n")
    out.write("SYSCALL 1\n")
def NewFunc():
    global fnptr
    out.write("lbl {}\n".format(fnptr))
    fnptr+=1
def codegenStatementList(stlist):
    for i in stlist:
        for x in i:
            if x == "PrintInt":
                PrintInteger(i[x])