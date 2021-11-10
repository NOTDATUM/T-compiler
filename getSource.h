#include <stdio.h>
#include <string.h>
#ifndef TBL
#define TBL
#endif
#define MAXNAME 31
#define MAXLINE 120   
#define MAXNUM  14
#define MAXCODE 200   
#define MAXMEM 2000   
#define MAXREG 20
#define MAXTABLE 100   
#define MAXLEVEL 5
#define MAXERROR 30
#define MINERROR 3
#define TAB   5   
#define INSERT_C  "#0000FF"
#define DELETE_C  "#FF0000"
#define TYPE_C  "#00FF00"
#define FIRSTADDR 2

typedef  enum  keys {
   Begin, End,   
   If, Then,
   While, Do,
   Ret, Func,
   Var, Const, Odd,
   Write, WriteLn,
   end_of_KeyWd,
   Plus, Minus,
   Mult, Div,   
   Lparen, Rparen,
   Equal, Lss, Gtr,
   NotEq, LssEq, GtrEq, 
   Comma, Period, Semicolon,
   Assign,
   end_of_KeySym,
   Id, Num, nul,
   end_of_Token,
   letter, digit, colon, others
} KeyId;

typedef  struct  token {
   KeyId kind;   
   union {
      char id[MAXNAME];
      int value;
   } u;
}Token;

typedef enum kindT {
   varId, funcId, parId, constId
}KindT;

typedef struct relAddr{   
   int level;
   int addr;
}RelAddr; 

typedef struct tableE {   
   KindT kind;      
   char name[MAXNAME];
   union {
      int value;
      struct {
         RelAddr raddr;
         int pars;
      }f;
      RelAddr raddr;
   }u;
}TabelE;

typedef enum codes{
   lit, opr, lod, sto, cal, ret, ict, jmp, jpc
}OpCode;

typedef enum ops{
   neg, add, sub, mul, div, odd, eq, ls, gr,
   neq, lseq, greq, wrt, wrl
}Operator;

typedef struct inst{
   OpCode  opCode;
   union{
      RelAddr addr;
      int value;
      Operator optr;
   }u;
}Inst;

void blockBegin(int firstAddr);
void blockEnd();
int bLevel();
int fPars();
int enterTfunc(char *id, int v);
int enterTvar(char *id);
int enterTpar(char *id);
int enterTconst(char *id, int v);
void endpar();
void changeV(int ti, int newVal);

int searchT(char *id, KindT k);
KindT kindT(int i);

RelAddr relAddr(int ti);
int val(int ti);
int pars(int ti);
int frameL();

static TabelE nameTable[MAXTABLE];
static int tIndex = 0;      
static int level = -1;   
static int index[MAXLEVEL];
static int addr[MAXLEVEL];
static int localAddr;
static int tfIndex;

static FILE *fpi;         
static FILE *fptex;      
static char line[MAXLINE];
static int lineIndex;      
static char ch;         

static Token cToken;      
static KindT idKind;      
static int spaces;      
static int CR;         
static int printed;      

static int errorNo = 0;   
static char nextChar();   
static int isKeySym(KeyId k);
static int isKeyWd(KeyId k);   
static void printSpaces();   
static void printcToken();

static Token token;
static void block(int pIndex);
static void constDecl();      
static void varDecl();         
static void funcDecl();   
static void statement();      
static void expression();   
static void term();         
static void factor();         
static void condition();         
static int isStBeginKey(Token t);   

Token nextToken();
Token checkGet(Token t, KeyId k);

int openSource(char fileName[]); 
void closeSource();   
void initSource();      
void finalSource();       
void errorType(char *m);   
void errorInsert(KeyId k);   
void errorMissingId();      
void errorMissingOp();   
void errorDelete();      
void errorMessage(char *m);   
void errorF(char *m);   
int errorN();

void setIdKind(KindT k);

int genCodeV(OpCode op, int v);   
int genCodeT(OpCode op, int ti);
int genCodeO(Operator p);
int genCodeR();   
void backPatch(int i);
int nextCode();
void listCode();
void execute();

struct keyWd {
   char *word;
   KeyId keyId;
};

static struct keyWd KeyWdT[] = {
   {"begin", Begin},
   {"end", End},
   {"if", If},
   {"then", Then},
   {"while", While},
   {"do", Do},
   {"return", Ret},
   {"function", Func},
   {"var", Var},
   {"const", Const},
   {"odd", Odd},
   {"write", Write},
   {"writeln",WriteLn},
   {"$dummy1",end_of_KeyWd},
   {"+", Plus},
   {"-", Minus},
   {"*", Mult},
   {"/", Div},
   {"(", Lparen},
   {")", Rparen},
   {"=", Equal},
   {"<", Lss},
   {">", Gtr},
   {"<>", NotEq},
   {"<=", LssEq},
   {">=", GtrEq},
   {",", Comma},
   {".", Period},
   {";", Semicolon},
   {":=", Assign},
   {"$dummy2",end_of_KeySym},
};

void errorNoCheck()      
{
   if (errorNo++ > MAXERROR){
      fprintf(fptex, "too many errors\n\\end{document}\n");
      printf("abort compilation\n");   
   }
}

void errorInsert(KeyId k)   
{
   if (k < end_of_KeyWd) 
      fprintf(fptex, "\\ \\insert{{\\bf %s}}", KeyWdT[k].word);
   else 
      fprintf(fptex, "\\ \\insert{$%s$}", KeyWdT[k].word);
   errorNoCheck();
}

void errorType(char *m)
{
   printSpaces();
   fprintf(fptex, "\\(\\stackrel{\\mbox{\\scriptsize %s}}{\\mbox{", m);
   printcToken();
   fprintf(fptex, "}}\\)");
   errorNoCheck();
}

void errorMissingId()      
{
   fprintf(fptex, "\\insert{Id}");
   errorNoCheck();
}

void errorMissingOp()   
{
   fprintf(fptex, "\\insert{$\\otimes$}");
   errorNoCheck();
}

void errorDelete()      
{
   int i=(int)cToken.kind;
   printSpaces();
   printed = 1;
   if (i < end_of_KeyWd)                   
      fprintf(fptex, "\\delete{{\\bf %s}}", KeyWdT[i].word);
   else if (i < end_of_KeySym)            
      fprintf(fptex, "\\delete{$%s$}", KeyWdT[i].word);
   else if (i==(int)Id)                     
      fprintf(fptex, "\\delete{%s}", cToken.u.id);
   else if (i==(int)Num)                  
      fprintf(fptex, "\\delete{%d}", cToken.u.value);
}

void errorMessage(char *m)
{
   fprintf(fptex, "$^{%s}$", m);
   errorNoCheck();
}

void errorF(char *m)      
{
   errorMessage(m);
   fprintf(fptex, "fatal errors\n\\end{document}\n");
   if (errorNo)
      printf("total %d errors\n", errorNo);
   printf("abort compilation\n");   
}

int errorN()      
{
   return errorNo;
}

int isKeyWd(KeyId k)
{
   return (k < end_of_KeyWd);
}

int isKeySym(KeyId k)
{
   if (k < end_of_KeyWd)
      return 0;
   return (k < end_of_KeySym);
}

static KeyId charClassT[256];
static void initCharClassT()
{
   int i;
   for (i=0; i<256; i++)
      charClassT[i] = others;
   for (i='0'; i<='9'; i++)
      charClassT[i] = digit;
   for (i='A'; i<='Z'; i++)
      charClassT[i] = letter;
   for (i='a'; i<='z'; i++)
      charClassT[i] = letter;
   charClassT['+'] = Plus; charClassT['-'] = Minus;
   charClassT['*'] = Mult; charClassT['/'] = Div;
   charClassT['('] = Lparen; charClassT[')'] = Rparen;
   charClassT['='] = Equal; charClassT['<'] = Lss;
   charClassT['>'] = Gtr; charClassT[','] = Comma;
   charClassT['.'] = Period; charClassT[';'] = Semicolon;
   charClassT[':'] = colon;
}

int openSource(char fileName[]) 
{
   char fileNameO[30];
   if ( (fpi = fopen(fileName,"r")) == NULL ) {
      printf("can't open %s\n", fileName);
      return 0;
   }
   strcpy(fileNameO, fileName);
   strcat(fileNameO,".tex");
   if ( (fptex = fopen(fileNameO,"w")) == NULL ) {
      printf("can't open %s\n", fileNameO);
      return 0;
   } 
   return 1;
}

void closeSource()
{
   fclose(fpi);
   fclose(fptex);
}

void initSource()
{
   lineIndex = -1;   
   ch = '\n';
   printed = 1;
   initCharClassT();
   fprintf(fptex,"\\documentstyle[12pt]{article}\n");
   fprintf(fptex,"\\begin{document}\n");
   fprintf(fptex,"\\fboxsep=0pt\n");
   fprintf(fptex,"\\def\\insert#1{$\\fbox{#1}$}\n");
   fprintf(fptex,"\\def\\delete#1{$\\fboxrule=.5mm\\fbox{#1}$}\n");
   fprintf(fptex,"\\rm\n");
}

void finalSource()
{
   if (cToken.kind==Period)
      printcToken();
   else
      errorInsert(Period);
   fprintf(fptex,"\n\\end{document}\n");
}
   
/*
void error(char *m)   
{
   if (lineIndex > 0)
      printf("%*s\n", lineIndex, "***^");
   else
      printf("^\n");
   printf("*** error *** %s\n", m);
   errorNo++;
   if (errorNo > MAXERROR){
      printf("too many errors\n");
      printf("abort compilation\n");   
      exit (1);
   }
}
*/

char nextChar()         
{
   char ch;
   if (lineIndex == -1){
      if (fgets(line, MAXLINE, fpi) != NULL){ 
         lineIndex = 0;
      } else {
         errorF("end of file\n");
      }
   }
   if ((ch = line[lineIndex++]) == '\n'){
      lineIndex = -1;
      return '\n';
   }
   return ch;
}

Token nextToken()
{
   int i = 0;
   int num;
   KeyId cc;
   Token temp;
   char ident[MAXNAME];
   printcToken();
   spaces = 0; CR = 0;
   while (1){
      if (ch == ' ')
         spaces++;
      else if   (ch == '\t')
         spaces+=TAB;
      else if (ch == '\n'){
         spaces = 0;  CR++;
      }
      else break;
      ch = nextChar();
   }
   switch (cc = charClassT[ch]) {
   case letter:
      do {
         if (i < MAXNAME)
            ident[i] = ch;
         i++; ch = nextChar();
      } while (  charClassT[ch] == letter
            || charClassT[ch] == digit );
      if (i >= MAXNAME){
         errorMessage("too long");
         i = MAXNAME - 1;
      }   
      ident[i] = '\0'; 
      for (i=0; i<end_of_KeyWd; i++)
         if (strcmp(ident, KeyWdT[i].word) == 0) {
            temp.kind = KeyWdT[i].keyId;
            cToken = temp; printed = 0;
            return temp;
         }
      temp.kind = Id;
      strcpy(temp.u.id, ident);
      break;
   case digit:
      num = 0;
      do {
         num = 10*num+(ch-'0');
         i++; ch = nextChar();
      } while (charClassT[ch] == digit);
            if (i>MAXNUM)
               errorMessage("too large");
            temp.kind = Num;
      temp.u.value = num;
      break;
   case colon:
      if ((ch = nextChar()) == '=') {
         ch = nextChar();
         temp.kind = Assign;
         break;
      } else {
         temp.kind = nul;
         break;
      }
   case Lss:
      if ((ch = nextChar()) == '=') {
         ch = nextChar();
         temp.kind = LssEq;
         break;
      } else if (ch == '>') {
         ch = nextChar();
         temp.kind = NotEq;
         break;
      } else {
         temp.kind = Lss;
         break;
      }
   case Gtr:
      if ((ch = nextChar()) == '=') {
         ch = nextChar();
         temp.kind = GtrEq;
         break;
      } else {
         temp.kind = Gtr;
         break;
      }
   default:
      temp.kind = cc;
      ch = nextChar(); break;
   }
   cToken = temp; printed = 0;
   return temp;
}

Token checkGet(Token t, KeyId k)
{
   if (t.kind==k)
         return nextToken();
   if ((isKeyWd(k) && isKeyWd(t.kind)) ||
      (isKeySym(k) && isKeySym(t.kind))){
         errorDelete();
         errorInsert(k);
         return nextToken();
   }
   errorInsert(k);
   return t;
}

static void printSpaces()
{
   while (CR-- > 0)
      fprintf(fptex, "\\ \\par\n");
   while (spaces-- > 0)
      fprintf(fptex, "\\ ");
   CR = 0; spaces = 0;
}

void printcToken()
{
   int i=(int)cToken.kind;
   if (printed){
      printed = 0; return;
   }
   printed = 1;
   printSpaces();
   if (i < end_of_KeyWd) 
      fprintf(fptex, "{\\bf %s}", KeyWdT[i].word);
   else if (i < end_of_KeySym)
      fprintf(fptex, "$%s$", KeyWdT[i].word);
   else if (i==(int)Id){
      switch (idKind) {
      case varId: 
         fprintf(fptex, "%s", cToken.u.id); return;
      case parId: 
         fprintf(fptex, "{\\sl %s}", cToken.u.id); return;
      case funcId: 
         fprintf(fptex, "{\\it %s}", cToken.u.id); return;
      case constId: 
         fprintf(fptex, "{\\sf %s}", cToken.u.id); return;
      }
   }else if (i==(int)Num)
      fprintf(fptex, "%d", cToken.u.value);
}

void setIdKind (KindT k)
{
   idKind = k;
}

int compile()
{
   int i;
   printf("start compilation\n");
   initSource();         
   token = nextToken();      
   blockBegin(FIRSTADDR);   
   block(0);
   finalSource();
   i = errorN();
   if (i!=0)
      printf("%d errors\n", i);
   return i<MINERROR;
}

void block(int pIndex)
{
   int backP;
   backP = genCodeV(jmp, 0);
   while (1) {   
      switch (token.kind){
      case Const:   
         token = nextToken();
         constDecl(); continue;
      case Var:
         token = nextToken();
         varDecl(); continue;
      case Func:
         token = nextToken();
         funcDecl(); continue;
      default:
         break;
      }
      break;
   }         
   backPatch(backP);
   changeV(pIndex, nextCode());
   genCodeV(ict, frameL());
   statement();
   genCodeR();
   blockEnd();
}   

void constDecl()
{
   Token temp;
   while(1){
      if (token.kind==Id){
         setIdKind(constId);         
         temp = token;             
         token = checkGet(nextToken(), Equal);   
         if (token.kind==Num)
            enterTconst(temp.u.id, token.u.value);
         else
            errorType("number");
         token = nextToken();
      }else
         errorMissingId();
      if (token.kind!=Comma){   
         if (token.kind==Id){
            errorInsert(Comma);
            continue;
         }else
            break;
      }
      token = nextToken();
   }
   token = checkGet(token, Semicolon);
}

void varDecl()
{
   while(1){
      if (token.kind==Id){
         setIdKind(varId);   
         enterTvar(token.u.id);   
         token = nextToken();
      }else
         errorMissingId();
      if (token.kind!=Comma){   
         if (token.kind==Id){
            errorInsert(Comma);
            continue;
         }else
            break;
      }
      token = nextToken();
   }
   token = checkGet(token, Semicolon);   
}

void funcDecl()   
{
   int fIndex;
   if (token.kind==Id){
      setIdKind(funcId);         
      fIndex = enterTfunc(token.u.id, nextCode());
      token = checkGet(nextToken(), Lparen);
      blockBegin(FIRSTADDR);
      while(1){
         if (token.kind==Id){
            setIdKind(parId);
            enterTpar(token.u.id);
            token = nextToken();
         }else
            break;
         if (token.kind!=Comma){
            if (token.kind==Id){
               errorInsert(Comma);
               continue;
            }else
               break;
         }
         token = nextToken();
      }
      token = checkGet(token, Rparen);
      endpar();
      if (token.kind==Semicolon){
         errorDelete();
         token = nextToken();
      }
      block(fIndex);
      token = checkGet(token, Semicolon);
   } else 
      errorMissingId();
}

void statement()
{
   int tIndex;
   KindT k;
   int backP, backP2;

   while(1) {
      switch (token.kind) {
      case Id:
         tIndex = searchT(token.u.id, varId);
         setIdKind(k=kindT(tIndex));   
         if (k != varId && k != parId) 
            errorType("var/par");
         token = checkGet(nextToken(), Assign);
         expression();            
         genCodeT(sto, tIndex);         
         return;
      case If:            
         token = nextToken();
         condition();            
         token = checkGet(token, Then);   
         backP = genCodeV(jpc, 0);      
         statement();   
         backPatch(backP);         
         return;
      case Ret:      
         token = nextToken();
         expression();            
         genCodeR();      
         return;
      case Begin:         
         token = nextToken();
         while(1){
            statement();      
            while(1){
               if (token.kind==Semicolon){   
                  token = nextToken();
                  break;
               }
               if (token.kind==End){
                  token = nextToken();
                  return;
               }
               if (isStBeginKey(token)){
                  errorInsert(Semicolon);
                  break;
               }
               errorDelete();
               token = nextToken();
            }
         }
      case While:   
         token = nextToken();
         backP2 = nextCode();
         condition();   
         token = checkGet(token, Do);
         backP = genCodeV(jpc, 0);
         statement();      
         genCodeV(jmp, backP2);   
         backPatch(backP);
         return;
      case Write:      
         token = nextToken();
         expression();            
         genCodeO(wrt);            
         return;
      case WriteLn:   
         token = nextToken();
         genCodeO(wrl);         
         return;
      case End: case Semicolon:      
         return;
      default:   
         errorDelete();         
         token = nextToken();
         continue;
      }      
   }
}

int isStBeginKey(Token t)
{
   switch (t.kind){
   case If: case Begin: case Ret:
   case While: case Write: case WriteLn:
      return 1;
   default:
      return 0;
   }
}

void expression()
{
   KeyId k;
   k = token.kind;
   if (k==Plus || k==Minus){
      token = nextToken();
      term();
      if (k==Minus)
         genCodeO(neg);
   }else
      term();
   k = token.kind;
   while (k==Plus || k==Minus){
      token = nextToken();
      term();
      if (k==Minus)
         genCodeO(sub);
      else
         genCodeO(add);
      k = token.kind;
   }
}

void term()
{
   KeyId k;
   factor();
   k = token.kind;
   while (k==Mult || k==Div){   
      token = nextToken();
      factor();
      if (k==Mult)
         genCodeO(mul);
      else
         genCodeO(div);
      k = token.kind;
   }
}

KindT kindT(int i)   
{
   return nameTable[i].kind;
}

RelAddr relAddr(int ti)   
{
   return nameTable[ti].u.raddr;
}

int val(int ti)   
{
   return nameTable[ti].u.value;
}

int pars(int ti)
{
   return nameTable[ti].u.f.pars;
}

int frameL()
{
   return localAddr;
}

void factor()   
{
   int tIndex, i;
   KindT k;
   if (token.kind==Id){
      tIndex = searchT(token.u.id, varId);
      setIdKind(k=kindT(tIndex));   
      switch (k) {
      case varId: case parId:   
         genCodeT(lod, tIndex);
         token = nextToken(); break;
      case constId:
         genCodeV(lit, val(tIndex));
         token = nextToken(); break;
      case funcId:
         token = nextToken();
         if (token.kind==Lparen){
            i=0; 
            token = nextToken();
            if (token.kind != Rparen) {
               for (; ; ) {
                  expression(); i++;
                  if (token.kind==Comma){
                     token = nextToken();
                     continue;
                  }
                  token = checkGet(token, Rparen);
                  break;
               }
            } else
               token = nextToken();
            if (pars(tIndex) != i) 
               errorMessage("\\#par");
         }else{
            errorInsert(Lparen);
            errorInsert(Rparen);
         }
         genCodeT(cal, tIndex);   
         break;
      }
   }else if (token.kind==Num){   
      genCodeV(lit, token.u.value);
      token = nextToken();
   }else if (token.kind==Lparen){
      token = nextToken();
      expression();
      token = checkGet(token, Rparen);
   }
   switch (token.kind){
   case Id: case Num: case Lparen:
      errorMissingOp();
      factor();
   default:
      return;
   }   
}
   
void condition()
{
   KeyId k;
   if (token.kind==Odd){
      token = nextToken();
      expression();
      genCodeO(odd);
   }else{
      expression();
      k = token.kind;
      switch(k){
      case Equal: case Lss: case Gtr:
      case NotEq: case LssEq: case GtrEq:
         break;
      default:
         errorType("rel-op");
         break;
      }
      token = nextToken();
      expression();
      switch(k){
      case Equal:   genCodeO(eq); break;
      case Lss:      genCodeO(ls); break;
      case Gtr:      genCodeO(gr); break;
      case NotEq:   genCodeO(neq); break;
      case LssEq:   genCodeO(lseq); break;
      case GtrEq:   genCodeO(greq); break;
      }
   }
} 

static char* kindName(KindT k)
{
   switch (k){
   case varId: return "var";
   case parId: return "par";
   case funcId: return "func";
   case constId: return "const";
   }
}

void blockBegin(int firstAddr)
{
   if (level == -1){
      localAddr = firstAddr;
      tIndex = 0;
      level++;
      return;
   }
   if (level == MAXLEVEL-1)
      errorF("too many nested blocks");
   index[level] = tIndex;
   addr[level] = localAddr;
   localAddr = firstAddr;
   level++;
   return;
}

void blockEnd()   
{
   level--;
   tIndex = index[level];
   localAddr = addr[level];
}

int bLevel()
{
   return level;
}

int fPars()   
{
   return nameTable[index[level-1]].u.f.pars;
}

void enterT(char *id)
{
   if (tIndex++ < MAXTABLE){
      strcpy(nameTable[tIndex].name, id);
   } else 
      errorF("too many names");
}
      
int enterTfunc(char *id, int v)
{
   enterT(id);
   nameTable[tIndex].kind = funcId;
   nameTable[tIndex].u.f.raddr.level = level;
   nameTable[tIndex].u.f.raddr.addr = v;  
   nameTable[tIndex].u.f.pars = 0;
   tfIndex = tIndex;
   return tIndex;
}

int enterTpar(char *id)   
{
   enterT(id);
   nameTable[tIndex].kind = parId;
   nameTable[tIndex].u.raddr.level = level;
   nameTable[tfIndex].u.f.pars++;
   return tIndex;
}

int enterTvar(char *id)
{
   enterT(id);
   nameTable[tIndex].kind = varId;
   nameTable[tIndex].u.raddr.level = level;
   nameTable[tIndex].u.raddr.addr = localAddr++;
   return tIndex;
}

int enterTconst(char *id, int v)
{
   enterT(id);
   nameTable[tIndex].kind = constId;
   nameTable[tIndex].u.value = v;
   return tIndex;
}

void endpar()
{
   int i;
   int pars = nameTable[tfIndex].u.f.pars;
   if (pars == 0)  return;
   for (i=1; i<=pars; i++)
       nameTable[tfIndex+i].u.raddr.addr = i-1-pars;
}

void changeV(int ti, int newVal)
{
   nameTable[ti].u.f.raddr.addr = newVal;
}

int searchT(char *id, KindT k)
{
   int i;
   i = tIndex;
   strcpy(nameTable[0].name, id);
   while( strcmp(id, nameTable[i].name) )
      i--;
   if ( i )
      return i;
   else {   
      errorType("undef");
      if (k==varId) return enterTvar(id);
      return 0;
   }
}

static Inst code[MAXCODE];   
static int cIndex = -1;         
static void checkMax();           
static void printCode(int i);

int nextCode()            
{
   return cIndex+1;
}

int genCodeV(OpCode op, int v)   
{
   checkMax();
   code[cIndex].opCode = op;
   code[cIndex].u.value = v;
   return cIndex;
}

int genCodeT(OpCode op, int ti)   
{
   checkMax();
   code[cIndex].opCode = op;
   code[cIndex].u.addr = relAddr(ti);
   return cIndex;
}

int genCodeO(Operator p)      
{
   checkMax();
   code[cIndex].opCode = opr;
   code[cIndex].u.optr = p;
   return cIndex;
}

int genCodeR()            
{
   if (code[cIndex].opCode == ret)
      return cIndex;      
   checkMax();
   code[cIndex].opCode = ret;
   code[cIndex].u.addr.level = bLevel();
   code[cIndex].u.addr.addr = fPars();   
   return cIndex;
}

void checkMax()   
{
   if (++cIndex < MAXCODE)
      return;
   errorF("too many code");
}
   
void backPatch(int i)
{
   code[i].u.value = cIndex+1;
}


void listCode()   
{
   int i;
   printf("\ncode\n");
   for(i=0; i<=cIndex; i++){
      printf("%3d: ", i);
      printCode(i);
   }
}

void printCode(int i)
{
   int flag;
   switch(code[i].opCode){
   case lit: printf("lit"); flag=1; break;
   case opr: printf("opr"); flag=3; break;
   case lod: printf("lod"); flag=2; break;
   case sto: printf("sto"); flag=2; break;
   case cal: printf("cal"); flag=2; break;
   case ret: printf("ret"); flag=2; break;
   case ict: printf("ict"); flag=1; break;
   case jmp: printf("jmp"); flag=1; break;
   case jpc: printf("jpc"); flag=1; break;
   }
   switch(flag){
   case 1:
      printf(",%d\n", code[i].u.value);
      return;
   case 2:
      printf(",%d", code[i].u.addr.level);
      printf(",%d\n", code[i].u.addr.addr);
      return;
   case 3:
      switch(code[i].u.optr){
      case neg: printf(",neg\n"); return;
      case add: printf(",add\n"); return;
      case sub: printf(",sub\n"); return;
      case mul: printf(",mul\n"); return;
      case div: printf(",div\n"); return;
      case odd: printf(",odd\n"); return;
      case eq: printf(",eq\n"); return;
      case ls: printf(",ls\n"); return;
      case gr: printf(",gr\n"); return;
      case neq: printf(",neq\n"); return;
      case lseq: printf(",lseq\n"); return;
      case greq: printf(",greq\n"); return;
      case wrt: printf(",wrt\n"); return;
      case wrl: printf(",wrl\n"); return;
      }
   }
}   

void execute()
{
   int stack[MAXMEM];
   int display[MAXLEVEL];
   int pc, top, lev, temp;
   Inst i;
   printf("start execution\n");
   top = 0;  pc = 0;
   stack[0] = 0;  stack[1] = 0;
   display[0] = 0;   
   do {
      i = code[pc++];      
      switch(i.opCode){
      case lit: stack[top++] = i.u.value; 
            break;
      case lod: stack[top++] = stack[display[i.u.addr.level] + i.u.addr.addr]; 
             break;
      case sto: stack[display[i.u.addr.level] + i.u.addr.addr] = stack[--top]; 
             break;
      case cal: lev = i.u.addr.level +1;
            stack[top] = display[lev]; 
            stack[top+1] = pc; display[lev] = top; 
            pc = i.u.addr.addr;
             break;
      case ret: temp = stack[--top];   
            top = display[i.u.addr.level];  
            display[i.u.addr.level] = stack[top];
            pc = stack[top+1];
            top -= i.u.addr.addr;
            stack[top++] = temp;
            break;
      case ict: top += i.u.value; 
            if (top >= MAXMEM-MAXREG)
               errorF("stack overflow");
            break;
      case jmp: pc = i.u.value; break;
      case jpc: if (stack[--top] == 0)
               pc = i.u.value;
            break;
      case opr: 
         switch(i.u.optr){
         case neg: stack[top-1] = -stack[top-1]; continue;
         case add: --top;  stack[top-1] += stack[top]; continue;
         case sub: --top; stack[top-1] -= stack[top]; continue;
         case mul: --top;  stack[top-1] *= stack[top];  continue;
         case div: --top;  stack[top-1] /= stack[top]; continue;
         case odd: stack[top-1] = stack[top-1] & 1; continue;
         case eq: --top;  stack[top-1] = (stack[top-1] == stack[top]); continue;
         case ls: --top;  stack[top-1] = (stack[top-1] < stack[top]); continue;
         case gr: --top;  stack[top-1] = (stack[top-1] > stack[top]); continue;
         case neq: --top;  stack[top-1] = (stack[top-1] != stack[top]); continue;
         case lseq: --top;  stack[top-1] = (stack[top-1] <= stack[top]); continue;
         case greq: --top;  stack[top-1] = (stack[top-1] >= stack[top]); continue;
         case wrt: printf("%d \n", stack[--top]); continue;
         case wrl: printf("\n"); continue;
         }
      }
      int j;
      for(j = 0; j<=10; j++) {
      	printf("%d ", stack[j]);
	  }
	  printf("\n");
   } while (pc != 0);
}
