/**************************************************************************
 * JmarsKoder Presents 
 * JmarsKoder's Sequitur Engine
 * Adapted from original sequitur c++ code
 * 
 * written by Jason Mars (jmarskoder@gmail.com){}
 * ***********************************************************************/

#include "seq_engine.h"

namespace k {
//Global Stuff-------------------------------------------------------------
#define HASH(one, two) (((one) << 16 | (two)) % SEQ_TAB_SIZE)
#define HASH2(one) (17 - ((one) % 17))
seq_element::seq_element(bool t, u_long v):is_term(t),value(v){}
//-------------------------------------------------------------------------


//Sequitur-----------------------------------------------------------------
seq_engine::seq_symbol **seq_engine::seq_find_digram(seq_symbol *s)
{
  u_long one=s->value();
  u_long two=s->next()->value();

  u_long jump=HASH2(one);
  u_long insert=SEQ_TAB_SIZE+1;
  u_long i=HASH(one, two);

  while (1) {
    seq_symbol *m=seq_table[i];
    if(!m) {
      if(insert>SEQ_TAB_SIZE) insert=i;
      return &seq_table[insert];
    } 
    else if(u_long(m)==1) insert=i;
    else if(m->value()==one && m->next()->value()==two) 
      return &seq_table[i];
    i=(i + jump) % SEQ_TAB_SIZE;
  }
}

seq_engine::seq_symbol::seq_symbol(seq_engine *m, u_long sym):
  master(m),is_t(true),s(sym),p(0),n(0){}

seq_engine::seq_symbol::seq_symbol(seq_engine *m, seq_rule *r):
  master(m),is_t(false),s((u_long)r),p(0),n(0) {rule()->reuse();}

void seq_engine::seq_symbol::delete_self() 
{
  join(p, n);
  if(!is_guard()) 
  {
    delete_digram();
    if(non_term())rule()->deuse(); 
  }
  delete this;
}

void seq_engine::seq_symbol::insert_after(seq_symbol *y) 
{
  join(y, n);
  join(this, y);
}

void seq_engine::seq_symbol::delete_digram() 
{
  if(is_guard() || n->is_guard()) return;
  seq_symbol **m=master->seq_find_digram(this);
  if(*m==this) *m=(seq_symbol*) 1;
}
  
bool seq_engine::seq_symbol::is_guard() 
  {return non_term() && rule()->first()->prev()==this;}

bool seq_engine::seq_symbol::non_term() {return !is_t && (s!=0);}
seq_engine::seq_symbol *seq_engine::seq_symbol::next() {return n;}
seq_engine::seq_symbol *seq_engine::seq_symbol::prev() {return p;};
inline u_long seq_engine::seq_symbol::value() {return s;}
  
seq_engine::seq_rule *seq_engine::seq_symbol::rule() {return (seq_rule*)s;}

void seq_engine::seq_symbol::substitute(seq_rule *r)
{
  seq_symbol *q=p;
  seq_engine *master_saved=master; //because i will delete myself
  q->next()->delete_self();
  q->next()->delete_self();
  q->insert_after(new seq_symbol(master_saved, r));
  if(!q->check()) q->n->check();
}

bool seq_engine::seq_symbol::check() 
{
  if(is_guard() || n->is_guard()) return 0;
  seq_symbol **x=master->seq_find_digram(this);
  if(u_long(*x) <=1) 
  {
    *x=this;
    return 0;
  }
  if(u_long(*x) > 1 && (*x)->next() !=this) match(this, *x);
  return 1;
}
  
void seq_engine::seq_symbol::expand() 
{
  seq_symbol *left=prev();
  seq_symbol *right=next();
  seq_symbol *f=rule()->first();
  seq_symbol *l=rule()->last();
  seq_engine *master_saved=master;

  rule()->delete_self();
  seq_symbol **m=master_saved->seq_find_digram(this);
  if(*m==this) *m=(seq_symbol *) 1;
  s=0; // ifwe don't do this, deleting the symbol tries to deuse the rule!
  delete_self();
  
  join(left, f);
  join(l, right);
  *master_saved->seq_find_digram(l)=l;
}

void seq_engine::seq_symbol::point_to_self() { join(this, this); };

void seq_engine::seq_symbol::join(seq_symbol *left, seq_symbol *right) 
{
  if(left->n) 
  {
    left->delete_digram();

    if(right->p&&right->n&&
        right->value()==right->p->value()&&
        right->value()==right->n->value()&& 
        right->non_term()==right->p->non_term()&&
        right->non_term()==right->n->non_term())
      *right->master->seq_find_digram(right)=right;
    
    if(left->p&&left->n&&
        left->value()==left->n->value()&&
        left->value()==left->p->value()&&
        left->non_term()==left->p->non_term()&&
        left->non_term()==left->n->non_term())
      *left->master->seq_find_digram(left->p)=left->p;
  }
  left->n=right; right->p=left;
}

void seq_engine::seq_symbol::match(seq_symbol *ss, seq_symbol *m) 
{
  seq_rule *r;
  if(m->prev()->is_guard() && m->next()->next()->is_guard()) 
  {
    r=m->prev()->rule();
    ss->substitute(r); 
  }
  else 
  {
    r=new seq_rule(ss->master);    
    if(ss->non_term())
      r->last()->insert_after(new seq_symbol(ss->master, ss->rule()));
    else
      r->last()->insert_after(new seq_symbol(ss->master, ss->value()));
    if(ss->next()->non_term())
      r->last()->insert_after(new seq_symbol(ss->master, ss->next()->rule()));
    else
      r->last()->insert_after(new seq_symbol(ss->master, ss->next()->value()));
    seq_engine *master_saved=ss->master;
    m->substitute(r);
    ss->substitute(r);
    *master_saved->seq_find_digram(r->first())=r->first();
  }
  if(r->first()->non_term() && r->first()->rule()->freq()==1) r->first()->expand();
}

seq_engine::seq_rule::seq_rule(seq_engine *m):master(m) 
{
  master->num_rules ++;
  guard=new seq_symbol(master, this);
  guard->point_to_self();
  count=number=0;
}

void seq_engine::seq_rule::delete_self() 
{ 
  master->num_rules --;
  guard->delete_self(); 
  delete this;
}

void seq_engine::seq_rule::reuse() { count ++; }
void seq_engine::seq_rule::deuse() { count --; };
seq_engine::seq_symbol *seq_engine::seq_rule::first() { return guard->next(); }
seq_engine::seq_symbol *seq_engine::seq_rule::last() { return guard->prev(); }
u_long seq_engine::seq_rule::freq() { return count; };
u_long seq_engine::seq_rule::index() { return number; };
void seq_engine::seq_rule::index(u_long i) { number=i; };

  
void seq_engine::clean(seq_rule *r)
{
  if(!r) return;
  for(seq_symbol *p=r->first(); !p->is_guard(); p=p->next())
    if(p->non_term())
    {
      clean(p->rule());
      s_scrub.insert(p);
    }
    else s_scrub.insert(p);
  s_scrub.insert(r->first()->prev());
  r_scrub.insert(r);
}

u_long seq_engine::calc_length(seq_rule *r)
{
  u_long ret=0;
  for(seq_symbol *p=r->first(); !p->is_guard(); p=p->next())
    if(p->non_term()) {if(p->rule()!=r) ret+=calc_length(p->rule());}
    else ret++;
  return ret;
}

seq_engine::seq_engine(){start=0; seq_table=new seq_symbol*[SEQ_TAB_SIZE]; reset();}
seq_engine::~seq_engine(){reset(); delete start; delete[]seq_table;}

void seq_engine::append(u_long in)
{
  input_size++;
  if(input_size>(SEQ_TAB_SIZE-100)){cerr << "Sequitur Hash Table OVERFULL!!" << endl;}
  if(start->last()==start->first())
  {
    start->last()->insert_after(new seq_symbol(this, in));
  }
  else
  {
    start->last()->insert_after(new seq_symbol(this, in));
    start->last()->prev()->check();
  }
}

void seq_engine::harvest()
{
  gram.clear();
  seq_rule **R = (seq_rule **) malloc(sizeof(seq_rule *) * num_rules);
  memset(R, 0, sizeof(seq_rule *) * num_rules);
  R[0] = start;
  u_long Ri = 1;

  for(u_long j=0; j<Ri; j++)
  {
    deque<seq_element> to_add;
    seq_element e(false,j);
    e.cold_uses=R[j]->freq();
    j ? e.length=calc_length(R[j]) : e.length=input_size ;
    e.coverage=(double)(e.cold_uses*e.length)/(double)input_size;
    to_add.push_back(e);
    
    for(seq_symbol *p=R[j]->first(); !p->is_guard(); p=p->next())
    {
      if(p->non_term())
      {
        u_long i;
        if(p->rule()->index()<num_rules&&R[p->rule()->index()]==p->rule())
          i=p->rule()->index();
        else
        {
          i=Ri;
          p->rule()->index(Ri);
          R[Ri++]=p->rule();
        }
        to_add.push_back(seq_element(false,i));
      }
      else
        to_add.push_back(seq_element(true,p->value()));
    }
    gram.push_back(to_add);
  }
  free(R);
}

void seq_engine::reset()
{
  clean(start);
  for(set<seq_symbol*>::iterator i=s_scrub.begin();
      i!=s_scrub.end(); i++){delete *i;}
  for(set<seq_rule*>::iterator i=r_scrub.begin();
      i!=r_scrub.end(); i++){delete *i;}
  s_scrub.clear(); r_scrub.clear();
  num_rules=input_size=0;
  memset(seq_table, 0, sizeof(seq_symbol *) * SEQ_TAB_SIZE);
  start=new seq_rule(this);
}

deque<u_long> seq_engine::expand_rule(u_long idx)
{
  deque<u_long> ret;
  if(idx>gram.size()) return ret;
  for(u_long j=1; j<gram[idx].size(); j++)
    if(gram[idx][j].is_term) ret.push_back(gram[idx][j].value);
    else 
    {
      deque<u_long> sub=expand_rule(gram[idx][j].value);
      for(u_long i=0; i<sub.size(); i++) ret.push_back(sub[i]);
    }
  return ret;
}

u_long seq_engine::length(){return input_size;}

void seq_engine::print_rule(u_long idx)
{
  if(idx>gram.size()) return;
  for(u_long j=0; j<gram[idx].size(); j++)
    if(j==0) cout << "<" << gram[idx][j].value << "> -> ";
    else if(gram[idx][j].is_term) cout << gram[idx][j].value << ' ';
    else cout << "<" << gram[idx][j].value << "> ";
}

void seq_engine::print_grammar()
{
  for(u_long i=0; i<gram.size(); i++)
  {
    print_rule(i);
    cout << endl;
  }
}

void seq_engine::print_stats()
{
  for(u_long i=0; i<gram.size(); i++)
  {
    cout << "rule #: " << gram[i][0].value << "\t"
         << "cold_uses: " << gram[i][0].cold_uses << "\t"
         << "length: " << gram[i][0].length << "\t"
         << "coverage: " << gram[i][0].coverage 
         << endl;
  }
}
//-------------------------------------------------------------------------
}

