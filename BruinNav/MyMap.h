// MyMap.h

// Skeleton for the MyMap class template.  You must implement the first six
// member functions.
#include <iostream>
template<typename KeyType, typename ValueType>
class MyMap
{
public:
    MyMap(){m_root = NULL; m_size = 0;};
    
	~MyMap()
    {
        clear();
    }
	
    void clear()
    {
        clear(m_root); // calls overloaded function.
        m_size =0;
        m_root = NULL;
    }
    
	int size() const
    {
        return m_size;
    }
    
	void associate(const KeyType& key, const ValueType& value)
    {
        if(m_root == NULL) // if the tree is empty then create the root.
        {
            m_root = new Node(key, value);
            m_size++;
            return;
        }
        Node *cur = m_root;
        for(;;)
        {
            if(key == cur->myKey)       // if the key is the same, then update the value.
            {
                cur->myValue = value;
                return;
            }
            
            if(key < cur->myKey)
            {
                if(cur->left != NULL)
                    cur = cur->left;    //if there is something to the left, move cur to the left
                else
                {
                    cur->left = new Node(key, value); // otherwise, add a new node.
                    m_size++;
                    return;
                }
            }
            else if(key > cur->myKey)
            {
                if(cur->right != NULL)
                    cur = cur->right;   //if there is something to the right, move cur to the right.
                else
                {
                    cur->right = new Node(key, value); //otherwise add a new node.
                    m_size++;
                    return;
                }
            }
        }
    }

	  // for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const
    {
        return (find(m_root, key));
    }

	  // for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
	}

	  // C++11 syntax for preventing copying and assignment
	MyMap(const MyMap&) = delete;
	MyMap& operator=(const MyMap&) = delete;

private:
    int m_size;
    struct Node
    {
        Node(const KeyType k, ValueType v)
        {
            myValue = v;
            myKey = k;
            left = right = NULL;
        }
        KeyType myKey;
        ValueType myValue;
        Node *left, *right;
    };
    Node *m_root;
    
    //overload of clear function to make deleting easier.
    void clear(Node* t)
    {
        if(t == NULL)
            return;
        if(t->left != NULL)
            clear(t->left);
        if(t->right != NULL)
            clear(t->right);
    
        delete t;
    }
    
    //overload of find function to make finding easier
    const ValueType* find(Node* ptr, const KeyType& key) const
    {
        if(ptr == NULL)         //return nullptr if we can't find it
            return nullptr;
        else if(ptr->myKey == key)      //return the ptr if we find it.
        {
            ValueType *ptrFound;
            ptrFound = &ptr->myValue;
            return ptrFound;
        }
        else if (key < ptr->myKey)
            return (find(ptr->left, key));      //otherwise, we recursively look for the key.
        else
            return (find(ptr->right, key));
    }
    
};
