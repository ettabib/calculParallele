class TableOfIdentifier {
public:
    typedef  pair<int,void  *> value; // typeid + data
    typedef map<string,value> maptype;
    typedef  maptype::iterator iterator;
    typedef  maptype::const_iterator const_iterator;
    maptype m;
      value Find(const string & id) const;
      void * Add(const string & id,int i=0,void *p=0)
      { m.insert(make_pair<string,value>(id,value(i,p))); return p;}
      void * Add(const string & id, R (*f)(R));
      void * TableOfIdentifier::Add(const string & id, R (*f)(R,R));
      const Exp::ExpBase * Add(const string & id,const Exp::ExpBase * f) ;
      void * Add(const string & id, Rn * p);
      
} tableId;
