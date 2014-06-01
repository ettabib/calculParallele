
int yylex() {
  typedef pair<int,void*>  value; 
  
  L1:                //  debut 
   if (ccin->eof())  // si fin de fichier 
     return ENDOFFILE;
     
   int c=ccin->peek();   // prendre  le caractere courant sans avancé
   
   if (isdigit(c))       // commence par un chiffre => nombre DNUM
        { *ccin >> yylval.dnum; 
           assert(ccin->eof() || ccin->good()); 
           cout << yylval.dnum;
           return DNUM;}
           
   else if (c=='.')   // commence par un point ( point ou chiffre ) 
        { ccin->get(); c=ccin->peek(); ccin->putback('.'); 
          if (isdigit(c)) // un chiffre
             { 
               *ccin >> yylval.dnum; 
                assert(ccin->eof() || ccin->good()); 
                cout << yylval.dnum;
                return DNUM;}
          cout << "." ;      
         return ccin->get();} // un point 
           
   else if (c=='+' || c=='*' || c=='(' || c==')' 
       ||   c=='-' || c=='/' || c=='{' || c=='}'  
       ||   c=='=' || c=='^' || c==';' || c==','  ) { 
           
            cout << char(c) ; return ccin->get(); }
            
   else if (c=='"') // un chaine STRING
     {
        yylval.str = new string();
        ccin->get();
        while ( (c=ccin->peek()) != '"' && c != EOF)
          *yylval.str += char(ccin->get());
        assert(ccin->peek() == '"' );
        ccin->get(); // 
        cout << '"'<< *yylval.str << '"' ;
        return STRING;
     }
     
   else if (isalpha(c)) {  // un identificateur 
      yylval.str = new string();
      do {
        *yylval.str += char(ccin->get());
       } while (isalnum(ccin->peek()));
      cout << *yylval.str;
      
      value p=tableId.Find(*yylval.str);  // est-il dans la table des symboles
     if (p.first==0) return NEW_ID;       // non
      else {
        delete yylval.str;
        yylval.p=p.second;                // oui le pointeur
        return p.first;} }                // la type de identicateur
         
   else if (isspace(c))  // un espace
         {ccin->get(); 
          cout << char(c) ;
          goto L1;}      // on saute
   else if ( c <0)
       return ENDOFFILE;
   else { 
       cerr << " caractere invalide " << char(c) << " " << c << endl;
       yyerror(" caractere invalide ");
   }
};