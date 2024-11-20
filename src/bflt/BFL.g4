grammar BFL;

prog: stat+
    | EOF
    ;

stat: blk=(IN|BF|OUT) '{' ID (',' ID)* '};' # block
    | ID '=' expr ';'                       # assign
    ;

expr: expr op=(AND|OR|XOR) expr # operation
    | NOT? '(' expr ')'         # parens
    | NOT? ID                   # id
    ;

IN: 'IN';
BF: 'BF';
OUT: 'OUT';

AND: '&&';
OR: '||';
XOR: '^';
NOT: '!';

ID: [_a-zA-Z]+[_a-zA-Z0-9]*;
WS: [ \t\n]+ -> skip;
