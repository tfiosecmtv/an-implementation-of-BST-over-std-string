
#include "set.h"

void print( std::ostream& out, const treenode* n, size_t indent )
{
    
    for( size_t i = 0; i != indent; ++ i )
        out << "|  ";
    if(n)
    {
        out << ( n -> val ) << "\n";
        print( out, n -> left, indent + 1 );
        print( out, n -> right, indent + 1 );
    }
    else
        out << "#\n";
}


void checksorted( 
                 treenode* n,
                 const std::string* lowerbound,
                 const std::string* upperbound )
{
    while(n)
    {
        if( lowerbound && !before( *lowerbound, n -> val ))
        {
            std::cout << "value " << ( n -> val );
            std::cout << " is not above lower bound " << *lowerbound << "\n";
            std::abort( );
        }
        
        if( upperbound && !before( n -> val, *upperbound ))
        {
            std::cout << "value " << ( n -> val );
            std::cout << "is not below upperbound " << *upperbound << "\n";
            std::abort( );
        }
        
        checksorted( n -> left, lowerbound, &( n -> val ));
        lowerbound = & ( n -> val );
        n = n -> right;
    }
} 


// Used by copy constructor and assignment:

void writecopy( treenode** to, treenode* from )
{
    while( from )
    {
        *to = new treenode{ from -> val };
        
        writecopy( & (*to) -> left, from -> left );
        to = & (*to) -> right;
        from = from -> right;
    }
}

// Used by destructor:

void deallocate( treenode* n )
{
    while(n)
    {
        deallocate( n -> left );
        treenode* bye = n;
        n = n -> right;
        delete bye;
    }
}



size_t log_base2( size_t s )
{
    int p = 0;
    int n = 2;
    if (s == 0 || s == 1)
        return 0;
    else {
        while (n <= s) {
            ++ p;
            n *=2;
        }
        return p;
    }
}

// From previous task:

bool equal( const std::string& s1, const std::string& s2 )
{
    size_t i=0,eq=0;
    if (s1.size()!=s2.size())
    {
        return false;
    } else
    {
        while (s1[i]!='\0')
        {
            char c1 = _tolower(s1[i]);
            char c2 = _tolower(s2[i]);
            if (c1==c2)
            {
                eq++;
            }
            i++;
        }
        
        if (eq==i)
        {
            return true;
        }
    }
    return false;
}

// Must be written for task 5(part 2) :

bool before( const std::string& s1, const std::string& s2 )
{
    int i1 = 0, i2 = 0,  n = 0;
    char c1, c2;
    while (s1[i1] != '\0' )
    {
        i1 ++;
    }
    while (s2[i2] != '\0' )
    {
        i2 ++;
    }
    if (i1 == i2)
    {
        while (n<i1)
        {
            c1 = tolower(s1[n]);
            c2 = tolower(s2[n]);
            if (c1<c2)
            {
                return true;
            } else if (c1>c2)
            {
                return false;
            }
            n ++;
        }
        return false;
        
    } else
    {
        int comp = (i1 < i2) ? i1 : i2;
        while (n<comp)
        {
            c1 = tolower(s1[n]);
            c2 = tolower(s2[n]);
            if (c1<c2)
            {
                return true;
            } else if (c1>c2)
            {
                return false;
            }
            n ++;
        }
        if (comp == i1)
        {
            return true;
        } else
            return false;
    }
}


const treenode* find( const treenode* n, const std::string& el )
{
    if(n == nullptr)
        return nullptr;
    else if(equal(n->val, el))
        return n;
    else if(before(n->val, el))
        return find(n->right, el);
    else return find(n->left, el);
}
// Write this one first.

treenode** find( treenode** n, const std::string& el )
{
    if ((*n) == nullptr || equal( (*n)->val, el) )
        return n;
    else if( before( (*n)->val, el) )
        return find(&((*n)->right), el);
    else return find(&((*n)->left), el);
}
// Modify the other find into this one, also have a look at
// stackscan4.pdf


// Insert n at the right most position in into: 

void rightinsert( treenode** into, treenode* n )
{
    while ((*into)->right!=nullptr)
    {
        into = &(*into)->right;
    }
    
    (*into)->right = n;
}


size_t size( const treenode* n )
{
    if (n == nullptr)
    {
        return 0;
    } else
    {
        return (size(n->left)+size(n->right)+1);
    }
}

size_t height( const treenode* n )
{
    size_t value;
    if (n == nullptr)
    {
        return 0;
    } else
        value = (height(n->left) > height(n->right) ? height(n->left) : height(n->right)) + 1;
    return value;
}


bool set::insert( const std::string& el )
{
    if (this->contains(el))
    {
        return false;
    } else
    {
        treenode **node = find(&tr, el);
        *node = new treenode(el);
        return true;
    }
}

bool set::contains( const std::string& el ) const 
{
    return find( tr, el ) != nullptr;
}


bool set::remove( const std::string& el )
{
    treenode **node = find(&tr, el);
    if (this->contains(el) == false)
    {
        return false;
    } else if ((*node)->left == nullptr && (*node)->right == nullptr)
    {
        treenode *temp = *node;
        *node = nullptr;
        delete temp;
        return true;
    } else if ( (*node)->left == nullptr)
    {
        treenode *temp = *node;
        *node = (*node)->right;
        delete temp;
        return true;
    } else if ( (*node)->right == nullptr)
    {
        treenode *temp = *node;
        *node = (*node)->left;
        delete temp;
        return true;
    } else
    {
        treenode *temp = *node;
        rightinsert(&((*node)->left), (*node)->right);
        *node = (*node)->left;
        delete temp;
        return true;
    }
    return 1;
}


void set::checksorted( ) const
{
    ::checksorted( tr, nullptr, nullptr );
    // :: are needed because the other checksorted is not in the class.
}


std::ostream& set::print( size_t indent, std::ostream& out ) const
{
    ::print( out, tr, indent );
    return out;
}


