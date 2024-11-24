grammar BFL;

prog: stat+
    | EOF
    ;

stat: blk=(INS|BFS|OUT) '{' ID (',' ID)* '};' # block
    | ID '=' expr ';'                       # assign
    ;

expr: expr AND expr     # andOp
    | expr OR expr      # orOp
    | expr XOR expr     # xorOp
    | NOT? '(' expr ')' # parens
    | NOT? ID           # id
    ;

INS: 'INS';
BFS: 'BFS';
OUT: 'OUT';

AND: '&&';
OR: '||';
XOR: '^';
NOT: '!';

ID: [_a-zA-Z]+[_a-zA-Z0-9]*;
WS: [ \t\n]+ -> skip;
