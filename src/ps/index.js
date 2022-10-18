const moo = require('moo')

let lexer = moo.compile({
    lparen: '(',
    rparen: ')',
    semi: { match: /;/, lineBreaks: true },
})

lexer.reset("();")
for (let i of lexer){
    console.log(i);
}